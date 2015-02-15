#include <stdexcept>
#include <locale>
#include <unistd.h>
#include <wx/xrc/xmlres.h>
#include "app.hpp"

/***************************************
 * OnInit()
 ***************************************/

bool SettingseditorApp::OnInit()
{
  std::locale::global(std::locale("")); // Initialise to current environment locale

  wxXmlResource::Get()->InitAllHandlers();
  wxXmlResource::Get()->Load("../ui/mainwindow.xrc");

  mp_mainwindow = wxXmlResource::Get()->LoadFrame(NULL, "mainwindow");

  setup_event_handlers();

  mp_mainwindow->Show(true);
  return true;
}

/***************************************
 * Helper functions
 ***************************************/

void SettingseditorApp::setup_event_handlers()
{
  // Menus
  mp_mainwindow->Bind(wxEVT_COMMAND_MENU_SELECTED, &SettingseditorApp::on_menu_file_quit, this, wxID_EXIT);

  XRCCTRL(*mp_mainwindow, "add_frame_button", wxButton)->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SettingseditorApp::on_add_frame_button_clicked, this, XRCID("add_frame_button"));
  XRCCTRL(*mp_mainwindow, "del_frame_button", wxButton)->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SettingseditorApp::on_del_frame_button_clicked, this, XRCID("del_frame_button"));
}

wxString SettingseditorApp::utf8_to_wxstr(const std::string& utf8)
{
  return wxString::FromUTF8(utf8.c_str()); // FromUTF8() copies its argument, so no memory problem.
}

std::string SettingseditorApp::wxstr_to_utf8(const wxString& wxstr)
{
  return std::string(wxstr.ToUTF8()); // Must be copied as the result of ToUTF8() is unstable and may vanish
}

void SettingseditorApp::add_frame(Pathie::Path path)
{
  m_frames.push_back(path);
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
		  "",
		  "",
		  "PNG files (*.png)|*.png|Any file (*.*)|*.*");

  if (fd.ShowModal() == wxID_CANCEL)
    return;

  add_frame(Pathie::Path(wxstr_to_utf8(fd.GetPath())));
}

void SettingseditorApp::on_del_frame_button_clicked(wxCommandEvent& evt)
{
  std::cout << "TODO" << std::endl;
}
