#ifndef TSC_SCRIPTEDITOR_APP_HPP
#define TSC_SCRIPTEDITOR_APP_HPP
#include <wx/wx.h>

class ScripteditorApp: public wxApp
{
  virtual bool OnInit();
private:
  wxFrame* mp_mainwindow;
};
#endif
