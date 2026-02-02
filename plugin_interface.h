#pragma once

#include <gtk/gtk.h>
#include <string>

class KdPlugin {
public:
    virtual ~KdPlugin() = default;

    virtual std::string get_name() const = 0;
    virtual std::string get_slug() const = 0;
    virtual bool init() = 0;
    virtual GtkWidget* create_config_widget() = 0;
};

typedef KdPlugin* (*CreatePluginFunc)();
