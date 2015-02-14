#include <wx/xrc/xmlres.h>
#include "app.hpp"

bool ScripteditorApp::OnInit()
{
  wxXmlResource::Get()->InitAllHandlers();
  wxXmlResource::Get()->Load("../ui/mainwindow.xrc");

  mp_mainwindow = wxXmlResource::Get()->LoadFrame(NULL, "mainwindow");
  mp_mainwindow->Show(true);
  return true;
}
