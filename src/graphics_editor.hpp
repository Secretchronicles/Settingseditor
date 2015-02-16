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
  virtual ~GraphicsEditor();

  void set_frame(Frame* p_frame);
  inline Frame* get_frame(){return mp_frame;}

private:
  void on_paint(wxPaintEvent& evt);
  void on_resize(wxSizeEvent& evt);

  wxBrush* mp_mask_brush;
  wxBrush* mp_border_brush;
  Frame* mp_frame;
  int m_scaled_x;
  int m_scaled_y;
  int m_scaled_w;
  int m_scaled_h;
};

#endif
