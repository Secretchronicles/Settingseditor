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
