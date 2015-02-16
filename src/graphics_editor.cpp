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

#include <wx/spinctrl.h>
#include <wx/xrc/xmlres.h>
#include <wx/graphics.h>
#include "frame.hpp"
#include "graphics_editor.hpp"
#include "cache.hpp"
#include "app.hpp"

GraphicsEditor::GraphicsEditor(wxWindow* p_parent,
		 wxWindowID winid,
		 const wxPoint& pos,
		 const wxSize& size,
		 long style,
		 const wxString& name)
  : wxPanel(p_parent, winid, pos, size, style, name)
{
  mp_frame = NULL;
  mp_mask_brush = new wxBrush(wxColour(100, 255, 255, 200));
  mp_border_brush = new wxBrush(wxColour(200, 200, 200, wxALPHA_OPAQUE));

  Bind(wxEVT_PAINT, &GraphicsEditor::on_paint, this, winid);
  Bind(wxEVT_SIZE, &GraphicsEditor::on_resize, this, winid);
  Bind(wxEVT_LEFT_DOWN, &GraphicsEditor::on_left_mouse_button_down, this, winid);
  Bind(wxEVT_LEFT_UP, &GraphicsEditor::on_left_mouse_button_up, this, winid);
  Bind(wxEVT_MOTION, &GraphicsEditor::on_mouse_motion, this, winid);
}

GraphicsEditor::~GraphicsEditor()
{
  delete mp_border_brush;
  delete mp_mask_brush;
}

void GraphicsEditor::set_frame(Frame* p_frame)
{
  mp_frame = p_frame;

  update_spin_widgets();

  Refresh();
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

  if (mp_frame) {
    Frame::TscSettings& settings = mp_frame->get_settings();
    int width = settings.get_width();
    int height = settings.get_height();
    wxImage scaled_image = mp_frame->get_image().Scale(width, height, wxIMAGE_QUALITY_HIGH);
    wxGraphicsBitmap bitmap = p_gc->CreateBitmapFromImage(scaled_image);
    // Center the image on the available drawing area
    m_scaled_w = scaled_image.GetWidth();
    m_scaled_h = scaled_image.GetHeight();
    m_scaled_x = (dimensions.GetWidth() - m_scaled_w) / 2;
    m_scaled_y = (dimensions.GetHeight() - m_scaled_h) / 2;

    // Draw the image
    p_gc->DrawBitmap(bitmap, m_scaled_x, m_scaled_y, width, height);

    /* Draw the collision rect mask. */
    wxGraphicsPath colmask = p_gc->CreatePath();
    colmask.AddRectangle(m_scaled_x + settings.get_col_x(),
			 m_scaled_y + settings.get_col_y(),
			 settings.get_col_width(),
			 settings.get_col_height());
    p_gc->SetBrush(*mp_mask_brush);
    p_gc->FillPath(colmask);

    // Cover the area outside the image borders. Overlapping pathes
    // seem to have a cut effect (I found that by accident).
    wxGraphicsPath bordermask = p_gc->CreatePath();
    bordermask.AddRectangle(0, 0, dimensions.GetWidth(), dimensions.GetHeight());
    bordermask.AddRectangle(m_scaled_x, m_scaled_y, m_scaled_w, m_scaled_h);
    p_gc->SetBrush(*mp_border_brush);
    p_gc->FillPath(bordermask);
  }

  delete p_gc;
}

void GraphicsEditor::on_resize(wxSizeEvent& evt)
{
  Refresh();
}

void GraphicsEditor::on_left_mouse_button_down(wxMouseEvent& evt)
{
  Frame::TscSettings& settings = mp_frame->get_settings();

  settings.set_col_x(evt.GetX() - m_scaled_x);
  settings.set_col_y(evt.GetY() - m_scaled_y);
  Refresh();
}

void GraphicsEditor::on_left_mouse_button_up(wxMouseEvent& evt)
{
  Frame::TscSettings& settings = mp_frame->get_settings();

  int width = evt.GetX() - m_scaled_x;
  int height = evt.GetY() - m_scaled_y;

  // If these values are negative, the user dragged from left to right or
  // from bottom to top.
  if (width < 0) {
    settings.set_col_x(settings.get_col_x() + width); // width is negative
    width *= -1; // Make it positive
  }
  if (height < 0) {
    settings.set_col_y(settings.get_col_y() + height); // height ist negative
    height *= -1; // Make it positive
  }

  settings.set_col_width(width);
  settings.set_col_height(height);

  update_spin_widgets();
  Refresh();
}

void GraphicsEditor::on_mouse_motion(wxMouseEvent& evt)
{
  if (evt.LeftIsDown()) {
    Frame::TscSettings& settings = mp_frame->get_settings();

    settings.set_col_width(evt.GetX() - m_scaled_x);
    settings.set_col_height(evt.GetY() - m_scaled_y);

    update_spin_widgets();
    Refresh();
  }
}

void GraphicsEditor::update_spin_widgets()
{
  SettingseditorApp* p_app = static_cast<SettingseditorApp*>(wxTheApp);
  Frame::TscSettings& settings = mp_frame->get_settings();

  XRCCTRL(*p_app->get_mainwindow(), "col_x_spin", wxSpinCtrl)->SetValue(settings.get_col_x());
  XRCCTRL(*p_app->get_mainwindow(), "col_y_spin", wxSpinCtrl)->SetValue(settings.get_col_y());
  XRCCTRL(*p_app->get_mainwindow(), "col_w_spin", wxSpinCtrl)->SetValue(settings.get_col_width());
  XRCCTRL(*p_app->get_mainwindow(), "col_h_spin", wxSpinCtrl)->SetValue(settings.get_col_height());
}
