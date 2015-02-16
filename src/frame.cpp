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

#include <cstdlib>
#include <cstring>
#include "frame.hpp"

Frame::Frame(Pathie::Path png_path)
  : m_png_path(png_path), m_settings(png_path.sub_ext(".settings"))
{
  //
}

Frame::TscSettings::TscSettings(const Pathie::Path& path)
{
  if (path.exists()) {
    parse_settingsfile(path);
  }
  else { // No settings file yet, provide some default settings.
    m_width = 64;
    m_height = 64;
    m_col_x = 0;
    m_col_y = 0;
    m_col_w = 64;
    m_col_h = 64;
    m_name = "object";
    m_license = "CC-BY_4.0";
    m_author = "nobody";
  }
}

void Frame::TscSettings::parse_settingsfile(const Pathie::Path& path)
{
  Pathie::ifstream file(path);

  while (!file.eof()) {
    char buf[256];
    memset(buf, '\0', 256);

    file.getline(buf, 256);
    std::string line(buf);

    // The single = in the below if/else if statements are intended.
    size_t pos = 0;
    if ((pos = line.find("width ")) != std::string::npos) {
      m_width = atoi(line.substr(pos + 6).c_str());
    }
    else if ((pos = line.find("height ")) != std::string::npos) {
      m_height = atoi(line.substr(pos + 7).c_str());
    }
    else if ((pos = line.find("name ")) != std::string::npos) {
      m_name = line.substr(pos + 5);
    }
    else if ((pos = line.find("author ")) != std::string::npos) {
      m_author = line.substr(pos + 7);
    }
    else if ((pos = line.find("license ")) != std::string::npos) {
      m_license = line.substr(pos + 8);
    }
    else if ((pos = line.find("col_rect ")) != std::string::npos) {
      size_t pos2 = 0;
      pos += 9;

      pos2 = line.find(" ", pos);
      m_col_x = atoi(line.substr(pos, pos2 - pos).c_str());

      pos = pos2 + 1;
      pos2 = line.find(" ", pos);
      m_col_y = atoi(line.substr(pos, pos2 - pos).c_str());

      pos = pos2 + 1;
      pos2 = line.find(" ", pos);
      m_col_w = atoi(line.substr(pos, pos2 - pos).c_str());

      pos = pos2 + 1;
      m_col_h = atoi(line.substr(pos).c_str());
    }
    else {
      // Store all other lines so we operate transparently on existing files
      // and do not delete statements we do not understand.
      m_other += line;
    }
  }

  file.close();
}

void Frame::TscSettings::save(const Pathie::Path& path)
{
  Pathie::ofstream file(path);

  file << "width "  << m_width << std::endl
       << "height " << m_height << std::endl
       << "col_rect " << m_col_x << " " << m_col_y << " " << m_col_w << " " << m_col_h << std::endl
       << "author " << m_author << std::endl
       << "name " << m_name << std::endl
       << "license " << m_license << std::endl
       << m_other;

  file.close();
}
