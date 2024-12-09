#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "consultation.h"
#include "patient.h"
#include "rendezvous.h"
#include <regex.h>

#include "payment_management.h"

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

static double get_double_from_entry(GtkWidget *entry) {
    GtkEntryBuffer *entryBuffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
    const char* string = gtk_entry_buffer_get_text(entryBuffer);
    const double value = strtod(string, NULL);
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

int isDateValid(int Year, int Month, int Day) {
    // Get current date
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    int currentYear = tm.tm_year + 1900;
    int currentMonth = tm.tm_mon + 1; // Months are 0-indexed in tm
    int currentDay = tm.tm_mday;

    if (Year < currentYear) {
        return 0;
    }

    if (Year == currentYear && Month < currentMonth) {
        return 0; // Month is invalid
    }

    // Check day if the year and month are the same
    if (Year == currentYear && Month == currentMonth && Day < currentDay) {
        return 0; // Day is invalid
    }

    return 1; // Date is valid
}

int isYearValid(int year) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return tm.tm_year <= year;
}

int isMonthValid(int month) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return tm.tm_mon <= month;
}

int isDayValid(int day) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return tm.tm_mday <= day;
}

int isHourValid(int hour) {
    return hour >= 8 && hour < 18;
}

const char* get_dropdown_item(GtkDropDown *dropdown) {
    GtkStringObject *item = gtk_drop_down_get_selected_item(dropdown);
    const char *year_str = gtk_string_object_get_string(item);
    return year_str;
}

int find_year_index(const char* target) {
    const char months[6][5] = {"2024", "2025", "2026", "2027", "2028", "2029"};
    for (int i = 0; i < 6 ; i++) {
        if (strcmp(target, months[i]) == 0) {
            return i;
        }
    }
    return -1;
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

int find_day_index(const char* target) {
    const char days[31][3] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31"};
    for (int i = 0; i < 31 ; i++) {
        if (strcmp(target, days[i]) == 0) {
            return i;
        }
    }
    return -1;
}

int find_hour_index(const char* target) {
    const char hours[10][3] = {"8", "9", "10", "11", "12", "13", "14", "15", "16", "17"};
    for (int i = 0; i < 10 ; i++) {
        if (strcmp(target, hours[i]) == 0) {
            return i;
        }
    }
    return -1;
}

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
    // printf("Total Consultation: %d\n", consult_count);
    // displayAllConsultation();

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
    GtkDropDown *dropdown_hour = GTK_DROP_DOWN(widgets[5]);
    GtkDropDown *dropdown_state = GTK_DROP_DOWN(widgets[6]);
    //Labels
    GtkLabel *label_id_rdv = GTK_LABEL(widgets[7]);
    GtkLabel *label_id_pt = GTK_LABEL(widgets[8]);
    GtkLabel *label_date = GTK_LABEL(widgets[9]);
    GtkLabel *label_state = GTK_LABEL(widgets[10]);

    if (!entry_id_rdv || !entry_id_pt || !dropdown_year || !dropdown_month || !dropdown_day || !dropdown_state) {
        g_printerr("Error: One of the entries is NULL\n");
        return;
    }

    // Retrieve values from each entry
    int rdv_id = get_int_from_entry(GTK_WIDGET(entry_id_rdv));
    const char* patient_id = get_text_from_entry(GTK_WIDGET(entry_id_pt));
    const char* rdv_year = get_dropdown_item(dropdown_year);
    int year = strtol(rdv_year, NULL, 10);
    const char* rdv_month = get_dropdown_item(dropdown_month);
    int month = find_month_index(rdv_month);
    const char* rdv_day = get_dropdown_item(dropdown_day);
    int day = strtol(rdv_day, NULL, 10);
    const char* rdv_hour = get_dropdown_item(dropdown_hour);
    int hour = atoi(rdv_hour);
    const char* rdv_state = get_dropdown_item(dropdown_state);

    if (!isCinValid(patient_id)) {
        gtk_label_set_text(label_id_pt, "CIN can only contain letters and numbers ([A-Za-z][0-9]) ()!");
        return;
    }
    gtk_label_set_text(label_id_pt, "");


    if (!isDateValid(year, month, day)) {
        gtk_label_set_text(label_date, "Date chosen is not valid");
        return;
    }
    gtk_label_set_text(label_state, "");

    if (!isHourValid(hour)) {
        printf("Invalid hour\n");
        return;
    }

    if (rdv_id != 0) {
        Rendezvous rendezvous;
        rendezvous.id = rdv_id;
        strcpy(rendezvous.id_pt, patient_id);
        rendezvous.day = day;
        rendezvous.month = month;
        rendezvous.year = year;
        rendezvous.hour = hour;
        strcpy(rendezvous.state, rdv_state);
        if (is_holiday(rendezvous)) {
            printf("Invalid Rendezvous: it's a holiday\n");
            return;
        }
        if (is_weekend(rendezvous)) {
            printf("Invalid Rendezvous: it's weekend\n");
            return;
        }
        if (ifPatientExists(patient_id)) {
            addRV(rendezvous);
            // displayAllRVs();
        } else {
            g_printerr("Patient with ID: %s does not exist\n", patient_id);
        }
    } else {
        g_printerr("Error: Rendezvous is NULL\n");
    }
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

    const char** days = malloc(32 * sizeof(char*));
    for (int i = 0; i < 31; ++i) {
        days[i] = malloc(3 * sizeof(char));
        sprintf((char*)days[i], "%d", i + 1);
    }
    days[31] = NULL;

    const char** hours = malloc(12 * sizeof(char*));
    for (int i = 0; i < 11; ++i) {
        hours[i] = malloc(3 * sizeof(char));
        sprintf((char*)hours[i], "%dh", i + 8);
    }
    hours[11] = NULL;

    GtkWidget *year_dropdown = gtk_drop_down_new(G_LIST_MODEL(year_model), NULL);
    GtkWidget *month_dropdown = gtk_drop_down_new(G_LIST_MODEL(month_model), NULL);
    GtkStringList * day_model = gtk_string_list_new(days);
    GtkWidget *day_dropdown = gtk_drop_down_new(G_LIST_MODEL(day_model), NULL);
    GtkStringList * hour_model = gtk_string_list_new(hours);
    GtkWidget *hour_dropdown = gtk_drop_down_new(G_LIST_MODEL(hour_model), NULL);

    gtk_box_append(GTK_BOX(dateBox), year_dropdown);
    gtk_box_append(GTK_BOX(dateBox), month_dropdown);
    gtk_box_append(GTK_BOX(dateBox), day_dropdown);
    gtk_box_append(GTK_BOX(dateBox), hour_dropdown);

    for (int i = 0; i < 31; ++i) {
        free((char*)days[i]);
    }
    free(days);

    for (int i = 0; i < 11; ++i) {
        free((char*)hours[i]);
    }
    free(hours);

    GtkWidget *state_err_label = gtk_label_new("");

    GtkStringList * rdv_state_model = gtk_string_list_new((const char *[]){"Pending", "Cancelled", "Confirmed", NULL});

    GtkWidget *state_dropdown = gtk_drop_down_new(G_LIST_MODEL(rdv_state_model), NULL);

    GtkWidget **widgets =  g_new(GtkWidget*, 11);
    widgets[0] = id_entry;
    widgets[1] = id_pt_entry;
    widgets[2] = year_dropdown;
    widgets[3] = month_dropdown;
    widgets[4] = day_dropdown;
    widgets[5] = hour_dropdown;
    widgets[6] = state_dropdown;
    widgets[7] = id_err_label;
    widgets[8] = id_pt_err_label;
    widgets[9] = date_err_label;
    widgets[10] = state_err_label;

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
    printf("RV with id: %d have been deleted\n", *id_value);
    cancelRV(*id_value);
}

static void delete_rdv_popup(GtkButton *btn, gpointer data) {
    int* id = data;
    GtkWidget *popup_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(popup_window), "confirmer la suppression ");
    gtk_window_set_default_size(GTK_WINDOW(popup_window), 400, 150);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(popup_window), box);

    char msg[100];
    sprintf(msg, "are you sure about deleting this rendezvous (ID: %d)", *id);

    GtkWidget *msg_label = gtk_label_new(msg);
    GtkWidget *del_btn = gtk_button_new_with_label("supprimer");
    GtkWidget *cancel_btn = gtk_button_new_with_label("Cancel");

    gtk_box_append(GTK_BOX(box), msg_label);
    gtk_box_append(GTK_BOX(box), del_btn);
    gtk_box_append(GTK_BOX(box), cancel_btn);

    g_signal_connect(del_btn, "clicked", G_CALLBACK(g_delete_rdv), data);
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
    GtkEntry *entry_id_rdv = GTK_ENTRY(widgets[0]);
    GtkEntry *entry_id_patient = GTK_ENTRY(widgets[1]);
    GtkDropDown *year_dropdown = GTK_DROP_DOWN(widgets[2]);
    GtkDropDown *month_dropdown = GTK_DROP_DOWN(widgets[3]);
    GtkDropDown *day_dropdown = GTK_DROP_DOWN(widgets[4]);
    GtkDropDown *hour_dropdown = GTK_DROP_DOWN(widgets[5]);
    GtkDropDown *state_dropdown = GTK_DROP_DOWN(widgets[6]);

    //Labels
    GtkLabel *label_id_rdv = GTK_LABEL(widgets[7]);
    GtkLabel *label_id_pt = GTK_LABEL(widgets[8]);
    GtkLabel *label_date = GTK_LABEL(widgets[9]);
    GtkLabel *label_state = GTK_LABEL(widgets[10]);

    if (!entry_id_rdv || !entry_id_patient || !year_dropdown || !month_dropdown || !day_dropdown || !hour_dropdown) {
        g_printerr("Error: One of the entries is NULL\n");
        return;
    }

    // Retrieve values from each entry
    int rdv_id = get_int_from_entry(GTK_WIDGET(entry_id_rdv));
    const char* patient_id = get_text_from_entry(GTK_WIDGET(entry_id_patient));
    const char* rdv_year = get_dropdown_item(year_dropdown);
    int year = strtol(rdv_year, NULL, 10);
    const char* rdv_month = get_dropdown_item(month_dropdown);
    int month = find_month_index(rdv_month);
    const char* rdv_day = get_dropdown_item(day_dropdown);
    int day = strtol(rdv_day, NULL, 10);
    const char* rdv_hour = get_dropdown_item(hour_dropdown);
    int hour = atoi(rdv_hour);
    const char* rdv_state = get_dropdown_item(state_dropdown);

    if (!isCinValid(patient_id)) {
        gtk_label_set_text(label_id_pt, "CIN can only contain letters and numbers ([A-Za-z][0-9]) ()!");
        return;
    }
    gtk_label_set_text(label_id_pt, "");


    if (!isDateValid(year, month, day)) {
        gtk_label_set_text(label_date, "Date chosen is not valid");
        return;
    }
    gtk_label_set_text(label_state, "");

    if (!isHourValid(hour)) {
        printf("Invalid hour\n");
        return;
    }

    if (rdv_id != 0) {
        Rendezvous rendezvous;
        rendezvous.id = rdv_id;
        strcpy(rendezvous.id_pt, patient_id);
        rendezvous.day = day;
        rendezvous.month = month;
        rendezvous.year = year;
        rendezvous.hour = hour;
        strcpy(rendezvous.state, rdv_state);

        if (is_holiday(rendezvous)) {
            printf("Invalid Rendezvous: it's a holiday\n");
            return;
        }
        if (is_weekend(rendezvous)) {
            printf("Invalid Rendezvous: it's weekend\n");
            return;
        }
        if (ifPatientExists(patient_id)) {
            modifyRV(rendezvous);
            // displayAllRVs();
        } else {
            g_printerr("Patient with ID: %s does not exist\n", patient_id);
        }
    } else {
        g_printerr("Error: Rendezvous is NULL\n");
    }
}

static void edit_rdv_popup(GtkButton *btn, gpointer data) {
    const Rendezvous *RV = data;
    // Create the popup window
    GtkWidget *popup_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(popup_window), "modifier un rendezvous");
    gtk_window_set_default_size(GTK_WINDOW(popup_window), 300, 600);

    // Create a vertical box for layout
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(popup_window), box);

    GtkWidget *id_err_label = gtk_label_new("");

    GtkWidget *id_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(id_entry), "Rendezvous Id");
    char id_text[10];
    sprintf(id_text, "%d", RV->id);
    set_text_entry(id_entry, id_text);

    GtkWidget *id_pt_err_label = gtk_label_new("");

    GtkWidget *id_pt_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(id_pt_entry), "Patient CIN");
    set_text_entry(id_pt_entry, RV->id_pt);

    GtkWidget *date_err_label = gtk_label_new("");

    GtkWidget *dateBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

    const char * years[] = {"2024", "2025", "2026", "2027", "2028", "2029", NULL};
    GtkStringList *year_model = gtk_string_list_new(years);

    const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec", NULL};
    GtkStringList * month_model = gtk_string_list_new(months);

    const char** days = malloc(32 * sizeof(char*));
    for (int i = 0; i < 31; ++i) {
        days[i] = malloc(3 * sizeof(char));
        sprintf((char*)days[i], "%d", i + 1);
    }
    days[31] = NULL;

    const char** hours = malloc(12 * sizeof(char*));
    for (int i = 0; i < 11; ++i) {
        hours[i] = malloc(3 * sizeof(char));
        sprintf((char*)hours[i], "%dh", i + 8);
    }
    hours[11] = NULL;

    char year_str[3];
    sprintf(year_str, "%d", RV->year);
    int year_pos = find_year_index(year_str);

    char hour_str[3];
    sprintf(hour_str, "%d", RV->hour);
    int hour_pos = find_hour_index(hour_str);


    GtkWidget *year_dropdown = gtk_drop_down_new(G_LIST_MODEL(year_model), NULL);
    gtk_drop_down_set_selected(GTK_DROP_DOWN(year_dropdown),year_pos);

    GtkWidget *month_dropdown = gtk_drop_down_new(G_LIST_MODEL(month_model), NULL);
    gtk_drop_down_set_selected(GTK_DROP_DOWN(month_dropdown),RV->month-1);
    GtkStringList * day_model = gtk_string_list_new(days);

    GtkWidget *day_dropdown = gtk_drop_down_new(G_LIST_MODEL(day_model), NULL);
    gtk_drop_down_set_selected(GTK_DROP_DOWN(day_dropdown),RV->day-1);
    GtkStringList * hour_model = gtk_string_list_new(hours);

    GtkWidget *hour_dropdown = gtk_drop_down_new(G_LIST_MODEL(hour_model), NULL);
    gtk_drop_down_set_selected(GTK_DROP_DOWN(hour_dropdown),hour_pos);

    gtk_box_append(GTK_BOX(dateBox), year_dropdown);
    gtk_box_append(GTK_BOX(dateBox), month_dropdown);
    gtk_box_append(GTK_BOX(dateBox), day_dropdown);
    gtk_box_append(GTK_BOX(dateBox), hour_dropdown);

    for (int i = 0; i < 31; ++i) {
        free((char*)days[i]);
    }
    free(days);

    for (int i = 0; i < 11; ++i) {
        free((char*)hours[i]);
    }
    free(hours);

    GtkWidget *state_err_label = gtk_label_new("");

    GtkStringList * rdv_state_model = gtk_string_list_new((const char *[]){"Pending", "Cancelled", "Confirmed", NULL});

    GtkWidget *state_dropdown = gtk_drop_down_new(G_LIST_MODEL(rdv_state_model), NULL);

    GtkWidget **widgets =  g_new(GtkWidget*, 11);
    widgets[0] = id_entry;
    widgets[1] = id_pt_entry;
    widgets[2] = year_dropdown;
    widgets[3] = month_dropdown;
    widgets[4] = day_dropdown;
    widgets[5] = hour_dropdown;
    widgets[6] = state_dropdown;
    widgets[7] = id_err_label;
    widgets[8] = id_pt_err_label;
    widgets[9] = date_err_label;
    widgets[10] = state_err_label;


    // Create the buttons
    GtkWidget *edit_btn = gtk_button_new_with_label("modifier");
    GtkWidget *cancel_btn = gtk_button_new_with_label("Cancel");

    // Connect the cancel button to close the window
    g_signal_connect(edit_btn, "clicked", G_CALLBACK(g_edit_rdv), widgets);
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(window_close), GTK_WINDOW(popup_window));

    // Append widgets to the box
    gtk_box_append(GTK_BOX(box), id_err_label);
    gtk_box_append(GTK_BOX(box), id_entry);
    gtk_box_append(GTK_BOX(box), id_pt_err_label);
    gtk_box_append(GTK_BOX(box), id_pt_entry);
    gtk_box_append(GTK_BOX(box), date_err_label);
    gtk_box_append(GTK_BOX(box), dateBox);
    gtk_box_append(GTK_BOX(box), state_err_label);
    gtk_box_append(GTK_BOX(box), state_dropdown);
    gtk_box_append(GTK_BOX(box), edit_btn);
    gtk_box_append(GTK_BOX(box), cancel_btn);

    // Show the popup window
    gtk_window_present(GTK_WINDOW(popup_window));
}

static void g_update_display_rdv(GtkButton *btn, gpointer data) {

    Rendezvous *RVs = getRVs();
    int rdv_count = getNumbOfRVs();
    displayAllRVs();
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

    for (int i = 0; i < rdv_count; i++) {
        char id_text[10];

        snprintf(id_text, sizeof(id_text), "%d", RVs[i].id);

        GtkWidget *id_label = gtk_label_new(id_text);
        GtkWidget *id_pt_label = gtk_label_new(RVs[i].id_pt);

        Patient* patient = getPatient(RVs[i].id_pt);
        if (patient == NULL) {
            printf("Patient with ID %s not found\n", RVs[i].id_pt);
            continue;
        }
        char fullName_pt[40];
        snprintf(fullName_pt, sizeof(fullName_pt), "%s %s", patient->lName, patient->fName);
        GtkWidget *name_label = gtk_label_new(fullName_pt);

        char date[12];
        snprintf(date, sizeof(date), "%d/%d/%d", RVs[i].year, RVs[i].month, RVs[i].day);
        GtkWidget *date_label = gtk_label_new(date);

        char time[3];
        snprintf(time, sizeof(time), "%dh", RVs[i].hour);
        GtkWidget *hour_label = gtk_label_new(time);

        GtkWidget *state_label = gtk_label_new(RVs[i].state);
        GtkWidget *edit_btn = gtk_button_new_with_label("edit");
        GtkWidget *delete_btn = gtk_button_new_with_label("delete");

        // Attach labels to the grid for each row of patient data
        gtk_grid_attach(grid, id_label, 0, i + 2, 1, 1);
        gtk_grid_attach(grid, id_pt_label, 1, i + 2, 1, 1);
        gtk_grid_attach(grid, name_label, 2, i + 2, 1, 1);
        gtk_grid_attach(grid, date_label, 3, i + 2, 1, 1);
        gtk_grid_attach(grid, hour_label, 4, i + 2, 1, 1);
        gtk_grid_attach(grid, state_label, 5, i + 2, 1, 1);
        gtk_grid_attach(grid, edit_btn, 6, i + 2, 1, 1);
        gtk_grid_attach(grid, delete_btn, 7, i + 2, 1, 1);

        g_signal_connect(delete_btn, "clicked", G_CALLBACK(delete_rdv_popup), &RVs[i].id);
        g_signal_connect(edit_btn, "clicked", G_CALLBACK(edit_rdv_popup), &RVs[i]);
    }
}

//===================================================================//
// Payment functionalities
//===================================================================//

static void g_add_payment(GtkButton *btn, gpointer data) {
    GtkWidget **widgets = data;
    if (!widgets) {
        g_printerr("Error: widgets is NULL\n");
        return;
    }

    //Entries
    GtkEntry *entry_payment_id = GTK_ENTRY(widgets[0]);
    GtkEntry *entry_consult_id = GTK_ENTRY(widgets[1]);
    GtkEntry *entry_pt_id = GTK_ENTRY(widgets[2]);
    GtkEntry *entry_amount = GTK_ENTRY(widgets[3]);
    //Labels
    GtkLabel *label_payment_id = GTK_LABEL(widgets[4]);
    GtkLabel *label_consult_id = GTK_LABEL(widgets[5]);
    GtkLabel *label_pt_id = GTK_LABEL(widgets[6]);
    GtkLabel *label_date = GTK_LABEL(widgets[7]);

    if (!entry_payment_id && !entry_consult_id && !entry_pt_id && !entry_amount) {
        g_printerr("Error: One of the entries is NULL\n");
        return;
    }

    // Retrieve values from each entry
    int payment_id = get_int_from_entry(GTK_WIDGET(entry_payment_id));
    int consult_id = get_int_from_entry(GTK_WIDGET(entry_consult_id));
    const char *patient_id = get_text_from_entry(GTK_WIDGET(entry_pt_id));
    double amount = get_double_from_entry(GTK_WIDGET(entry_amount));

    if (!isCinValid(patient_id)) {
        gtk_label_set_text(label_pt_id, "CIN can only contain letters and numbers ([A-Za-z][0-9]) ()!");
        return;
    }
    gtk_label_set_text(label_pt_id, "");

    if (payment_id && consult_id && patient_id )  {
        Payment payment;
        payment.id = payment_id;
        payment.id_consult = consult_id;
        strcpy(payment.id_pt, patient_id);
        payment.amount = amount;
        strcpy(payment.state, "pending");

        if (ifPaymentExists(payment_id)) {
            g_printerr("Payment with ID: %d already exists\n", payment_id);
            return;
        }

        if (!ifPatientExists(patient_id)) {
            g_printerr("Patient with ID: %s does not exist\n", patient_id);
            return;
        }

        if (!ifConsultExists(consult_id)) {
            g_printerr("Consulting with ID: %d does not exist\n", consult_id);
            return;
        }

        addPayment(&payment);
        displayAllPayments();
    } else {
        g_printerr("Error: Rendezvous is NULL\n");
    }
}

static void add_payment_popup(GtkButton *btn, gpointer data) {
    // Create the popup window
    GtkWidget *popup_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(popup_window), "Ajouter un payment");
    gtk_window_set_default_size(GTK_WINDOW(popup_window), 300, 600);

    // Create a vertical box for layout
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(popup_window), box);

    GtkWidget *id_err_label = gtk_label_new("");
    GtkWidget *id_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(id_entry), "Payment Id");

    GtkWidget *id_consult_err_label = gtk_label_new("");
    GtkWidget *id_consult_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(id_consult_entry), "Consult Id");

    GtkWidget *id_pt_err_label = gtk_label_new("");
    GtkWidget *id_pt_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(id_pt_entry), "Patient Id (CIN)");

    GtkWidget *amount_err_label = gtk_label_new("");
    GtkWidget *amount_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(amount_entry), "Amount");

    GtkWidget **widgets =  g_new(GtkWidget*, 8);
    widgets[0] = id_entry;
    widgets[1] = id_consult_entry;
    widgets[2] = id_pt_entry;
    widgets[3] = amount_entry;
    widgets[4] = id_err_label;
    widgets[5] = id_consult_err_label;
    widgets[6] = id_pt_err_label;
    widgets[7] = amount_err_label;

    // Create the buttons
    GtkWidget *add_btn = gtk_button_new_with_label("ajouter");
    GtkWidget *cancel_btn = gtk_button_new_with_label("Cancel");

    // Connect the cancel button to close the window
    g_signal_connect(add_btn, "clicked", G_CALLBACK(g_add_payment), widgets);
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(window_close), GTK_WINDOW(popup_window));
    // g_signal_connect(year_dropdown, "notify::selected", G_CALLBACK(on_year_changed), NULL);

    // Append widgets to the box
    gtk_box_append(GTK_BOX(box), id_err_label);
    gtk_box_append(GTK_BOX(box), id_entry);
    gtk_box_append(GTK_BOX(box), id_consult_err_label);
    gtk_box_append(GTK_BOX(box), id_consult_entry);
    gtk_box_append(GTK_BOX(box), id_pt_err_label);
    gtk_box_append(GTK_BOX(box), id_pt_entry);
    gtk_box_append(GTK_BOX(box), amount_err_label);
    gtk_box_append(GTK_BOX(box), amount_entry);
    gtk_box_append(GTK_BOX(box), add_btn);
    gtk_box_append(GTK_BOX(box), cancel_btn);

    // Show the popup window
    gtk_window_present(GTK_WINDOW(popup_window));
}

static void g_delete_payment(GtkButton *btn, gpointer data) {
    int *id_value = data;
    printf("Payment with id: %d have been deleted\n", *id_value);
    deletePayment(*id_value);
}

static void delete_payment_popup(GtkButton *btn, gpointer data) {
    int* id = data;
    GtkWidget *popup_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(popup_window), "confirmer la suppression ");
    gtk_window_set_default_size(GTK_WINDOW(popup_window), 400, 150);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(popup_window), box);

    char msg[100];
    sprintf(msg, "are you sure about deleting this payment (ID: %d)", *id);

    GtkWidget *msg_label = gtk_label_new(msg);
    GtkWidget *del_btn = gtk_button_new_with_label("supprimer");
    GtkWidget *cancel_btn = gtk_button_new_with_label("Cancel");

    gtk_box_append(GTK_BOX(box), msg_label);
    gtk_box_append(GTK_BOX(box), del_btn);
    gtk_box_append(GTK_BOX(box), cancel_btn);

    g_signal_connect(del_btn, "clicked", G_CALLBACK(g_delete_payment), data);
    g_signal_connect(del_btn, "clicked", G_CALLBACK(window_close), GTK_WIDGET(popup_window));
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(window_close), GTK_WIDGET(popup_window));

    gtk_window_present(GTK_WINDOW(popup_window));
}

static void g_edit_payment(GtkButton *btn, gpointer data) {
    GtkWidget **widgets = data;
    if (!widgets) {
        g_printerr("Error: widgets is NULL\n");
        return;
    }

    //Entries
    GtkEntry *entry_id_payment = GTK_ENTRY(widgets[0]);
    GtkEntry *entry_id_consult = GTK_ENTRY(widgets[1]);
    GtkEntry *entry_id_pt = GTK_ENTRY(widgets[2]);
    GtkEntry *entry_amount = GTK_ENTRY(widgets[3]);
    // Dropdowns
    GtkDropDown *dropdown_state = GTK_DROP_DOWN(widgets[4]);
    //Labels
    GtkLabel *label_id_rdv = GTK_LABEL(widgets[5]);
    GtkLabel *label_id_pt = GTK_LABEL(widgets[6]);
    GtkLabel *label_date = GTK_LABEL(widgets[7]);
    GtkLabel *label_state = GTK_LABEL(widgets[8]);

    if (!entry_id_payment || !entry_id_consult || !entry_id_pt || !entry_amount ) {
        g_printerr("Error: One of the entries is NULL\n");
        return;
    }

    // Retrieve values from each entry
    int payment_id = get_int_from_entry(GTK_WIDGET(entry_id_payment));
    int consult_id = get_int_from_entry(GTK_WIDGET(entry_id_consult));
    const char* patient_id = get_text_from_entry(GTK_WIDGET(entry_id_pt));
    double amount = get_double_from_entry(GTK_WIDGET(entry_amount));
    const char* state = get_dropdown_item(dropdown_state);

    if (!isCinValid(patient_id)) {
        gtk_label_set_text(label_id_pt, "CIN can only contain letters and numbers ([A-Za-z][0-9]) ()!");
        return;
    }
    gtk_label_set_text(label_id_pt, "");


    if (payment_id && consult_id && patient_id) {
        Payment payment;
        payment.id = payment_id;
        payment.id_consult = consult_id;
        strcpy(payment.id_pt, patient_id);
        payment.amount = amount;
        strcpy(payment.state, state);
        payment.DateTime = getPayment(payment_id)->DateTime;

        if (!ifPaymentExists(payment_id)) {
            g_printerr("Payment with ID: %does not exists\n", payment_id);
            return;
        }
        if (!ifPatientExists(patient_id)) {
            g_printerr("Patient with ID: %s does not exist\n", patient_id);
            return;
        }

        if (!ifConsultExists(consult_id)) {
            g_printerr("Consulting with ID: %d does not exist\n", consult_id);
            return;
        }

        modifyPayment(payment);
        displayAllPayments();
    } else {
        g_printerr("Error: Rendezvous is NULL\n");
    }
}

static void edit_payment_popup(GtkButton *btn, gpointer data) {
    const Payment *payment = data;
    // Create the popup window
    GtkWidget *popup_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(popup_window), "modifier un rendezvous");
    gtk_window_set_default_size(GTK_WINDOW(popup_window), 300, 600);

    // Create a vertical box for layout
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(popup_window), box);

    GtkWidget *id_err_label = gtk_label_new("");
    GtkWidget *id_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(id_entry), "Payment Id");
    char id_text[10];
    sprintf(id_text, "%d", payment->id);
    set_text_entry(id_entry, id_text);

    GtkWidget *id_consult_err_label = gtk_label_new("");
    GtkWidget *id_consult_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(id_consult_entry), "Consult Id");
    char id_consult_text[10];
    sprintf(id_consult_text, "%d", payment->id_consult);
    set_text_entry(id_consult_entry, id_consult_text);

    GtkWidget *id_pt_err_label = gtk_label_new("");
    GtkWidget *id_pt_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(id_pt_entry), "Patient Id (CIN)");
    set_text_entry(id_pt_entry, payment->id_pt);

    GtkWidget *amount_err_label = gtk_label_new("");
    GtkWidget *amount_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(amount_entry), "Amount");
    char amount_text[10];
    sprintf(amount_text, "%.2lf", payment->amount);
    set_text_entry(amount_entry, amount_text);

    GtkWidget *state_err_label = gtk_label_new("");
    GtkStringList * state_model = gtk_string_list_new((const char *[]){"Pending", "Cancelled", "Confirmed", NULL});
    GtkWidget *state_dropdown = gtk_drop_down_new(G_LIST_MODEL(state_model), NULL);


    GtkWidget **widgets =  g_new(GtkWidget*, 9);
    widgets[0] = id_entry;
    widgets[1] = id_consult_entry;
    widgets[2] = id_pt_entry;
    widgets[3] = amount_entry;
    widgets[4] = state_dropdown;
    widgets[5] = id_err_label;
    widgets[6] = id_consult_err_label;
    widgets[7] = id_pt_err_label;
    widgets[8] = amount_err_label;


    // Create the buttons
    GtkWidget *edit_btn = gtk_button_new_with_label("modifier");
    GtkWidget *cancel_btn = gtk_button_new_with_label("Cancel");

    // Connect the cancel button to close the window
    g_signal_connect(edit_btn, "clicked", G_CALLBACK(g_edit_payment), widgets);
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(window_close), GTK_WINDOW(popup_window));

    // Append widgets to the box
    gtk_box_append(GTK_BOX(box), id_err_label);
    gtk_box_append(GTK_BOX(box), id_entry);
    gtk_box_append(GTK_BOX(box), id_consult_err_label);
    gtk_box_append(GTK_BOX(box), id_consult_entry);
    gtk_box_append(GTK_BOX(box), id_pt_err_label);
    gtk_box_append(GTK_BOX(box), id_pt_entry);
    gtk_box_append(GTK_BOX(box), amount_err_label);
    gtk_box_append(GTK_BOX(box), amount_entry);
    gtk_box_append(GTK_BOX(box), state_dropdown);
    gtk_box_append(GTK_BOX(box), edit_btn);
    gtk_box_append(GTK_BOX(box), cancel_btn);

    // Show the popup window
    gtk_window_present(GTK_WINDOW(popup_window));
}

static void g_update_display_payment(GtkButton *btn, gpointer data) {

    Payment *payments = getPayments();
    int payment_count = getNumbOfPayments();
    displayAllPayments();
    GtkGrid *grid = GTK_GRID(data);

    int col= 0, row = 2;
    while (gtk_grid_get_child_at(grid, col, row) != NULL) {
        while (col < 10) {
            GtkWidget *child = gtk_grid_get_child_at(grid, col, row);
            if (child != NULL) {
                gtk_widget_unparent(child);
            }
            col++;
        }
        row++;
        col = 0;
    }

    for (int i = 0; i < payment_count; i++) {
        char id_payment_text[10];
        snprintf(id_payment_text, sizeof(id_payment_text), "%d", payments[i].id);
        GtkWidget *id_payment_label = gtk_label_new(id_payment_text);

        char id_consult_text[10];
        snprintf(id_consult_text, sizeof(id_consult_text), "%d", payments[i].id);
        GtkWidget *id_consult_label = gtk_label_new(id_consult_text);

        GtkWidget *id_pt_label = gtk_label_new(payments[i].id_pt);

        Patient* patient = getPatient(payments[i].id_pt);
        if (patient == NULL) {
            printf("Patient with ID %s not found\n", payments[i].id_pt);
            continue;
        }
        char fullName_pt[40];
        snprintf(fullName_pt, sizeof(fullName_pt), "%s %s", patient->lName, patient->fName);
        GtkWidget *name_label = gtk_label_new(fullName_pt);

        char amount[10];
        snprintf(amount, sizeof(amount), "%.2lf", payments[i].amount);
        GtkWidget *amount_label = gtk_label_new(amount);

        char date[10];
        struct tm tm = *localtime(&payments[i].DateTime);
        snprintf(date, sizeof(date), "%d/%d/%d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
        GtkWidget *date_label = gtk_label_new(date);

        char time[3];
        snprintf(time, sizeof(time), "%dh", tm.tm_hour);
        GtkWidget *hour_label = gtk_label_new(time);

        GtkWidget *state_label = gtk_label_new(payments[i].state);

        GtkWidget *edit_btn = gtk_button_new_with_label("edit");
        GtkWidget *delete_btn = gtk_button_new_with_label("delete");

        // Attach labels to the grid for each row of patient data
        gtk_grid_attach(grid, id_payment_label, 0, i + 2, 1, 1);
        gtk_grid_attach(grid, id_consult_label, 1, i + 2, 1, 1);
        gtk_grid_attach(grid, id_pt_label, 2, i + 2, 1, 1);
        gtk_grid_attach(grid, name_label, 3, i + 2, 1, 1);
        gtk_grid_attach(grid, amount_label, 4, i + 2, 1, 1);
        gtk_grid_attach(grid, date_label, 5, i + 2, 1, 1);
        gtk_grid_attach(grid, hour_label, 6, i + 2, 1, 1);
        gtk_grid_attach(grid, state_label, 7, i + 2, 1, 1);
        gtk_grid_attach(grid, edit_btn, 8, i + 2, 1, 1);
        gtk_grid_attach(grid, delete_btn, 9, i + 2, 1, 1);

        g_signal_connect(delete_btn, "clicked", G_CALLBACK(delete_payment_popup), &payments[i].id);
        g_signal_connect(edit_btn, "clicked", G_CALLBACK(edit_payment_popup), &payments[i]);
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

    wins[4] = gtk_builder_get_object(builder, "rendezvousWindow");
    if (!wins[4]) {
        g_printerr("Failed to get 'Rendezvous' window from builder UI file\n");
        g_free(wins);
        return NULL;
    }

    wins[5] = gtk_builder_get_object(builder, "paymentWindow");
    if (!wins[5]) {
        g_printerr("Failed to get 'payment' window from builder UI file\n");
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

static void switch_to_payment(GtkButton *btn, gpointer user_data) {
    GObject **windows = (GObject **)user_data;
    GtkWidget *window_payment = GTK_WIDGET(windows[5]);
    GtkWidget *window_prev = gtk_widget_get_ancestor(GTK_WIDGET(btn), GTK_TYPE_WINDOW);

    if (GTK_IS_WINDOW(window_prev)) {
        gtk_widget_set_visible(window_prev, FALSE);
        gtk_widget_set_visible(window_payment, TRUE);
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

// set menu windows objects
static void set_menu_win_objs(GtkBuilder *builder, GObject **win) {

    GObject *btn_patient = gtk_builder_get_object(builder, "btn_patient");
    GObject *btn_rdv = gtk_builder_get_object(builder, "btn_rdv");
    GObject *btn_consult = gtk_builder_get_object(builder, "btn_consult");
    GObject *btn_payment = gtk_builder_get_object(builder, "btn_payment");
    GObject *btn_out = gtk_builder_get_object(builder, "btn_out");

    g_signal_connect(btn_patient, "clicked", G_CALLBACK(switch_to_patient), win);
    g_signal_connect(btn_rdv, "clicked", G_CALLBACK(switch_to_rendezvous), win);
    g_signal_connect(btn_consult, "clicked", G_CALLBACK(switch_to_consult), win);
    g_signal_connect(btn_payment, "clicked", G_CALLBACK(switch_to_payment), win);
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
    GtkWidget *name_pt_header = gtk_label_new("Patient Name");
    GtkWidget *date_header = gtk_label_new("Date");
    GtkWidget *time_header = gtk_label_new("Time");
    GtkWidget *state_header = gtk_label_new("state");

    // Add headers to the first row of the grid
    gtk_grid_attach(grid, id_header, 0, 1, 1, 1);
    gtk_grid_attach(grid, id_pt_header, 1, 1, 1, 1);
    gtk_grid_attach(grid, name_pt_header, 2, 1, 1, 1);
    gtk_grid_attach(grid, date_header, 3, 1, 1, 1);
    gtk_grid_attach(grid, time_header, 4, 1, 1, 1);
    gtk_grid_attach(grid, state_header, 5, 1, 1, 1);

    g_update_display_rdv(NULL, grid);

    // Create and attach buttons after the last row of patient data
    GObject *refresh_rendezvous_btn = gtk_builder_get_object(builder, "refresh_rendezvous_btn");
    GObject *add_rendezvous_btn = gtk_builder_get_object(builder, "add_rendezvous_btn");
    GObject *return_rendezvous_btn = gtk_builder_get_object(builder, "return_rendezvous_btn");

    // Connect button signals to their respective callbacks
    g_signal_connect(refresh_rendezvous_btn, "clicked", G_CALLBACK(g_update_display_rdv), grid);
    g_signal_connect(add_rendezvous_btn,"clicked", G_CALLBACK(add_rdv_popup), NULL);
    g_signal_connect(return_rendezvous_btn, "clicked", G_CALLBACK(switch_to_menu), win);
}

// set payment windows objects
static void set_payment_win(GtkBuilder *builder, GObject **win) {
    GtkGrid *grid = GTK_GRID(gtk_builder_get_object(builder, "grid_payment"));

    // Define grid headers
    GtkWidget *id_header = gtk_label_new("Payment ID");
    GtkWidget *id_consult_header = gtk_label_new("Consultation ID");
    GtkWidget *id_pt_header = gtk_label_new("Patient ID");
    GtkWidget *name_pt_header = gtk_label_new("Patient Name");
    GtkWidget *amount_header = gtk_label_new("Amount");
    GtkWidget *date_header = gtk_label_new("Date");
    GtkWidget *hour_header = gtk_label_new("Hour");
    GtkWidget *state_header = gtk_label_new("State");

    // Add headers to the first row of the grid
    gtk_grid_attach(grid, id_header, 0, 1, 1, 1);
    gtk_grid_attach(grid, id_consult_header, 1, 1, 1, 1);
    gtk_grid_attach(grid, id_pt_header, 2, 1, 1, 1);
    gtk_grid_attach(grid, name_pt_header, 3, 1, 1, 1);
    gtk_grid_attach(grid, amount_header, 4, 1, 1, 1);
    gtk_grid_attach(grid, date_header, 5, 1, 1, 1);
    gtk_grid_attach(grid, hour_header, 6, 1, 1, 1);
    gtk_grid_attach(grid, state_header, 7, 1, 1, 1);

    g_update_display_payment(NULL, grid);

    // Create and attach buttons after the last row of patient data
    GObject *refresh_payment_btn = gtk_builder_get_object(builder, "refresh_payment_btn");
    GObject *add_payment_btn = gtk_builder_get_object(builder, "add_payment_btn");
    GObject *return_payment_btn = gtk_builder_get_object(builder, "return_payment_btn");

    // Connect button signals to their respective callbacks
    g_signal_connect(refresh_payment_btn, "clicked", G_CALLBACK(g_update_display_payment), grid);
    g_signal_connect(add_payment_btn,"clicked", G_CALLBACK(add_payment_popup), NULL);
    g_signal_connect(return_payment_btn, "clicked", G_CALLBACK(switch_to_menu), win);
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
    gtk_window_set_application(GTK_WINDOW(wins[5]), app);

    set_login_win_objs(builder, wins);
    set_menu_win_objs(builder, wins);
    set_patient_win(builder, wins);
    set_consult_win(builder, wins);
    set_rendezvous_win(builder, wins);
    set_payment_win(builder, wins);

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

