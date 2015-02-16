/* This file is part of the TSC Settingseditor.
 * Copyright © 2015 Marvin Gülker
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdexcept>
#include <locale>
#include <unistd.h>
#include <wx/statline.h>
#include <wx/spinctrl.h>
#include <wx/xrc/xmlres.h>
#include "utf8.hpp"
#include "cache.hpp"
#include "graphics_editor.hpp"
#include "frame.hpp"
#include "app.hpp"

/***************************************
 * OnInit() and OnExit()
 ***************************************/

bool SettingseditorApp::OnInit()
{
  std::locale::global(std::locale("")); // Initialise to current environment locale

  wxXmlResource::Get()->InitAllHandlers();
  wxXmlResource::Get()->Load("../ui/mainwindow.xrc");

  mp_mainwindow = wxXmlResource::Get()->LoadFrame(NULL, "mainwindow");
  mp_cache_info = new CacheInfo(Pathie::Path::cache_dir() / "tsc-scripteditor" / "data.ini");
  m_last_selected_frame = -1;

  setup_graphicseditor();
  setup_event_handlers();

  mp_mainwindow->Show(true);
  return true;
}

int SettingseditorApp::OnExit()
{
  //delete mp_graphicseditor; // Memory managed by wx? Segfaults if deleted here with a double-free message by glibc.

  // Free all the frames
  std::vector<Frame*>::iterator iter;
  for(iter = m_frames.begin(); iter != m_frames.end(); iter++)
    delete (*iter);

  delete mp_cache_info;
  return 0;
}

/***************************************
 * Helper functions
 ***************************************/

void SettingseditorApp::setup_graphicseditor()
{
  // wxWidgets doesn’t allow us to get a sizer out of the XRC directly, so we have to
  // jump through some hoops to get it. Retrieve a child widget and ask it for its
  // sizer.
  wxStaticLine* p_line = XRCCTRL(*mp_mainwindow, "image_separator_line", wxStaticLine);
  wxBoxSizer* p_sizer = static_cast<wxBoxSizer*>(p_line->GetContainingSizer());

  mp_graphicseditor = new GraphicsEditor(XRCCTRL(*mp_mainwindow, "image_page", wxPanel), wxID_ANY);
  p_sizer->Prepend(mp_graphicseditor, 1, wxEXPAND);
}

void SettingseditorApp::setup_event_handlers()
{
  // Menus
  mp_mainwindow->Bind(wxEVT_COMMAND_MENU_SELECTED, &SettingseditorApp::on_menu_file_quit, this, wxID_EXIT);

  XRCCTRL(*mp_mainwindow, "add_frame_button", wxButton)->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SettingseditorApp::on_add_frame_button_clicked, this, XRCID("add_frame_button"));
  XRCCTRL(*mp_mainwindow, "del_frame_button", wxButton)->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SettingseditorApp::on_del_frame_button_clicked, this, XRCID("del_frame_button"));

  // List
  XRCCTRL(*mp_mainwindow, "frame_listbox", wxListBox)->Bind(wxEVT_LISTBOX, &SettingseditorApp::on_frame_list_item_selected, this, wxID_ANY);
}

void SettingseditorApp::add_frame(Pathie::Path path)
{
  m_frames.push_back(new Frame(path));
  XRCCTRL(*mp_mainwindow, "frame_listbox", wxListBox)->Append(utf8_to_wxstr(path.basename().str()));
}

/***************************************
 * Event handlers
 ***************************************/

void SettingseditorApp::on_menu_file_quit(wxCommandEvent& evt)
{
  mp_mainwindow->Close();
}

void SettingseditorApp::on_add_frame_button_clicked(wxCommandEvent& evt)
{
  wxFileDialog fd(mp_mainwindow,
		  "Select file",
		  utf8_to_wxstr(mp_cache_info->get_last_dir().str()),
		  "",
		  "PNG files (*.png)|*.png|Any file (*.*)|*.*");

  if (fd.ShowModal() == wxID_CANCEL)
    return;

  Pathie::Path path(wxstr_to_utf8(fd.GetPath()));
  add_frame(path);

  mp_cache_info->set_last_dir(path.dirname());
}

void SettingseditorApp::on_del_frame_button_clicked(wxCommandEvent& evt)
{
  std::cout << "TODO" << std::endl;
}

void SettingseditorApp::on_frame_list_item_selected(wxCommandEvent& evt)
{
  wxSpinCtrl* p_width_spin   = XRCCTRL(*mp_mainwindow, "width_spin", wxSpinCtrl);
  wxSpinCtrl* p_height_spin  = XRCCTRL(*mp_mainwindow, "height_spin", wxSpinCtrl);
  wxTextCtrl* p_name_text    = XRCCTRL(*mp_mainwindow, "name_text", wxTextCtrl);
  wxTextCtrl* p_author_text  = XRCCTRL(*mp_mainwindow, "author_text", wxTextCtrl);
  wxTextCtrl* p_license_text = XRCCTRL(*mp_mainwindow, "license_text", wxTextCtrl);
  wxTextCtrl* p_other_text   = XRCCTRL(*mp_mainwindow, "other_text", wxTextCtrl);

  // Save the values of the last frame, if any
  if (m_last_selected_frame >= 0) {
    Frame* p_lastframe = m_frames[m_last_selected_frame];

    p_lastframe->get_settings().set_width(p_width_spin->GetValue());
    p_lastframe->get_settings().set_height(p_height_spin->GetValue());
    p_lastframe->get_settings().set_name(wxstr_to_utf8(p_name_text->GetValue()));
    p_lastframe->get_settings().set_author(wxstr_to_utf8(p_author_text->GetValue()));
    p_lastframe->get_settings().set_license(wxstr_to_utf8(p_license_text->GetValue()));
    p_lastframe->get_settings().set_other(wxstr_to_utf8(p_other_text->GetValue()));
  }

  // Read values of the new frame
  Frame* p_frame = m_frames[evt.GetInt()];
  p_width_spin   ->SetValue(p_frame->get_settings().get_width());
  p_height_spin  ->SetValue(p_frame->get_settings().get_height());
  p_name_text    ->SetValue(utf8_to_wxstr(p_frame->get_settings().get_name()));
  p_author_text  ->SetValue(utf8_to_wxstr(p_frame->get_settings().get_author()));
  p_license_text ->SetValue(utf8_to_wxstr(p_frame->get_settings().get_license()));
  p_other_text   ->SetValue(utf8_to_wxstr(p_frame->get_settings().get_other()));

  // Set last frame for saving next time
  m_last_selected_frame = evt.GetInt();
}
