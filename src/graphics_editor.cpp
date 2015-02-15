#include <wx/graphics.h>
#include "graphics_editor.hpp"

GraphicsEditor::GraphicsEditor(wxWindow* p_parent,
		 wxWindowID winid,
		 const wxPoint& pos,
		 const wxSize& size,
		 long style,
		 const wxString& name)
  : wxPanel(p_parent, winid, pos, size, style, name)
{
  Bind(wxEVT_PAINT, &GraphicsEditor::on_paint, this, winid);
  //Bind(wxEVT_SIZE, &GraphicsEditor::on_resize, this, winid);
}

void GraphicsEditor::on_paint(wxPaintEvent& evt)
{
  wxPaintDC dc(this);
  wxGraphicsContext* p_gc = wxGraphicsContext::Create(dc);

  if (!p_gc)
    return;

  p_gc->SetPen(*wxBLACK_PEN);
  p_gc->StrokeLine(0, 0, 200, 200);

  delete p_gc;
}

void GraphicsEditor::on_resize(wxSizeEvent& evt)
{

}
