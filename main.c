#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "consultation.h"
#include "patient.h"

static void window_close(GtkButton btn, GtkWindow *window) {
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

// Patient

static void g_add_patient(GtkButton *btn, gpointer data) {
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

    if (!entry_id || !entry_lName || !entry_fName || !entry_age || !entry_addr || !entry_phone) {
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
        if (!ifPatientExists(id_value)){
            addPatient(patient);
        } else {
            printf("Patient already exists\n");
        }

    } else {
        g_printerr("Error: Patient is NULL\n");
    }
}

static void add_patient_popup(GtkButton *btn, gpointer data) {
    // Create the popup window
    GtkWidget *popup_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(popup_window), "Ajouter un patient");
    gtk_window_set_default_size(GTK_WINDOW(popup_window), 300, 600);

    // Create a vertical box for layout
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(popup_window), box);

    // Create and set up the label for ID
    GtkWidget *id_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(id_entry), "Id");

    GtkWidget *lName_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(lName_entry), "Last Name");

    GtkWidget *fName_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(fName_entry), "First Name");

    GtkWidget *age_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(age_entry), "Age");

    GtkWidget *addr_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(addr_entry), "Address");

    GtkWidget *phone_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(phone_entry), "Phone");

    GtkWidget **entries =  g_new(GtkWidget*, 6);
    entries[0] = id_entry;
    entries[1] = lName_entry;
    entries[2] = fName_entry;
    entries[3] = age_entry;
    entries[4] = addr_entry;
    entries[5] = phone_entry;

    // Create the buttons
    GtkWidget *add_btn = gtk_button_new_with_label("ajouter");
    GtkWidget *cancel_btn = gtk_button_new_with_label("Cancel");

    // Connect the cancel button to close the window
    g_signal_connect(add_btn, "clicked", G_CALLBACK(g_add_patient), entries);
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(window_close), GTK_WINDOW(popup_window));

    // Append widgets to the box
    gtk_box_append(GTK_BOX(box), id_entry);
    gtk_box_append(GTK_BOX(box), lName_entry);
    gtk_box_append(GTK_BOX(box), fName_entry);
    gtk_box_append(GTK_BOX(box), age_entry);
    gtk_box_append(GTK_BOX(box), addr_entry);
    gtk_box_append(GTK_BOX(box), phone_entry);
    gtk_box_append(GTK_BOX(box), add_btn);
    gtk_box_append(GTK_BOX(box), cancel_btn);

    // Show the popup window
    gtk_window_present(GTK_WINDOW(popup_window));
}

static void g_delete_patient(GtkButton *btn, gpointer data) {
    int *id_value = data;
    printf("Patient with id: %d have been deleted\n", *id_value);
    deletePatient(*id_value);
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
    Patient *patient = getPatient(id_pt);
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

static void g_edit_patient(GtkButton *btn, gpointer data) {
    GObject **entries = (GObject **)data;
    if (!entries) {
        g_printerr("Error: entries is NULL\n");
        return;
    }

    // Access each entry and print debug information
    GtkLabel *label = GTK_LABEL(entries[0]);
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
    const char* id_text = gtk_label_get_label(label);
    int id_value = strtol(id_text, NULL, 10);
    g_print("id: %d\n", id_value);

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
}

static void edit_patient_popup(GtkButton *btn, gpointer data) {
    const int *id = (int *) data;

    // Create the popup window
    GtkWidget *popup_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(popup_window), "Edit Patient");
    gtk_window_set_default_size(GTK_WINDOW(popup_window), 300, 600);

    // Create a vertical box for layout
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(popup_window), box);

    // Create and set up the label for ID
    GtkWidget *id_label = gtk_label_new(NULL);
    char id_text[10];
    snprintf(id_text, sizeof(id_text), "%d", *id);
    gtk_label_set_text(GTK_LABEL(id_label), id_text);

    // Create the entry fields
    GtkWidget *lName_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(lName_entry), "Last Name");

    GtkWidget *fName_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(fName_entry), "First Name");

    GtkWidget *age_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(age_entry), "Age");

    GtkWidget *addr_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(addr_entry), "Address");

    GtkWidget *phone_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(phone_entry), "Phone");

    GtkWidget **entries =  g_new(GtkWidget*, 6);
    entries[0] = id_label;
    entries[1] = lName_entry;
    entries[2] = fName_entry;
    entries[3] = age_entry;
    entries[4] = addr_entry;
    entries[5] = phone_entry;

    // Create the buttons
    GtkWidget *modify_btn = gtk_button_new_with_label("Modify");
    GtkWidget *cancel_btn = gtk_button_new_with_label("Cancel");

    // Connect the cancel button to close the window
    g_signal_connect(modify_btn, "clicked", G_CALLBACK(g_edit_patient), entries);
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(window_close), GTK_WINDOW(popup_window));

    // Append widgets to the box
    gtk_box_append(GTK_BOX(box), id_label);
    gtk_box_append(GTK_BOX(box), lName_entry);
    gtk_box_append(GTK_BOX(box), fName_entry);
    gtk_box_append(GTK_BOX(box), age_entry);
    gtk_box_append(GTK_BOX(box), addr_entry);
    gtk_box_append(GTK_BOX(box), phone_entry);
    gtk_box_append(GTK_BOX(box), modify_btn);
    gtk_box_append(GTK_BOX(box), cancel_btn);

    // Show the popup window
    gtk_window_present(GTK_WINDOW(popup_window));
}


static void g_update_display_patient(GtkButton *btn, gpointer data) {

    Patient *patients = getPatients();
    int patient_count = getNumbOfPatient();
    GtkGrid *grid = GTK_GRID(data);

    int col= 0, row = 2;
    while (gtk_grid_get_child_at(grid, col, row) != NULL) {
        while (col < 8) {
            GtkWidget *child = gtk_grid_get_child_at(grid, col, row);
            if (child != NULL) {
                gtk_widget_unparent(child);
            }
            col++;
        }
        row++;
        col = 0;
    }

    for (int i = 0; i < patient_count; i++) {
        char id_text[20], age_text[10];
        snprintf(id_text, sizeof(id_text), "%d", patients[i].id);
        snprintf(age_text, sizeof(age_text), "%d", patients[i].age);
        GtkWidget *id_label = gtk_label_new(id_text);
        GtkWidget *lName_label = gtk_label_new(patients[i].lName);
        GtkWidget *fName_label = gtk_label_new(patients[i].fName);
        GtkWidget *age_label = gtk_label_new(age_text);
        GtkWidget *addr_label = gtk_label_new(patients[i].address);
        GtkWidget *phone_label = gtk_label_new(patients[i].phone);
        GtkWidget *edit_btn = gtk_button_new_with_label("edit");
        GtkWidget *delete_btn = gtk_button_new_with_label("delete");

        // Attach labels to the grid for each row of patient data
        gtk_grid_attach(grid, id_label, 0, i + 2, 1, 1);
        gtk_grid_attach(grid, lName_label, 1, i + 2, 1, 1);
        gtk_grid_attach(grid, fName_label, 2, i + 2, 1, 1);
        gtk_grid_attach(grid, age_label, 3, i + 2, 1, 1);
        gtk_grid_attach(grid, addr_label, 4, i + 2, 1, 1);
        gtk_grid_attach(grid, phone_label, 5, i + 2, 1, 1);
        gtk_grid_attach(grid, edit_btn, 6, i + 2, 1, 1);
        gtk_grid_attach(grid, delete_btn, 7, i + 2, 1, 1);

        g_signal_connect(delete_btn, "clicked", G_CALLBACK(g_delete_patient), &patients[i].id);
        g_signal_connect(edit_btn, "clicked", G_CALLBACK(edit_patient_popup), &patients[i].id);
    }
}

// Consultation
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
        Consult consult;
        consult.id = id_consult_value;
        consult.patient_id = id_patient_value;
        strcpy(consult.symptoms, symptoms);
        strcpy(consult.diagnosis, diagnosis);
        strcpy(consult.treatmentPlan, treatmentPlan);
        addConsultation(&consult);
    } else {
        g_printerr("Error: Consultation is NULL\n");
    }
}

static void add_consult_popup(GtkButton *btn, gpointer data) {
    // Create the popup window
    GtkWidget *popup_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(popup_window), "Ajouter une consultation");
    gtk_window_set_default_size(GTK_WINDOW(popup_window), 300, 600);

    // Create a vertical box for layout
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(popup_window), box);

    GtkWidget *id_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(id_entry), "Id");

    GtkWidget *id_pt_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(id_pt_entry), "Patient Id");

    GtkWidget *name_pt_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(name_pt_entry), "Patient Name");

    GtkWidget *symptoms_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(symptoms_entry), "Symptoms");

    GtkWidget *diagnosis_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(diagnosis_entry), "Diagnosis");

    GtkWidget *treatmentPlan_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(treatmentPlan_entry), "Treatment plan");


    GtkWidget **entries =  g_new(GtkWidget*, 6);
    entries[0] = id_entry;
    entries[1] = id_pt_entry;
    entries[2] = symptoms_entry;
    entries[3] = diagnosis_entry;
    entries[4] = treatmentPlan_entry;

    // Create the buttons
    GtkWidget *add_btn = gtk_button_new_with_label("ajouter");
    GtkWidget *cancel_btn = gtk_button_new_with_label("Cancel");

    // Connect the cancel button to close the window
    g_signal_connect(add_btn, "clicked", G_CALLBACK(g_add_consult), entries);
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(window_close), GTK_WINDOW(popup_window));

    // Append widgets to the box
    gtk_box_append(GTK_BOX(box), id_entry);
    gtk_box_append(GTK_BOX(box), id_pt_entry);
    gtk_box_append(GTK_BOX(box), name_pt_entry);
    gtk_box_append(GTK_BOX(box), symptoms_entry);
    gtk_box_append(GTK_BOX(box), diagnosis_entry);
    gtk_box_append(GTK_BOX(box), treatmentPlan_entry);
    gtk_box_append(GTK_BOX(box), add_btn);
    gtk_box_append(GTK_BOX(box), cancel_btn);

    // Show the popup window
    gtk_window_present(GTK_WINDOW(popup_window));
}

static void g_delete_consult(GtkButton *btn, gpointer data) {
    int *id_value = data;
    printf("Consultation with id: %d have been deleted\n", *id_value);
    deleteConsultation(*id_value);
}

static void g_edit_consult(GtkButton *btn, gpointer data) {}

static void edit_consult_popup(GtkButton *btn, gpointer data) {}

static void g_update_display_consult(GtkButton *btn, gpointer data) {
    Consult *consults = getConsultations();
    int consult_count = getNumbOfConsults();
    printf("Total Consultation: %d\n", consult_count);
    displayAllConsultation();

    GtkGrid *grid = GTK_GRID(data);

    int col= 0, row = 2;
    while (gtk_grid_get_child_at(grid, col, row) != NULL) {
        while (col < 8) {
            GtkWidget *child = gtk_grid_get_child_at(grid, col, row);
            if (child != NULL) {
                gtk_widget_unparent(child);
            }
            col++;
        }
        row++;
        col = 0;
    }

    for (int i = 0; i < consult_count; i++) {
        char id_text[10], id_pt_text[10];

        snprintf(id_text, sizeof(id_text), "%d", consults[i].id);
        snprintf(id_pt_text, sizeof(id_pt_text), "%d", consults[i].patient_id);

        GtkWidget *id_label = gtk_label_new(id_text);
        GtkWidget *id_pt_label = gtk_label_new(id_pt_text);

        Patient* patient = getPatient(consults[i].patient_id);
        if (patient == NULL) {
            printf("Patient with ID %d not found\n", consults[i].patient_id);
            continue;
        }

        char fullName_pt[40];
        snprintf(fullName_pt, sizeof(fullName_pt), "%s %s", patient->lName, patient->fName);

        GtkWidget *name_label = gtk_label_new(fullName_pt);
        GtkWidget *symptoms_label = gtk_label_new(consults[i].symptoms);
        GtkWidget *diagnosis_label = gtk_label_new(consults[i].diagnosis);
        GtkWidget *treatmentPlan_label = gtk_label_new(consults[i].treatmentPlan);
        GtkWidget *edit_btn = gtk_button_new_with_label("edit");
        GtkWidget *delete_btn = gtk_button_new_with_label("delete");

        // Attach labels to the grid for each row of patient data
        gtk_grid_attach(grid, id_label, 0, i + 2, 1, 1);
        gtk_grid_attach(grid, id_pt_label, 1, i + 2, 1, 1);
        gtk_grid_attach(grid, name_label, 2, i + 2, 1, 1);
        gtk_grid_attach(grid, symptoms_label, 3, i + 2, 1, 1);
        gtk_grid_attach(grid, diagnosis_label, 4, i + 2, 1, 1);
        gtk_grid_attach(grid, treatmentPlan_label, 5, i + 2, 1, 1);
        gtk_grid_attach(grid, edit_btn, 6, i + 2, 1, 1);
        gtk_grid_attach(grid, delete_btn, 7, i + 2, 1, 1);

        g_signal_connect(delete_btn, "clicked", G_CALLBACK(g_delete_consult), &consults[i].id);
        g_signal_connect(edit_btn, "clicked", G_CALLBACK(edit_consult_popup), &consults[i].id);
    }
}

//===================================================================//
// window creation & filling
//===================================================================//

static GObject** init_GObj_win(GtkBuilder *builder) {

    GObject **wins = g_malloc(6*sizeof(GObject*));

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

    wins[2] = gtk_builder_get_object(builder, "patientWin");
    if (!wins[2]) {
        g_printerr("Failed to get 'Display Patient' window from builder UI file\n");
        g_free(wins);
        return NULL;
    }

    wins[3] = gtk_builder_get_object(builder, "consultWindow");
    if (!wins[3]) {
        g_printerr("Failed to get 'Consultation' window from builder UI file\n");
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

static void switch_to_consult(GtkButton *btn, gpointer user_data) {
    GObject **windows = (GObject **)user_data;
    GtkWidget *window_consult = GTK_WIDGET(windows[3]);
    GtkWidget *window_prev = gtk_widget_get_ancestor(GTK_WIDGET(btn), GTK_TYPE_WINDOW);

    if (GTK_IS_WINDOW(window_prev)) {
        gtk_widget_set_visible(window_prev, FALSE);
        gtk_widget_set_visible(window_consult, TRUE);
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
static void set_patient_win(GtkBuilder *builder, GObject **win) {

    GtkGrid *grid = GTK_GRID(gtk_builder_get_object(builder, "grid_patient"));

    // Define grid headers
    GtkWidget *id_header = gtk_label_new("ID");
    GtkWidget *lName_header = gtk_label_new("Last Name");
    GtkWidget *fName_header = gtk_label_new("First Name");
    GtkWidget *age_header = gtk_label_new("Age");
    GtkWidget *addr_header = gtk_label_new("Address");
    GtkWidget *phone_header = gtk_label_new("Phone");

    // Add headers to the first row of the grid
    gtk_grid_attach(grid, id_header, 0, 1, 1, 1);
    gtk_grid_attach(grid, lName_header, 1, 1, 1, 1);
    gtk_grid_attach(grid, fName_header, 2, 1, 1, 1);
    gtk_grid_attach(grid, age_header, 3, 1, 1, 1);
    gtk_grid_attach(grid, addr_header, 4, 1, 1, 1);
    gtk_grid_attach(grid, phone_header, 5, 1, 1, 1);

    g_update_display_patient(NULL, grid);

    // Create and attach buttons after the last row of patient data
    GObject *refresh_pt_btn = gtk_builder_get_object(builder, "refresh_pt_btn");
    GObject *add_pt_btn = gtk_builder_get_object(builder, "add_pt_btn");
    GObject *return_pt_btn = gtk_builder_get_object(builder, "return_pt_btn");

    // Connect button signals to their respective callbacks
    g_signal_connect(refresh_pt_btn, "clicked", G_CALLBACK(g_update_display_patient), grid);
    g_signal_connect(add_pt_btn,"clicked", G_CALLBACK(add_patient_popup), NULL);
    g_signal_connect(return_pt_btn, "clicked", G_CALLBACK(switch_to_menu), win);

}

// set consultation windows objects
static void set_consult_win(GtkBuilder *builder, GObject **win) {

    GtkGrid *grid = GTK_GRID(gtk_builder_get_object(builder, "grid_consult"));

    // Define grid headers
    GtkWidget *id_header = gtk_label_new("Consultation ID");
    GtkWidget *id_pt_header = gtk_label_new("Patient ID");
    GtkWidget *name_pt_header = gtk_label_new("Patient Name");
    GtkWidget *symptoms_header = gtk_label_new("Symptoms");
    GtkWidget *diagnosis_header = gtk_label_new("Diagnosis");
    GtkWidget *treatmentPlan_header = gtk_label_new("Treatment Plan");

    // Add headers to the first row of the grid
    gtk_grid_attach(grid, id_header, 0, 1, 1, 1);
    gtk_grid_attach(grid, id_pt_header, 1, 1, 1, 1);
    gtk_grid_attach(grid, name_pt_header, 2, 1, 1, 1);
    gtk_grid_attach(grid, symptoms_header, 3, 1, 1, 1);
    gtk_grid_attach(grid, diagnosis_header, 4, 1, 1, 1);
    gtk_grid_attach(grid, treatmentPlan_header, 5, 1, 1, 1);

    g_update_display_consult(NULL, grid);

    // Create and attach buttons after the last row of patient data
    GObject *refresh_consult_btn = gtk_builder_get_object(builder, "refresh_consult_btn");
    GObject *add_consult_btn = gtk_builder_get_object(builder, "add_consult_btn");
    GObject *return_consult_btn = gtk_builder_get_object(builder, "return_consult_btn");

    // Connect button signals to their respective callbacks
    g_signal_connect(refresh_consult_btn, "clicked", G_CALLBACK(g_update_display_consult), grid);
    g_signal_connect(add_consult_btn,"clicked", G_CALLBACK(add_consult_popup), NULL);
    g_signal_connect(return_consult_btn, "clicked", G_CALLBACK(switch_to_menu), win);

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

    set_login_win_objs(builder, wins);
    set_menu_win_objs(builder, wins);
    set_patient_win(builder, wins);
    set_consult_win(builder, wins);

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

