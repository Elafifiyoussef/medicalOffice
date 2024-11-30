#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "consultation.h"
#include "patient.h"
#include "rendezvous.h"

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

static const char* get_text_from_entry(GtkWidget *entry) {
    GtkEntryBuffer *entryBuffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
    const char* string = gtk_entry_buffer_get_text(entryBuffer);
    return string;
}

static void set_text_entry(GtkWidget *entry, const char *text) {
    GtkEntryBuffer *buffer = gtk_entry_buffer_new(text, -1);
    // Create a GtkEntry and set its buffer
    gtk_entry_set_buffer(GTK_ENTRY(entry), buffer);
}

static int get_int_from_entry(GtkWidget *entry) {
    GtkEntryBuffer *entryBuffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
    const char* string = gtk_entry_buffer_get_text(entryBuffer);
    const int value = strtol(string, NULL, 10);
    return value;
}

int isCinValid(const char* cin) {
    if (cin == NULL || strlen(cin) == 0 || strlen(cin) > 9) {
        return 0;
    }
    for (int i = 0; cin[i] != '\0'; i++) {
        if ((cin[i] > 'z' || cin[i] < 'a') && (cin[i] > 'Z' || cin[i] < 'A') && (cin[i] > '9' || cin[i] < '0')) {
            return 0;
        }
    }
    return 1;
}

int isNameValid(const char* name) {
    //checking the length of name
    if (strlen(name) > 20 || strlen(name) < 3) {
        return 0;
    }
    // checking if name contain a number
    for (int i = 0; name[i] != '\0'; i++) {
        if ((name[i] > 'z' || name[i] < 'A') && name[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

int isAgeValid(int age) {
    if (age < 18 || age > 120) {
        return 0;
    }
    return 1;
}

int isPhoneValid(const char* phone) {
    if (strlen(phone) != 10) {
        return 0;
    }
    for (int i = 0; phone[i] != '\0'; i++) {
        if (phone[i] < '0' || phone[i] > '9') {
            return 0;
        }
    }
    return 1;
}

int isTextValid(const char* text) {
    if (!strlen(text)) {
        return 0;
    }
    return 1;
}

const char* get_dropdown_item(GtkDropDown *dropdown) {
    GtkStringObject *item = gtk_drop_down_get_selected_item(dropdown);
    const char *year_str = gtk_string_object_get_string(item);
    return year_str;
}

int find_month_index(const char* target) {
    const char months[12][12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    for (int i = 0; i < 12 ; i++) {
        if (strcmp(target, months[i]) == 0) {
            return i+1;
        }
    }
    return -1;
}

// The mapping function that converts month names to integers
void display_string_months(GListModel *model) {
    guint n_items = g_list_model_get_n_items(model); // Get number of items in the model
    // for (guint i = 0; i < n_items; i++) {
        gpointer *item = g_list_model_get_item(model, 1); // Retrieve the item at index i
        const char* month = gtk_string_object_get_string(GTK_STRING_OBJECT(item));
        g_print("Month %d: %s\n", 1, month);
    // }
}

//===================================================================//
// Patient functionalities
//===================================================================//

static void g_add_patient(GtkButton *btn, gpointer data) {
    GObject **object = (GObject **)data;
    if (!object) {
        g_printerr("Error: Object is NULL\n");
        return;
    }
    // Entries
    GtkEntry *entry_cin = GTK_ENTRY(object[0]);
    GtkEntry *entry_lName = GTK_ENTRY(object[1]);
    GtkEntry *entry_fName = GTK_ENTRY(object[2]);
    GtkEntry *entry_age = GTK_ENTRY(object[3]);
    GtkEntry *entry_addr = GTK_ENTRY(object[4]);
    GtkEntry *entry_phone = GTK_ENTRY(object[5]);

    // Labels
    GtkLabel *label_cin = GTK_LABEL(object[6]);
    GtkLabel *label_lName = GTK_LABEL(object[7]);
    GtkLabel *label_fName = GTK_LABEL(object[8]);
    GtkLabel *label_age = GTK_LABEL(object[9]);
    GtkLabel *label_addr = GTK_LABEL(object[10]);
    GtkLabel *label_phone = GTK_LABEL(object[11]);

    // Retrieve values from each entry
    const char* cin_id = get_text_from_entry(GTK_WIDGET(entry_cin));
    const char* lName = get_text_from_entry(GTK_WIDGET(entry_lName));
    const char* fName = get_text_from_entry(GTK_WIDGET(entry_fName));
    int age_value = get_int_from_entry(GTK_WIDGET(entry_age));
    const char* addr = get_text_from_entry(GTK_WIDGET(entry_addr));
    const char* phone = get_text_from_entry(GTK_WIDGET(entry_phone));

    if (!isCinValid(cin_id)) {
        gtk_label_set_text(label_cin, "CIN can only contain letters and numbers ([A-Za-z][0-9]) ()!");
    } else {
        gtk_label_set_text(label_cin, "");
    }

    if (!isNameValid(lName)) {
        gtk_label_set_text(label_lName, "Name can only contain letters (A-Z or a-z) and space if needed!");
    } else {
        gtk_label_set_text(label_lName, "");
    }
    if (!isNameValid(fName)) {
        gtk_label_set_text(label_fName, "Name can only contain letters (A-Z or a-z) and space if needed!");
    } else {
        gtk_label_set_text(label_fName, "");
    }
    if (!isAgeValid(age_value)) {
        gtk_label_set_text(label_age, "Age must be a number between 18 and 120!");
    } else {
        gtk_label_set_text(label_age, "");
    }
    if (!isTextValid(addr)) {
        gtk_label_set_text(label_addr, "address should not left empty!");
    } else {
        gtk_label_set_text(label_addr, "");
    }
    if (!isPhoneValid(phone)) {
        gtk_label_set_text(label_phone, "Phone can only contain numbers (0-9)!");
    } else {
        gtk_label_set_text(label_phone, "");
    }

    if (cin_id != NULL && isNameValid(lName) && isNameValid(fName) && isAgeValid(age_value) && isTextValid(addr) && isPhoneValid(phone) ) {
        Patient patient;
        strcpy(patient.cin, cin_id);
        strcpy(patient.lName, lName);
        strcpy(patient.fName, fName);
        patient.age = age_value;
        strcpy(patient.address, addr);
        strcpy(patient.phone, phone);
        if (!ifPatientExists(cin_id)){
            addPatient(patient);
        } else {
            printf("Patient already exists\n");
        }
    } else {
        g_printerr("Error: Patient information are not correct\n");
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

    GtkWidget *cin_err_label = gtk_label_new("");

    GtkWidget *cin_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(cin_entry), "CIN");

    GtkWidget *lName_err_label = gtk_label_new("");

    GtkWidget *lName_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(lName_entry), "Last Name");

    GtkWidget *fName_err_label = gtk_label_new("");

    GtkWidget *fName_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(fName_entry), "First Name");

    GtkWidget *age_err_label = gtk_label_new("");

    GtkWidget *age_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(age_entry), "Age");

    GtkWidget *addr_err_label = gtk_label_new("");

    GtkWidget *addr_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(addr_entry), "Address");

    GtkWidget *phone_err_label = gtk_label_new("");

    GtkWidget *phone_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(phone_entry), "Phone");

    GtkWidget **widget =  g_new(GtkWidget*, 12);
    widget[0] = cin_entry;
    widget[1] = lName_entry;
    widget[2] = fName_entry;
    widget[3] = age_entry;
    widget[4] = addr_entry;
    widget[5] = phone_entry;
    widget[6] = cin_err_label;
    widget[7] = lName_err_label;
    widget[8] = fName_err_label;
    widget[9] = age_err_label;
    widget[10] = addr_err_label;
    widget[11] = phone_err_label;


    // Create the buttons
    GtkWidget *add_btn = gtk_button_new_with_label("ajouter");
    GtkWidget *cancel_btn = gtk_button_new_with_label("Cancel");

    // Connect the cancel button to close the window
    g_signal_connect(add_btn, "clicked", G_CALLBACK(g_add_patient), widget);
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(window_close), GTK_WIDGET(popup_window));

    // Append widgets to the box
    gtk_box_append(GTK_BOX(box), cin_err_label);
    gtk_box_append(GTK_BOX(box), cin_entry);
    gtk_box_append(GTK_BOX(box), lName_err_label);
    gtk_box_append(GTK_BOX(box), lName_entry);
    gtk_box_append(GTK_BOX(box), fName_err_label);
    gtk_box_append(GTK_BOX(box), fName_entry);
    gtk_box_append(GTK_BOX(box), age_err_label);
    gtk_box_append(GTK_BOX(box), age_entry);
    gtk_box_append(GTK_BOX(box), addr_err_label);
    gtk_box_append(GTK_BOX(box), addr_entry);
    gtk_box_append(GTK_BOX(box), phone_err_label);
    gtk_box_append(GTK_BOX(box), phone_entry);
    gtk_box_append(GTK_BOX(box), add_btn);
    gtk_box_append(GTK_BOX(box), cancel_btn);

    // Show the popup window
    gtk_window_present(GTK_WINDOW(popup_window));
}

static void g_delete_patient(GtkButton *btn, gpointer data) {
    char *cin_id = data;
    printf("Patient with id: %s have been deleted\n", cin_id);
    deletePatient(cin_id);
}

static void delete_patient_popup(GtkButton *btn, gpointer data) {
    char* cin = data;
    GtkWidget *popup_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(popup_window), "confirmer la suppression ");
    gtk_window_set_default_size(GTK_WINDOW(popup_window), 400, 400);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(popup_window), box);

    char msg[100];
    sprintf(msg, "are you sure about deleting the patient %s", cin);

    GtkWidget *msg_label = gtk_label_new(msg);
    GtkWidget *del_btn = gtk_button_new_with_label("supprimer");
    GtkWidget *cancel_btn = gtk_button_new_with_label("Cancel");

    gtk_box_append(GTK_BOX(box), msg_label);
    gtk_box_append(GTK_BOX(box), del_btn);
    gtk_box_append(GTK_BOX(box), cancel_btn);

    g_signal_connect(del_btn, "clicked", G_CALLBACK(g_delete_patient), data);
    g_signal_connect(del_btn, "clicked", G_CALLBACK(window_close), GTK_WIDGET(popup_window));
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(window_close), GTK_WIDGET(popup_window));

    gtk_window_present(GTK_WINDOW(popup_window));
}

static void g_edit_patient(GtkButton *btn, gpointer data) {
    GtkWidget **widgets = data;
    if (!widgets) {
        g_printerr("Error: widgets is NULL\n");
        return;
    }

    // Access each entry and print debug information
    GtkEntry *entry_cin = GTK_ENTRY(widgets[0]);
    GtkEntry *entry_lName = GTK_ENTRY(widgets[1]);
    GtkEntry *entry_fName = GTK_ENTRY(widgets[2]);
    GtkEntry *entry_age = GTK_ENTRY(widgets[3]);
    GtkEntry *entry_addr = GTK_ENTRY(widgets[4]);
    GtkEntry *entry_phone = GTK_ENTRY(widgets[5]);

    // Labels
    GtkLabel *label_cin = GTK_LABEL(widgets[6]);
    GtkLabel *label_lName = GTK_LABEL(widgets[7]);
    GtkLabel *label_fName = GTK_LABEL(widgets[8]);
    GtkLabel *label_age = GTK_LABEL(widgets[9]);
    GtkLabel *label_addr = GTK_LABEL(widgets[10]);
    GtkLabel *label_phone = GTK_LABEL(widgets[11]);

    if (!entry_lName || !entry_fName || !entry_age || !entry_addr || !entry_phone) {
        g_printerr("Error: One of the entries is NULL\n");
        return;
    }

    // Retrieve values from each entry
    const char* cin_id = get_text_from_entry(GTK_WIDGET(entry_cin));
    const char* lName = get_text_from_entry(GTK_WIDGET(entry_lName));
    const char* fName = get_text_from_entry(GTK_WIDGET(entry_fName));
    int age_value = get_int_from_entry(GTK_WIDGET(entry_age));
    const char* addr = get_text_from_entry(GTK_WIDGET(entry_addr));
    const char* phone = get_text_from_entry(GTK_WIDGET(entry_phone));

    if (!isCinValid(cin_id)) {
        gtk_label_set_text(label_cin, "CIN can only contain letters and numbers ([A-Za-z][0-9]) ()!");
    } else {
        gtk_label_set_text(label_cin, "");
    }

    if (!isNameValid(lName)) {
        gtk_label_set_text(label_lName, "Name can only contain letters (A-Z or a-z) and space if needed!");
    } else {
        gtk_label_set_text(label_lName, "");
    }
    if (!isNameValid(fName)) {
        gtk_label_set_text(label_fName, "Name can only contain letters (A-Z or a-z) and space if needed!");
    } else {
        gtk_label_set_text(label_fName, "");
    }
    if (!isAgeValid(age_value)) {
        gtk_label_set_text(label_age, "Age must be a number between 18 and 120!");
    } else {
        gtk_label_set_text(label_age, "");
    }
    if (!isTextValid(addr)) {
        gtk_label_set_text(label_addr, "address should not left empty!");
    } else {
        gtk_label_set_text(label_addr, "");
    }
    if (!isPhoneValid(phone)) {
        gtk_label_set_text(label_phone, "Phone can only contain numbers (0-9)!");
    } else {
        gtk_label_set_text(label_phone, "");
    }

    if (cin_id != NULL && isNameValid(lName) && isNameValid(fName) && isAgeValid(age_value) && isTextValid(addr) && isPhoneValid(phone) ) {
        Patient patient;
        strcpy(patient.cin, cin_id);
        strcpy(patient.lName, lName);
        strcpy(patient.fName, fName);
        patient.age = age_value;
        strcpy(patient.address, addr);
        strcpy(patient.phone, phone);
        modifyPatient(patient);

    } else {
        g_printerr("Error: Patient information are not correct\n");
    }
}

static void edit_patient_popup(GtkButton *btn, gpointer data) {
    const Patient *patient = data;

    // Create the popup window
    GtkWidget *popup_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(popup_window), "Edit Patient");
    gtk_window_set_default_size(GTK_WINDOW(popup_window), 300, 600);

    // Create a vertical box for layout
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(popup_window), box);

    GtkWidget *cin_err_label = gtk_label_new("");

    GtkWidget *cin_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(cin_entry), "CIN");
    set_text_entry(cin_entry, patient->cin);

    GtkWidget *lName_err_label = gtk_label_new("");

    GtkWidget *lName_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(lName_entry), "Last Name");
    set_text_entry(lName_entry, patient->lName);

    GtkWidget *fName_err_label = gtk_label_new("");

    GtkWidget *fName_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(fName_entry), "First Name");
    set_text_entry(fName_entry, patient->fName);

    GtkWidget *age_err_label = gtk_label_new("");

    GtkWidget *age_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(age_entry), "Age");
    char age_text[4];
    sprintf(age_text, "%d", patient->age);
    set_text_entry(age_entry, age_text);

    GtkWidget *addr_err_label = gtk_label_new("");

    GtkWidget *addr_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(addr_entry), "Address");
    set_text_entry(addr_entry, patient->address);

    GtkWidget *phone_err_label = gtk_label_new("");

    GtkWidget *phone_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(phone_entry), "Phone");
    set_text_entry(phone_entry, patient->phone);

    GtkWidget **widget =  g_new(GtkWidget*, 12);
    widget[0] = cin_entry;
    widget[1] = lName_entry;
    widget[2] = fName_entry;
    widget[3] = age_entry;
    widget[4] = addr_entry;
    widget[5] = phone_entry;
    widget[6] = cin_err_label;
    widget[7] = lName_err_label;
    widget[8] = fName_err_label;
    widget[9] = age_err_label;
    widget[10] = addr_err_label;
    widget[11] = phone_err_label;

    // Create the buttons
    GtkWidget *modify_btn = gtk_button_new_with_label("Modify");
    GtkWidget *cancel_btn = gtk_button_new_with_label("Cancel");

    // Connect the cancel button to close the window
    g_signal_connect(modify_btn, "clicked", G_CALLBACK(g_edit_patient), widget);
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(window_close), GTK_WINDOW(popup_window));

    // Append widgets to the box
    gtk_box_append(GTK_BOX(box), cin_err_label);
    gtk_box_append(GTK_BOX(box), cin_entry);
    gtk_box_append(GTK_BOX(box), lName_err_label);
    gtk_box_append(GTK_BOX(box), lName_entry);
    gtk_box_append(GTK_BOX(box), fName_err_label);
    gtk_box_append(GTK_BOX(box), fName_entry);
    gtk_box_append(GTK_BOX(box), age_err_label);
    gtk_box_append(GTK_BOX(box), age_entry);
    gtk_box_append(GTK_BOX(box), addr_err_label);
    gtk_box_append(GTK_BOX(box), addr_entry);
    gtk_box_append(GTK_BOX(box), phone_err_label);
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
        char age_text[10];
        snprintf(age_text, sizeof(age_text), "%d", patients[i].age);
        GtkWidget *cin_label = gtk_label_new(patients[i].cin);
        GtkWidget *lName_label = gtk_label_new(patients[i].lName);
        GtkWidget *fName_label = gtk_label_new(patients[i].fName);
        GtkWidget *age_label = gtk_label_new(age_text);
        GtkWidget *addr_label = gtk_label_new(patients[i].address);
        GtkWidget *phone_label = gtk_label_new(patients[i].phone);
        GtkWidget *edit_btn = gtk_button_new_with_label("edit");
        GtkWidget *delete_btn = gtk_button_new_with_label("delete");

        // Attach labels to the grid for each row of patient data
        gtk_grid_attach(grid, cin_label, 0, i + 2, 1, 1);
        gtk_grid_attach(grid, lName_label, 1, i + 2, 1, 1);
        gtk_grid_attach(grid, fName_label, 2, i + 2, 1, 1);
        gtk_grid_attach(grid, age_label, 3, i + 2, 1, 1);
        gtk_grid_attach(grid, addr_label, 4, i + 2, 1, 1);
        gtk_grid_attach(grid, phone_label, 5, i + 2, 1, 1);
        gtk_grid_attach(grid, edit_btn, 6, i + 2, 1, 1);
        gtk_grid_attach(grid, delete_btn, 7, i + 2, 1, 1);

        g_signal_connect(delete_btn, "clicked", G_CALLBACK(delete_patient_popup), patients[i].cin);
        g_signal_connect(edit_btn, "clicked", G_CALLBACK(edit_patient_popup), &patients[i]);
    }
}

//===================================================================//
// Consultation functionalities
//===================================================================//

static void g_add_consult(GtkButton *btn, gpointer data) {
    GtkWidget **widgets = data;
    if (!widgets) {
        g_printerr("Error: objects is NULL\n");
        return;
    }

    //Entries
    GtkEntry *entry_id_consult = GTK_ENTRY(widgets[0]);
    GtkEntry *entry_id_patient = GTK_ENTRY(widgets[1]);
    GtkEntry *entry_symptoms = GTK_ENTRY(widgets[2]);
    GtkEntry *entry_diagnosis = GTK_ENTRY(widgets[3]);
    GtkEntry *entry_treatmentPlan = GTK_ENTRY(widgets[4]);

    //Labels
    GtkLabel *label_id_consult = GTK_LABEL(widgets[5]);
    GtkLabel *label_id_patient = GTK_LABEL(widgets[6]);
    GtkLabel *label_symptoms = GTK_LABEL(widgets[7]);
    GtkLabel *label_diagnosis = GTK_LABEL(widgets[8]);
    GtkLabel *label_treatmentPlan = GTK_LABEL(widgets[9]);

    if (!entry_id_consult || !entry_id_patient || !entry_symptoms || !entry_diagnosis || !entry_treatmentPlan) {
        g_printerr("Error: One of the entries is NULL\n");
        return;
    }

    // Retrieve values from each entry
    int consult_id = get_int_from_entry(GTK_WIDGET(entry_id_consult));
    const char* patient_id = get_text_from_entry(GTK_WIDGET(entry_id_patient));
    const char* symptoms = get_text_from_entry(GTK_WIDGET(entry_symptoms));
    const char* diagnosis = get_text_from_entry(GTK_WIDGET(entry_diagnosis));
    const char* treatmentPlan = get_text_from_entry(GTK_WIDGET(entry_treatmentPlan));

    if (!isCinValid(patient_id)) {
        gtk_label_set_text(label_id_patient, "CIN can only contain letters and numbers ([A-Za-z][0-9]) ()!");
    } else {
        gtk_label_set_text(label_id_patient, "");
    }

    if (!isTextValid(symptoms)) {
        gtk_label_set_text(label_symptoms, "symptoms should not left empty!");
    } else {
        gtk_label_set_text(label_symptoms, "");
    }

    if (!isTextValid(diagnosis)) {
        gtk_label_set_text(label_diagnosis, "diagnosis should not left empty!");
    } else {
        gtk_label_set_text(label_diagnosis, "");
    }

    if (!isTextValid(treatmentPlan)) {
        gtk_label_set_text(label_treatmentPlan, "treatment plan should not left empty!");
    } else {
        gtk_label_set_text(label_treatmentPlan, "");
    }

    if (consult_id != 0 && isCinValid(patient_id) && isTextValid(symptoms) && isTextValid(diagnosis) && isTextValid(treatmentPlan)) {
        Consult consult;
        consult.id = consult_id;
        strcpy(consult.id_pt, patient_id);
        strcpy(consult.symptoms, symptoms);
        strcpy(consult.diagnosis, diagnosis);
        strcpy(consult.treatmentPlan, treatmentPlan);
        if (!ifConsultExists(consult_id) && ifPatientExists(patient_id)) {
            addConsultation(&consult);
        } else {
            g_printerr("Consult already exist\n");
        }
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

    GtkWidget *id_err_label = gtk_label_new("");

    GtkWidget *id_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(id_entry), "Consult Id");

    GtkWidget *id_pt_err_label = gtk_label_new("");

    GtkWidget *id_pt_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(id_pt_entry), "Patient Id (CIN)");

    GtkWidget *symptoms_err_label = gtk_label_new("");

    GtkWidget *symptoms_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(symptoms_entry), "Symptoms");

    GtkWidget *diagnosis_err_label = gtk_label_new("");

    GtkWidget *diagnosis_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(diagnosis_entry), "Diagnosis");

    GtkWidget *treatmentPlan_err_label = gtk_label_new("");

    GtkWidget *treatmentPlan_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(treatmentPlan_entry), "Treatment plan");


    GtkWidget **widgets =  g_new(GtkWidget*, 12);
    widgets[0] = id_entry;
    widgets[1] = id_pt_entry;
    widgets[2] = symptoms_entry;
    widgets[3] = diagnosis_entry;
    widgets[4] = treatmentPlan_entry;
    widgets[5] = id_err_label;
    widgets[6] = id_pt_err_label;
    widgets[7] = symptoms_err_label;
    widgets[8] = diagnosis_err_label;
    widgets[9] = treatmentPlan_err_label;


    // Create the buttons
    GtkWidget *add_btn = gtk_button_new_with_label("ajouter");
    GtkWidget *cancel_btn = gtk_button_new_with_label("Cancel");

    // Connect the cancel button to close the window
    g_signal_connect(add_btn, "clicked", G_CALLBACK(g_add_consult), widgets);
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(window_close), GTK_WINDOW(popup_window));

    // Append widgets to the box
    gtk_box_append(GTK_BOX(box), id_err_label);
    gtk_box_append(GTK_BOX(box), id_entry);
    gtk_box_append(GTK_BOX(box), id_pt_err_label);
    gtk_box_append(GTK_BOX(box), id_pt_entry);
    gtk_box_append(GTK_BOX(box), symptoms_err_label);
    gtk_box_append(GTK_BOX(box), symptoms_entry);
    gtk_box_append(GTK_BOX(box), diagnosis_err_label);
    gtk_box_append(GTK_BOX(box), diagnosis_entry);
    gtk_box_append(GTK_BOX(box), treatmentPlan_err_label);
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

static void delete_consult_popup(GtkButton *btn, gpointer data) {
    int* id = data;
    GtkWidget *popup_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(popup_window), "confirmer la suppression ");
    gtk_window_set_default_size(GTK_WINDOW(popup_window), 400, 150);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(popup_window), box);

    char msg[100];
    sprintf(msg, "are you sure about deleting this consultation (%d)", *id);

    GtkWidget *msg_label = gtk_label_new(msg);
    GtkWidget *del_btn = gtk_button_new_with_label("supprimer");
    GtkWidget *cancel_btn = gtk_button_new_with_label("Cancel");

    gtk_box_append(GTK_BOX(box), msg_label);
    gtk_box_append(GTK_BOX(box), del_btn);
    gtk_box_append(GTK_BOX(box), cancel_btn);

    g_signal_connect(del_btn, "clicked", G_CALLBACK(g_delete_consult), data);
    g_signal_connect(del_btn, "clicked", G_CALLBACK(window_close), GTK_WIDGET(popup_window));
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(window_close), GTK_WIDGET(popup_window));

    gtk_window_present(GTK_WINDOW(popup_window));
}

static void g_edit_consult(GtkButton *btn, gpointer data) {
    GtkWidget **widgets = data;
    if (!widgets) {
        g_printerr("Error: widgets is NULL\n");
        return;
    }

    //Entries
    GtkEntry *entry_id_consult = GTK_ENTRY(widgets[0]);
    GtkEntry *entry_id_patient = GTK_ENTRY(widgets[1]);
    GtkEntry *entry_symptoms = GTK_ENTRY(widgets[2]);
    GtkEntry *entry_diagnosis = GTK_ENTRY(widgets[3]);
    GtkEntry *entry_treatmentPlan = GTK_ENTRY(widgets[4]);

    //Labels
    GtkLabel *label_id_patient = GTK_LABEL(widgets[5]);
    GtkLabel *label_symptoms = GTK_LABEL(widgets[6]);
    GtkLabel *label_diagnosis = GTK_LABEL(widgets[7]);
    GtkLabel *label_treatmentPlan = GTK_LABEL(widgets[8]);

    if (!entry_id_consult || !entry_id_patient || !entry_symptoms || !entry_diagnosis || !entry_treatmentPlan) {
        g_printerr("Error: One of the entries is NULL\n");
        return;
    }

    // Retrieve values from each entry
    int consult_id = get_int_from_entry(GTK_WIDGET(entry_id_consult));
    const char* patient_id = get_text_from_entry(GTK_WIDGET(entry_id_patient));
    const char* symptoms = get_text_from_entry(GTK_WIDGET(entry_symptoms));
    const char* diagnosis = get_text_from_entry(GTK_WIDGET(entry_diagnosis));
    const char* treatmentPlan = get_text_from_entry(GTK_WIDGET(entry_treatmentPlan));

    if (!isCinValid(patient_id)) {
        gtk_label_set_text(label_id_patient, "CIN can only contain letters and numbers ([A-Za-z][0-9]) ()!");
    } else {
        gtk_label_set_text(label_id_patient, "");
    }

    if (!isTextValid(symptoms)) {
        gtk_label_set_text(label_symptoms, "symptoms should not left empty!");
    } else {
        gtk_label_set_text(label_symptoms, "");
    }

    if (!isTextValid(diagnosis)) {
        gtk_label_set_text(label_diagnosis, "diagnosis should not left empty!");
    } else {
        gtk_label_set_text(label_diagnosis, "");
    }

    if (!isTextValid(treatmentPlan)) {
        gtk_label_set_text(label_treatmentPlan, "treatment plan should not left empty!");
    } else {
        gtk_label_set_text(label_treatmentPlan, "");
    }

    if (consult_id != 0 && isCinValid(patient_id) && isTextValid(symptoms) && isTextValid(diagnosis) && isTextValid(treatmentPlan)) {
        Consult consult;
        consult.id = consult_id;
        strcpy(consult.id_pt, patient_id);
        strcpy(consult.symptoms, symptoms);
        strcpy(consult.diagnosis, diagnosis);
        strcpy(consult.treatmentPlan, treatmentPlan);
        if (ifPatientExists(patient_id)){
            modifyConsultation(consult);
            // note that the time is not fixed yet !!!
        } else {
            printf("Patient ID does not exist\n");
        }
    } else {
        g_printerr("Error: Consultation information are not correct\n");
    }
}

static void edit_consult_popup(GtkButton *btn, gpointer data) {
    const Consult *consult = data;
    // Create the popup window
    GtkWidget *popup_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(popup_window), "Ajouter une consultation");
    gtk_window_set_default_size(GTK_WINDOW(popup_window), 300, 600);

    // Create a vertical box for layout
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(popup_window), box);

    GtkWidget *id_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(id_entry), "Consult Id");
    char id_text[10];
    sprintf(id_text, "%d", consult->id);
    set_text_entry(id_entry, id_text);

    GtkWidget *id_pt_err_label = gtk_label_new("");

    GtkWidget *id_pt_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(id_pt_entry), "Patient CIN");
    set_text_entry(id_pt_entry, consult->id_pt);

    GtkWidget *symptoms_err_label = gtk_label_new("");

    GtkWidget *symptoms_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(symptoms_entry), "Symptoms");
    set_text_entry(symptoms_entry, consult->symptoms);

    GtkWidget *diagnosis_err_label = gtk_label_new("");

    GtkWidget *diagnosis_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(diagnosis_entry), "Diagnosis");
    set_text_entry(diagnosis_entry, consult->diagnosis);

    GtkWidget *treatmentPlan_err_label = gtk_label_new("");

    GtkWidget *treatmentPlan_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(treatmentPlan_entry), "Treatment plan");
    set_text_entry(treatmentPlan_entry, consult->treatmentPlan);


    GtkWidget **widget =  g_new(GtkWidget*, 9);
    widget[0] = id_entry;
    widget[1] = id_pt_entry;
    widget[2] = symptoms_entry;
    widget[3] = diagnosis_entry;
    widget[4] = treatmentPlan_entry;
    widget[5] = id_pt_err_label;
    widget[6] = symptoms_err_label;
    widget[7] = diagnosis_err_label;
    widget[8] = treatmentPlan_err_label;


    // Create the buttons
    GtkWidget *add_btn = gtk_button_new_with_label("modifier");
    GtkWidget *cancel_btn = gtk_button_new_with_label("Cancel");

    // Connect the cancel button to close the window
    g_signal_connect(add_btn, "clicked", G_CALLBACK(g_edit_consult), widget);
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(window_close), GTK_WINDOW(popup_window));

    // Append widgets to the box
    gtk_box_append(GTK_BOX(box), id_entry);
    gtk_box_append(GTK_BOX(box), id_pt_err_label);
    gtk_box_append(GTK_BOX(box), id_pt_entry);
    gtk_box_append(GTK_BOX(box), symptoms_err_label);
    gtk_box_append(GTK_BOX(box), symptoms_entry);
    gtk_box_append(GTK_BOX(box), diagnosis_err_label);
    gtk_box_append(GTK_BOX(box), diagnosis_entry);
    gtk_box_append(GTK_BOX(box), treatmentPlan_err_label);
    gtk_box_append(GTK_BOX(box), treatmentPlan_entry);
    gtk_box_append(GTK_BOX(box), add_btn);
    gtk_box_append(GTK_BOX(box), cancel_btn);

    // Show the popup window
    gtk_window_present(GTK_WINDOW(popup_window));
}

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
        char id_text[10];

        snprintf(id_text, sizeof(id_text), "%d", consults[i].id);

        GtkWidget *id_label = gtk_label_new(id_text);
        GtkWidget *id_pt_label = gtk_label_new(consults[i].id_pt);
        Patient* patient = getPatient(consults[i].id_pt);
        if (patient == NULL) {
            printf("Patient with ID %s not found\n", consults[i].id_pt);
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

        g_signal_connect(delete_btn, "clicked", G_CALLBACK(delete_consult_popup), &consults[i].id);
        g_signal_connect(edit_btn, "clicked", G_CALLBACK(edit_consult_popup), &consults[i]);
    }
}

//===================================================================//
// Rendezvous functionalities
//===================================================================//

static void g_add_rdv(GtkButton *btn, gpointer data) {
    GtkWidget **widgets = data;
    if (!widgets) {
        g_printerr("Error: widgets is NULL\n");
        return;
    }

    //Entries
    GtkEntry *entry_id_rdv = GTK_ENTRY(widgets[0]);
    GtkEntry *entry_id_pt = GTK_ENTRY(widgets[1]);
    GtkDropDown *dropdown_year = GTK_DROP_DOWN(widgets[2]);
    GtkDropDown *dropdown_month = GTK_DROP_DOWN(widgets[3]);
    GtkDropDown *dropdown_day = GTK_DROP_DOWN(widgets[4]);
    GtkDropDown *dropdown_state = GTK_DROP_DOWN(widgets[5]);
    //Labels
    GtkLabel *label_id_rdv = GTK_LABEL(widgets[6]);
    GtkLabel *label_id_pt = GTK_LABEL(widgets[7]);
    GtkLabel *label_date = GTK_LABEL(widgets[8]);
    GtkLabel *label_state = GTK_LABEL(widgets[9]);

    if (!entry_id_rdv || !entry_id_pt || !dropdown_year || !dropdown_month || !dropdown_day || !dropdown_state) {
        g_printerr("Error: One of the entries is NULL\n");
        return;
    }

    // Retrieve values from each entry
    int consult_id = get_int_from_entry(GTK_WIDGET(entry_id_rdv));
    const char* patient_id = get_text_from_entry(GTK_WIDGET(entry_id_pt));
    const char* rdv_year = get_dropdown_item(dropdown_year);
    const char* rdv_month = get_dropdown_item(dropdown_month);
    const int month_index = find_month_index(rdv_month);
    const char* rdv_day = get_dropdown_item(dropdown_day);
    printf("Selected year: %s\n", rdv_year);
    printf("Selected month: %s %d\n", rdv_month, month_index);
    printf("Selected day: %s\n", rdv_day);

    // const char* rdv_state = get_text_from_entry(GTK_WIDGET(dropdown_state));

    // if (consult_id != 0 && isCinValid(patient_id) && isTextValid(symptoms) && isTextValid(diagnosis) && isTextValid(treatmentPlan)) {
    //     Consult consult;
    //     consult.id = consult_id;
    //     strcpy(consult.id_pt, patient_id);
    //     strcpy(consult.symptoms, symptoms);
    //     strcpy(consult.diagnosis, diagnosis);
    //     strcpy(consult.treatmentPlan, treatmentPlan);
    //     if (!ifConsultExists(consult_id) && ifPatientExists(patient_id)) {
    //         addConsultation(&consult);
    //     } else {
    //         g_printerr("Consult already exist\n");
    //     }
    // } else {
    //     g_printerr("Error: Consultation is NULL\n");
    // }
}

static void add_rdv_popup(GtkButton *btn, gpointer data) {
    // Create the popup window
    GtkWidget *popup_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(popup_window), "Ajouter un rendez-vous");
    gtk_window_set_default_size(GTK_WINDOW(popup_window), 300, 600);

    // Create a vertical box for layout
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(popup_window), box);

    GtkWidget *id_err_label = gtk_label_new("");

    GtkWidget *id_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(id_entry), "Rendez-vous Id");

    GtkWidget *id_pt_err_label = gtk_label_new("");

    GtkWidget *id_pt_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(id_pt_entry), "Patient Id (CIN)");

    GtkWidget *date_err_label = gtk_label_new("");

    GtkWidget *dateBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

    // years
    const char * years[] = {"2024", "2025", "2026", "2027", "2028", "2029", NULL};
    GtkStringList *year_model = gtk_string_list_new(years);

    // months
    const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec", NULL};
    GtkStringList * month_model = gtk_string_list_new(months);

    // GtkMapListModel *month_map_list_model = gtk_map_list_model_new(G_LIST_MODEL(month_model), map_month_to_int, months, NULL);
    // GListModel *month_list_model = G_LIST_MODEL(month_map_list_model);
    // guint n_items = g_list_model_get_n_items(month_list_model);


    const char** days = malloc(32 * sizeof(char*));
    for (int i = 0; i < 31; ++i) {
        days[i] = malloc(3 * sizeof(char));
        sprintf((char*)days[i], "%d", i + 1);
    }
    days[31] = NULL;

    GtkStringList * day_model = gtk_string_list_new(days);
    GtkWidget *year_dropdown = gtk_drop_down_new(G_LIST_MODEL(year_model), NULL);
    GtkWidget *month_dropdown = gtk_drop_down_new(G_LIST_MODEL(month_model), NULL);
    GtkWidget *day_dropdown = gtk_drop_down_new(G_LIST_MODEL(day_model), NULL);

    gtk_box_append(GTK_BOX(dateBox), year_dropdown);
    gtk_box_append(GTK_BOX(dateBox), month_dropdown);
    gtk_box_append(GTK_BOX(dateBox), day_dropdown);

    for (int i = 0; i < 31; ++i) {
        free((char*)days[i]);
    }
    free(days);

    GtkWidget *state_err_label = gtk_label_new("");

    GtkStringList * rdv_state_model = gtk_string_list_new((const char *[]){"Pending", "Cancelled", "Confirmed", NULL});

    GtkWidget *state_dropdown = gtk_drop_down_new(G_LIST_MODEL(rdv_state_model), NULL);

    GtkWidget **widgets =  g_new(GtkWidget*, 10);
    widgets[0] = id_entry;
    widgets[1] = id_pt_entry;
    widgets[2] = year_dropdown;
    widgets[3] = month_dropdown;
    widgets[4] = day_dropdown;
    widgets[5] = state_dropdown;
    widgets[6] = id_err_label;
    widgets[7] = id_pt_err_label;
    widgets[8] = date_err_label;
    widgets[9] = state_err_label;

    // Create the buttons
    GtkWidget *add_btn = gtk_button_new_with_label("ajouter");
    GtkWidget *cancel_btn = gtk_button_new_with_label("Cancel");

    // Connect the cancel button to close the window
    g_signal_connect(add_btn, "clicked", G_CALLBACK(g_add_rdv), widgets);
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(window_close), GTK_WINDOW(popup_window));
    // g_signal_connect(year_dropdown, "notify::selected", G_CALLBACK(on_year_changed), NULL);
    //g_signal_connect(month_dropdown, "changed", G_CALLBACK(on_dropdown_changed), NULL);
    //g_signal_connect(day_dropdown, "changed", G_CALLBACK(on_dropdown_changed), NULL);


    // Append widgets to the box
    gtk_box_append(GTK_BOX(box), id_err_label);
    gtk_box_append(GTK_BOX(box), id_entry);
    gtk_box_append(GTK_BOX(box), id_pt_err_label);
    gtk_box_append(GTK_BOX(box), id_pt_entry);
    gtk_box_append(GTK_BOX(box), date_err_label);
    gtk_box_append(GTK_BOX(box), dateBox);
    gtk_box_append(GTK_BOX(box), state_err_label);
    gtk_box_append(GTK_BOX(box), state_dropdown);
    gtk_box_append(GTK_BOX(box), add_btn);
    gtk_box_append(GTK_BOX(box), cancel_btn);

    // Show the popup window
    gtk_window_present(GTK_WINDOW(popup_window));
}

static void g_delete_rdv(GtkButton *btn, gpointer data) {
    int *id_value = data;
    printf("Consultation with id: %d have been deleted\n", *id_value);
    deleteConsultation(*id_value);
}

static void delete_rdv_popup(GtkButton *btn, gpointer data) {
    int* id = data;
    GtkWidget *popup_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(popup_window), "confirmer la suppression ");
    gtk_window_set_default_size(GTK_WINDOW(popup_window), 400, 150);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(popup_window), box);

    char msg[100];
    sprintf(msg, "are you sure about deleting this consultation (%d)", *id);

    GtkWidget *msg_label = gtk_label_new(msg);
    GtkWidget *del_btn = gtk_button_new_with_label("supprimer");
    GtkWidget *cancel_btn = gtk_button_new_with_label("Cancel");

    gtk_box_append(GTK_BOX(box), msg_label);
    gtk_box_append(GTK_BOX(box), del_btn);
    gtk_box_append(GTK_BOX(box), cancel_btn);

    g_signal_connect(del_btn, "clicked", G_CALLBACK(g_delete_consult), data);
    g_signal_connect(del_btn, "clicked", G_CALLBACK(window_close), GTK_WIDGET(popup_window));
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(window_close), GTK_WIDGET(popup_window));

    gtk_window_present(GTK_WINDOW(popup_window));
}

static void g_edit_rdv(GtkButton *btn, gpointer data) {
    GtkWidget **widgets = data;
    if (!widgets) {
        g_printerr("Error: widgets is NULL\n");
        return;
    }

    //Entries
    GtkEntry *entry_id_consult = GTK_ENTRY(widgets[0]);
    GtkEntry *entry_id_patient = GTK_ENTRY(widgets[1]);
    GtkEntry *entry_symptoms = GTK_ENTRY(widgets[2]);
    GtkEntry *entry_diagnosis = GTK_ENTRY(widgets[3]);
    GtkEntry *entry_treatmentPlan = GTK_ENTRY(widgets[4]);

    //Labels
    GtkLabel *label_id_patient = GTK_LABEL(widgets[5]);
    GtkLabel *label_symptoms = GTK_LABEL(widgets[6]);
    GtkLabel *label_diagnosis = GTK_LABEL(widgets[7]);
    GtkLabel *label_treatmentPlan = GTK_LABEL(widgets[8]);

    if (!entry_id_consult || !entry_id_patient || !entry_symptoms || !entry_diagnosis || !entry_treatmentPlan) {
        g_printerr("Error: One of the entries is NULL\n");
        return;
    }

    // Retrieve values from each entry
    int consult_id = get_int_from_entry(GTK_WIDGET(entry_id_consult));
    const char* patient_id = get_text_from_entry(GTK_WIDGET(entry_id_patient));
    const char* symptoms = get_text_from_entry(GTK_WIDGET(entry_symptoms));
    const char* diagnosis = get_text_from_entry(GTK_WIDGET(entry_diagnosis));
    const char* treatmentPlan = get_text_from_entry(GTK_WIDGET(entry_treatmentPlan));

    if (!isCinValid(patient_id)) {
        gtk_label_set_text(label_id_patient, "CIN can only contain letters and numbers ([A-Za-z][0-9]) ()!");
    } else {
        gtk_label_set_text(label_id_patient, "");
    }

    if (!isTextValid(symptoms)) {
        gtk_label_set_text(label_symptoms, "symptoms should not left empty!");
    } else {
        gtk_label_set_text(label_symptoms, "");
    }

    if (!isTextValid(diagnosis)) {
        gtk_label_set_text(label_diagnosis, "diagnosis should not left empty!");
    } else {
        gtk_label_set_text(label_diagnosis, "");
    }

    if (!isTextValid(treatmentPlan)) {
        gtk_label_set_text(label_treatmentPlan, "treatment plan should not left empty!");
    } else {
        gtk_label_set_text(label_treatmentPlan, "");
    }

    if (consult_id != 0 && isCinValid(patient_id) && isTextValid(symptoms) && isTextValid(diagnosis) && isTextValid(treatmentPlan)) {
        Consult consult;
        consult.id = consult_id;
        strcpy(consult.id_pt, patient_id);
        strcpy(consult.symptoms, symptoms);
        strcpy(consult.diagnosis, diagnosis);
        strcpy(consult.treatmentPlan, treatmentPlan);
        if (ifPatientExists(patient_id)){
            modifyConsultation(consult);
            // note that the time is not fixed yet !!!
        } else {
            printf("Patient ID does not exist\n");
        }
    } else {
        g_printerr("Error: Consultation information are not correct\n");
    }
}

static void edit_rdv_popup(GtkButton *btn, gpointer data) {
    const Consult *consult = data;
    // Create the popup window
    GtkWidget *popup_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(popup_window), "Ajouter une consultation");
    gtk_window_set_default_size(GTK_WINDOW(popup_window), 300, 600);

    // Create a vertical box for layout
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(popup_window), box);

    GtkWidget *id_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(id_entry), "Consult Id");
    char id_text[10];
    sprintf(id_text, "%d", consult->id);
    set_text_entry(id_entry, id_text);

    GtkWidget *id_pt_err_label = gtk_label_new("");

    GtkWidget *id_pt_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(id_pt_entry), "Patient CIN");
    set_text_entry(id_pt_entry, consult->id_pt);

    GtkWidget *symptoms_err_label = gtk_label_new("");

    GtkWidget *symptoms_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(symptoms_entry), "Symptoms");
    set_text_entry(symptoms_entry, consult->symptoms);

    GtkWidget *diagnosis_err_label = gtk_label_new("");

    GtkWidget *diagnosis_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(diagnosis_entry), "Diagnosis");
    set_text_entry(diagnosis_entry, consult->diagnosis);

    GtkWidget *treatmentPlan_err_label = gtk_label_new("");

    GtkWidget *treatmentPlan_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(treatmentPlan_entry), "Treatment plan");
    set_text_entry(treatmentPlan_entry, consult->treatmentPlan);


    GtkWidget **widget =  g_new(GtkWidget*, 9);
    widget[0] = id_entry;
    widget[1] = id_pt_entry;
    widget[2] = symptoms_entry;
    widget[3] = diagnosis_entry;
    widget[4] = treatmentPlan_entry;
    widget[5] = id_pt_err_label;
    widget[6] = symptoms_err_label;
    widget[7] = diagnosis_err_label;
    widget[8] = treatmentPlan_err_label;


    // Create the buttons
    GtkWidget *add_btn = gtk_button_new_with_label("modifier");
    GtkWidget *cancel_btn = gtk_button_new_with_label("Cancel");

    // Connect the cancel button to close the window
    g_signal_connect(add_btn, "clicked", G_CALLBACK(g_edit_consult), widget);
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(window_close), GTK_WINDOW(popup_window));

    // Append widgets to the box
    gtk_box_append(GTK_BOX(box), id_entry);
    gtk_box_append(GTK_BOX(box), id_pt_err_label);
    gtk_box_append(GTK_BOX(box), id_pt_entry);
    gtk_box_append(GTK_BOX(box), symptoms_err_label);
    gtk_box_append(GTK_BOX(box), symptoms_entry);
    gtk_box_append(GTK_BOX(box), diagnosis_err_label);
    gtk_box_append(GTK_BOX(box), diagnosis_entry);
    gtk_box_append(GTK_BOX(box), treatmentPlan_err_label);
    gtk_box_append(GTK_BOX(box), treatmentPlan_entry);
    gtk_box_append(GTK_BOX(box), add_btn);
    gtk_box_append(GTK_BOX(box), cancel_btn);

    // Show the popup window
    gtk_window_present(GTK_WINDOW(popup_window));
}

// static void g_update_display_rdv(GtkButton *btn, gpointer data) {
//     Rendezvous *rdvs = getRVs();
//     int rdv_count = getNumbOfRVs();
//     // printf("Total Consultation: %d\n", consult_count);
//     // displayAllConsultation();
//
//     GtkGrid *grid = GTK_GRID(data);
//
//     int col= 0, row = 2;
//     while (gtk_grid_get_child_at(grid, col, row) != NULL) {
//         while (col < 4) {
//             GtkWidget *child = gtk_grid_get_child_at(grid, col, row);
//             if (child != NULL) {
//                 gtk_widget_unparent(child);
//             }
//             col++;
//         }
//         row++;
//         col = 0;
//     }
//
//     for (int i = 0; i < rdv_count; i++) {
//         char id_text[10];
//
//         snprintf(id_text, sizeof(id_text), "%d", rdvs[i].id);
//
//         GtkWidget *id_label = gtk_label_new(id_text);
//         GtkWidget *id_pt_label = gtk_label_new(rdvs[i].id_pt);
//         Patient* patient = getPatient(rdvs[i].id_pt);
//         if (patient == NULL) {
//             printf("Patient with ID %s not found\n", rdvs[i].id_pt);
//             continue;
//         }
//
//         char fullName_pt[40];
//         snprintf(fullName_pt, sizeof(fullName_pt), "%s %s", patient->lName, patient->fName);
//
//         GtkWidget *name_label = gtk_label_new(fullName_pt);
//         GtkWidget *symptoms_label = gtk_label_new(rdvs[i].date);
//         GtkWidget *diagnosis_label = gtk_label_new(rdvs[i].diagnosis);
//         GtkWidget *treatmentPlan_label = gtk_label_new(rdvs[i].treatmentPlan);
//         GtkWidget *edit_btn = gtk_button_new_with_label("edit");
//         GtkWidget *delete_btn = gtk_button_new_with_label("delete");
//
//         // Attach labels to the grid for each row of patient data
//         gtk_grid_attach(grid, id_label, 0, i + 2, 1, 1);
//         gtk_grid_attach(grid, id_pt_label, 1, i + 2, 1, 1);
//         gtk_grid_attach(grid, name_label, 2, i + 2, 1, 1);
//         gtk_grid_attach(grid, symptoms_label, 3, i + 2, 1, 1);
//         gtk_grid_attach(grid, diagnosis_label, 4, i + 2, 1, 1);
//         gtk_grid_attach(grid, treatmentPlan_label, 5, i + 2, 1, 1);
//         gtk_grid_attach(grid, edit_btn, 6, i + 2, 1, 1);
//         gtk_grid_attach(grid, delete_btn, 7, i + 2, 1, 1);
//
//         g_signal_connect(delete_btn, "clicked", G_CALLBACK(delete_rdv_popup), &rdvs[i].id);
//         g_signal_connect(edit_btn, "clicked", G_CALLBACK(edit_rdv_popup), &rdvs[i]);
//     }
// }

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

    wins[4] = gtk_builder_get_object(builder, "rendezvousWindow");
    if (!wins[4]) {
        g_printerr("Failed to get 'Rendezvous' window from builder UI file\n");
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

static void switch_to_rendezvous(GtkButton *btn, gpointer user_data) {
    GObject **windows = (GObject **)user_data;
    GtkWidget *window_rdv = GTK_WIDGET(windows[4]);
    GtkWidget *window_prev = gtk_widget_get_ancestor(GTK_WIDGET(btn), GTK_TYPE_WINDOW);

    if (GTK_IS_WINDOW(window_prev)) {
        gtk_widget_set_visible(window_prev, FALSE);
        gtk_widget_set_visible(window_rdv, TRUE);
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
    g_signal_connect(btn_rdv, "clicked", G_CALLBACK(switch_to_rendezvous), win);
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

// set rendezvous windows objects
static void set_rendezvous_win(GtkBuilder *builder, GObject **win) {
    GtkGrid *grid = GTK_GRID(gtk_builder_get_object(builder, "grid_rendezvous"));

    // Define grid headers
    GtkWidget *id_header = gtk_label_new("Rendezvous ID");
    GtkWidget *id_pt_header = gtk_label_new("Patient ID");
    GtkWidget *date_header = gtk_label_new("Date");
    GtkWidget *state_header = gtk_label_new("state");

    // Add headers to the first row of the grid
    gtk_grid_attach(grid, id_header, 0, 1, 1, 1);
    gtk_grid_attach(grid, id_pt_header, 1, 1, 1, 1);
    gtk_grid_attach(grid, date_header, 2, 1, 1, 1);
    gtk_grid_attach(grid, state_header, 3, 1, 1, 1);

    // g_update_display_rdv(NULL, grid);

    // Create and attach buttons after the last row of patient data
    GObject *refresh_rendezvous_btn = gtk_builder_get_object(builder, "refresh_rendezvous_btn");
    GObject *add_rendezvous_btn = gtk_builder_get_object(builder, "add_rendezvous_btn");
    GObject *return_rendezvous_btn = gtk_builder_get_object(builder, "return_rendezvous_btn");

    // Connect button signals to their respective callbacks
    // g_signal_connect(refresh_rendezvous_btn, "clicked", G_CALLBACK(g_update_display_rdv), grid);
    g_signal_connect(add_rendezvous_btn,"clicked", G_CALLBACK(add_rdv_popup), NULL);
    g_signal_connect(return_rendezvous_btn, "clicked", G_CALLBACK(switch_to_menu), win);
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
    set_patient_win(builder, wins);
    set_consult_win(builder, wins);
    set_rendezvous_win(builder, wins);

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

