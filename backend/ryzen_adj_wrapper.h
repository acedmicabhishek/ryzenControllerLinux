#pragma once
#include <string>

class RyzenAdjWrapper {
public:
    static bool is_supported();
    static void init();
    static bool apply_limits(int stapm, int fast, int slow, int stapm_time, int slow_time, int tctl_temp, int co_all);
};
