#ifndef TSC_SCRIPTEDITOR_APP_HPP
#define TSC_SCRIPTEDITOR_APP_HPP
#include <wx/wx.h>

class ScripteditorApp: public wxApp
{
  virtual bool OnInit();
private:
  void setup_menu_bar();

  void On_Menu_File_Quit(wxCommandEvent& evt);

  wxFrame* mp_mainwindow;
};
#endif
