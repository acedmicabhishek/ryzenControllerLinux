#pragma once

#include "plugin_interface.h"
#include <gtk/gtk.h>
#include <adwaita.h>
#include <string>

class RyzenPlugin : public KdPlugin {
public:
    std::string get_name() const override { return "Ryzen Controller"; }
    std::string get_slug() const override { return "ryzen_controller"; }
    
    bool init() override;
    GtkWidget* create_config_widget() override;
    
    ~RyzenPlugin() override;
};

extern "C" {
    KdPlugin* create_plugin();
}
