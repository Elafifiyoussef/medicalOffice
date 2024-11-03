#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include "patient.h"

static void click1_cb(GtkButton *btn) {
    const char *s = gtk_button_get_label(btn);
    if (g_strcmp0(s, "Hello.") == 0)
        gtk_button_set_label(btn, "Good-bye.");
    else
        gtk_button_set_label(btn, "Hello.");
}

static void print_hello(GtkWidget *widget, gpointer data) {
    g_print("Hello World\n");
}

static void quit_cb(GtkWindow *window) {
    gtk_window_close(window);
}

static void on_print_entry_text(GtkButton *button, gpointer user_data) {
    GtkEntry *entry = GTK_ENTRY(user_data);
    GtkEntryBuffer *buffer = gtk_entry_get_buffer(entry);
    // Get the text from the entry buffer
    const gchar *text = gtk_entry_buffer_get_text(buffer);
    // Print the text to the console
    g_print("Entry text: %s\n", text);
}

static void print_btn_name(GtkButton *btn) {
    const char *s = gtk_button_get_label(btn);
    g_print("Button name: %s\n", s);
}

static void g_add_patient(GtkButton *btn, gpointer data) {
    GObject **entries = (GObject **)data;
    if (!entries) {
        g_printerr("Error: entries is NULL\n");
        return;
    }

    // Access each entry and print debug information
    GtkEntry *entry_lName = GTK_ENTRY(entries[0]);
    GtkEntry *entry_fName = GTK_ENTRY(entries[1]);
    GtkEntry *entry_age = GTK_ENTRY(entries[2]);
    GtkEntry *entry_addr = GTK_ENTRY(entries[3]);
    GtkEntry *entry_phone = GTK_ENTRY(entries[4]);

    if (!entry_lName || !entry_fName || !entry_age || !entry_addr || !entry_phone) {
        g_printerr("Error: One of the entries is NULL\n");
        return;
    }

    // Retrieve values from each entry
    GtkEntryBuffer *buffer_lName = gtk_entry_get_buffer(entry_lName);
    const char* lName = gtk_entry_buffer_get_text(buffer_lName);

    GtkEntryBuffer *buffer_fName = gtk_entry_get_buffer(entry_fName);
    const char* fName = gtk_entry_buffer_get_text(buffer_fName);

    GtkEntryBuffer *buffer_age = gtk_entry_get_buffer(entry_age);
    const char* age = gtk_entry_buffer_get_text(buffer_age);
    int age_value = strtol(age, NULL, 10);

    GtkEntryBuffer *buffer_addr = gtk_entry_get_buffer(entry_addr);
    const char* addr = gtk_entry_buffer_get_text(buffer_addr);

    GtkEntryBuffer *buffer_phone = gtk_entry_get_buffer(entry_phone);
    const char* phone = gtk_entry_buffer_get_text(buffer_phone);

    Patient patient;
    patient.id = 4;
    strcpy(patient.lName, lName);
    strcpy(patient.fName, fName);
    patient.age = age_value;
    strcpy(patient.address, addr);
    strcpy(patient.phone, phone);

    addPatient(patient);
    // displayAllPatients();
}

static void g_modify_patient(GtkButton *btn, gpointer data) {

    GObject **entries = (GObject **)data;
    if (!entries) {
        g_printerr("Error: entries is NULL\n");
        return;
    }

    // Access each entry and print debug information
    GtkEntry *entry_id = GTK_ENTRY(entries[0]);
    GtkEntry *entry_lName = GTK_ENTRY(entries[1]);
    GtkEntry *entry_fName = GTK_ENTRY(entries[2]);
    GtkEntry *entry_age = GTK_ENTRY(entries[3]);
    GtkEntry *entry_addr = GTK_ENTRY(entries[4]);
    GtkEntry *entry_phone = GTK_ENTRY(entries[5]);

    if (!entry_lName || !entry_fName || !entry_age || !entry_addr || !entry_phone) {
        g_printerr("Error: One of the entries is NULL\n");
        return;
    }

    // Retrieve values from each entry
    GtkEntryBuffer *buffer_id = gtk_entry_get_buffer(entry_id);
    const char* id = gtk_entry_buffer_get_text(buffer_id);
    int id_value = strtol(id, NULL, 10);

    GtkEntryBuffer *buffer_lName = gtk_entry_get_buffer(entry_lName);
    const char* lName = gtk_entry_buffer_get_text(buffer_lName);

    GtkEntryBuffer *buffer_fName = gtk_entry_get_buffer(entry_fName);
    const char* fName = gtk_entry_buffer_get_text(buffer_fName);

    GtkEntryBuffer *buffer_age = gtk_entry_get_buffer(entry_age);
    const char* age = gtk_entry_buffer_get_text(buffer_age);
    int age_value = strtol(age, NULL, 10);

    GtkEntryBuffer *buffer_addr = gtk_entry_get_buffer(entry_addr);
    const char* addr = gtk_entry_buffer_get_text(buffer_addr);

    GtkEntryBuffer *buffer_phone = gtk_entry_get_buffer(entry_phone);
    const char* phone = gtk_entry_buffer_get_text(buffer_phone);

    Patient *patient = malloc(sizeof(Patient));
    patient->id = id_value;
    strcpy(patient->lName, lName);
    strcpy(patient->fName, fName);
    patient->age = age_value;
    strcpy(patient->address, addr);
    strcpy(patient->phone, phone);

    modifyPatient(*patient);
    free(patient);
    // empty buffers for each entry to clear them individually
    buffer_id = gtk_entry_buffer_new("", -1);
    gtk_entry_set_buffer(entry_id, buffer_id);

    buffer_lName = gtk_entry_buffer_new("", -1);
    gtk_entry_set_buffer(entry_lName, buffer_lName);

    buffer_fName = gtk_entry_buffer_new("", -1);
    gtk_entry_set_buffer(entry_fName, buffer_fName);

    buffer_age = gtk_entry_buffer_new("", -1);
    gtk_entry_set_buffer(entry_age, buffer_age);

    buffer_addr = gtk_entry_buffer_new("", -1);
    gtk_entry_set_buffer(entry_addr, buffer_addr);

    buffer_phone = gtk_entry_buffer_new("", -1);
    gtk_entry_set_buffer(entry_phone, buffer_phone);

    gtk_editable_set_editable(GTK_EDITABLE(GTK_ENTRY(entry_lName)), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(entry_lName), FALSE);
    gtk_editable_set_editable(GTK_EDITABLE(GTK_ENTRY(entry_fName)), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(entry_fName), FALSE);
    gtk_editable_set_editable(GTK_EDITABLE(GTK_ENTRY(entry_age)), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(entry_age), FALSE);
    gtk_editable_set_editable(GTK_EDITABLE(GTK_ENTRY(entry_addr)), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(entry_addr), FALSE);
    gtk_editable_set_editable(GTK_EDITABLE(GTK_ENTRY(entry_phone)), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(entry_phone), FALSE);
}

static void g_find_patient(GtkButton *btn, gpointer data) {
    GObject **entries = (GObject **)data;
    if (!entries) {
        g_printerr("Error: entries is NULL\n");
        return;
    }
    GtkEntry *entry_id = GTK_ENTRY(entries[0]);
    GtkEntry *entry_lName = GTK_ENTRY(entries[1]);
    GtkEntry *entry_fName = GTK_ENTRY(entries[2]);
    GtkEntry *entry_age = GTK_ENTRY(entries[3]);
    GtkEntry *entry_addr = GTK_ENTRY(entries[4]);
    GtkEntry *entry_phone = GTK_ENTRY(entries[5]);

    GtkEntryBuffer *buffer_id = gtk_entry_get_buffer(entry_id);
    const char *s = gtk_entry_buffer_get_text(buffer_id);
    const int id_pt = strtol(s, NULL, 10);
    Patient *patient = findPatient(id_pt);
    if (patient == NULL) {

        g_print("No such patient with id %d\n",id_pt);

        // Create new, empty buffers for each entry to clear them individually
        buffer_id = gtk_entry_buffer_new("", -1);
        gtk_entry_set_buffer(entry_id, buffer_id);

        GtkEntryBuffer *buffer_lName = gtk_entry_buffer_new("", -1);
        gtk_entry_set_buffer(entry_lName, buffer_lName);

        GtkEntryBuffer *buffer_fName = gtk_entry_buffer_new("", -1);
        gtk_entry_set_buffer(entry_fName, buffer_fName);

        GtkEntryBuffer *buffer_age = gtk_entry_buffer_new("", -1);
        gtk_entry_set_buffer(entry_age, buffer_age);

        GtkEntryBuffer *buffer_addr = gtk_entry_buffer_new("", -1);
        gtk_entry_set_buffer(entry_addr, buffer_addr);

        GtkEntryBuffer *buffer_phone = gtk_entry_buffer_new("", -1);
        gtk_entry_set_buffer(entry_phone, buffer_phone);


        gtk_editable_set_editable(GTK_EDITABLE(GTK_ENTRY(entry_lName)), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(entry_lName), FALSE);
        gtk_editable_set_editable(GTK_EDITABLE(GTK_ENTRY(entry_fName)), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(entry_fName), FALSE);
        gtk_editable_set_editable(GTK_EDITABLE(GTK_ENTRY(entry_age)), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(entry_age), FALSE);
        gtk_editable_set_editable(GTK_EDITABLE(GTK_ENTRY(entry_addr)), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(entry_addr), FALSE);
        gtk_editable_set_editable(GTK_EDITABLE(GTK_ENTRY(entry_phone)), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(entry_phone), FALSE);

    } else {
        gtk_editable_set_editable(GTK_EDITABLE(entry_lName), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(entry_lName), TRUE);
        gtk_editable_set_editable(GTK_EDITABLE(entry_fName), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(entry_fName), TRUE);
        gtk_editable_set_editable(GTK_EDITABLE(entry_age), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(entry_age), TRUE);
        gtk_editable_set_editable(GTK_EDITABLE(entry_addr), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(entry_addr), TRUE);
        gtk_editable_set_editable(GTK_EDITABLE(entry_phone), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(entry_phone), TRUE);

        GtkEntryBuffer *buffer_lName = gtk_entry_buffer_new(patient->lName, -1);
        gtk_entry_set_buffer(entry_lName, buffer_lName);

        GtkEntryBuffer *buffer_fName = gtk_entry_buffer_new(patient->fName, -1);
        gtk_entry_set_buffer(entry_fName, buffer_fName);

        char age_str[3];
        sprintf(age_str, "%d", patient->age);
        GtkEntryBuffer *buffer_age = gtk_entry_buffer_new(age_str, -1);
        gtk_entry_set_buffer(entry_age, buffer_age);

        GtkEntryBuffer *buffer_addr = gtk_entry_buffer_new(patient->address, -1);
        gtk_entry_set_buffer(entry_addr, buffer_addr);

        GtkEntryBuffer *buffer_phone = gtk_entry_buffer_new(patient->phone, -1);
        gtk_entry_set_buffer(entry_phone, buffer_phone);
    }
}
//===================================================================//
// window creation & filling
//===================================================================//
static GObject** init_GObj_win(GtkBuilder *builder) {
    GObject **wins = g_malloc(5*sizeof(GObject*));
    wins[0] = gtk_builder_get_object(builder, "loginWindow");
    if (!wins[0]) {
        g_printerr("Failed to get 'Login' window from builder UI file\n");
        g_free(wins);
        return NULL;
    }
    wins[1] = gtk_builder_get_object(builder, "menuWindow");
    if (! wins[1]) {
        g_printerr("Failed to get 'Menu' window from main UI file\n");
        g_free(wins);
        return NULL;
    }
    wins[2] = gtk_builder_get_object(builder, "patientWindow");
    if (!wins[2]) {
        g_printerr("Failed to get 'Patient' window from main UI file\n");
        g_free(wins);
        return NULL;
    }

    wins[3] = gtk_builder_get_object(builder, "AddPatientWin");
    if (!wins[3]) {
        g_printerr("Failed to get 'Add Patient' window from main UI file\n");
        g_free(wins);
        return NULL;
    }

    wins[4] = gtk_builder_get_object(builder, "ModifyPatientWin");
    if (!wins[4]) {
        g_printerr("Failed to get 'Modify Patient' window from main UI file\n");
        g_free(wins);
        return NULL;
    }

    return wins;
}

static void switch_to_login(GtkButton *btn, gpointer user_data) {
    GObject **windows = (GObject **)user_data;
    GtkWidget *window_login = GTK_WIDGET(windows[0]);     // Menu window
    GtkWidget *window_prev = gtk_widget_get_ancestor(GTK_WIDGET(btn), GTK_TYPE_WINDOW); // Login window

    if (GTK_IS_WINDOW(window_prev)) {
        gtk_widget_set_visible(window_prev, FALSE);   // Hide login window
        gtk_widget_set_visible(window_login, TRUE);     // Show menu window
    }
}

static void switch_to_menu(GtkButton *btn, gpointer user_data) {
    GObject **windows = (GObject **)user_data;
    GtkWidget *window_menu = GTK_WIDGET(windows[1]);
    GtkWidget *window_prev = gtk_widget_get_ancestor(GTK_WIDGET(btn), GTK_TYPE_WINDOW); // Login window

    if (GTK_IS_WINDOW(window_prev)) {
        gtk_widget_set_visible(window_prev, FALSE);   // Hide login window
        gtk_widget_set_visible(window_menu, TRUE);     // Show menu window
    }
}

static void switch_to_patient(GtkButton *btn, gpointer user_data) {
    GObject **windows = (GObject **)user_data;
    GtkWidget *window_patient = GTK_WIDGET(windows[2]);     // Patient window
    GtkWidget *window_prev = gtk_widget_get_ancestor(GTK_WIDGET(btn), GTK_TYPE_WINDOW); // Login window

    if (GTK_IS_WINDOW(window_prev)) {
        gtk_widget_set_visible(window_prev, FALSE);   // Hide login window
        gtk_widget_set_visible(window_patient, TRUE);     // Show menu window
    }
}

static void switch_to_add_patient(GtkButton *btn, gpointer user_data) {
    GObject **windows = (GObject **)user_data;
    GtkWidget *window_add_pt = GTK_WIDGET(windows[3]);     // Patient window
    GtkWidget *window_prev = gtk_widget_get_ancestor(GTK_WIDGET(btn), GTK_TYPE_WINDOW); // Login window

    if (GTK_IS_WINDOW(window_prev)) {
        gtk_widget_set_visible(window_prev, FALSE);   // Hide login window
        gtk_widget_set_visible(window_add_pt, TRUE);     // Show menu window
    }
}

static void switch_to_modify_patient(GtkButton *btn, gpointer user_data){
    GObject **windows = (GObject **)user_data;
    GtkWidget *window_modify_pt = GTK_WIDGET(windows[4]);     // Patient window
    GtkWidget *window_prev = gtk_widget_get_ancestor(GTK_WIDGET(btn), GTK_TYPE_WINDOW); // Login window

    if (GTK_IS_WINDOW(window_prev)) {
        gtk_widget_set_visible(window_prev, FALSE);   // Hide login window
        gtk_widget_set_visible(window_modify_pt, TRUE);     // Show menu window
    }
}

static void set_login_win_objs(GtkBuilder *builder, GObject **win) {

    GObject *button_login = gtk_builder_get_object(builder, "login");
    if (!button_login) {
        g_printerr("Failed to get 'login' button from builder UI file\n");
        return;
    }
    GObject *name_entry = gtk_builder_get_object(builder, "nameEntry");
    if (!name_entry) {
        g_printerr("Failed to get 'nameEntry' from builder UI file\n");
        return;
    }
    GObject *password_entry = gtk_builder_get_object(builder, "passwordEntry");
    if (!password_entry) {
        g_printerr("Failed to get 'passwordEntry' from builder UI file\n");
        return;
    }

    g_signal_connect(button_login, "clicked", G_CALLBACK(on_print_entry_text), name_entry);
    g_signal_connect(button_login, "clicked", G_CALLBACK(on_print_entry_text), password_entry);
    g_signal_connect(button_login, "clicked", G_CALLBACK(switch_to_menu), win);
}

static void set_menu_win_objs(GtkBuilder *builder, GObject **win) {

    GObject *btn_patient = gtk_builder_get_object(builder, "btn_patient");
    GObject *btn_rdv = gtk_builder_get_object(builder, "btn_rdv");
    GObject *btn_consult = gtk_builder_get_object(builder, "btn_consult");
    GObject *btn_payment = gtk_builder_get_object(builder, "btn_payment");
    GObject *btn_out = gtk_builder_get_object(builder, "btn_out");

    g_signal_connect(btn_patient, "clicked", G_CALLBACK(switch_to_patient), win);
    g_signal_connect(btn_rdv, "clicked", G_CALLBACK(print_btn_name), NULL);
    g_signal_connect(btn_consult, "clicked", G_CALLBACK(print_btn_name), NULL);
    g_signal_connect(btn_payment, "clicked", G_CALLBACK(print_btn_name), NULL);
    g_signal_connect(btn_out, "clicked", G_CALLBACK(switch_to_login), win);
}

static void set_patient_win_objs(GtkBuilder *builder, GObject **win) {
    GObject *btn_add_patient = gtk_builder_get_object(builder, "btn_add_patient");
    GObject *btn_modify_patient = gtk_builder_get_object(builder, "btn_modify_patient");
    GObject *btn_remove_patient = gtk_builder_get_object(builder, "btn_remove_patient");
    GObject *btn_display_patient = gtk_builder_get_object(builder, "btn_display_patient");
    GObject *btn_p_to_menu = gtk_builder_get_object(builder, "btn_p_to_menu");

    g_signal_connect(btn_add_patient, "clicked", G_CALLBACK(switch_to_add_patient), win);
    g_signal_connect(btn_modify_patient, "clicked", G_CALLBACK(switch_to_modify_patient), win);
    g_signal_connect(btn_remove_patient, "clicked", G_CALLBACK(print_btn_name), NULL);
    g_signal_connect(btn_display_patient, "clicked", G_CALLBACK(print_btn_name), NULL);
    g_signal_connect(btn_p_to_menu, "clicked", G_CALLBACK(switch_to_menu), win);
}

static void set_add_patient_win_objs(GtkBuilder *builder, GObject **win) {

    GObject *lName_label = gtk_builder_get_object(builder, "lName_label");
    GObject *fName_label = gtk_builder_get_object(builder, "fName_label");
    GObject *age_label = gtk_builder_get_object(builder, "age_label");
    GObject *addr_label = gtk_builder_get_object(builder, "addr_label");
    GObject *phone_label = gtk_builder_get_object(builder, "phone_label");

    GObject *lName_entry = gtk_builder_get_object(builder, "lName_entry");
    GObject *fName_entry = gtk_builder_get_object(builder, "fName_entry");
    GObject *age_entry = gtk_builder_get_object(builder, "age_entry");
    GObject *addr_entry = gtk_builder_get_object(builder, "addr_entry");
    GObject *phone_entry = gtk_builder_get_object(builder, "phone_entry");

    GObject *submit_pt_btn = gtk_builder_get_object(builder, "submit_pt_btn");
    GObject *return_btn = gtk_builder_get_object(builder, "return_btn");

    GObject **entries = g_new(GObject*, 5);
    entries[0] = lName_entry;
    entries[1] = fName_entry;
    entries[2] = age_entry;
    entries[3] = addr_entry;
    entries[4] = phone_entry;

    g_signal_connect(submit_pt_btn, "clicked", G_CALLBACK(g_add_patient), entries);
    g_signal_connect(submit_pt_btn, "destroy", G_CALLBACK(g_free), entries);

    g_signal_connect(return_btn, "clicked", G_CALLBACK(switch_to_patient), win);
}

static void set_modify_patient_win_objs(GtkBuilder *builder, GObject **win) {
    GObject *id_mod_label = gtk_builder_get_object(builder, "id_mod_label");
    GObject *lName_mod_label = gtk_builder_get_object(builder, "lName_mod_label");
    GObject *fName_mod_label = gtk_builder_get_object(builder, "fName_mod_label");
    GObject *age_mod_label = gtk_builder_get_object(builder, "age_mod_label");
    GObject *addr_mod_label = gtk_builder_get_object(builder, "addr_mod_label");
    GObject *phone_mod_label = gtk_builder_get_object(builder, "phone_mod_label");

    GObject *id_mod_entry = gtk_builder_get_object(builder, "id_mod_entry");
    GObject *lName_mod_entry = gtk_builder_get_object(builder, "lName_mod_entry");
    GObject *fName_mod_entry = gtk_builder_get_object(builder, "fName_mod_entry");
    GObject *age_mod_entry = gtk_builder_get_object(builder, "age_mod_entry");
    GObject *addr_mod_entry = gtk_builder_get_object(builder, "addr_mod_entry");
    GObject *phone_mod_entry = gtk_builder_get_object(builder, "phone_mod_entry");

    GObject *id_btn = gtk_builder_get_object(builder, "id_mod_btn");
    GObject *mod_pt_btn = gtk_builder_get_object(builder, "mod_pt_btn");
    GObject *return_btn = gtk_builder_get_object(builder, "return_mod_btn");

    GObject **entries = g_new(GObject*, 6);
    entries[0] = id_mod_entry;
    entries[1] = lName_mod_entry;
    entries[2] = fName_mod_entry;
    entries[3] = age_mod_entry;
    entries[4] = addr_mod_entry;
    entries[5] = phone_mod_entry;

    g_signal_connect(id_btn, "clicked", G_CALLBACK(g_find_patient), entries);
    g_signal_connect(mod_pt_btn, "clicked", G_CALLBACK(g_modify_patient), entries);
    g_signal_connect(return_btn, "clicked", G_CALLBACK(switch_to_patient), win);
}
//===================================================================//
static void activate(GtkApplication *app, gpointer user_data) {
    /* Construct a GtkBuilder instance and load UI description */
    GtkBuilder *builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "builder.ui", NULL);

    GObject **wins = init_GObj_win(builder);

    if (!wins) {
        g_printerr("Failed to initialize windows\n");
        g_object_unref(builder);
        return;
    }
    gtk_window_set_application(GTK_WINDOW(wins[0]), app);
    gtk_window_set_application(GTK_WINDOW(wins[1]), app);
    gtk_window_set_application(GTK_WINDOW(wins[2]), app);
    gtk_window_set_application(GTK_WINDOW(wins[3]), app);
    gtk_window_set_application(GTK_WINDOW(wins[4]), app);

    set_login_win_objs(builder, wins);
    set_menu_win_objs(builder, wins);
    set_patient_win_objs(builder, wins);
    set_add_patient_win_objs(builder, wins);
    set_modify_patient_win_objs(builder, wins);

    gtk_widget_set_visible(GTK_WIDGET(wins[4]), TRUE);
    g_object_unref(builder);

}

int main(int argc, char *argv[]) {
    #ifdef GTK_SRCDIR
    g_chdir(GTK_SRCDIR);
    #endif
    GtkApplication *app = gtk_application_new("org.gtk.medicalOffice", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

