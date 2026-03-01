#ifndef TABS_H
#define TABS_H

#include <gtk/gtk.h>

typedef struct {
    // CPU dynamic widgets
    GtkWidget *cpu_freq_labels[128];
    int num_cores;
    // Mem dynamic widgets
    GtkWidget *mem_used_label;
    GtkWidget *mem_free_label;
    GtkWidget *mem_bar;
    GtkWidget *swap_bar;
    // Sensor dynamic widgets
    GtkWidget *sensor_labels[64];
    int num_sensors;
} AppWidgets;

GtkWidget* create_overview_tab(AppWidgets *widgets);
GtkWidget* create_os_info_tab(AppWidgets *widgets);
GtkWidget* create_cpu_tab(AppWidgets *widgets);
GtkWidget* create_mem_tab(AppWidgets *widgets);
GtkWidget* create_mobo_tab(AppWidgets *widgets);
GtkWidget* create_gpu_tab(AppWidgets *widgets);
GtkWidget* create_sensors_tab(AppWidgets *widgets);
GtkWidget* create_storage_tab(AppWidgets *widgets);

gboolean update_gui_timer(gpointer user_data);

#endif // TABS_H
