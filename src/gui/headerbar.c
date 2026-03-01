#include "headerbar.h"

static void on_close_clicked(GtkButton *btn, gpointer user_data) {
    (void)btn;
    GtkWindow *window = GTK_WINDOW(user_data);
    gtk_window_close(window);
}

static void on_minimize_clicked(GtkButton *btn, gpointer user_data) {
    (void)btn;
    GtkWindow *window = GTK_WINDOW(user_data);
    gtk_window_iconify(window);
}

static void on_maximize_clicked(GtkButton *btn, gpointer user_data) {
    (void)btn;
    GtkWindow *window = GTK_WINDOW(user_data);
    if (gtk_window_is_maximized(window))
        gtk_window_unmaximize(window);
    else
        gtk_window_maximize(window);
}

static gboolean on_headerbar_button_press(GtkWidget *widget, GdkEventButton *event, gpointer user_data) {
    (void)widget;
    GtkWindow *window = GTK_WINDOW(user_data);
    if (event->type == GDK_BUTTON_PRESS && event->button == 1) {
        gtk_window_begin_move_drag(window,
                                   event->button,
                                   event->x_root,
                                   event->y_root,
                                   event->time);
        return TRUE;
    }
    return FALSE;
}

static GtkWidget* create_window_button(const char *css_class, GCallback callback, gpointer user_data) {
    GtkWidget *btn = gtk_button_new();
    GtkStyleContext *context = gtk_widget_get_style_context(btn);
    gtk_style_context_add_class(context, "vdl-window-btn");
    gtk_style_context_add_class(context, css_class);
    gtk_widget_set_can_focus(btn, FALSE);
    g_signal_connect(btn, "clicked", callback, user_data);
    return btn;
}

GtkWidget* create_custom_headerbar(GtkWindow *window, const char *title_text) {
    // We create an EventBox to catch mouse events for dragging
    GtkWidget *event_box = gtk_event_box_new();
    gtk_widget_add_events(event_box, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(event_box, "button-press-event", G_CALLBACK(on_headerbar_button_press), window);

    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkStyleContext *context = gtk_widget_get_style_context(hbox);
    gtk_style_context_add_class(context, "vdl-headerbar");

    gtk_container_add(GTK_CONTAINER(event_box), hbox);

    // Right side: Window control buttons
    GtkWidget *btn_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_margin_start(btn_box, 16);
    gtk_widget_set_valign(btn_box, GTK_ALIGN_CENTER);

    GtkWidget *minimize_btn = create_window_button("vdl-btn-minimize", G_CALLBACK(on_minimize_clicked), window);
    GtkWidget *maximize_btn = create_window_button("vdl-btn-maximize", G_CALLBACK(on_maximize_clicked), window);
    GtkWidget *close_btn = create_window_button("vdl-btn-close", G_CALLBACK(on_close_clicked), window);

    gtk_widget_set_tooltip_text(minimize_btn, "Minimize");
    gtk_widget_set_tooltip_text(maximize_btn, "Maximize");
    gtk_widget_set_tooltip_text(close_btn, "Close");

    gtk_box_pack_start(GTK_BOX(btn_box), minimize_btn, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(btn_box), maximize_btn, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(btn_box), close_btn, FALSE, FALSE, 0);

    // Left side space
    GtkWidget *start_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_widget_set_valign(start_box, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_start(start_box, 16);
    gtk_widget_set_margin_end(start_box, 16);

    // Center Title
    GtkWidget *title_label = gtk_label_new(title_text);
    GtkStyleContext *lbl_ctx = gtk_widget_get_style_context(title_label);
    gtk_style_context_add_class(lbl_ctx, "title");
    gtk_widget_set_halign(title_label, GTK_ALIGN_START);

    // Spacer to push buttons to the right
    GtkWidget *spacer = gtk_label_new("");
    gtk_widget_set_hexpand(spacer, TRUE);

    // Assemble
    gtk_box_pack_start(GTK_BOX(hbox), start_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), title_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), spacer, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), btn_box, FALSE, FALSE, 0);

    return event_box;
}
