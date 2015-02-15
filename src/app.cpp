#include <stdexcept>
#include <unistd.h>
#include <wx/xrc/xmlres.h>
#include "app.hpp"

bool SettingseditorApp::OnInit()
{
  wxXmlResource::Get()->InitAllHandlers();
  wxXmlResource::Get()->Load("../ui/mainwindow.xrc");

  mp_mainwindow = wxXmlResource::Get()->LoadFrame(NULL, "mainwindow");

  setup_event_handlers();

  mp_mainwindow->Show(true);
  return true;
}

void SettingseditorApp::setup_event_handlers()
{
  mp_mainwindow->Bind(wxEVT_COMMAND_MENU_SELECTED, &SettingseditorApp::on_menu_file_quit, this, wxID_EXIT);
}

void SettingseditorApp::on_menu_file_quit(wxCommandEvent& evt)
{
  mp_mainwindow->Close();
}
