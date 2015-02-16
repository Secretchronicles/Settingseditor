#ifndef TSC_SETTINGSEDITOR_UTF8_HPP
#define TSC_SETTINGSEDITOR_UTF8_HPP
#include <string>
#include <wx/wx.h>

wxString utf8_to_wxstr(const std::string& utf8);
std::string wxstr_to_utf8(const wxString& wxstr);
#endif
