#include "ryzen_page.h"
#include "../backend/ryzen_adj_wrapper.h"

#include "ryzen_page.h"
#include "../backend/ryzen_adj_wrapper.h"

struct RyzenData {
    GtkWidget* stapm_spin;
    GtkWidget* fast_spin;
    GtkWidget* slow_spin;
    GtkWidget* stapm_time_spin;
    GtkWidget* slow_time_spin;
    GtkWidget* temp_spin;
    GtkWidget* co_spin;
};

GtkWidget* RyzenPage::create() {
    GtkWidget* page = adw_preferences_page_new();
    
    GtkWidget* power_group = adw_preferences_group_new();
    adw_preferences_group_set_title(ADW_PREFERENCES_GROUP(power_group), "Power Limits (W)");
    adw_preferences_group_set_description(ADW_PREFERENCES_GROUP(power_group), "Set TDP targets.");
    
    RyzenData* data = new RyzenData();

    auto create_row = [&](const char* title, double min, double max, double step, double def, GtkWidget** out_spin) {
        GtkWidget* row = adw_action_row_new();
        adw_preferences_row_set_title(ADW_PREFERENCES_ROW(row), title);
        
        GtkWidget* spin = gtk_spin_button_new_with_range(min, max, step);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin), def);
        gtk_widget_set_valign(spin, GTK_ALIGN_CENTER);
        adw_action_row_add_suffix(ADW_ACTION_ROW(row), spin);
        adw_preferences_group_add(ADW_PREFERENCES_GROUP(power_group), row);
        *out_spin = spin;
    };

    create_row("STAPM Limit (Sustained)", 5, 100, 1, 35, &data->stapm_spin);
    create_row("Fast PPT Limit (Short Boost)", 5, 100, 1, 50, &data->fast_spin);
    create_row("Slow PPT Limit (Long Boost)", 5, 100, 1, 45, &data->slow_spin);
    
    adw_preferences_page_add(ADW_PREFERENCES_PAGE(page), ADW_PREFERENCES_GROUP(power_group));

    GtkWidget* time_group = adw_preferences_group_new();
    adw_preferences_group_set_title(ADW_PREFERENCES_GROUP(time_group), "Boost Duration (s)");
    
    auto create_time_row = [&](const char* title, double min, double max, double step, double def, GtkWidget** out_spin) {
        GtkWidget* row = adw_action_row_new();
        adw_preferences_row_set_title(ADW_PREFERENCES_ROW(row), title);
        GtkWidget* spin = gtk_spin_button_new_with_range(min, max, step);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin), def);
        gtk_widget_set_valign(spin, GTK_ALIGN_CENTER);
        adw_action_row_add_suffix(ADW_ACTION_ROW(row), spin);
        adw_preferences_group_add(ADW_PREFERENCES_GROUP(time_group), row);
        *out_spin = spin;
    };
    
    create_time_row("Slow PPT Time", 1, 128, 1, 5, &data->slow_time_spin);
    create_time_row("STAPM Time", 1, 1000, 10, 60, &data->stapm_time_spin);
    
    adw_preferences_page_add(ADW_PREFERENCES_PAGE(page), ADW_PREFERENCES_GROUP(time_group));

    GtkWidget* adv_group = adw_preferences_group_new();
    adw_preferences_group_set_title(ADW_PREFERENCES_GROUP(adv_group), "Advanced");
    
    GtkWidget* temp_row = adw_action_row_new();
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(temp_row), "Temp Limit (°C)");
    GtkWidget* temp_spin = gtk_spin_button_new_with_range(45, 100, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(temp_spin), 95);
    gtk_widget_set_valign(temp_spin, GTK_ALIGN_CENTER);
    adw_action_row_add_suffix(ADW_ACTION_ROW(temp_row), temp_spin);
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(adv_group), temp_row);
    data->temp_spin = temp_spin;

    GtkWidget* co_row = adw_action_row_new();
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(co_row), "UnderVolting");
    adw_action_row_set_subtitle(ADW_ACTION_ROW(co_row), "Negative Offset (All Core). Risk of instability!");
    GtkWidget* co_spin = gtk_spin_button_new_with_range(-30, 0, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(co_spin), 0);
    gtk_widget_set_valign(co_spin, GTK_ALIGN_CENTER);
    adw_action_row_add_suffix(ADW_ACTION_ROW(co_row), co_spin);
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(adv_group), co_row);
    data->co_spin = co_spin;
    
    adw_preferences_page_add(ADW_PREFERENCES_PAGE(page), ADW_PREFERENCES_GROUP(adv_group));

    GtkWidget* btn_group = adw_preferences_group_new();
    GtkWidget* apply_btn = gtk_button_new_with_label("Apply Settings");
    gtk_widget_add_css_class(apply_btn, "pill");
    gtk_widget_add_css_class(apply_btn, "suggested-action");
    gtk_widget_set_margin_top(apply_btn, 10);
    gtk_widget_set_margin_bottom(apply_btn, 10);
    gtk_widget_set_halign(apply_btn, GTK_ALIGN_CENTER);
    
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(btn_group), apply_btn);
    adw_preferences_page_add(ADW_PREFERENCES_PAGE(page), ADW_PREFERENCES_GROUP(btn_group));

    g_object_set_data_full(G_OBJECT(page), "ryzen_data", data, [](gpointer d){ delete (RyzenData*)d; });
    
    g_signal_connect(apply_btn, "clicked", G_CALLBACK(+[](GtkWidget*, gpointer user_data) {
        RyzenData* d = (RyzenData*)user_data;
        int stapm = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(d->stapm_spin));
        int fast = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(d->fast_spin));
        int slow = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(d->slow_spin));
        int slow_time = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(d->slow_time_spin));
        int stapm_time = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(d->stapm_time_spin));
        int temp = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(d->temp_spin));
        int co = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(d->co_spin));
        
        RyzenAdjWrapper::apply_limits(stapm, fast, slow, stapm_time, slow_time, temp, co);
      
    }), data);
    
    return page;
}
