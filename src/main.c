#include <gtk/gtk.h>
#include "gui/main_window.h"

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    // Use G_APPLICATION_FLAGS_NONE for compatibility
    app = gtk_application_new("com.vaxp.vsysinfo", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(show_main_window), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
