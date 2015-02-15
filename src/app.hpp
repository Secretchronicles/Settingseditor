#ifndef TSC_SCRIPTEDITOR_APP_HPP
#define TSC_SCRIPTEDITOR_APP_HPP
#include <vector>
#include <wx/wx.h>
#include <wx/config.h>
#include <pathie.hpp>

class SettingseditorApp: public wxApp
{
  virtual bool OnInit();
  virtual int OnExit();
private:
  void setup_graphicseditor();
  void setup_event_handlers();

  void add_frame(Pathie::Path path);
  wxString utf8_to_wxstr(const std::string& utf8);
  std::string wxstr_to_utf8(const wxString& wxstr);

  void on_menu_file_quit(wxCommandEvent& evt);
  void on_add_frame_button_clicked(wxCommandEvent& evt);
  void on_del_frame_button_clicked(wxCommandEvent& evt);

  CacheInfo* mp_cache_info;
  wxFrame* mp_mainwindow;
  GraphicsEditor* mp_graphicseditor;
  std::vector<Pathie::Path> m_frames;
};
#endif
