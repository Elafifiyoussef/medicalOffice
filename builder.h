#ifndef GTK_FUNCS_H
#define GTK_FUNCS_H

#include <gtk/gtk.h>
#include <gdk/gdk.h>

typedef enum {
    MAIN,
    REGISTER,
    LOGIN,
    ADMIN_MENU,
    ADMIN_PATIENT_INFO,
    ADMIN_RENDEZVOUS_INFO,
    ADMIN_CONSULT_INFO,
    ADMIN_PAYMENT_INFO,
    USER_MENU,
    USER_INFO,
    USER_RENDEZVOUS_INFO,
    USER_CONSULT_INFO,
    USER_PAYMENT_INFO,
} WINS;

GtkBuilder* get_builder_instance();
GObject** get_windows_instance();
void switch_to_window(GtkButton *btn, gpointer user_data) ;
void load_widget_css(GtkWidget *widget, const char* class_name, const char *css_style);
#endif //GTK_FUNCS_H

