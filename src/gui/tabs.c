#include "tabs.h"
#include "../core/cpu.h"
#include "../core/mem.h"
#include "../core/mobo.h"
#include "../core/gpu.h"
#include "../core/sensors.h"
#include "../core/storage.h"
#include <stdio.h>

static GtkWidget* create_row(const char* label_text, const char* value_text) {
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget *lbl_name = gtk_label_new(label_text);
    GtkWidget *lbl_val = gtk_label_new(value_text);
    
    gtk_widget_set_halign(lbl_name, GTK_ALIGN_START);
    gtk_widget_set_halign(lbl_val, GTK_ALIGN_END);
    
    PangoAttrList *attrs = pango_attr_list_new();
    pango_attr_list_insert(attrs, pango_attr_weight_new(PANGO_WEIGHT_BOLD));
    gtk_label_set_attributes(GTK_LABEL(lbl_name), attrs);
    pango_attr_list_unref(attrs);

    gtk_box_pack_start(GTK_BOX(hbox), lbl_name, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), lbl_val, FALSE, FALSE, 0);
    return hbox;
}

static GtkWidget* create_container() {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    gtk_container_set_border_width(GTK_CONTAINER(box), 20);
    return box;
}

static GtkWidget* create_header(const char* text) {
    GtkWidget *label = gtk_label_new(text);
    PangoAttrList *attrs = pango_attr_list_new();
    pango_attr_list_insert(attrs, pango_attr_scale_new(1.5));
    pango_attr_list_insert(attrs, pango_attr_weight_new(PANGO_WEIGHT_BOLD));
    gtk_label_set_attributes(GTK_LABEL(label), attrs);
    pango_attr_list_unref(attrs);
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    gtk_widget_set_margin_bottom(label, 10);
    return label;
}

GtkWidget* create_overview_tab(AppWidgets *widgets) {
    (void)widgets;
    GtkWidget *box = create_container();
    gtk_box_pack_start(GTK_BOX(box), create_header("System Overview"), FALSE, FALSE, 0);
    
    CpuInfo cpu;
    read_static_cpu_info(&cpu);
    gtk_box_pack_start(GTK_BOX(box), create_row("CPU Model", cpu.model_name), FALSE, FALSE, 0);
    
    MoboInfo mobo;
    read_mobo_info(&mobo);
    gtk_box_pack_start(GTK_BOX(box), create_row("Motherboard", mobo.name), FALSE, FALSE, 0);
    
    MemInfo mem;
    read_mem_info(&mem);
    char mem_str[64];
    sprintf(mem_str, "%.2f GB", mem.total_gb);
    gtk_box_pack_start(GTK_BOX(box), create_row("Total Memory", mem_str), FALSE, FALSE, 0);

    return box;
}

GtkWidget* create_cpu_tab(AppWidgets *widgets) {
    GtkWidget *box = create_container();
    gtk_box_pack_start(GTK_BOX(box), create_header("Processor (CPU)"), FALSE, FALSE, 0);

    CpuInfo cpu;
    read_static_cpu_info(&cpu);
    
    gtk_box_pack_start(GTK_BOX(box), create_row("Model Name", cpu.model_name), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), create_row("Vendor", cpu.vendor_id), FALSE, FALSE, 0);
    char cores_str[64];
    sprintf(cores_str, "%d Cores / %d Threads", cpu.physical_cores, cpu.logical_cores);
    gtk_box_pack_start(GTK_BOX(box), create_row("Cores", cores_str), FALSE, FALSE, 0);
    
    char cache_str[64];
    sprintf(cache_str, "%.0f KB", cpu.cache_size_kb);
    gtk_box_pack_start(GTK_BOX(box), create_row("L3 Cache", cache_str), FALSE, FALSE, 0);

    GtkWidget *sep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(box), sep, FALSE, FALSE, 10);
    
    gtk_box_pack_start(GTK_BOX(box), create_header("Live Frequencies"), FALSE, FALSE, 0);

    // Scrollable area for cores
    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_vexpand(scroll, TRUE);
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 15);
    gtk_container_add(GTK_CONTAINER(scroll), grid);
    gtk_box_pack_start(GTK_BOX(box), scroll, TRUE, TRUE, 0);

    widgets->num_cores = cpu.logical_cores;
    for (int i = 0; i < cpu.logical_cores && i < 128; i++) {
        char core_lbl[32];
        sprintf(core_lbl, "Core %d", i);
        GtkWidget *lbl = gtk_label_new(core_lbl);
        gtk_widget_set_halign(lbl, GTK_ALIGN_START);
        
        widgets->cpu_freq_labels[i] = gtk_label_new("0.00 MHz");
        gtk_widget_set_halign(widgets->cpu_freq_labels[i], GTK_ALIGN_END);
        
        gtk_grid_attach(GTK_GRID(grid), lbl, 0, i, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), widgets->cpu_freq_labels[i], 1, i, 1, 1);
    }
    
    return box;
}

GtkWidget* create_mem_tab(AppWidgets *widgets) {
    GtkWidget *box = create_container();
    gtk_box_pack_start(GTK_BOX(box), create_header("Memory (RAM)"), FALSE, FALSE, 0);

    MemInfo mem;
    read_mem_info(&mem);
    
    char total_str[64];
    sprintf(total_str, "%.2f GB", mem.total_gb);
    gtk_box_pack_start(GTK_BOX(box), create_row("Total RAM", total_str), FALSE, FALSE, 0);

    GtkWidget *hbox_used = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget *lbl_used_title = gtk_label_new("Used RAM: ");
    widgets->mem_used_label = gtk_label_new("0.00 GB");
    gtk_box_pack_start(GTK_BOX(hbox_used), lbl_used_title, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_used), widgets->mem_used_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), hbox_used, FALSE, FALSE, 0);

    widgets->mem_bar = gtk_progress_bar_new();
    gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(widgets->mem_bar), TRUE);
    gtk_box_pack_start(GTK_BOX(box), widgets->mem_bar, FALSE, FALSE, 5);

    GtkWidget *sep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(box), sep, FALSE, FALSE, 10);

    char swap_total_str[64];
    sprintf(swap_total_str, "%.2f GB", mem.swap_total_gb);
    gtk_box_pack_start(GTK_BOX(box), create_row("Total Swap", swap_total_str), FALSE, FALSE, 0);

    widgets->swap_bar = gtk_progress_bar_new();
    gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(widgets->swap_bar), TRUE);
    gtk_box_pack_start(GTK_BOX(box), widgets->swap_bar, FALSE, FALSE, 5);

    return box;
}

GtkWidget* create_mobo_tab(AppWidgets *widgets) {
    (void)widgets;
    GtkWidget *box = create_container();
    gtk_box_pack_start(GTK_BOX(box), create_header("Motherboard / BIOS"), FALSE, FALSE, 0);

    MoboInfo mobo;
    read_mobo_info(&mobo);

    gtk_box_pack_start(GTK_BOX(box), create_row("Vendor", mobo.vendor), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), create_row("Model Name", mobo.name), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), create_row("Version", mobo.version), FALSE, FALSE, 0);

    GtkWidget *sep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(box), sep, FALSE, FALSE, 10);
    
    gtk_box_pack_start(GTK_BOX(box), create_header("BIOS"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), create_row("BIOS Vendor", mobo.bios_vendor), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), create_row("BIOS Version", mobo.bios_version), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), create_row("BIOS Date", mobo.bios_date), FALSE, FALSE, 0);

    return box;
}

GtkWidget* create_gpu_tab(AppWidgets *widgets) {
    (void)widgets;
    GtkWidget *box = create_container();
    gtk_box_pack_start(GTK_BOX(box), create_header("Graphics Adapters"), FALSE, FALSE, 0);

    GpuInfo gpus[10];
    int count = 0;
    read_gpu_info(gpus, &count, 10);

    if (count == 0) {
        gtk_box_pack_start(GTK_BOX(box), gtk_label_new("No graphics adapters found."), FALSE, FALSE, 0);
    } else {
        // Scrollable list
        GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
        gtk_widget_set_vexpand(scroll, TRUE);
        GtkWidget *list = gtk_list_box_new();
        gtk_list_box_set_selection_mode(GTK_LIST_BOX(list), GTK_SELECTION_NONE);
        gtk_container_add(GTK_CONTAINER(scroll), list);
        gtk_box_pack_start(GTK_BOX(box), scroll, TRUE, TRUE, 0);

        for (int i = 0; i < count; i++) {
            GtkWidget *row = gtk_list_box_row_new();
            GtkWidget *lbl = gtk_label_new(gpus[i].name);
            gtk_widget_set_margin_top(lbl, 10);
            gtk_widget_set_margin_bottom(lbl, 10);
            gtk_widget_set_margin_start(lbl, 10);
            gtk_widget_set_halign(lbl, GTK_ALIGN_START);
            gtk_label_set_line_wrap(GTK_LABEL(lbl), TRUE);
            gtk_container_add(GTK_CONTAINER(row), lbl);
            gtk_list_box_insert(GTK_LIST_BOX(list), row, -1);
        }
    }

    return box;
}

GtkWidget* create_sensors_tab(AppWidgets *widgets) {
    GtkWidget *box = create_container();
    gtk_box_pack_start(GTK_BOX(box), create_header("Hardware Sensors"), FALSE, FALSE, 0);

    // Scrollable list
    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_vexpand(scroll, TRUE);
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 20);
    gtk_container_add(GTK_CONTAINER(scroll), grid);
    gtk_box_pack_start(GTK_BOX(box), scroll, TRUE, TRUE, 0);

    SensorInfo sensors[64];
    int count = 0;
    read_sensors_info(sensors, &count, 64);
    widgets->num_sensors = count;

    for (int i = 0; i < count; i++) {
        GtkWidget *lbl = gtk_label_new(sensors[i].label);
        gtk_widget_set_halign(lbl, GTK_ALIGN_START);
        
        widgets->sensor_labels[i] = gtk_label_new("0.0 °C");
        gtk_widget_set_halign(widgets->sensor_labels[i], GTK_ALIGN_END);
        
        PangoAttrList *attrs = pango_attr_list_new();
        pango_attr_list_insert(attrs, pango_attr_weight_new(PANGO_WEIGHT_BOLD));
        gtk_label_set_attributes(GTK_LABEL(lbl), attrs);
        pango_attr_list_unref(attrs);

        gtk_grid_attach(GTK_GRID(grid), lbl, 0, i, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), widgets->sensor_labels[i], 1, i, 1, 1);
    }
    
    if (count == 0) {
        gtk_grid_attach(GTK_GRID(grid), gtk_label_new("No sensors detected."), 0, 0, 1, 1);
    }

    return box;
}

GtkWidget* create_storage_tab(AppWidgets *widgets) {
    (void)widgets;
    GtkWidget *box = create_container();
    gtk_box_pack_start(GTK_BOX(box), create_header("Storage Devices"), FALSE, FALSE, 0);

    StorageInfo storages[10];
    int count = 0;
    read_storage_info(storages, &count);

    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_vexpand(scroll, TRUE);
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    gtk_container_add(GTK_CONTAINER(scroll), vbox);
    gtk_box_pack_start(GTK_BOX(box), scroll, TRUE, TRUE, 0);

    for (int i = 0; i < count; i++) {
        GtkWidget *sbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
        
        char title[512];
        sprintf(title, "Mount: %s (%.2f GB Total)", storages[i].mount_point, storages[i].total_gb);
        GtkWidget *lbl = gtk_label_new(title);
        gtk_widget_set_halign(lbl, GTK_ALIGN_START);
        PangoAttrList *attrs = pango_attr_list_new();
        pango_attr_list_insert(attrs, pango_attr_weight_new(PANGO_WEIGHT_BOLD));
        gtk_label_set_attributes(GTK_LABEL(lbl), attrs);
        pango_attr_list_unref(attrs);
        
        gtk_box_pack_start(GTK_BOX(sbox), lbl, FALSE, FALSE, 0);
        
        GtkWidget *bar = gtk_progress_bar_new();
        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(bar), storages[i].percentage_used / 100.0);
        char text[64];
        sprintf(text, "%.2f GB Used / %.2f GB Free", storages[i].used_gb, storages[i].free_gb);
        gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(bar), TRUE);
        gtk_progress_bar_set_text(GTK_PROGRESS_BAR(bar), text);
        
        gtk_box_pack_start(GTK_BOX(sbox), bar, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox), sbox, FALSE, FALSE, 0);
    }

    return box;
}

// Timer update callback
gboolean update_gui_timer(gpointer user_data) {
    AppWidgets *widgets = (AppWidgets*)user_data;

    // Update CPU
    if (widgets->num_cores > 0) {
        CpuInfo cpu;
        memset(&cpu, 0, sizeof(CpuInfo));
        cpu.logical_cores = widgets->num_cores;
        read_dynamic_cpu_info(&cpu);
        for (int i = 0; i < widgets->num_cores; i++) {
            char freq_str[32];
            sprintf(freq_str, "%.2f MHz", cpu.current_freq_mhz[i]);
            gtk_label_set_text(GTK_LABEL(widgets->cpu_freq_labels[i]), freq_str);
        }
    }

    // Update Memory
    if (widgets->mem_bar) {
        MemInfo mem;
        read_mem_info(&mem);
        
        char used_str[32];
        sprintf(used_str, "%.2f GB", mem.used_gb);
        gtk_label_set_text(GTK_LABEL(widgets->mem_used_label), used_str);

        if (mem.total_gb > 0) {
            double fraction = mem.used_gb / mem.total_gb;
            gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(widgets->mem_bar), fraction);
            char text[32];
            sprintf(text, "%.1f%% Used", fraction * 100.0);
            gtk_progress_bar_set_text(GTK_PROGRESS_BAR(widgets->mem_bar), text);
        }

        if (mem.swap_total_gb > 0) {
            double s_fraction = mem.swap_used_gb / mem.swap_total_gb;
            gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(widgets->swap_bar), s_fraction);
            char s_text[32];
            sprintf(s_text, "%.2f GB / %.2f GB", mem.swap_used_gb, mem.swap_total_gb);
            gtk_progress_bar_set_text(GTK_PROGRESS_BAR(widgets->swap_bar), s_text);
        }
    }

    // Update Sensors
    if (widgets->num_sensors > 0) {
        SensorInfo sensors[64];
        int count = 0;
        read_sensors_info(sensors, &count, 64);
        for (int i = 0; i < count && i < widgets->num_sensors; i++) {
            char temp_str[32];
            sprintf(temp_str, "%.1f °C", sensors[i].temp_celsius);
            gtk_label_set_text(GTK_LABEL(widgets->sensor_labels[i]), temp_str);
        }
    }

    return G_SOURCE_CONTINUE; // Continue timer
}
