#pragma once
#include <string>
#include <glib.h>

class RyzenConfig {
public:
    static void init();
    static void save();

    static void set_int(const std::string& key, int val);
    static int get_int(const std::string& key, int def);

private:
    static GKeyFile* key_file;
    static std::string config_path;
    static void ensure_dir(const std::string& path);
};
