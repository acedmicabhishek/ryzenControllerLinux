#include "ryzen_adj_wrapper.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <memory>
#include <array>

#ifndef RYZEN_HELPER_PATH
#define RYZEN_HELPER_PATH "/usr/local/libexec/kerneldrive/ryzen_helper"
#endif

#include <filesystem>
#include <unistd.h>
#include <limits.h>

namespace fs = std::filesystem;

namespace {
    std::string get_executable_path() {
        char result[PATH_MAX];
        ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
        if (count != -1) {
            return std::string(result, count);
        }
        return "";
    }

    std::string get_helper_path() {
        if (fs::exists(RYZEN_HELPER_PATH)) {
            return RYZEN_HELPER_PATH;
        }
        std::string exe_path = get_executable_path();
        if (!exe_path.empty()) {
             fs::path build_dir = fs::path(exe_path).parent_path();
             fs::path local_helper = build_dir / "plugins/ryzen_controller/ryzen_helper";
             if (fs::exists(local_helper)) {
                 return local_helper.string();
             }
        }

        return RYZEN_HELPER_PATH;
    }

    std::string exec(const char* cmd) {
        std::array<char, 128> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
        if (!pipe) {
            return "";
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    }
}

bool RyzenAdjWrapper::is_supported() {

    return true; 
}

bool RyzenAdjWrapper::apply_limits(int stapm, int fast, int slow, int stapm_time, int slow_time, int tctl_temp, int co_all) {
    std::stringstream ss;
    ss << "pkexec " << get_helper_path();
    
    if (stapm > 0) ss << " --stapm-limit=" << stapm * 1000;
    if (fast > 0) ss << " --fast-limit=" << fast * 1000;
    if (slow > 0) ss << " --slow-limit=" << slow * 1000;
    if (stapm_time > 0) ss << " --stapm-time=" << stapm_time;
    if (slow_time > 0) ss << " --slow-time=" << slow_time;
    if (tctl_temp > 0) ss << " --tctl-temp=" << tctl_temp;
    if (co_all != 0) ss << " --set-coall=" << co_all; 

    std::string cmd = ss.str();
    std::cout << "[RyzenAdj] Executing: " << cmd << std::endl;
    
    std::string output = exec(cmd.c_str());
    // std::cout << "[RyzenAdj] Output: " << output << std::endl;
    
    return true;
}
