/* This file is part of the TSC Settingseditor.
 * Copyright © 2015 Marvin Gülker
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
