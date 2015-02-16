#include "utf8.hpp"

wxString utf8_to_wxstr(const std::string& utf8)
{
  return wxString::FromUTF8(utf8.c_str()); // FromUTF8() copies its argument, so no memory problem.
}

std::string wxstr_to_utf8(const wxString& wxstr)
{
  return std::string(wxstr.ToUTF8()); // Must be copied as the result of ToUTF8() is unstable and may vanish
}
