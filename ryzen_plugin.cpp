#include "ryzen_plugin.h"
#include "ui/ryzen_page.h"
#include <iostream>
#include "backend/ryzen_adj_wrapper.h"

bool RyzenPlugin::init() {
    RyzenAdjWrapper::init();
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
