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
