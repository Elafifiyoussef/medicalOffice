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
    ADMIN_INVOICE_INFO,
    USER_MENU,
    USER_INFO,
    USER_RENDEZVOUS_INFO,
    USER_CONSULT_INFO,
    USER_INVOICE_INFO,
} WINS;

GtkBuilder* get_builder_instance();
GObject** get_windows_instance();
void switch_to_window(GtkButton *btn, gpointer user_data) ;
void set_widget_css(GtkWidget *widget, const char* class_name, const char *css_style);
void set_text_entry(GtkWidget *entry, const char *text);
const char* get_text_from_entry(GtkWidget *entry);
int get_int_from_entry(GtkWidget *entry);
double get_double_from_entry(GtkWidget *entry);
const char* get_dropdown_item(GtkDropDown *dropdown);
void display_string_months(GListModel *model);

#endif //GTK_FUNCS_H

