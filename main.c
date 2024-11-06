#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "consultation.h"
#include "patient.h"

// static void click1_cb(GtkButton *btn) {
//     const char *s = gtk_button_get_label(btn);
//     if (g_strcmp0(s, "Hello.") == 0)
//         gtk_button_set_label(btn, "Good-bye.");
//     else
//         gtk_button_set_label(btn, "Hello.");
// }
//
// static void print_hello(GtkWidget *widget, gpointer data) {
//     g_print("Hello World\n");
// }
//
// static void quit_cb(GtkWindow *window) {
//     gtk_window_close(window);
// }

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
    GtkEntry *entry_id= GTK_ENTRY(entries[5]);
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

    if (id_value != 0) {
        Patient patient;
        patient.id = id_value;
        strcpy(patient.lName, lName);
        strcpy(patient.fName, fName);
        patient.age = age_value;
        strcpy(patient.address, addr);
        strcpy(patient.phone, phone);

        addPatient(patient);
    } else {
        g_printerr("Error: Patient is NULL\n");
    }
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

static void g_delete_patient(GtkButton *btn, gpointer data) {
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

    deletePatient(id_value);

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

static void g_update_display_patient(GtkButton *btn, gpointer data) {

    Patient *patients = getPatients();
    int patient_count = getPatientCount();
    GtkGrid *grid = GTK_GRID(data);

    for (int i = 0; i < patient_count; i++) {
        // Convert data fields to strings
        char id_text[20], age_text[10];
        snprintf(id_text, sizeof(id_text), "%d", patients[i].id);
        snprintf(age_text, sizeof(age_text), "%d", patients[i].age);

        // Attempt to retrieve existing labels for each field in the current row
        GtkWidget *id_label = gtk_grid_get_child_at(grid, 0, i + 1);
        GtkWidget *lName_label = gtk_grid_get_child_at(grid, 1, i + 1);
        GtkWidget *fName_label = gtk_grid_get_child_at(grid, 2, i + 1);
        GtkWidget *age_label = gtk_grid_get_child_at(grid, 3, i + 1);
        GtkWidget *addr_label = gtk_grid_get_child_at(grid, 4, i + 1);
        GtkWidget *phone_label = gtk_grid_get_child_at(grid, 5, i + 1);

        // If the label exists, update its text; otherwise, create a new label and attach it to the grid
        if (id_label) {
            gtk_label_set_text(GTK_LABEL(id_label), id_text);
            gtk_label_set_text(GTK_LABEL(lName_label), patients[i].lName);
            gtk_label_set_text(GTK_LABEL(fName_label), patients[i].fName);
            gtk_label_set_text(GTK_LABEL(age_label), age_text);
            gtk_label_set_text(GTK_LABEL(addr_label), patients[i].address);
            gtk_label_set_text(GTK_LABEL(phone_label), patients[i].phone);
        } else {
            // Create and attach new labels to the grid for new rows
            id_label = gtk_label_new(id_text);
            lName_label = gtk_label_new(patients[i].lName);
            fName_label = gtk_label_new(patients[i].fName);
            age_label = gtk_label_new(age_text);
            addr_label = gtk_label_new(patients[i].address);
            phone_label = gtk_label_new(patients[i].phone);

            gtk_grid_attach(grid, id_label, 0, i + 1, 1, 1);
            gtk_grid_attach(grid, lName_label, 1, i + 1, 1, 1);
            gtk_grid_attach(grid, fName_label, 2, i + 1, 1, 1);
            gtk_grid_attach(grid, age_label, 3, i + 1, 1, 1);
            gtk_grid_attach(grid, addr_label, 4, i + 1, 1, 1);
            gtk_grid_attach(grid, phone_label, 5, i + 1, 1, 1);
        }
    }
}

static void g_add_consult(GtkButton *btn, gpointer data) {
    GObject **entries = (GObject **)data;
    if (!entries) {
        g_printerr("Error: entries is NULL\n");
        return;
    }

    // Access each entry and print debug information
    GtkEntry *entry_id_consult = GTK_ENTRY(entries[0]);
    GtkEntry *entry_id_patient = GTK_ENTRY(entries[1]);
    GtkEntry *entry_symptoms = GTK_ENTRY(entries[2]);
    GtkEntry *entry_diagnosis = GTK_ENTRY(entries[3]);
    GtkEntry *entry_treatmentPlan = GTK_ENTRY(entries[4]);

    if (!entry_id_consult || !entry_id_patient || !entry_symptoms || !entry_diagnosis || !entry_treatmentPlan) {
        g_printerr("Error: One of the entries is NULL\n");
        return;
    }

    // Retrieve values from each entry
    GtkEntryBuffer *buffer_id_consult = gtk_entry_get_buffer(entry_id_consult);
    const char* id_consult = gtk_entry_buffer_get_text(buffer_id_consult);
    int id_consult_value = strtol(id_consult, NULL, 10);

    GtkEntryBuffer *buffer_id_patient = gtk_entry_get_buffer(entry_id_patient);
    const char* id_patient = gtk_entry_buffer_get_text(buffer_id_patient);
    int id_patient_value = strtol(id_patient, NULL, 10);

    GtkEntryBuffer *buffer_symptoms = gtk_entry_get_buffer(entry_symptoms);
    const char* symptoms = gtk_entry_buffer_get_text(buffer_symptoms);

    GtkEntryBuffer *buffer_diagnosis = gtk_entry_get_buffer(entry_diagnosis);
    const char* diagnosis = gtk_entry_buffer_get_text(buffer_diagnosis);

    GtkEntryBuffer *buffer_treatmentPlan = gtk_entry_get_buffer(entry_treatmentPlan);
    const char* treatmentPlan = gtk_entry_buffer_get_text(buffer_treatmentPlan);

    if (id_consult_value != 0 || id_patient_value != 0) {
        Consultation consult;
        consult.id = id_consult_value;
        consult.patient_id = id_patient_value;
        strcpy(consult.symptoms, symptoms);
        strcpy(consult.diagnosis, diagnosis);
        strcpy(consult.treatmentPlan, treatmentPlan);

        createConsultation(&consult);
    } else {
        g_printerr("Error: Consultation is NULL\n");
    }
}

static void g_modify_consult(GtkButton *btn, gpointer data) {}

static void g_delete_consult(GtkButton *btn, gpointer data) {}

static void g_find_consult(GtkButton *btn, gpointer data) {}

static void g_update_display_consult(GtkButton *btn, gpointer data){}

//===================================================================//
// window creation & filling
//===================================================================//
static GObject** init_GObj_win(GtkBuilder *builder) {

    GObject **wins = g_malloc(12*sizeof(GObject*));

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

    wins[5] = gtk_builder_get_object(builder, "DeletePatientWin");
    if (!wins[5]) {
        g_printerr("Failed to get 'Delete Patient' window from builder UI file\n");
        g_free(wins);
        return NULL;
    }

    wins[6] = gtk_builder_get_object(builder, "DisplayPatientWin");
    if (!wins[6]) {
        g_printerr("Failed to get 'Display Patient' window from builder UI file\n");
        g_free(wins);
        return NULL;
    }

    wins[7] = gtk_builder_get_object(builder, "consultWindow");
    if (!wins[7]) {
        g_printerr("Failed to get 'Consultation' window from builder UI file\n");
        g_free(wins);
        return NULL;
    }

    wins[8] = gtk_builder_get_object(builder, "AddConsultWin");
    if (!wins[8]) {
        g_printerr("Failed to get 'Add Consultation' window from builder UI file\n");
        g_free(wins);
        return NULL;
    }

    wins[9] = gtk_builder_get_object(builder, "ModifyConsultWin");
    if (wins[9] == NULL) {
        g_printerr("Failed to get 'Modify Consultation' window from builder UI file\n");
        g_free(wins);
        return NULL;
    }

    // wins[10] = gtk_builder_get_object(builder, "DeleteConsultWin");
    // if (!wins[10]) {
    //     g_printerr("Failed to get 'Delete Consultation' window from builder UI file\n");
    //     g_free(wins);
    //     return NULL;
    // }


    // wins[11] = gtk_builder_get_object(builder, "DisplayConsultWin");
    // if (!wins[11]) {
    //     g_printerr("Failed to get 'Display Consultation' window from builder UI file\n");
    //     g_free(wins);
    //     return NULL;
    // }

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

static void switch_to_delete_patient(GtkButton *btn, gpointer user_data) {
    GObject **windows = (GObject **)user_data;
    GtkWidget *window_delete_pt = GTK_WIDGET(windows[5]);     // Patient window
    GtkWidget *window_prev = gtk_widget_get_ancestor(GTK_WIDGET(btn), GTK_TYPE_WINDOW); // Login window

    if (GTK_IS_WINDOW(window_prev)) {
        gtk_widget_set_visible(window_prev, FALSE);   // Hide login window
        gtk_widget_set_visible(window_delete_pt, TRUE);     // Show menu window
    }
}

static void switch_to_display_patient(GtkButton *btn, gpointer user_data) {
    GObject **windows = (GObject **)user_data;
    GtkWidget *window_display_pt = GTK_WIDGET(windows[6]);
    GtkWidget *window_prev = gtk_widget_get_ancestor(GTK_WIDGET(btn), GTK_TYPE_WINDOW);

    if (GTK_IS_WINDOW(window_prev)) {
        gtk_widget_set_visible(window_prev, FALSE);
        gtk_widget_set_visible(window_display_pt, TRUE);
    }
}

static void switch_to_consult(GtkButton *btn, gpointer user_data) {
    GObject **windows = (GObject **)user_data;
    GtkWidget *window_consult = GTK_WIDGET(windows[7]);
    GtkWidget *window_prev = gtk_widget_get_ancestor(GTK_WIDGET(btn), GTK_TYPE_WINDOW);

    if (GTK_IS_WINDOW(window_prev)) {
        gtk_widget_set_visible(window_prev, FALSE);
        gtk_widget_set_visible(window_consult, TRUE);
    }
}

static void switch_to_add_consult(GtkButton *btn, gpointer user_data) {
    GObject **windows = (GObject **)user_data;
    GtkWidget *window_add_consult = GTK_WIDGET(windows[8]);
    GtkWidget *window_prev = gtk_widget_get_ancestor(GTK_WIDGET(btn), GTK_TYPE_WINDOW);

    if (GTK_IS_WINDOW(window_prev)) {
        gtk_widget_set_visible(window_prev, FALSE);
        gtk_widget_set_visible(window_add_consult, TRUE);
    }
}

static void switch_to_modify_consult(GtkButton *btn, gpointer user_data) {
    GObject **windows = (GObject **)user_data;
    GtkWidget *window_modify_consult = GTK_WIDGET(windows[9]);     // Patient window
    GtkWidget *window_prev = gtk_widget_get_ancestor(GTK_WIDGET(btn), GTK_TYPE_WINDOW); // Login window

    if (GTK_IS_WINDOW(window_prev)) {
        gtk_widget_set_visible(window_prev, FALSE);   // Hide login window
        gtk_widget_set_visible(window_modify_consult, TRUE);     // Show menu window
    }
}

static void switch_to_delete_consult(GtkButton *btn, gpointer user_data) {
    GObject **windows = (GObject **)user_data;
    GtkWidget *window_delete_consult = GTK_WIDGET(windows[10]);     // Patient window
    GtkWidget *window_prev = gtk_widget_get_ancestor(GTK_WIDGET(btn), GTK_TYPE_WINDOW); // Login window

    if (GTK_IS_WINDOW(window_prev)) {
        gtk_widget_set_visible(window_prev, FALSE);   // Hide login window
        gtk_widget_set_visible(window_delete_consult, TRUE);     // Show menu window
    }
}

static void switch_to_display_consult(GtkButton *btn, gpointer user_data) {
    GObject **windows = (GObject **)user_data;
    GtkWidget *window_display_consult = GTK_WIDGET(windows[11]);     // Patient window
    GtkWidget *window_prev = gtk_widget_get_ancestor(GTK_WIDGET(btn), GTK_TYPE_WINDOW); // Login window

    if (GTK_IS_WINDOW(window_prev)) {
        gtk_widget_set_visible(window_prev, FALSE);   // Hide login window
        gtk_widget_set_visible(window_display_consult, TRUE);     // Show menu window
    }
}

// set windows objects
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
    g_signal_connect(btn_consult, "clicked", G_CALLBACK(switch_to_consult), win);
    g_signal_connect(btn_payment, "clicked", G_CALLBACK(print_btn_name), NULL);
    g_signal_connect(btn_out, "clicked", G_CALLBACK(switch_to_login), win);
}

// set patient windows objects
static void set_patient_win_objs(GtkBuilder *builder, GObject **win) {
    GObject *btn_add_patient = gtk_builder_get_object(builder, "btn_add_patient");
    GObject *btn_modify_patient = gtk_builder_get_object(builder, "btn_modify_patient");
    GObject *btn_remove_patient = gtk_builder_get_object(builder, "btn_remove_patient");
    GObject *btn_display_patient = gtk_builder_get_object(builder, "btn_display_patient");
    GObject *btn_p_to_menu = gtk_builder_get_object(builder, "btn_p_to_menu");

    g_signal_connect(btn_add_patient, "clicked", G_CALLBACK(switch_to_add_patient), win);
    g_signal_connect(btn_modify_patient, "clicked", G_CALLBACK(switch_to_modify_patient), win);
    g_signal_connect(btn_remove_patient, "clicked", G_CALLBACK(switch_to_delete_patient), win);
    g_signal_connect(btn_display_patient, "clicked", G_CALLBACK(switch_to_display_patient), win);
    g_signal_connect(btn_p_to_menu, "clicked", G_CALLBACK(switch_to_menu), win);
}

static void set_add_patient_win_objs(GtkBuilder *builder, GObject **win) {

    GObject *id_label = gtk_builder_get_object(builder, "id_label");
    GObject *lName_label = gtk_builder_get_object(builder, "lName_label");
    GObject *fName_label = gtk_builder_get_object(builder, "fName_label");
    GObject *age_label = gtk_builder_get_object(builder, "age_label");
    GObject *addr_label = gtk_builder_get_object(builder, "addr_label");
    GObject *phone_label = gtk_builder_get_object(builder, "phone_label");

    GObject *id_entry = gtk_builder_get_object(builder, "id_entry");
    GObject *lName_entry = gtk_builder_get_object(builder, "lName_entry");
    GObject *fName_entry = gtk_builder_get_object(builder, "fName_entry");
    GObject *age_entry = gtk_builder_get_object(builder, "age_entry");
    GObject *addr_entry = gtk_builder_get_object(builder, "addr_entry");
    GObject *phone_entry = gtk_builder_get_object(builder, "phone_entry");

    GObject *submit_pt_btn = gtk_builder_get_object(builder, "submit_pt_btn");
    GObject *return_btn = gtk_builder_get_object(builder, "return_btn");

    GObject **entries = g_new(GObject*, 6);
    entries[5] = id_entry;
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

static void set_delete_patient_win_objs(GtkBuilder *builder, GObject **win) {
    GObject *id_del_label = gtk_builder_get_object(builder, "id_del_label");
    GObject *lName_del_label = gtk_builder_get_object(builder, "lName_del_label");
    GObject *fName_del_label = gtk_builder_get_object(builder, "fName_del_label");
    GObject *age_del_label = gtk_builder_get_object(builder, "age_del_label");
    GObject *addr_del_label = gtk_builder_get_object(builder, "addr_del_label");
    GObject *phone_del_label = gtk_builder_get_object(builder, "phone_del_label");

    GObject *id_del_entry = gtk_builder_get_object(builder, "id_del_entry");
    GObject *lName_del_entry = gtk_builder_get_object(builder, "lName_del_entry");
    GObject *fName_del_entry = gtk_builder_get_object(builder, "fName_del_entry");
    GObject *age_del_entry = gtk_builder_get_object(builder, "age_del_entry");
    GObject *addr_del_entry = gtk_builder_get_object(builder, "addr_del_entry");
    GObject *phone_del_entry = gtk_builder_get_object(builder, "phone_del_entry");

    GObject *id_btn = gtk_builder_get_object(builder, "id_del_btn");
    GObject *del_pt_btn = gtk_builder_get_object(builder, "del_pt_btn");
    GObject *return_btn = gtk_builder_get_object(builder, "return_del_btn");

    GObject **entries = g_new(GObject*, 6);
    entries[0] = id_del_entry;
    entries[1] = lName_del_entry;
    entries[2] = fName_del_entry;
    entries[3] = age_del_entry;
    entries[4] = addr_del_entry;
    entries[5] = phone_del_entry;

    g_signal_connect(id_btn, "clicked", G_CALLBACK(g_find_patient), entries);
    g_signal_connect(del_pt_btn, "clicked", G_CALLBACK(g_delete_patient), entries);
    g_signal_connect(return_btn, "clicked", G_CALLBACK(switch_to_patient), win);
}

static void set_display_patient_win_objs(GtkBuilder *builder, GObject **win) {

    Patient *patients = getPatients();
    int patient_count = getPatientCount();

    GObject *grid = gtk_builder_get_object(builder, "grid_dspl_patient");

    GObject **labels = g_new(GObject*, 6);

    // Set up the grid headers
    GtkWidget *id_header = gtk_label_new("ID");
    GtkWidget *lName_header = gtk_label_new("Last Name");
    GtkWidget *fName_header = gtk_label_new("First Name");
    GtkWidget *age_header = gtk_label_new("Age");
    GtkWidget *addr_header = gtk_label_new("Address");
    GtkWidget *phone_header = gtk_label_new("Phone");

    // Add headers to the first row of the grid
    gtk_grid_attach(GTK_GRID(grid), id_header, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), lName_header, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), fName_header, 2, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), age_header, 3, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), addr_header, 4, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), phone_header, 5, 0, 1, 1);

    // Add each patient's data to the grid
    for (int i = 0; i < patient_count; i++) {
        // Convert data fields to strings
        char id_text[20], age_text[10];
        snprintf(id_text, sizeof(id_text), "%d", patients[i].id);
        snprintf(age_text, sizeof(age_text), "%d", patients[i].age);

        // Create labels for each field
        GtkWidget *id_label = gtk_label_new(id_text);
        GtkWidget *lName_label = gtk_label_new(patients[i].lName);
        GtkWidget *fName_label = gtk_label_new(patients[i].fName);
        GtkWidget *age_label = gtk_label_new(age_text);
        GtkWidget *addr_label = gtk_label_new(patients[i].address);
        GtkWidget *phone_label = gtk_label_new(patients[i].phone);

        // Attach labels to the grid
        gtk_grid_attach(GTK_GRID(grid), id_label, 0, i + 1, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), lName_label, 1, i + 1, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), fName_label, 2, i + 1, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), age_label, 3, i + 1, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), addr_label, 4, i + 1, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), phone_label, 5, i + 1, 1, 1);
    }

    GObject *return_btn = gtk_builder_get_object(builder, "return_dspl_btn");
    GObject *update_btn = gtk_builder_get_object(builder, "update_dspl_btn");

    g_signal_connect(update_btn, "clicked", G_CALLBACK(g_update_display_patient), grid);
    g_signal_connect(return_btn, "clicked", G_CALLBACK(switch_to_patient), win);
}

// set consultation windows objects
static void set_consult_win_objs(GtkBuilder *builder, GObject **win) {
    GObject *btn_add_consult = gtk_builder_get_object(builder, "btn_add_consult");
    GObject *btn_modify_consult = gtk_builder_get_object(builder, "btn_modify_consult");
    GObject *btn_remove_consult = gtk_builder_get_object(builder, "btn_remove_consult");
    GObject *btn_display_consult = gtk_builder_get_object(builder, "btn_display_consult");
    GObject *btn_consult_to_menu = gtk_builder_get_object(builder, "btn_consult_to_menu");

    g_signal_connect(btn_add_consult, "clicked", G_CALLBACK(switch_to_add_consult), win);
    g_signal_connect(btn_modify_consult, "clicked", G_CALLBACK(switch_to_modify_consult), win);
    g_signal_connect(btn_remove_consult, "clicked", G_CALLBACK(switch_to_delete_consult), win);
    g_signal_connect(btn_display_consult, "clicked", G_CALLBACK(switch_to_display_consult), win);
    g_signal_connect(btn_consult_to_menu, "clicked", G_CALLBACK(switch_to_menu), win);
}

static void set_add_consult_win_objs(GtkBuilder *builder, GObject **win) {
    GObject *id_consult_label = gtk_builder_get_object(builder, "id_consult_label");
    GObject *id_patient_label = gtk_builder_get_object(builder, "id_patient_label");
    GObject *symptoms_label = gtk_builder_get_object(builder, "symptoms_label");
    GObject *diagnosis_label = gtk_builder_get_object(builder, "diagnosis_label");
    GObject *treatmentPlan_label = gtk_builder_get_object(builder, "treatmentPlan_label");

    GObject *id_consult_entry = gtk_builder_get_object(builder, "id_consult_entry");
    GObject *id_patient_entry = gtk_builder_get_object(builder, "id_patient_entry");
    GObject *symptoms_entry = gtk_builder_get_object(builder, "symptoms_entry");
    GObject *diagnosis_entry = gtk_builder_get_object(builder, "diagnosis_entry");
    GObject *treatmentPlan_entry = gtk_builder_get_object(builder, "treatmentPlan_entry");

    GObject *submit_consult_btn = gtk_builder_get_object(builder, "submit_consult_btn");
    GObject *return_btn = gtk_builder_get_object(builder, "return_add_consult_btn");

    GObject **entries = g_new(GObject*, 5);
    entries[0] = id_consult_entry;
    entries[1] = id_patient_entry;
    entries[2] = symptoms_entry;
    entries[3] = diagnosis_entry;
    entries[4] = treatmentPlan_entry;

    g_signal_connect(submit_consult_btn, "clicked", G_CALLBACK(g_add_consult), entries);
    g_signal_connect(return_btn, "clicked", G_CALLBACK(switch_to_consult), win);
}

static void set_modify_consult_win_objs(GtkBuilder *builder, GObject **win) {
    GObject *id_mod_patient_label = gtk_builder_get_object(builder, "id_mod_patient_label");
    GObject *id_mod_consult_label = gtk_builder_get_object(builder, "id_mod_consult_label");
    GObject *symptoms_mod_label = gtk_builder_get_object(builder, "symptoms_mod_label");
    GObject *diagnosis_mod_label = gtk_builder_get_object(builder, "diagnosis_mod_label");
    GObject *treatmentPlan_mod_label = gtk_builder_get_object(builder, "treatmentPlan_mod_label");

    GObject *id_mod_patient_entry = gtk_builder_get_object(builder, "id_mod_patient_entry");
    GObject *id_mod_consult_entry = gtk_builder_get_object(builder, "id_mod_consult_entry");
    GObject *symptoms_mod_entry = gtk_builder_get_object(builder, "symptoms_mod_entry");
    GObject *diagnosis_mod_entry = gtk_builder_get_object(builder, "diagnosis_mod_entry");
    GObject *treatmentPlan_mod_entry = gtk_builder_get_object(builder, "treatmentPlan_mod_entry");

    GObject *find_id_patient_btn = gtk_builder_get_object(builder, "find_id_pt_consult_btn");
    GObject *find_id_consult_btn = gtk_builder_get_object(builder, "find_id_consult_btn");
    GObject *mod_consult_btn = gtk_builder_get_object(builder, "mod_consult_btn");
    GObject *return_mod_consult_btn = gtk_builder_get_object(builder, "return_mod_consult_btn");

    GObject **entries = g_new(GObject*, 5);
    entries[0] = id_mod_consult_entry;
    entries[1] = id_mod_patient_entry;
    entries[2] = symptoms_mod_entry;
    entries[3] = diagnosis_mod_entry;
    entries[4] = treatmentPlan_mod_entry;

    g_signal_connect(find_id_patient_btn, "clicked", G_CALLBACK(g_find_consult), entries);
    g_signal_connect(find_id_consult_btn, "clicked", G_CALLBACK(g_find_consult), entries);
    g_signal_connect(mod_consult_btn, "clicked", G_CALLBACK(g_modify_consult), entries);
    g_signal_connect(return_mod_consult_btn, "clicked", G_CALLBACK(switch_to_consult), win);
}

static void set_delete_consult_win_objs(GtkBuilder *builder, GObject **win) {
    GObject *id_del_consult_label = gtk_builder_get_object(builder, "id_del_consult_label");
    GObject *id_del_patient_label = gtk_builder_get_object(builder, "id_del_patient_label");
    GObject *symptoms_del_label = gtk_builder_get_object(builder, "symptoms_del_label");
    GObject *diagnosis_del_label = gtk_builder_get_object(builder, "diagnosis_del_label");
    GObject *treatmentPlan_del_label = gtk_builder_get_object(builder, "treatmentPlan_del_label");

    GObject *id_del_consult_entry = gtk_builder_get_object(builder, "id_del_consult_entry");
    GObject *id_del_patient_entry = gtk_builder_get_object(builder, "id_del_patient_entry");
    GObject *symptoms_del_entry = gtk_builder_get_object(builder, "symptoms_del_entry");
    GObject *diagnosis_del_entry = gtk_builder_get_object(builder, "diagnosis_del_entry");
    GObject *treatmentPlan_del_entry = gtk_builder_get_object(builder, "treatmentPlan_del_entry");

    GObject *id_del_consult_btn = gtk_builder_get_object(builder, "id_del_consult_btn");
    GObject *del_consult_btn = gtk_builder_get_object(builder, "del_consult_btn");
    GObject *return_del_consult_btn = gtk_builder_get_object(builder, "return_del_consult_btn");

    GObject **entries = g_new(GObject*, 5);
    entries[0] = id_del_consult_entry;
    entries[1] = id_del_patient_entry;
    entries[2] = symptoms_del_entry;
    entries[3] = diagnosis_del_entry;
    entries[4] = treatmentPlan_del_entry;

    g_signal_connect(id_del_consult_btn, "clicked", G_CALLBACK(g_find_consult), entries);
    g_signal_connect(del_consult_btn, "clicked", G_CALLBACK(g_modify_consult), entries);
    g_signal_connect(return_del_consult_btn, "clicked", G_CALLBACK(switch_to_consult), win);
}

static void set_display_consult_win_objs(GtkBuilder *builder, GObject **win){}

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
    gtk_window_set_application(GTK_WINDOW(wins[5]), app);
    gtk_window_set_application(GTK_WINDOW(wins[6]), app);
    gtk_window_set_application(GTK_WINDOW(wins[7]), app);
    gtk_window_set_application(GTK_WINDOW(wins[8]), app);
    // gtk_window_set_application(GTK_WINDOW(wins[9]), app);
    // gtk_window_set_application(GTK_WINDOW(wins[10]), app);
    // gtk_window_set_application(GTK_WINDOW(wins[11]), app);

    set_login_win_objs(builder, wins);
    set_menu_win_objs(builder, wins);
    set_patient_win_objs(builder, wins);
    set_add_patient_win_objs(builder, wins);
    set_modify_patient_win_objs(builder, wins);
    set_delete_patient_win_objs(builder, wins);
    set_display_patient_win_objs(builder, wins);
    set_consult_win_objs(builder, wins);
    set_add_consult_win_objs(builder, wins);
    set_modify_consult_win_objs(builder, wins);
    // set_delete_consult_win_objs(builder, wins);
    // set_display_consult_win_objs(builder, wins);

    gtk_widget_set_visible(GTK_WIDGET(wins[0]), TRUE);
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

