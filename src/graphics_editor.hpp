#ifndef TSC_SETTINGSEDITOR_GRAPHICS_EDITOR_HPP
#define TSC_SETTINGSEDITOR_GRAPHICS_EDITOR_HPP
#include <wx/wx.h>

class GraphicsEditor: public wxPanel
{
public:
  GraphicsEditor(wxWindow* p_parent,
		 wxWindowID winid,
		 const wxPoint& pos = wxDefaultPosition,
		 const wxSize& size = wxDefaultSize,
		 long style = wxNO_BORDER,
		 const wxString& name = "GraphicsEditor");

private:
  void on_paint(wxPaintEvent& evt);
  void on_resize(wxSizeEvent& evt);
};

#endif
