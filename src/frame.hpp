#ifndef TSC_SETTINGSEDITOR_FRAME_HPP
#define TSC_SETTINGSEDITOR_FRAME_HPP
#include <pathie.hpp>

class Frame
{
public:
  class TscSettings
  {
  public:
    TscSettings(const Pathie::Path& path);
    TscSettings(std::string settings);

    inline int get_width(){return m_width;}
    inline int get_height(){return m_height;}
    inline int get_col_x(){return m_col_x;}
    inline int get_col_y(){return m_col_y;}
    inline int get_col_width(){return m_col_w;}
    inline int get_col_height(){return m_col_h;}
    inline std::string get_name(){return m_name;}
    inline std::string get_author(){return m_author;}
    inline std::string get_license(){return m_license;}
    inline std::string get_other(){return m_other;}

    inline void set_width(int val){m_width = val;}
    inline void set_height(int val){m_height = val;}
    inline void set_col_x(int val){m_col_x = val;}
    inline void set_col_y(int val){m_col_y = val;}
    inline void set_col_width(int val){m_col_w = val;}
    inline void set_col_height(int val){m_col_h = val;}
    inline void set_name(std::string val){m_name = val;}
    inline void set_author(std::string val){m_author = val;}
    inline void set_license(std::string val){m_license = val;}
    inline void set_other(std::string val){m_other = val;}

    void save(const Pathie::Path& path);
  private:
    void parse_settingsfile(const Pathie::Path& path);

    int m_width;
    int m_height;
    int m_col_x;
    int m_col_y;
    int m_col_w;
    int m_col_h;
    std::string m_name;
    std::string m_author;
    std::string m_license;
    std::string m_other;
  };

  Frame(Pathie::Path png_path);

  inline TscSettings& get_settings(){return m_settings;}
private:
  Pathie::Path m_png_path;
  TscSettings m_settings;
};

#endif
