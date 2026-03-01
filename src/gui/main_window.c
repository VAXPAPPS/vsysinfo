#include "main_window.h"

static GtkWidget* create_tab_content(const char* label_text) {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_top(box, 20);
    gtk_widget_set_margin_bottom(box, 20);
    gtk_widget_set_margin_start(box, 20);
    gtk_widget_set_margin_end(box, 20);

    GtkWidget *label = gtk_label_new(label_text);
    // Make the label prominent
    PangoAttrList *attrs = pango_attr_list_new();
    pango_attr_list_insert(attrs, pango_attr_scale_new(2.0));
    gtk_label_set_attributes(GTK_LABEL(label), attrs);
    pango_attr_list_unref(attrs);

    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

    return box;
}

void show_main_window(GtkApplication *app, gpointer user_data) {
    (void)user_data;

    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "VAXP Hardware Info");
    gtk_window_set_default_size(GTK_WINDOW(window), 900, 600);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    // Apply dark theme preference if available
    g_object_set(gtk_settings_get_default(), "gtk-application-prefer-dark-theme", TRUE, NULL);

    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_container_add(GTK_CONTAINER(window), hbox);

    // Main content area: a Stack
    GtkWidget *stack = gtk_stack_new();
    gtk_stack_set_transition_type(GTK_STACK(stack), GTK_STACK_TRANSITION_TYPE_SLIDE_UP_DOWN);

    // Sidebar
    GtkWidget *sidebar = gtk_stack_sidebar_new();
    gtk_stack_sidebar_set_stack(GTK_STACK_SIDEBAR(sidebar), GTK_STACK(stack));
    gtk_widget_set_size_request(sidebar, 220, -1);
    
    // Add sidebar to left
    gtk_box_pack_start(GTK_BOX(hbox), sidebar, FALSE, FALSE, 0);

    // Add a separator between sidebar and content
    GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
    gtk_box_pack_start(GTK_BOX(hbox), separator, FALSE, FALSE, 0);

    // Add stack to right, expanding
    gtk_box_pack_start(GTK_BOX(hbox), stack, TRUE, TRUE, 0);
    
    // Add pages to the stack
    const char *tabs[] = { "Overview", "CPU", "Memory", "Motherboard", "Graphics", "Sensors", "Storage" };
    int num_tabs = sizeof(tabs) / sizeof(tabs[0]);
    
    for (int i = 0; i < num_tabs; i++) {
        GtkWidget *content = create_tab_content(tabs[i]);
        gtk_stack_add_titled(GTK_STACK(stack), content, tabs[i], tabs[i]);
    }

    gtk_widget_show_all(window);
}
