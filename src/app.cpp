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
#include <wx/notebook.h>
#include <wx/aboutdlg.h>
#include <wx/xrc/xmlres.h>
#include "utf8.hpp"
#include "cache.hpp"
#include "frame.hpp"
#include "graphics_editor.hpp"
#include "app.hpp"

/***************************************
 * OnInit() and OnExit()
 ***************************************/

bool SettingseditorApp::OnInit()
{
  std::locale::global(std::locale("")); // Initialise to current environment locale

  wxImage::AddHandler(new wxPNGHandler);
  wxXmlResource::Get()->InitAllHandlers();

  Pathie::Path xrcfile = Pathie::Path::exe().dirname().parent() / "share" / "tsc-settingseditor" / "mainwindow.xrc";
  wxXmlResource::Get()->Load(wxString::FromUTF8(xrcfile.str().c_str()));

  mp_mainwindow = wxXmlResource::Get()->LoadFrame(NULL, "mainwindow");
  mp_cache_info = new CacheInfo(Pathie::Path::cache_dir() / "tsc-scripteditor" / "data.ini");
  m_last_selected_frame = -1;

  setup_graphicseditor();
  setup_event_handlers();

  // Disable the notebook before we have at least one frame
  XRCCTRL(*mp_mainwindow, "notebook", wxNotebook)->Disable();

  // Set minimum size of the arrow buttons
  XRCCTRL(*mp_mainwindow, "up_frame_button", wxButton)->SetMinClientSize(wxSize(30, -1));
  XRCCTRL(*mp_mainwindow, "down_frame_button", wxButton)->SetMinClientSize(wxSize(30, -1));

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
  mp_mainwindow->Bind(wxEVT_COMMAND_MENU_SELECTED, &SettingseditorApp::on_menu_file_save, this, wxID_SAVE);
  mp_mainwindow->Bind(wxEVT_COMMAND_MENU_SELECTED, &SettingseditorApp::on_menu_help_about, this, wxID_ABOUT);
  mp_mainwindow->Bind(wxEVT_COMMAND_MENU_SELECTED, &SettingseditorApp::on_menu_edit_apply_to_all, this, XRCID("menu_edit_apply_to_all"));

  // Frame handling
  XRCCTRL(*mp_mainwindow, "add_frame_button", wxButton)->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SettingseditorApp::on_add_frame_button_clicked, this, XRCID("add_frame_button"));
  XRCCTRL(*mp_mainwindow, "del_frame_button", wxButton)->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SettingseditorApp::on_del_frame_button_clicked, this, XRCID("del_frame_button"));
  XRCCTRL(*mp_mainwindow, "up_frame_button", wxButton)->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SettingseditorApp::on_up_frame_button_clicked, this, XRCID("up_frame_button"));
  XRCCTRL(*mp_mainwindow, "down_frame_button", wxButton)->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SettingseditorApp::on_down_frame_button_clicked, this, XRCID("down_frame_button"));

  // List
  XRCCTRL(*mp_mainwindow, "frame_listbox", wxListBox)->Bind(wxEVT_LISTBOX, &SettingseditorApp::on_frame_list_item_selected, this, wxID_ANY);

  // Collision rectangle spin buttons
  XRCCTRL(*mp_mainwindow, "col_x_spin", wxSpinCtrl)->Bind(wxEVT_SPINCTRL, &SettingseditorApp::on_colrect_spin_changed, this, wxID_ANY);
  XRCCTRL(*mp_mainwindow, "col_y_spin", wxSpinCtrl)->Bind(wxEVT_SPINCTRL, &SettingseditorApp::on_colrect_spin_changed, this, wxID_ANY);
  XRCCTRL(*mp_mainwindow, "col_w_spin", wxSpinCtrl)->Bind(wxEVT_SPINCTRL, &SettingseditorApp::on_colrect_spin_changed, this, wxID_ANY);
  XRCCTRL(*mp_mainwindow, "col_h_spin", wxSpinCtrl)->Bind(wxEVT_SPINCTRL, &SettingseditorApp::on_colrect_spin_changed, this, wxID_ANY);

  // Bounding borders rectangle spin button
  XRCCTRL(*mp_mainwindow, "width_spin", wxSpinCtrl)->Bind(wxEVT_SPINCTRL, &SettingseditorApp::on_borders_spin_changed, this, wxID_ANY);
  XRCCTRL(*mp_mainwindow, "height_spin", wxSpinCtrl)->Bind(wxEVT_SPINCTRL, &SettingseditorApp::on_borders_spin_changed, this, wxID_ANY);
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

void SettingseditorApp::on_menu_file_save(wxCommandEvent& evt)
{
  std::vector<Frame*>::iterator iter;
  for(iter=m_frames.begin(); iter != m_frames.end(); iter++) {
    Frame* p_frame = *iter;
    Pathie::Path target_path = p_frame->get_png_path().sub_ext(".settings");

    p_frame->get_settings().save(target_path);
  }
}

void SettingseditorApp::on_menu_edit_apply_to_all(wxCommandEvent& evt)
{
  wxListBox* p_listbox = XRCCTRL(*mp_mainwindow, "frame_listbox", wxListBox);
  int index = p_listbox->GetSelection();

  if (index == wxNOT_FOUND) // Nothing selected
    return;

  Frame::TscSettings template_settings(m_frames[index]->get_settings()); // Copy
  std::vector<Frame*>::iterator iter;
  for(iter=m_frames.begin(); iter != m_frames.end(); iter++) {
    Frame::TscSettings& settings = (*iter)->get_settings();

    settings.set_width(template_settings.get_width());
    settings.set_height(template_settings.get_height());
    settings.set_col_x(template_settings.get_col_x());
    settings.set_col_y(template_settings.get_col_y());
    settings.set_col_width(template_settings.get_col_width());
    settings.set_col_height(template_settings.get_col_height());
  }

  mp_graphicseditor->Refresh();
}

void SettingseditorApp::on_menu_help_about(wxCommandEvent& evt)
{
  wxAboutDialogInfo info;
  info.SetName("TSC Settingseditor");
  //info.setVersion
  info.SetDescription("A GUI for editing the Secretchronicles settings files.");
  info.SetCopyright("Copyright © 2015 Marvin Gülker");
  info.SetWebSite("http://www.secretchronicles.de");
  info.AddDeveloper("Marvin Gülker (Quintus) <quintus@quintilianus.eu>");
  info.SetLicense("\
A specialised TSC settings files editor.\n\
Copyright © 2015 Marvin Gülker\n\
\n\
This program is free software: you can redistribute it and/or modify\n\
it under the terms of the GNU General Public License as published by\n\
the Free Software Foundation, either version 3 of the License, or\n\
(at your option) any later version.\n\
\n\
This program is distributed in the hope that it will be useful,\n\
but WITHOUT ANY WARRANTY; without even the implied warranty of\n\
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n\
GNU General Public License for more details.\n\
\n\
You should have received a copy of the GNU General Public License\n\
along with this program.  If not, see <http://www.gnu.org/licenses/>.\n\
\n\
------------------ Other licenses --------------------\n\
\n\
This program includes source code from different authors. The following\n\
sections list these libraries.\n\
\n\
--- inih ---\n\
\n\
The \"inih\" library is distributed under the New BSD license:\n\
Copyright (c) 2009, Brush Technology\n\
All rights reserved.\n\
\n\
Redistribution and use in source and binary forms, with or without\n\
modification, are permitted provided that the following conditions are met:\n\
    * Redistributions of source code must retain the above copyright\n\
      notice, this list of conditions and the following disclaimer.\n\
    * Redistributions in binary form must reproduce the above copyright\n\
      notice, this list of conditions and the following disclaimer in the\n\
      documentation and/or other materials provided with the distribution.\n\
    * Neither the name of Brush Technology nor the names of its contributors\n\
      may be used to endorse or promote products derived from this software\n\
      without specific prior written permission.\n\
\n\
THIS SOFTWARE IS PROVIDED BY BRUSH TECHNOLOGY ''AS IS'' AND ANY\n\
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED\n\
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE\n\
DISCLAIMED. IN NO EVENT SHALL BRUSH TECHNOLOGY BE LIABLE FOR ANY\n\
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES\n\
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;\n\
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND\n\
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT\n\
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS\n\
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n\
\n\
--- Pathie ---\n\
Pathie is a path management library.\n\
\n\
Copyright © 2015 Marvin Gülker\n\
\n\
This program is free software: you can redistribute it and/or modify\n\
it under the terms of the GNU General Public License as published by\n\
the Free Software Foundation, either version 3 of the License, or\n\
(at your option) any later version.\n\
\n\
This program is distributed in the hope that it will be useful,\n\
but WITHOUT ANY WARRANTY; without even the implied warranty of\n\
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n\
GNU General Public License for more details.\n\
\n\
You should have received a copy of the GNU General Public License\n\
along with this program.  If not, see <http://www.gnu.org/licenses/>.");

  wxAboutBox(info);
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
  wxListBox* p_listbox = XRCCTRL(*mp_mainwindow, "frame_listbox", wxListBox);
  int index = p_listbox->GetSelection();

  if (index == wxNOT_FOUND) // no selection
    return;

  p_listbox->Deselect(index);
  p_listbox->Delete(index);
  m_frames.erase(m_frames.begin() + index);

  // Unshow any image and disable the notebook as now nothing is selected
  mp_graphicseditor->set_frame(NULL);
  XRCCTRL(*mp_mainwindow, "notebook", wxNotebook)->Disable();
}

void SettingseditorApp::on_up_frame_button_clicked(wxCommandEvent& evt)
{
  wxListBox* p_listbox = XRCCTRL(*mp_mainwindow, "frame_listbox", wxListBox);
  int index = p_listbox->GetSelection();

  if (index == wxNOT_FOUND) // no selection
    return;
  else if (index <= 0) // already at the beginning
    return;

  wxString itemstr = p_listbox->GetString(index);
  Frame* p_frame = m_frames[index];

  p_listbox->Delete(index);
  m_frames.erase(m_frames.begin() + index);

  p_listbox->Insert(itemstr, index - 1);
  m_frames.insert(m_frames.begin() + (index - 1), p_frame);

  p_listbox->SetSelection(index - 1); // Emits no selection event
  m_last_selected_frame = index - 1;
}

void SettingseditorApp::on_down_frame_button_clicked(wxCommandEvent& evt)
{
  wxListBox* p_listbox = XRCCTRL(*mp_mainwindow, "frame_listbox", wxListBox);
  int index = p_listbox->GetSelection();

  if (index == wxNOT_FOUND) // no selection
    return;
  else if (static_cast<unsigned int>(index) >= p_listbox->GetCount() - 1) // already at the end; index must be positive at this point
    return;

  wxString itemstr = p_listbox->GetString(index);
  Frame* p_frame = m_frames[index];

  p_listbox->Delete(index);
  m_frames.erase(m_frames.begin() + index);

  p_listbox->Insert(itemstr, index + 1);
  m_frames.insert(m_frames.begin() + (index + 1), p_frame);

  p_listbox->SetSelection(index + 1); // Emits no selection event
  m_last_selected_frame = index + 1;
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

  // Enable the notebook if it wasn't enabled yet (no selection yet)
  XRCCTRL(*mp_mainwindow, "notebook", wxNotebook)->Enable();

  // Display newly selected image
  mp_graphicseditor->set_frame(p_frame);

  // Set last frame for saving next time
  m_last_selected_frame = evt.GetInt();
}

void SettingseditorApp::on_colrect_spin_changed(wxSpinEvent& evt)
{
  wxSpinCtrl* p_col_x_spin = XRCCTRL(*mp_mainwindow, "col_x_spin", wxSpinCtrl);
  wxSpinCtrl* p_col_y_spin = XRCCTRL(*mp_mainwindow, "col_y_spin", wxSpinCtrl);
  wxSpinCtrl* p_col_w_spin = XRCCTRL(*mp_mainwindow, "col_w_spin", wxSpinCtrl);
  wxSpinCtrl* p_col_h_spin = XRCCTRL(*mp_mainwindow, "col_h_spin", wxSpinCtrl);

  int x = p_col_x_spin->GetValue();
  int y = p_col_y_spin->GetValue();
  int w = p_col_w_spin->GetValue();
  int h = p_col_h_spin->GetValue();

  // Adjust all the values. This might cause some redundant logic to run,
  // but that’s better than duplicating all the code here.
  mp_graphicseditor->update_collision_rect(x, y, w, h);
  mp_graphicseditor->Refresh();
}

void SettingseditorApp::on_borders_spin_changed(wxSpinEvent& evt)
{
  wxSpinCtrl* p_width_spin  = XRCCTRL(*mp_mainwindow, "width_spin", wxSpinCtrl);
  wxSpinCtrl* p_height_spin = XRCCTRL(*mp_mainwindow, "height_spin", wxSpinCtrl);
  Frame::TscSettings& settings = m_frames[XRCCTRL(*mp_mainwindow, "frame_listbox", wxListBox)->GetSelection()]->get_settings();

  settings.set_width(p_width_spin->GetValue());
  settings.set_height(p_height_spin->GetValue());

  mp_graphicseditor->update_collision_rect(settings.get_col_x(), settings.get_col_y(), settings.get_col_width(), settings.get_col_height());
  mp_graphicseditor->Refresh();
}
