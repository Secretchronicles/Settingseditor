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

#include <cstring>
#include "ini.h"
#include "cache.hpp"

static int ini_parse_handler(void* ptr, const char* section, const char* name, const char* value);

CacheInfo::CacheInfo(Pathie::Path cachefile)
{
  m_cachefile = cachefile;
  m_last_dir = Pathie::Path::pwd();

  // Create the cache directory if it doesn’t exist yet
  m_cachefile.dirname().mktree();

  parse_file();
}

/**
 * Destructor writes out the cache values to the cache file.
 */
CacheInfo::~CacheInfo()
{
  Pathie::ofstream file(m_cachefile);

  file << "[Cache]" << std::endl
       << std::endl
       << "last_dir = " << m_last_dir.str() << std::endl;

  file.close();
}

void CacheInfo::parse_file()
{
  if (!m_cachefile.exists())
    m_cachefile.touch();

  // Hand to inih. Note that only by handing the FILE* ourselves
  // we can ensure it works with a unicode filename (username!)
  // on Windows.
  FILE* p_file = m_cachefile.fopen("r");
  ini_parse_file(p_file, ini_parse_handler, this);
  fclose(p_file);
}

int ini_parse_handler(void* ptr, const char* section, const char* name, const char* value)
{
  CacheInfo* p_cache = static_cast<CacheInfo*>(ptr);

  if (strcmp(name, "last_dir") == 0)
    p_cache->set_last_dir(Pathie::Path(value));

  return 1; // Success
}
