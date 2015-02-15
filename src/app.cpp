#include <stdexcept>
#include <locale>
#include <unistd.h>
#include <wx/statline.h>
#include <wx/xrc/xmlres.h>
#include "cache.hpp"
#include "graphics_editor.hpp"
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

  setup_graphicseditor();
  setup_event_handlers();

  mp_mainwindow->Show(true);
  return true;
}

int SettingseditorApp::OnExit()
{
  delete mp_graphicseditor;
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
