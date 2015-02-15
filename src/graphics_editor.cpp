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

  // Draw a chessboard pattern over the entire area as the background
  wxSize dimensions = GetClientSize();
  wxGraphicsPath grey_path = p_gc->CreatePath();
  wxGraphicsPath white_path = p_gc->CreatePath();
  int x = 0;
  int y = 0;
  bool white = true;
  for(x = 0; x < dimensions.GetWidth(); x += 16) {
    for(y = 0; y < dimensions.GetHeight(); y += 16) {
      if (white)
	white_path.AddRectangle(x, y, 16, 16);
      else
	grey_path.AddRectangle(x, y, 16, 16);

      white = !white;
    }
  }

  p_gc->SetBrush(*wxWHITE_BRUSH);
  p_gc->FillPath(white_path);
  p_gc->SetBrush(*wxGREY_BRUSH);
  p_gc->FillPath(grey_path);

  p_gc->SetPen(*wxBLACK_PEN);
  p_gc->StrokeLine(0, 0, 200, 200);

  delete p_gc;
}

void GraphicsEditor::on_resize(wxSizeEvent& evt)
{

}
