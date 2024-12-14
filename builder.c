//
// Created by asus on 12/11/2024.
//

#include "builder.h"
#include <gtk/gtk.h>
#include <gdk/gdk.h>

GtkBuilder* get_builder_instance() {
    static GtkBuilder *builder = NULL; // Static variable to hold the singleton instance
    if (builder == NULL) {
        // Create the GtkBuilder instance if it doesn't already exist
        builder = gtk_builder_new();
        if (!gtk_builder_add_from_file(builder, "builder.ui", NULL)) {
            g_warning("Failed to load UI file: builder.ui");
            g_object_unref(builder);
            builder = NULL;
        }
    }
    return builder;
}

GObject** get_windows_instance() {

    GtkBuilder *builder = get_builder_instance();

    static GObject **windows_instance = NULL;
    if (windows_instance == NULL) {
        windows_instance = g_malloc(14*sizeof(GObject*));

        windows_instance[0] = gtk_builder_get_object(builder, "main_window");
        if (!windows_instance[0]) {
            g_printerr("Failed to get 'main' window from builder UI file\n");
            g_free(windows_instance);
            return NULL;
        }

        windows_instance[1] = gtk_builder_get_object(builder, "registerWindow");
        if (!windows_instance[1]) {
            g_printerr("Failed to get 'register' window from builder UI file\n");
            g_free(windows_instance);
            return NULL;
        }

        windows_instance[2] = gtk_builder_get_object(builder, "loginWindow");
        if (!windows_instance[2]) {
            g_printerr("Failed to get 'Login' window from builder UI file\n");
            g_free(windows_instance);
            return NULL;
        }

        windows_instance[3] = gtk_builder_get_object(builder, "menuWindow");
        if (!windows_instance[3]) {
            g_printerr("Failed to get 'Menu' window from main UI file\n");
            g_free(windows_instance);
            return NULL;
        }

        windows_instance[4] = gtk_builder_get_object(builder, "patientWin");
        if (!windows_instance[4]) {
            g_printerr("Failed to get 'Display Patient' window from builder UI file\n");
            g_free(windows_instance);
            return NULL;
        }

        windows_instance[5] = gtk_builder_get_object(builder, "rendezvousWindow");
        if (!windows_instance[5]) {
            g_printerr("Failed to get 'Rendezvous' window from builder UI file\n");
            g_free(windows_instance);
            return NULL;
        }

        windows_instance[6] = gtk_builder_get_object(builder, "consultWindow");
        if (!windows_instance[6]) {
            g_printerr("Failed to get 'Consultation' window from builder UI file\n");
            g_free(windows_instance);
            return NULL;
        }

        windows_instance[7] = gtk_builder_get_object(builder, "paymentWindow");
        if (!windows_instance[7]) {
            g_printerr("Failed to get 'payment' window from builder UI file\n");
            g_free(windows_instance);
            return NULL;
        }

        windows_instance[8] = gtk_builder_get_object(builder, "userMenuWindow");
        if (!windows_instance[8]) {
            g_printerr("Failed to get 'User_main' window from builder UI file\n");
            g_free(windows_instance);
            return NULL;
        }

        windows_instance[9] = gtk_builder_get_object(builder, "userInfoWindow");
        if (!windows_instance[9]) {
            g_printerr("Failed to get 'User information' window from builder UI file\n");
            g_free(windows_instance);
            return NULL;
        }

        windows_instance[10] = gtk_builder_get_object(builder, "userRdvWindow");
        if (!windows_instance[10]) {
            g_printerr("Failed to get 'User Rendezvous' window from builder UI file\n");
            g_free(windows_instance);
            return NULL;
        }

        windows_instance[11] = gtk_builder_get_object(builder, "userConsultWindow");
        if (!windows_instance[11]) {
            g_printerr("Failed to get 'User Consultation' window from builder UI file\n");
            g_free(windows_instance);
            return NULL;
        }

        windows_instance[12] = gtk_builder_get_object(builder, "userInvoiceWindow");
        if (!windows_instance[12]) {
            g_printerr("Failed to get 'User Consultation' window from builder UI file\n");
            g_free(windows_instance);
            return NULL;
        }
    }
    return windows_instance;
}

void switch_to_window(GtkButton *btn, gpointer user_data) {
    GObject **windows = get_windows_instance();
    WINS index = GPOINTER_TO_INT(user_data);
    GtkWidget *window_next = GTK_WIDGET(windows[index]);
    GtkWidget *window_prev = gtk_widget_get_ancestor(GTK_WIDGET(btn), GTK_TYPE_WINDOW);

    if (GTK_IS_WINDOW(window_prev)) {
        gtk_widget_set_visible(window_prev, FALSE);
        gtk_widget_set_visible(window_next, TRUE);
    }
}

void load_widget_css(GtkWidget *widget, const char *class_name, const char *css_style) {
    GtkCssProvider *provider = gtk_css_provider_new();
    // Load CSS string
    gtk_css_provider_load_from_data(provider, css_style, -1);

    // Get the style context and attach the provider to it
    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Set the CSS class properly
    gtk_widget_set_name(widget, class_name);
}