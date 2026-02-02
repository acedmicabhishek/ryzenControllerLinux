#include "config.h"
#include <filesystem>
#include <iostream>

GKeyFile* RyzenConfig::key_file = nullptr;
std::string RyzenConfig::config_path = "";

void RyzenConfig::init() {
    if (key_file) return;

    key_file = g_key_file_new();
    const char* config_home = g_get_user_config_dir();
    std::string dir = std::string(config_home) + "/kerneldrive/ryzen_controller";
    
    ensure_dir(dir);
    
    config_path = dir + "/tuning.ini";

    GError* error = nullptr;
    if (!g_key_file_load_from_file(key_file, config_path.c_str(), G_KEY_FILE_NONE, &error)) {
        if (!g_error_matches(error, G_FILE_ERROR, G_FILE_ERROR_NOENT)) {
           // std::cerr << "[RyzenConfig] Load failed: " << error->message << std::endl;
        }
        g_error_free(error);
    }
}

void RyzenConfig::ensure_dir(const std::string& path) {
    if (!std::filesystem::exists(path)) {
        std::filesystem::create_directories(path);
    }
}

void RyzenConfig::save() {
    if (!key_file) return;
    
    GError* error = nullptr;
    if (!g_key_file_save_to_file(key_file, config_path.c_str(), &error)) {
        std::cerr << "[RyzenConfig] Save failed: " << error->message << std::endl;
        g_error_free(error);
    }
}

void RyzenConfig::set_int(const std::string& key, int val) {
    if (!key_file) init();
    g_key_file_set_integer(key_file, "Tuning", key.c_str(), val);
    save();
}

int RyzenConfig::get_int(const std::string& key, int def) {
    if (!key_file) init();
    GError* err = nullptr;
    int val = g_key_file_get_integer(key_file, "Tuning", key.c_str(), &err);
    if (err) {
        g_error_free(err);
        return def;
    }
    return val;
}
