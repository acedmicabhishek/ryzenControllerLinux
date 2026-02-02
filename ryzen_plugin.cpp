#include "ryzen_plugin.h"
#include "ui/ryzen_page.h"
#include <iostream>

bool RyzenPlugin::init() {
    std::cout << "[RyzenPlugin] Initialized." << std::endl;
    return true;
}

RyzenPlugin::~RyzenPlugin() {
    std::cout << "[RyzenPlugin] Destroyed." << std::endl;
}

GtkWidget* RyzenPlugin::create_config_widget() {
    return RyzenPage::create();
}

extern "C" {
    KdPlugin* create_plugin() {
        return new RyzenPlugin();
    }
}
