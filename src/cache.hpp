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

#ifndef TSC_SETTINGSEDITOR_CACHE_HPP
#define TSC_SETTINGSEDITOR_CACHE_HPP
#include <pathie.hpp>

class CacheInfo
{
public:
  CacheInfo(Pathie::Path cachefile);
  ~CacheInfo();

  inline Pathie::Path get_last_dir(){return m_last_dir;}
  inline void set_last_dir(Pathie::Path lastdir){m_last_dir = lastdir;}
private:
  void parse_file();

  Pathie::Path m_cachefile;
  Pathie::Path m_last_dir;
};

#endif
