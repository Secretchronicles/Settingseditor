#ifndef TSC_SCRIPTEDITOR_APP_HPP
#define TSC_SCRIPTEDITOR_APP_HPP
#include <wx/wx.h>

class SettingseditorApp: public wxApp
{
  virtual bool OnInit();
private:
  void setup_event_handlers();

  void on_menu_file_quit(wxCommandEvent& evt);

  wxFrame* mp_mainwindow;
};
#endif
