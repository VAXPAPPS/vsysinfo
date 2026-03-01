#include "main_window.h"
#include "tabs.h"

// Global widgets structure to prevent memory leaks from user_data
static AppWidgets global_widgets;

void show_main_window(GtkApplication *app, gpointer user_data) {
    (void)user_data;
    // Clear structure
    memset(&global_widgets, 0, sizeof(global_widgets));

    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "VAXP Hardware Info");
    gtk_window_set_default_size(GTK_WINDOW(window), 850, 600);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    // Modern aesthetic with dark theme pref
    g_object_set(gtk_settings_get_default(), "gtk-application-prefer-dark-theme", TRUE, NULL);

    // Set RGBA visual for transparent background
    GdkScreen *screen = gtk_widget_get_screen(window);
    GdkVisual *visual = gdk_screen_get_rgba_visual(screen);
    if (visual != NULL && gdk_screen_is_composited(screen)) {
        gtk_widget_set_visual(window, visual);
        gtk_widget_set_app_paintable(window, TRUE);
    }

    // Apply custom background color
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
        "window { background-color: rgba(0, 0, 0, 0.392); }", -1, NULL);
    gtk_style_context_add_provider_for_screen(screen,
        GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);

    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_container_add(GTK_CONTAINER(window), hbox);

    // Stack and Sidebar
    GtkWidget *stack = gtk_stack_new();
    gtk_stack_set_transition_type(GTK_STACK(stack), GTK_STACK_TRANSITION_TYPE_SLIDE_UP_DOWN);

    GtkWidget *sidebar = gtk_stack_sidebar_new();
    gtk_stack_sidebar_set_stack(GTK_STACK_SIDEBAR(sidebar), GTK_STACK(stack));
    gtk_widget_set_size_request(sidebar, 200, -1);
    
    gtk_box_pack_start(GTK_BOX(hbox), sidebar, FALSE, FALSE, 0);
    
    GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
    gtk_box_pack_start(GTK_BOX(hbox), separator, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), stack, TRUE, TRUE, 0);
    
    // Add tabs
    gtk_stack_add_titled(GTK_STACK(stack), create_os_info_tab(&global_widgets), "os_info", "System Info");
    gtk_stack_add_titled(GTK_STACK(stack), create_overview_tab(&global_widgets), "overview", "Overview");
    gtk_stack_add_titled(GTK_STACK(stack), create_cpu_tab(&global_widgets), "cpu", "Processor");
    gtk_stack_add_titled(GTK_STACK(stack), create_mem_tab(&global_widgets), "ram", "Memory");
    gtk_stack_add_titled(GTK_STACK(stack), create_mobo_tab(&global_widgets), "mobo", "Motherboard");
    gtk_stack_add_titled(GTK_STACK(stack), create_gpu_tab(&global_widgets), "gpu", "Graphics");
    gtk_stack_add_titled(GTK_STACK(stack), create_sensors_tab(&global_widgets), "sensors", "Sensors");
    gtk_stack_add_titled(GTK_STACK(stack), create_storage_tab(&global_widgets), "storage", "Storage");

    gtk_widget_show_all(window);

    // Add timer 
    // 1000 milliseconds = 1 second
    g_timeout_add(1000, update_gui_timer, &global_widgets);
}
