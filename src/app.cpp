#include <stdexcept>
#include <unistd.h>
#include <wx/xrc/xmlres.h>
#include "app.hpp"

bool ScripteditorApp::OnInit()
{
  wxXmlResource::Get()->InitAllHandlers();
  wxXmlResource::Get()->Load("../ui/mainwindow.xrc");

  mp_mainwindow = wxXmlResource::Get()->LoadFrame(NULL, "mainwindow");

  setup_menu_bar();

  mp_mainwindow->Show(true);
  return true;
}

void ScripteditorApp::setup_menu_bar()
{
  mp_mainwindow->Bind(wxEVT_COMMAND_MENU_SELECTED, &ScripteditorApp::On_Menu_File_Quit, this, wxID_EXIT);
}

void ScripteditorApp::On_Menu_File_Quit(wxCommandEvent& evt)
{
  mp_mainwindow->Close();
}
