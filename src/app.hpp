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

#ifndef TSC_SCRIPTEDITOR_APP_HPP
#define TSC_SCRIPTEDITOR_APP_HPP
#include <vector>
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <pathie.hpp>

class SettingseditorApp: public wxApp
{
public:
  inline wxFrame* get_mainwindow(){return mp_mainwindow;}

private:
  virtual bool OnInit();
  virtual int OnExit();

  void setup_graphicseditor();
  void setup_event_handlers();

  void add_frame(Pathie::Path path);

  void on_menu_file_quit(wxCommandEvent& evt);
  void on_add_frame_button_clicked(wxCommandEvent& evt);
  void on_del_frame_button_clicked(wxCommandEvent& evt);
  void on_frame_list_item_selected(wxCommandEvent& evt);
  void on_colrect_spin_changed(wxSpinEvent& evt);

  CacheInfo* mp_cache_info;
  wxFrame* mp_mainwindow;
  GraphicsEditor* mp_graphicseditor;
  std::vector<Frame*> m_frames;
  int m_last_selected_frame;
};
#endif
