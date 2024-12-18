#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <regex.h>
#include "consultation.h"
#include "login.h"
#include "patient.h"
#include "rendezvous.h"
#include "payment.h"
#include "builder.h"
#include "utils.h"

void window_close(GtkButton *btn, GtkWindow *window) {
    gtk_window_close(window);
}

//===================================================================//
// Patient functionalities
//===================================================================//

void g_add_patient(GtkButton *btn, gpointer data) {
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
    const char* cin = get_text_from_entry(GTK_WIDGET(entry_cin));
    const char* lName = get_text_from_entry(GTK_WIDGET(entry_lName));
    const char* fName = get_text_from_entry(GTK_WIDGET(entry_fName));
    int age = get_int_from_entry(GTK_WIDGET(entry_age));
    const char* addr = get_text_from_entry(GTK_WIDGET(entry_addr));
    const char* phone = get_text_from_entry(GTK_WIDGET(entry_phone));

    int cin_valid = isCinValid(cin);
    int patient_exists = ifPatientExists(cin);
    int first_name_valid = isNameValid(fName);
    int last_name_valid = isNameValid(lName);
    int age_valid = isAgeValid(age);
    int address_valid = isTextValid(addr);
    int phone_valid = isPhoneValid(phone);

    if (!cin_valid) {
        gtk_label_set_text(GTK_LABEL(label_cin), "CIN can only contain letters and numbers ([A-Za-z][0-9])!");
    } else {
        if (patient_exists) {
            gtk_label_set_text(GTK_LABEL(label_cin), "patient already exists");
        } else {
            gtk_label_set_text(GTK_LABEL(label_cin), "");
        }
    }

    if (!first_name_valid) {
        gtk_label_set_text(GTK_LABEL(label_lName), "Name can only contain letters (A-Z or a-z) and space if needed!");
    } else {
        gtk_label_set_text(GTK_LABEL(label_lName), "");
    }

    if (!last_name_valid) {
        gtk_label_set_text(GTK_LABEL(label_fName), "Name can only contain letters (A-Z or a-z) and space if needed!");
    } else {
        gtk_label_set_text(GTK_LABEL(label_fName), "");
    }

    if (!age_valid) {
        gtk_label_set_text(GTK_LABEL(label_age), "age must be between 18 and 120");
    } else {
        gtk_label_set_text(GTK_LABEL(label_age), "");
    }

    if (!address_valid) {
        gtk_label_set_text(GTK_LABEL(label_addr), "address should not left empty!");
    } else {
        gtk_label_set_text(GTK_LABEL(label_addr), "");
    }

    if (!phone_valid) {
        gtk_label_set_text(GTK_LABEL(label_phone), "The phone number must be exactly 10 digits and contain only numeric characters (0–9)!");
    } else {
        gtk_label_set_text(GTK_LABEL(label_phone), "");
    }

    if (cin_valid && !patient_exists && first_name_valid && last_name_valid && age_valid && address_valid && phone_valid) {
        Patient patient;
        strcpy(patient.cin, cin);
        strcpy(patient.lName, lName);
        strcpy(patient.fName, fName);
        patient.age = age;
        strcpy(patient.address, addr);
        strcpy(patient.phone, phone);

        addPatient(patient);
        window_close(NULL, GTK_WINDOW(gtk_widget_get_ancestor(GTK_WIDGET(btn), GTK_TYPE_WINDOW)));

    } else {
        g_printerr("Error: Patient information are incorrect\n");
    }
}

void add_patient_popup(GtkButton *btn, gpointer data) {
    // Create the popup window
    GtkWidget *popup_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(popup_window), "Add patient");
    gtk_window_set_default_size(GTK_WINDOW(popup_window), 360, 500);

    gtk_window_set_modal(GTK_WINDOW(popup_window), TRUE);

    // Create a vertical box for layout
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_size_request(box, 360, 500);
    gtk_window_set_child(GTK_WINDOW(popup_window), box);
    gtk_widget_set_halign(GTK_WIDGET(box), GTK_ALIGN_CENTER);
    gtk_widget_set_valign(GTK_WIDGET(box), GTK_ALIGN_CENTER);
    set_widget_css(box, "edit_box", "box#edit_box{padding: 10px;}");
    gtk_window_set_child(GTK_WINDOW(popup_window), box);

    // Create and set up the label for ID

    GtkWidget *cin_err_label = gtk_label_new("");
    set_widget_css(GTK_WIDGET(cin_err_label), "cin_err_label", "label#cin_err_label{color: red;}");
    GtkWidget *cin_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(cin_entry), "CIN");

    GtkWidget *lName_err_label = gtk_label_new("");
    set_widget_css(GTK_WIDGET(lName_err_label), "lName_err_label", "label#lName_err_label{color: red;}");
    GtkWidget *lName_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(lName_entry), "Last Name");

    GtkWidget *fName_err_label = gtk_label_new("");
    set_widget_css(GTK_WIDGET(fName_err_label), "fName_err_label", "label#fName_err_label{color: red;}");
    GtkWidget *fName_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(fName_entry), "First Name");

    GtkWidget *age_err_label = gtk_label_new("");
    set_widget_css(GTK_WIDGET(age_err_label), "age_err_label", "label#age_err_label{color: red;}");
    GtkWidget *age_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(age_entry), "Age");

    GtkWidget *addr_err_label = gtk_label_new("");
    set_widget_css(GTK_WIDGET(addr_err_label), "addr_err_label", "label#addr_err_label{color: red;}");
    GtkWidget *addr_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(addr_entry), "Address");

    GtkWidget *phone_err_label = gtk_label_new("");
    set_widget_css(GTK_WIDGET(phone_err_label), "phone_err_label", "label#phone_err_label{color: red;}");
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
    GtkWidget *add_btn = gtk_button_new_with_label("Add");
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

void g_delete_patient(GtkButton *btn, gpointer data) {
    char *cin_id = data;
    printf("Patient with id: %s have been deleted\n", cin_id);
    deletePatient(cin_id);
    // delete patient Rendezvous when he is deleted
    Rendezvous *rendezvous = getRVsbyCin(cin_id);
    int count = getNumbOfRVsbyCin(cin_id);
    for (int i = 0; i < count; i++) {
        cancelRV(rendezvous[i].id);
    }
    deleteAccount(cin_id);
}

void delete_patient_popup(GtkButton *btn, gpointer data) {
    char* cin = data;
    GtkWidget *popup_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(popup_window), "confirmer la suppression ");
    gtk_window_set_default_size(GTK_WINDOW(popup_window), 300, 150);

    gtk_window_set_modal(GTK_WINDOW(popup_window), TRUE);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(popup_window), box);
    gtk_widget_set_halign(GTK_WIDGET(box), GTK_ALIGN_CENTER);
    gtk_widget_set_valign(GTK_WIDGET(box), GTK_ALIGN_CENTER);
    set_widget_css(box, "del_pt_box", "box#del_pt_box{padding: 20px;}");

    char msg[100];
    sprintf(msg, "are you sure about deleting the patient %s", cin);

    GtkWidget *msg_label = gtk_label_new(msg);
    GtkWidget *del_btn = gtk_button_new_with_label("delete");
    set_widget_css(del_btn, "del_btn", "button#del_btn{background: #ff5040;}");
    GtkWidget *cancel_btn = gtk_button_new_with_label("Cancel");

    gtk_box_append(GTK_BOX(box), msg_label);
    gtk_box_append(GTK_BOX(box), del_btn);
    gtk_box_append(GTK_BOX(box), cancel_btn);

    g_signal_connect(del_btn, "clicked", G_CALLBACK(g_delete_patient), data);
    g_signal_connect(del_btn, "clicked", G_CALLBACK(window_close), GTK_WIDGET(popup_window));
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(window_close), GTK_WIDGET(popup_window));

    gtk_window_present(GTK_WINDOW(popup_window));
}

void g_edit_patient(GtkButton *btn, gpointer data) {
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
    const char* cin = get_text_from_entry(GTK_WIDGET(entry_cin));
    const char* lName = get_text_from_entry(GTK_WIDGET(entry_lName));
    const char* fName = get_text_from_entry(GTK_WIDGET(entry_fName));
    int age = get_int_from_entry(GTK_WIDGET(entry_age));
    const char* addr = get_text_from_entry(GTK_WIDGET(entry_addr));
    const char* phone = get_text_from_entry(GTK_WIDGET(entry_phone));

    int cin_valid = isCinValid(cin);
    int patient_exist = ifPatientExists(cin);
    int first_name_valid = isNameValid(fName);
    int last_name_valid = isNameValid(lName);
    int age_valid = isAgeValid(age);
    int address_valid = isTextValid(addr);
    int phone_valid = isPhoneValid(phone);

    if (!cin_valid) {
        gtk_label_set_text(GTK_LABEL(label_cin), "CIN can only contain letters and numbers ([A-Za-z][0-9])!");
    } else {
        if (patient_exist) {
            gtk_label_set_text(GTK_LABEL(label_cin), "patient already exists");
        } else {
            gtk_label_set_text(GTK_LABEL(label_cin), "");
        }
    }

    if (!first_name_valid) {
        gtk_label_set_text(GTK_LABEL(label_lName), "Name can only contain letters (A-Z or a-z) and space if needed!");
    } else {
        gtk_label_set_text(GTK_LABEL(label_lName), "");
    }

    if (!last_name_valid) {
        gtk_label_set_text(GTK_LABEL(label_fName), "Name can only contain letters (A-Z or a-z) and space if needed!");
    } else {
        gtk_label_set_text(GTK_LABEL(label_fName), "");
    }

    if (!age_valid) {
        gtk_label_set_text(GTK_LABEL(label_age), "age must be between 18 and 120");
    } else {
        gtk_label_set_text(GTK_LABEL(label_age), "");
    }

    if (!address_valid) {
        gtk_label_set_text(GTK_LABEL(label_addr), "address should not left empty!");
    } else {
        gtk_label_set_text(GTK_LABEL(label_addr), "");
    }

    if (!phone_valid) {
        gtk_label_set_text(GTK_LABEL(label_phone), "The phone number must be exactly 10 digits and contain only numeric characters (0–9)!");
    } else {
        gtk_label_set_text(GTK_LABEL(label_phone), "");
    }

    if (cin_valid && patient_exist && first_name_valid && last_name_valid && age_valid && address_valid && phone_valid) {
        Patient patient;
        strcpy(patient.cin, cin);
        strcpy(patient.lName, lName);
        strcpy(patient.fName, fName);
        patient.age = age;
        strcpy(patient.address, addr);
        strcpy(patient.phone, phone);
        modifyPatient(patient);
        window_close(NULL, GTK_WINDOW(gtk_widget_get_ancestor(GTK_WIDGET(btn), GTK_TYPE_WINDOW)));

    } else {
        g_printerr("Error: Patient information are not correct\n");
    }
}

void edit_patient_popup(GtkButton *btn, gpointer data) {
    const Patient *patient = data;

    // Create the popup window
    GtkWidget *popup_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(popup_window), "Edit Patient");
    gtk_window_set_default_size(GTK_WINDOW(popup_window), 360, 400);

    gtk_window_set_modal(GTK_WINDOW(popup_window), TRUE);

    // Create a vertical box for layout
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_size_request(box, 360, 500);
    gtk_window_set_child(GTK_WINDOW(popup_window), box);
    gtk_widget_set_halign(GTK_WIDGET(box), GTK_ALIGN_CENTER);
    gtk_widget_set_valign(GTK_WIDGET(box), GTK_ALIGN_CENTER);
    set_widget_css(box, "edit_box", "box#edit_box{padding: 10px;}");
    gtk_window_set_child(GTK_WINDOW(popup_window), box);

    GtkWidget *cin_err_label = gtk_label_new("");
    set_widget_css(GTK_WIDGET(cin_err_label), "cin_err_label", "label#cin_err_label{color: red;}");

    GtkWidget *cin_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(cin_entry), "CIN");
    set_text_entry(cin_entry, patient->cin);

    GtkWidget *lName_err_label = gtk_label_new("");
    set_widget_css(GTK_WIDGET(lName_err_label), "lName_err_label", "label#lName_err_label{color: red;}");

    GtkWidget *lName_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(lName_entry), "Last Name");
    set_text_entry(lName_entry, patient->lName);

    GtkWidget *fName_err_label = gtk_label_new("");
    set_widget_css(GTK_WIDGET(fName_err_label), "fName_err_label", "label#fName_err_label{color: red;}");

    GtkWidget *fName_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(fName_entry), "First Name");
    set_text_entry(fName_entry, patient->fName);

    GtkWidget *age_err_label = gtk_label_new("");
    set_widget_css(GTK_WIDGET(age_err_label), "age_err_label", "label#age_err_label{color: red;}");

    GtkWidget *age_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(age_entry), "Age");
    char age_text[4];
    sprintf(age_text, "%d", patient->age);
    set_text_entry(age_entry, age_text);

    GtkWidget *addr_err_label = gtk_label_new("");
    set_widget_css(GTK_WIDGET(addr_err_label), "addr_err_label", "label#addr_err_label{color: red;}");

    GtkWidget *addr_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(addr_entry), "Address");
    set_text_entry(addr_entry, patient->address);

    GtkWidget *phone_err_label = gtk_label_new("");
    set_widget_css(GTK_WIDGET(phone_err_label), "phone_err_label", "label#phone_err_label{color: red;}");

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
    GtkWidget *edit_btn = gtk_button_new_with_label("Edit");
    GtkWidget *cancel_btn = gtk_button_new_with_label("Cancel");

    // Connect the cancel button to close the window
    g_signal_connect(edit_btn, "clicked", G_CALLBACK(g_edit_patient), widget);
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
    gtk_box_append(GTK_BOX(box), edit_btn);
    gtk_box_append(GTK_BOX(box), cancel_btn);

    // Show the popup window
    gtk_window_present(GTK_WINDOW(popup_window));
}

void g_update_display_patient(GtkButton *btn, gpointer data) {

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
        set_widget_css(GTK_WIDGET(cin_label), "header",
        "label#header {"
            "   font-size: 18px;"
            "   border: 1px solid #f9d4e4;"
            "   padding: 10px 30px;"
            "}");
        GtkWidget *lName_label = gtk_label_new(patients[i].lName);
        set_widget_css(GTK_WIDGET(lName_label), "header",
        "label#header {"
            "   font-size: 18px;"
            "   border: 1px solid #f9d4e4;"
            "   padding: 10px 30px;"

            "}");
        GtkWidget *fName_label = gtk_label_new(patients[i].fName);
        set_widget_css(GTK_WIDGET(fName_label), "header",
        "label#header {"
            "   font-size: 18px;"
            "   border: 1px solid #f9d4e4;"
            "   padding: 10px 30px;"
            "}");
        GtkWidget *age_label = gtk_label_new(age_text);
        set_widget_css(GTK_WIDGET(age_label), "header",
        "label#header {"
            "   font-size: 18px;"
            "   border: 1px solid #f9d4e4;"
            "   padding: 10px 30px;"
            "}");
        GtkWidget *addr_label = gtk_label_new(patients[i].address);
        set_widget_css(GTK_WIDGET(addr_label), "header",
        "label#header {"
            "   font-size: 18px;"
            "   border: 1px solid #f9d4e4;"
            "   padding: 10px 30px;"
            "}");
        GtkWidget *phone_label = gtk_label_new(patients[i].phone);
        set_widget_css(GTK_WIDGET(phone_label), "header",
        "label#header {"
            "   font-size: 18px;"
            "   border: 1px solid #f9d4e4;"
            "   padding: 10px 30px;"
            "}");
        GtkWidget *edit_btn = gtk_button_new_with_label("edit");
        set_widget_css(GTK_WIDGET(edit_btn), "btn",
        "button#btn {"
        "   border: 1px solid #f9d4e4;"
        "   padding: 10px 30px;"
        "   font-size: 18px;"
        "   border-radius: 0px;"
        "   background: transparent;"
        "   box-shadow: none;"
            "}");
        GtkWidget *edit_image = gtk_image_new_from_file("imgs/edit_1.svg");
        gtk_image_set_pixel_size(GTK_IMAGE(edit_image), 25);
        set_widget_css(GTK_WIDGET(edit_image), "edit_image",
        "image#edit_image {"
            "   transition: transform 0.3s;"
        "}"
        "image#edit_image:hover {"
            "   transform: scale(1.1);"
            "   transition: transform 0.3s;"
        "}");

        gtk_button_set_child(GTK_BUTTON(edit_btn), edit_image);

        GtkWidget *delete_btn = gtk_button_new_with_label("delete");
        set_widget_css(GTK_WIDGET(delete_btn), "btn",
        "button#btn {"
        "   border: 1px solid #f9d4e4;"
        "   padding: 10px 30px;"
        "   font-size: 18px;"
        "   border-radius: 0px;"
        "   background: transparent;"
        "   box-shadow: none;"
            "}");
        GtkWidget *delete_image = gtk_image_new_from_file("imgs/delete_1.svg");
        gtk_image_set_pixel_size(GTK_IMAGE(delete_image), 25);
        set_widget_css(GTK_WIDGET(delete_image), "delete_img",
        "image#delete_img {"
            "   transition: transform 0.3s;"
        "}"
        "image#delete_img:hover {"
            "   transform: scale(1.1);"
            "   transition: transform 0.3s;"
        "}");
        gtk_button_set_child(GTK_BUTTON(delete_btn), delete_image);

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

void g_add_consult(GtkButton *btn, gpointer data) {
    GtkWidget **widgets = data;
    if (!widgets) {
        g_printerr("Error: objects is NULL\n");
        return;
    }

    //Entries
    GtkEntry *entry_id_patient = GTK_ENTRY(widgets[0]);
    GtkEntry *entry_symptoms = GTK_ENTRY(widgets[1]);
    GtkEntry *entry_diagnosis = GTK_ENTRY(widgets[2]);
    GtkEntry *entry_treatmentPlan = GTK_ENTRY(widgets[3]);

    //Labels
    GtkLabel *label_id_patient = GTK_LABEL(widgets[4]);
    GtkLabel *label_symptoms = GTK_LABEL(widgets[5]);
    GtkLabel *label_diagnosis = GTK_LABEL(widgets[6]);
    GtkLabel *label_treatmentPlan = GTK_LABEL(widgets[7]);

    if (!entry_id_patient || !entry_symptoms || !entry_diagnosis || !entry_treatmentPlan) {
        g_printerr("Error: One of the entries is NULL\n");
        return;
    }

    // Retrieve values from each entry
    const char* patient_id = get_text_from_entry(GTK_WIDGET(entry_id_patient));
    const char* symptoms = get_text_from_entry(GTK_WIDGET(entry_symptoms));
    const char* diagnosis = get_text_from_entry(GTK_WIDGET(entry_diagnosis));
    const char* treatmentPlan = get_text_from_entry(GTK_WIDGET(entry_treatmentPlan));

    int cin_valid = isCinValid(patient_id);
    int patient_exists = ifPatientExists(patient_id);
    int symptoms_valid = isTextValid(symptoms);
    int diagnosis_valid = isTextValid(diagnosis);
    int treatment_plan_valid = isTextValid(treatmentPlan);

    if (!cin_valid) {
        gtk_label_set_text(GTK_LABEL(label_id_patient), "CIN can only contain letters and numbers ([A-Za-z][0-9])!");
    } else {
        if (!patient_exists) {
            gtk_label_set_text(GTK_LABEL(label_id_patient), "patient does not exists");
        } else {
            gtk_label_set_text(GTK_LABEL(label_id_patient), "");
        }
    }

    if (!symptoms_valid) {
        gtk_label_set_text(label_symptoms, "symptoms should not left empty!");
    } else {
        gtk_label_set_text(label_symptoms, "");
    }

    if (!diagnosis_valid) {
        gtk_label_set_text(label_diagnosis, "diagnosis should not left empty!");
    } else {
        gtk_label_set_text(label_diagnosis, "");
    }

    if (!treatment_plan_valid) {
        gtk_label_set_text(label_treatmentPlan, "treatment plan should not left empty!");
    } else {
        gtk_label_set_text(label_treatmentPlan, "");
    }

    if (cin_valid && patient_exists && symptoms_valid && diagnosis_valid && treatment_plan_valid) {
        Consult consult;
        Patient *patient = getPatient(patient_id);
        consult.id = get_next_valid_consult_id();
        strcpy(consult.id_pt, patient_id);
        strcpy(consult.patient_lName, patient->lName);
        strcpy(consult.patient_fName, patient->fName);
        strcpy(consult.symptoms, symptoms);
        strcpy(consult.diagnosis, diagnosis);
        strcpy(consult.treatmentPlan, treatmentPlan);
        consult.dateTime = time(NULL);
        addConsultation(consult);
        free(patient);
        window_close(NULL, GTK_WINDOW(gtk_widget_get_ancestor(GTK_WIDGET(btn), GTK_TYPE_WINDOW)));
    }
}

void add_consult_popup(GtkButton *btn, gpointer data) {
    // Create the popup window
    GtkWidget *popup_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(popup_window), "Add consultation");
    gtk_window_set_default_size(GTK_WINDOW(popup_window), 360, 300);

    gtk_window_set_modal(GTK_WINDOW(popup_window), TRUE);

    // Create a vertical box for layout
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_size_request(box, 360, 400);
    gtk_window_set_child(GTK_WINDOW(popup_window), box);
    gtk_widget_set_halign(GTK_WIDGET(box), GTK_ALIGN_CENTER);
    gtk_widget_set_valign(GTK_WIDGET(box), GTK_ALIGN_CENTER);
    set_widget_css(box, "edit_box", "box#edit_box{padding: 10px;}");
    gtk_window_set_child(GTK_WINDOW(popup_window), box);

    GtkWidget *id_pt_err_label = gtk_label_new("");
    set_widget_css(GTK_WIDGET(id_pt_err_label), "id_pt_err_label", "label#id_pt_err_label{color: red;}");
    GtkWidget *id_pt_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(id_pt_entry), "Patient Id (CIN)");

    GtkWidget *symptoms_err_label = gtk_label_new("");
    set_widget_css(GTK_WIDGET(symptoms_err_label), "symptoms_err_label", "label#symptoms_err_label{color: red;}");
    GtkWidget *symptoms_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(symptoms_entry), "Symptoms");

    GtkWidget *diagnosis_err_label = gtk_label_new("");
    set_widget_css(GTK_WIDGET(diagnosis_err_label), "diagnosis_err_label", "label#diagnosis_err_label{color: red;}");
    GtkWidget *diagnosis_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(diagnosis_entry), "Diagnosis");

    GtkWidget *treatmentPlan_err_label = gtk_label_new("");
    set_widget_css(GTK_WIDGET(treatmentPlan_err_label), "treatmentPlan_err_label", "label#treatmentPlan_err_label{color: red;}");
    GtkWidget *treatmentPlan_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(treatmentPlan_entry), "Treatment plan");


    GtkWidget **widgets =  g_new(GtkWidget*, 8);
    widgets[0] = id_pt_entry;
    widgets[1] = symptoms_entry;
    widgets[2] = diagnosis_entry;
    widgets[3] = treatmentPlan_entry;
    widgets[4] = id_pt_err_label;
    widgets[5] = symptoms_err_label;
    widgets[6] = diagnosis_err_label;
    widgets[7] = treatmentPlan_err_label;

    // Create the buttons
    GtkWidget *add_btn = gtk_button_new_with_label("Add");
    GtkWidget *cancel_btn = gtk_button_new_with_label("Cancel");

    // Connect the cancel button to close the window
    g_signal_connect(add_btn, "clicked", G_CALLBACK(g_add_consult), widgets);
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(window_close), GTK_WINDOW(popup_window));

    // Append widgets to the box
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

void g_delete_consult(GtkButton *btn, gpointer data) {
    int *id_value = data;
    printf("Consultation with id: %d have been deleted\n", *id_value);
    deleteConsultation(*id_value);
}

void delete_consult_popup(GtkButton *btn, gpointer data) {
    int* id = data;
    GtkWidget *popup_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(popup_window), "Confirm deletion");
    gtk_window_set_default_size(GTK_WINDOW(popup_window), 200, 100);

    gtk_window_set_modal(GTK_WINDOW(popup_window), TRUE);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(popup_window), box);
    gtk_widget_set_halign(GTK_WIDGET(box), GTK_ALIGN_CENTER);
    gtk_widget_set_valign(GTK_WIDGET(box), GTK_ALIGN_CENTER);
    set_widget_css(box, "del_box", "box#del_box{padding: 20px;}");

    char msg[100];
    sprintf(msg, "are you sure about deleting this consultation (%d)", *id);

    GtkWidget *msg_label = gtk_label_new(msg);
    GtkWidget *del_btn = gtk_button_new_with_label("Delete");
    set_widget_css(del_btn, "del_btn", "button#del_btn{background: #ff5040;}");
    GtkWidget *cancel_btn = gtk_button_new_with_label("Cancel");

    gtk_box_append(GTK_BOX(box), msg_label);
    gtk_box_append(GTK_BOX(box), del_btn);
    gtk_box_append(GTK_BOX(box), cancel_btn);

    g_signal_connect(del_btn, "clicked", G_CALLBACK(g_delete_consult), data);
    g_signal_connect(del_btn, "clicked", G_CALLBACK(window_close), GTK_WIDGET(popup_window));
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(window_close), GTK_WIDGET(popup_window));

    gtk_window_present(GTK_WINDOW(popup_window));
}

void g_edit_consult(GtkButton *btn, gpointer data) {

    GtkWidget **widgets = data;
    if (!widgets) {
        g_printerr("Error: widgets is NULL\n");
        return;
    }

    //Entries
    GtkEntry *entry_symptoms = GTK_ENTRY(widgets[0]);
    GtkEntry *entry_diagnosis = GTK_ENTRY(widgets[1]);
    GtkEntry *entry_treatmentPlan = GTK_ENTRY(widgets[2]);

    //Labels
    GtkLabel *label_symptoms = GTK_LABEL(widgets[3]);
    GtkLabel *label_diagnosis = GTK_LABEL(widgets[4]);
    GtkLabel *label_treatmentPlan = GTK_LABEL(widgets[5]);

    int consult_id = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(btn), "consultID"));
    // Retrieve values from each entry
    const char* symptoms = get_text_from_entry(GTK_WIDGET(entry_symptoms));
    const char* diagnosis = get_text_from_entry(GTK_WIDGET(entry_diagnosis));
    const char* treatmentPlan = get_text_from_entry(GTK_WIDGET(entry_treatmentPlan));

    int symptoms_valid = isTextValid(symptoms);
    int diagnosis_valid = isTextValid(diagnosis);
    int treatment_plan_valid = isTextValid(treatmentPlan);

    if (!symptoms_valid) {
        gtk_label_set_text(label_symptoms, "symptoms should not left empty!");
    } else {
        gtk_label_set_text(label_symptoms, "");
    }

    if (!diagnosis_valid) {
        gtk_label_set_text(label_diagnosis, "diagnosis should not left empty!");
    } else {
        gtk_label_set_text(label_diagnosis, "");
    }

    if (!treatment_plan_valid) {
        gtk_label_set_text(label_treatmentPlan, "treatment plan should not left empty!");
    } else {
        gtk_label_set_text(label_treatmentPlan, "");
    }

    if (symptoms_valid && diagnosis_valid && treatment_plan_valid) {
        Consult *consult = getConsultation(consult_id);
        consult->id = consult_id;
        strcpy(consult->symptoms, symptoms);
        strcpy(consult->diagnosis, diagnosis);
        strcpy(consult->treatmentPlan, treatmentPlan);
        modifyConsultation(*consult);
        free(consult);
        window_close(NULL, GTK_WINDOW(gtk_widget_get_ancestor(GTK_WIDGET(btn), GTK_TYPE_WINDOW)));
    }
}

void edit_consult_popup(GtkButton *btn, gpointer data) {
    const Consult *consult = data;
    // Create the popup window
    GtkWidget *popup_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(popup_window), "Edit consultation");
    gtk_window_set_default_size(GTK_WINDOW(popup_window), 360, 400);

    gtk_window_set_modal(GTK_WINDOW(popup_window), TRUE);

    // Create a vertical box for layout
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_size_request(box, 360, 400);
    gtk_window_set_child(GTK_WINDOW(popup_window), box);
    gtk_widget_set_halign(GTK_WIDGET(box), GTK_ALIGN_CENTER);
    gtk_widget_set_valign(GTK_WIDGET(box), GTK_ALIGN_CENTER);
    set_widget_css(box, "edit_box", "box#edit_box{padding: 10px;}");
    gtk_window_set_child(GTK_WINDOW(popup_window), box);

    GtkWidget *symptoms_err_label = gtk_label_new("");
    set_widget_css(GTK_WIDGET(symptoms_err_label), "symptoms_err_label", "label#symptoms_err_label{color: red;}");

    GtkWidget *symptoms_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(symptoms_entry), "Symptoms");
    set_text_entry(symptoms_entry, consult->symptoms);

    GtkWidget *diagnosis_err_label = gtk_label_new("");
    set_widget_css(GTK_WIDGET(diagnosis_err_label), "diagnosis_err_label", "label#diagnosis_err_label{color: red;}");

    GtkWidget *diagnosis_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(diagnosis_entry), "Diagnosis");
    set_text_entry(diagnosis_entry, consult->diagnosis);

    GtkWidget *treatmentPlan_err_label = gtk_label_new("");
    set_widget_css(GTK_WIDGET(treatmentPlan_err_label), "treatmentPlan_err_label", "label#treatmentPlan_err_label{color: red;}");

    GtkWidget *treatmentPlan_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(treatmentPlan_entry), "Treatment plan");
    set_text_entry(treatmentPlan_entry, consult->treatmentPlan);

    GtkWidget **widget =  g_new(GtkWidget*, 6);
    widget[0] = symptoms_entry;
    widget[1] = diagnosis_entry;
    widget[2] = treatmentPlan_entry;
    widget[3] = symptoms_err_label;
    widget[4] = diagnosis_err_label;
    widget[5] = treatmentPlan_err_label;

    // Create the buttons
    GtkWidget *edit_btn = gtk_button_new_with_label("Edit");
    GtkWidget *cancel_btn = gtk_button_new_with_label("Cancel");

    g_object_set_data(G_OBJECT(edit_btn), "consultID", GINT_TO_POINTER(consult->id));

    // Connect the cancel button to close the window
    g_signal_connect(edit_btn, "clicked", G_CALLBACK(g_edit_consult), widget);
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(window_close), GTK_WINDOW(popup_window));

    // Append widgets to the box
    gtk_box_append(GTK_BOX(box), symptoms_err_label);
    gtk_box_append(GTK_BOX(box), symptoms_entry);
    gtk_box_append(GTK_BOX(box), diagnosis_err_label);
    gtk_box_append(GTK_BOX(box), diagnosis_entry);
    gtk_box_append(GTK_BOX(box), treatmentPlan_err_label);
    gtk_box_append(GTK_BOX(box), treatmentPlan_entry);
    gtk_box_append(GTK_BOX(box), edit_btn);
    gtk_box_append(GTK_BOX(box), cancel_btn);

    // Show the popup window
    gtk_window_present(GTK_WINDOW(popup_window));
}

void g_update_display_consult(GtkButton *btn, gpointer data) {
    Consult *consults = getConsultations();
    int consult_count = getNumbOfConsults();

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

    for (int i = 0; i < consult_count; i++) {
        char id_text[10];
        snprintf(id_text, sizeof(id_text), "%d", consults[i].id);

        GtkWidget *id_label = gtk_label_new(id_text);
        set_widget_css(GTK_WIDGET(id_label), "header",
        "label#header {"
        "   font-size: 18px;"
        "   border: 1px solid #b5e5dc;"
        "   padding: 10px 30px;"
        "}");

        GtkWidget *id_pt_label = gtk_label_new(consults[i].id_pt);
        set_widget_css(GTK_WIDGET(id_pt_label), "header",
        "label#header {"
        "   font-size: 18px;"
        "   border: 1px solid #b5e5dc;"
        "   padding: 10px 30px;"
        "}");

        char fullName_pt[40];
        snprintf(fullName_pt, sizeof(fullName_pt), "%s %s", consults[i].patient_lName, consults[i].patient_fName);
        GtkWidget *name_label = gtk_label_new(fullName_pt);
        set_widget_css(GTK_WIDGET(name_label), "header",
        "label#header {"
        "   font-size: 18px;"
        "   border: 1px solid #b5e5dc;"
        "   padding: 10px 30px;"
        "}");

        GtkWidget *symptoms_label = gtk_label_new(consults[i].symptoms);
        set_widget_css(GTK_WIDGET(symptoms_label), "header",
        "label#header {"
        "   font-size: 18px;"
        "   border: 1px solid #b5e5dc;"
        "   padding: 10px 30px;"
        "}");

        GtkWidget *diagnosis_label = gtk_label_new(consults[i].diagnosis);
        set_widget_css(GTK_WIDGET(diagnosis_label), "header",
        "label#header {"
        "   font-size: 18px;"
        "   border: 1px solid #b5e5dc;"
        "   padding: 10px 30px;"
        "}");

        GtkWidget *treatmentPlan_label = gtk_label_new(consults[i].treatmentPlan);
        set_widget_css(GTK_WIDGET(treatmentPlan_label), "header",
        "label#header {"
        "   font-size: 18px;"
        "   border: 1px solid #b5e5dc;"
        "   padding: 10px 30px;"
        "}");

        time_t t = consults[i].dateTime;
        struct tm *consult_tm = localtime(&t);

        char consult_date[12];
        snprintf(consult_date, sizeof(consult_date), "%d/%02d/%02d", consult_tm->tm_year+1900, consult_tm->tm_mon+1, consult_tm->tm_mday);
        GtkWidget *date_label = gtk_label_new(consult_date);
        set_widget_css(GTK_WIDGET(date_label), "header",
        "label#header {"
        "   font-size: 18px;"
        "   border: 1px solid #b5e5dc;"
        "   padding: 10px 30px;"
        "}");

        char consult_time[10];
        snprintf(consult_time, sizeof(consult_time), "%d:%02d", consult_tm->tm_hour, consult_tm->tm_min);
        GtkWidget *time_label = gtk_label_new(consult_time);
        set_widget_css(GTK_WIDGET(time_label), "header",
        "label#header {"
        "   font-size: 18px;"
        "   border: 1px solid #b5e5dc;"
        "   padding: 10px 30px;"
        "}");

        GtkWidget *edit_btn = gtk_button_new_with_label("edit");
        set_widget_css(GTK_WIDGET(edit_btn), "btn",
        "button#btn {"
        "   border: 1px solid #b5e5dc;"
        "   padding: 10px 30px;"
        "   font-size: 18px;"
        "   border-radius: 0px;"
        "   background: transparent;"
        "   box-shadow: none;"
        "}");

        GtkWidget *edit_image = gtk_image_new_from_file("imgs/edit_3.svg");
        gtk_image_set_pixel_size(GTK_IMAGE(edit_image), 25);
        set_widget_css(GTK_WIDGET(edit_image), "edit_image",
        "image#edit_image {"
        "   transition: transform 0.3s;"
        "}"
        "image#edit_image:hover {"
        "   transform: scale(1.1);"
        "   transition: transform 0.3s;"
        "}");

        gtk_button_set_child(GTK_BUTTON(edit_btn), edit_image);

        GtkWidget *delete_btn = gtk_button_new_with_label("delete");
        set_widget_css(GTK_WIDGET(delete_btn), "btn",
        "button#btn {"
        "   border: 1px solid #b5e5dc;"
        "   padding: 10px 30px;"
        "   font-size: 18px;"
        "   border-radius: 0px;"
        "   background: transparent;"
        "   box-shadow: none;"
        "}");

        GtkWidget *delete_image = gtk_image_new_from_file("imgs/delete_3.svg");
        gtk_image_set_pixel_size(GTK_IMAGE(delete_image), 25);
        set_widget_css(GTK_WIDGET(delete_image), "delete_img",
        "image#delete_img {"
        "   transition: transform 0.3s;"
        "}"
        "image#delete_img:hover {"
        "   transform: scale(1.1);"
        "   transition: transform 0.3s;"
        "}");

        gtk_button_set_child(GTK_BUTTON(delete_btn), delete_image);


        // Attach labels to the grid for each row of patient data
        gtk_grid_attach(grid, id_label, 0, i + 2, 1, 1);
        gtk_grid_attach(grid, id_pt_label, 1, i + 2, 1, 1);
        gtk_grid_attach(grid, name_label, 2, i + 2, 1, 1);
        gtk_grid_attach(grid, symptoms_label, 3, i + 2, 1, 1);
        gtk_grid_attach(grid, diagnosis_label, 4, i + 2, 1, 1);
        gtk_grid_attach(grid, treatmentPlan_label, 5, i + 2, 1, 1);
        gtk_grid_attach(grid, date_label, 6, i + 2, 1, 1);
        gtk_grid_attach(grid, time_label, 7, i + 2, 1, 1);
        gtk_grid_attach(grid, edit_btn, 8, i + 2, 1, 1);
        gtk_grid_attach(grid, delete_btn, 9, i + 2, 1, 1);

        g_signal_connect(delete_btn, "clicked", G_CALLBACK(delete_consult_popup), &consults[i].id);
        g_signal_connect(edit_btn, "clicked", G_CALLBACK(edit_consult_popup), &consults[i]);
    }
}

//===================================================================//
// Rendezvous functionalities
//===================================================================//

void g_add_rdv(GtkButton *btn, gpointer data) {
    GtkWidget **widgets = data;
    if (!widgets) {
        g_printerr("Error: widgets is NULL\n");
        return;
    }

    //Entries
    GtkEntry *entry_id_pt = GTK_ENTRY(widgets[0]);
    GtkDropDown *dropdown_year = GTK_DROP_DOWN(widgets[1]);
    GtkDropDown *dropdown_month = GTK_DROP_DOWN(widgets[2]);
    GtkDropDown *dropdown_day = GTK_DROP_DOWN(widgets[3]);
    GtkDropDown *dropdown_hour = GTK_DROP_DOWN(widgets[4]);
    GtkDropDown *dropdown_state = GTK_DROP_DOWN(widgets[5]);
    //Labels
    GtkLabel *label_id_pt = GTK_LABEL(widgets[6]);
    GtkLabel *label_date = GTK_LABEL(widgets[7]);
    GtkLabel *label_state = GTK_LABEL(widgets[8]);

    // Retrieve values from each entry
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

    int cin_valid = isCinValid(patient_id);
    int patient_exists = ifPatientExists(patient_id);
    int date_valid = isDateValid(year, month, day);
    int hour_valid = isDateTimeValid(year, month, day, hour);


    if (!cin_valid) {
        gtk_label_set_text(GTK_LABEL(label_id_pt), "CIN can only contain letters and numbers ([A-Za-z][0-9])!");
    } else {
        if (!patient_exists) {
            gtk_label_set_text(GTK_LABEL(label_id_pt), "patient does not exists");
        } else {
            gtk_label_set_text(GTK_LABEL(label_id_pt), "");
        }
    }

    if (!date_valid) {
        gtk_label_set_text(GTK_LABEL(label_date), "Date chosen is not valid");
    } else {
        if (!hour_valid) {
            gtk_label_set_text(GTK_LABEL(label_date), "Hour chosen is not valid");
        } else {
            gtk_label_set_text(GTK_LABEL(label_date), "");
        }
    }

    if (cin_valid && patient_exists && date_valid) {
        Rendezvous rendezvous;
        rendezvous.id = get_next_valid_RV_id();
        strcpy(rendezvous.id_pt, patient_id);
        rendezvous.day = day;
        rendezvous.month = month;
        rendezvous.year = year;
        rendezvous.hour = hour;
        strcpy(rendezvous.state, rdv_state);

        int weekend = is_weekend(rendezvous);
        int holiday = is_holiday(rendezvous);
        int full_hour_reserved = isHourFullyReserved(year, month, day, hour);
        int same_user_reserved = isSameUserReserved(patient_id , year,  month,  day, hour);

        if (holiday) {
            gtk_label_set_text(GTK_LABEL(label_date), "Invalid Rendezvous: it's a holiday");
        } else {
            if (weekend) {
                gtk_label_set_text(GTK_LABEL(label_date), "Invalid Rendezvous: it's weekend");
            } else {
                if (full_hour_reserved) {
                    gtk_label_set_text(GTK_LABEL(label_date), "The selected hour is fully booked. Please choose a different time slot.");
                } else {
                    if (same_user_reserved) {
                        gtk_label_set_text(GTK_LABEL(label_date), "You already reserved this hour. Please choose a different time.");
                    } else {
                        gtk_label_set_text(GTK_LABEL(label_date), "");
                    }
                }
            }
        }
        if (!weekend && !holiday && !full_hour_reserved && !same_user_reserved) {
            addRV(rendezvous);
            window_close(NULL, GTK_WINDOW(gtk_widget_get_ancestor(GTK_WIDGET(btn), GTK_TYPE_WINDOW)));
        }
    }
}

void add_rdv_popup(GtkButton *btn, gpointer data) {
    // Create the popup window
    GtkWidget *popup_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(popup_window), "Add Rendezvous");
    gtk_window_set_default_size(GTK_WINDOW(popup_window), 360, 300);

    gtk_window_set_modal(GTK_WINDOW(popup_window), TRUE);

    // Create a vertical box for layout
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_size_request(box, 360, 400);
    gtk_window_set_child(GTK_WINDOW(popup_window), box);
    gtk_widget_set_halign(GTK_WIDGET(box), GTK_ALIGN_CENTER);
    gtk_widget_set_valign(GTK_WIDGET(box), GTK_ALIGN_CENTER);
    set_widget_css(box, "edit_box", "box#edit_box{padding: 10px;}");
    gtk_window_set_child(GTK_WINDOW(popup_window), box);

    GtkWidget *id_pt_err_label = gtk_label_new("");
    set_widget_css(GTK_WIDGET(id_pt_err_label), "id_pt_err_label", "label#id_pt_err_label{color: red;}");

    GtkWidget *id_pt_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(id_pt_entry), "Patient Id (CIN)");

    GtkWidget *date_err_label = gtk_label_new("");
    set_widget_css(GTK_WIDGET(date_err_label), "date_err_label", "label#date_err_label{color: red;}");

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
    set_widget_css(GTK_WIDGET(state_err_label), "state_err_label", "label#state_err_label{color: red;}");

    GtkStringList * rdv_state_model = gtk_string_list_new((const char *[]){"Pending", "Cancelled", "Confirmed", NULL});

    GtkWidget *state_dropdown = gtk_drop_down_new(G_LIST_MODEL(rdv_state_model), NULL);

    GtkWidget **widgets =  g_new(GtkWidget*, 9);
    widgets[0] = id_pt_entry;
    widgets[1] = year_dropdown;
    widgets[2] = month_dropdown;
    widgets[3] = day_dropdown;
    widgets[4] = hour_dropdown;
    widgets[5] = state_dropdown;
    widgets[6] = id_pt_err_label;
    widgets[7] = date_err_label;
    widgets[8] = state_err_label;

    // Create the buttons
    GtkWidget *add_btn = gtk_button_new_with_label("Add");
    GtkWidget *cancel_btn = gtk_button_new_with_label("Cancel");


    // Connect the cancel button to close the window
    g_signal_connect(add_btn, "clicked", G_CALLBACK(g_add_rdv), widgets);
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(window_close), GTK_WINDOW(popup_window));
    // g_signal_connect(year_dropdown, "notify::selected", G_CALLBACK(on_year_changed), NULL);


    // Append widgets to the box
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

void g_delete_rdv(GtkButton *btn, gpointer data) {
    int *id_value = data;
    printf("RV with id: %d have been deleted\n", *id_value);
    cancelRV(*id_value);
}

void delete_rdv_popup(GtkButton *btn, gpointer data) {
    int* id = data;
    GtkWidget *popup_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(popup_window), "Confirm deletion");
    gtk_window_set_default_size(GTK_WINDOW(popup_window), 200, 100);

    gtk_window_set_modal(GTK_WINDOW(popup_window), TRUE);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(popup_window), box);
    gtk_widget_set_halign(GTK_WIDGET(box), GTK_ALIGN_CENTER);
    gtk_widget_set_valign(GTK_WIDGET(box), GTK_ALIGN_CENTER);
    set_widget_css(box, "del_box", "box#del_box{padding: 20px;}");

    char msg[100];
    sprintf(msg, "are you sure about deleting this rendezvous (ID: %d)", *id);

    GtkWidget *msg_label = gtk_label_new(msg);
    GtkWidget *del_btn = gtk_button_new_with_label("Delete");
    GtkWidget *cancel_btn = gtk_button_new_with_label("Cancel");

    gtk_box_append(GTK_BOX(box), msg_label);
    gtk_box_append(GTK_BOX(box), del_btn);
    gtk_box_append(GTK_BOX(box), cancel_btn);

    g_signal_connect(del_btn, "clicked", G_CALLBACK(g_delete_rdv), data);
    g_signal_connect(del_btn, "clicked", G_CALLBACK(window_close), GTK_WIDGET(popup_window));
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(window_close), GTK_WIDGET(popup_window));

    gtk_window_present(GTK_WINDOW(popup_window));
}

void g_edit_rdv(GtkButton *btn, gpointer data) {
    GtkWidget **widgets = data;
    if (!widgets) {
        g_printerr("Error: widgets is NULL\n");
        return;
    }

    //Entries
    GtkDropDown *year_dropdown = GTK_DROP_DOWN(widgets[0]);
    GtkDropDown *month_dropdown = GTK_DROP_DOWN(widgets[1]);
    GtkDropDown *day_dropdown = GTK_DROP_DOWN(widgets[2]);
    GtkDropDown *hour_dropdown = GTK_DROP_DOWN(widgets[3]);
    GtkDropDown *state_dropdown = GTK_DROP_DOWN(widgets[4]);

    //Labels
    GtkLabel *label_date = GTK_LABEL(widgets[5]);
    // GtkLabel *label_state = GTK_LABEL(widgets[6]);

    int rdv_id = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(btn), "rdvID"));
    // Retrieve values from each entry
    const char* rdv_year = get_dropdown_item(year_dropdown);
    int year = strtol(rdv_year, NULL, 10);
    const char* rdv_month = get_dropdown_item(month_dropdown);
    int month = find_month_index(rdv_month);
    const char* rdv_day = get_dropdown_item(day_dropdown);
    int day = strtol(rdv_day, NULL, 10);
    const char* rdv_hour = get_dropdown_item(hour_dropdown);
    int hour = atoi(rdv_hour);
    const char* rdv_state = get_dropdown_item(state_dropdown);


    int date_valid = isDateValid(year, month, day);
    int hour_valid = isDateTimeValid(year, month, day, hour);

    if (!date_valid) {
        gtk_label_set_text(GTK_LABEL(label_date), "Date chosen is not valid");
    } else {
        if (!hour_valid) {
            gtk_label_set_text(GTK_LABEL(label_date), "Hour chosen is not valid");
        } else {
            gtk_label_set_text(GTK_LABEL(label_date), "");
        }
    }

    if (date_valid && hour_valid) {
        Rendezvous *rendezvous = getRV(rdv_id);
        rendezvous->id = rdv_id;
        rendezvous->day = day;
        rendezvous->month = month;
        rendezvous->year = year;
        rendezvous->hour = hour;
        strcpy(rendezvous->state, rdv_state);

        int weekend = is_weekend(*rendezvous);
        int holiday = is_holiday(*rendezvous);
        int full_hour_reserved = isHourFullyReserved(year, month, day, hour);

        if (holiday) {
            gtk_label_set_text(GTK_LABEL(label_date), "Invalid Rendezvous: it's a holiday");
        } else {
            if (weekend) {
                gtk_label_set_text(GTK_LABEL(label_date), "Invalid Rendezvous: it's weekend");
            } else {
                if (full_hour_reserved) {
                    gtk_label_set_text(GTK_LABEL(label_date), "The selected hour is fully booked. Please choose a different time slot.");
                } else {
                        gtk_label_set_text(GTK_LABEL(label_date), "");

                }
            }
        }
        if (!weekend && !holiday && !full_hour_reserved) {
            modifyRV(*rendezvous);
            free(rendezvous);
            window_close(NULL, GTK_WINDOW(gtk_widget_get_ancestor(GTK_WIDGET(btn), GTK_TYPE_WINDOW)));
        }
    }
}

void edit_rdv_popup(GtkButton *btn, gpointer data) {
    const Rendezvous *RV = data;
    // Create the popup window
    GtkWidget *popup_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(popup_window), "Edit rendezvous");
    gtk_window_set_default_size(GTK_WINDOW(popup_window), 360, 400);

    gtk_window_set_modal(GTK_WINDOW(popup_window), TRUE);

    // Create a vertical box for layout
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_size_request(box, 360, 400);
    gtk_window_set_child(GTK_WINDOW(popup_window), box);
    gtk_widget_set_halign(GTK_WIDGET(box), GTK_ALIGN_CENTER);
    gtk_widget_set_valign(GTK_WIDGET(box), GTK_ALIGN_CENTER);
    set_widget_css(box, "edit_box", "box#edit_box{padding: 10px;}");
    gtk_window_set_child(GTK_WINDOW(popup_window), box);

    GtkWidget *date_err_label = gtk_label_new("");
    set_widget_css(GTK_WIDGET(date_err_label), "date_err_label", "label#date_err_label{color: red;}");

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

    const char** hours = malloc(11 * sizeof(char*));
    for (int i = 0; i < 10; ++i) {
        hours[i] = malloc(3 * sizeof(char));
        sprintf((char*)hours[i], "%dh", i + 8);
    }
    hours[10] = NULL;

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
    set_widget_css(GTK_WIDGET(state_err_label), "state_err_label", "label#state_err_label{color: red;}");

    GtkStringList * rdv_state_model = gtk_string_list_new((const char *[]){"Pending", "Cancelled", "Confirmed", NULL});

    GtkWidget *state_dropdown = gtk_drop_down_new(G_LIST_MODEL(rdv_state_model), NULL);

    GtkWidget **widgets =  g_new(GtkWidget*, 7);
    widgets[0] = year_dropdown;
    widgets[1] = month_dropdown;
    widgets[2] = day_dropdown;
    widgets[3] = hour_dropdown;
    widgets[4] = state_dropdown;
    widgets[5] = date_err_label;
    widgets[6] = state_err_label;


    // Create the buttons
    GtkWidget *edit_btn = gtk_button_new_with_label("Edit");
    GtkWidget *cancel_btn = gtk_button_new_with_label("Cancel");

    g_object_set_data(G_OBJECT(edit_btn), "rdvID", GINT_TO_POINTER(RV->id));

    // Connect the cancel button to close the window
    g_signal_connect(edit_btn, "clicked", G_CALLBACK(g_edit_rdv), widgets);
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(window_close), GTK_WINDOW(popup_window));

    // Append widgets to the box
    gtk_box_append(GTK_BOX(box), date_err_label);
    gtk_box_append(GTK_BOX(box), dateBox);
    gtk_box_append(GTK_BOX(box), state_err_label);
    gtk_box_append(GTK_BOX(box), state_dropdown);
    gtk_box_append(GTK_BOX(box), edit_btn);
    gtk_box_append(GTK_BOX(box), cancel_btn);

    // Show the popup window
    gtk_window_present(GTK_WINDOW(popup_window));
}

void g_update_display_rdv(GtkButton *btn, gpointer data) {

    Rendezvous *RVs = getRVs();
    int rdv_count = getNumbOfRVs();
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
        set_widget_css(GTK_WIDGET(id_label), "header",
        "label#header {"
        "   font-size: 18px;"
        "   border: 1px solid #d8d5f7;"
        "   padding: 10px 30px;"
        "}");

        GtkWidget *id_pt_label = gtk_label_new(RVs[i].id_pt);
        set_widget_css(GTK_WIDGET(id_pt_label), "header",
        "label#header {"
        "   font-size: 18px;"
        "   border: 1px solid #d8d5f7;"
        "   padding: 10px 30px;"
        "}");

        Patient* patient = getPatient(RVs[i].id_pt);
        if (patient == NULL) {
            printf("Patient with ID %s not found\n", RVs[i].id_pt);
            continue;
        }

        char fullName_pt[40];
        snprintf(fullName_pt, sizeof(fullName_pt), "%s %s", patient->lName, patient->fName);
        GtkWidget *name_label = gtk_label_new(fullName_pt);
        set_widget_css(GTK_WIDGET(name_label), "header",
        "label#header {"
        "   font-size: 18px;"
        "   border: 1px solid #d8d5f7;"
        "   padding: 10px 30px;"
        "}");

        char date[12];
        snprintf(date, sizeof(date), "%d/%d/%d", RVs[i].year, RVs[i].month, RVs[i].day);
        GtkWidget *date_label = gtk_label_new(date);
        set_widget_css(GTK_WIDGET(date_label), "header",
        "label#header {"
        "   font-size: 18px;"
        "   border: 1px solid #d8d5f7;"
        "   padding: 10px 30px;"
        "}");

        char time[4];
        snprintf(time, sizeof(time), "%dh", RVs[i].hour);
        GtkWidget *hour_label = gtk_label_new(time);
        set_widget_css(GTK_WIDGET(hour_label), "header",
        "label#header {"
        "   font-size: 18px;"
        "   border: 1px solid #d8d5f7;"
        "   padding: 10px 30px;"
        "}");

        GtkWidget *state_label = gtk_label_new(RVs[i].state);
        set_widget_css(GTK_WIDGET(state_label), "header",
        "label#header {"
        "   font-size: 18px;"
        "   border: 1px solid #d8d5f7;"
        "   padding: 10px 30px;"
        "}");

        GtkWidget *edit_btn = gtk_button_new_with_label("edit");
        set_widget_css(GTK_WIDGET(edit_btn), "btn",
        "button#btn {"
        "   border: 1px solid #d8d5f7;"
        "   padding: 10px 30px;"
        "   font-size: 18px;"
        "   border-radius: 0px;"
        "   background: transparent;"
        "   box-shadow: none;"
        "}");

        GtkWidget *edit_image = gtk_image_new_from_file("imgs/edit_2.svg");
        gtk_image_set_pixel_size(GTK_IMAGE(edit_image), 25);
        set_widget_css(GTK_WIDGET(edit_image), "edit_image",
        "image#edit_image {"
        "   transition: transform 0.3s;"
        "}"
        "image#edit_image:hover {"
        "   transform: scale(1.1);"
        "   transition: transform 0.3s;"
        "}");

        gtk_button_set_child(GTK_BUTTON(edit_btn), edit_image);

        GtkWidget *delete_btn = gtk_button_new_with_label("delete");
        set_widget_css(GTK_WIDGET(delete_btn), "btn",
        "button#btn {"
        "   border: 1px solid #d8d5f7;"
        "   padding: 10px 30px;"
        "   font-size: 18px;"
        "   border-radius: 0px;"
        "   background: transparent;"
        "   box-shadow: none;"
        "}");

        GtkWidget *delete_image = gtk_image_new_from_file("imgs/delete_2.svg");
        gtk_image_set_pixel_size(GTK_IMAGE(delete_image), 25);
        set_widget_css(GTK_WIDGET(delete_image), "delete_img",
        "image#delete_img {"
        "   transition: transform 0.3s;"
        "}"
        "image#delete_img:hover {"
        "   transform: scale(1.1);"
        "   transition: transform 0.3s;"
        "}");

        gtk_button_set_child(GTK_BUTTON(delete_btn), delete_image);


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

void g_add_payment(GtkButton *btn, gpointer data) {
    GtkWidget **widgets = data;
    if (!widgets) {
        g_printerr("Error: widgets is NULL\n");
        return;
    }

    //Entries
    GtkEntry *entry_consult_id = GTK_ENTRY(widgets[0]);
    GtkEntry *entry_amount = GTK_ENTRY(widgets[1]);
    //Dropdown
    GtkDropDown *dropdown_state = GTK_DROP_DOWN(widgets[2]);
    //Labels
    GtkLabel *label_consult_id = GTK_LABEL(widgets[3]);
    GtkLabel *label_amount = GTK_LABEL(widgets[4]);

    // Retrieve values from each entry
    int consult_id = get_int_from_entry(GTK_WIDGET(entry_consult_id));
    double amount = get_double_from_entry(GTK_WIDGET(entry_amount));
    const char* state = get_dropdown_item(dropdown_state);

    int consult_exists = ifConsultExists(consult_id);

    int amount_valid = isAmountValid(amount);

    if (!consult_exists) {
        gtk_label_set_text(GTK_LABEL(label_consult_id), "consultation does not exists");
    } else {
        gtk_label_set_text(GTK_LABEL(label_consult_id), "");
    }

    if (!amount_valid) {
        gtk_label_set_text(GTK_LABEL(label_amount), "Invalid amount specified");
    } else {
        gtk_label_set_text(GTK_LABEL(label_amount), "");
    }

    if (consult_exists && amount_valid ) {
        Payment *payment = malloc(sizeof(Payment));
        Consult *consult = getConsultation(consult_id);
        payment->id = get_next_valid_invoice_id();
        payment->id_consult = consult_id;
        strcpy(payment->id_pt, consult->id_pt);
        strcpy(payment->patient_lName, consult->patient_lName);
        strcpy(payment->patient_fName, consult->patient_fName);
        payment->amount = amount;
        strcpy(payment->state, state);
        payment->dateTime = time(NULL);
        addPayment(*payment);
        free(consult);
        free(payment);
        window_close(NULL, GTK_WINDOW(gtk_widget_get_ancestor(GTK_WIDGET(btn), GTK_TYPE_WINDOW)));
    }
}

void add_payment_popup(GtkButton *btn, gpointer data) {
    // Create the popup window
    GtkWidget *popup_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(popup_window), "Add invoice");
    gtk_window_set_default_size(GTK_WINDOW(popup_window), 360, 300);

    gtk_window_set_modal(GTK_WINDOW(popup_window), TRUE);

    // Create a vertical box for layout
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_size_request(box, 360, 400);
    gtk_window_set_child(GTK_WINDOW(popup_window), box);
    gtk_widget_set_halign(GTK_WIDGET(box), GTK_ALIGN_CENTER);
    gtk_widget_set_valign(GTK_WIDGET(box), GTK_ALIGN_CENTER);
    set_widget_css(box, "edit_box", "box#edit_box{padding: 10px;}");
    gtk_window_set_child(GTK_WINDOW(popup_window), box);


    GtkWidget *id_consult_err_label = gtk_label_new("");
    set_widget_css(GTK_WIDGET(id_consult_err_label), "err_label", "label#err_label{color: red;}");
    GtkWidget *id_consult_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(id_consult_entry), "Consult Id");

    GtkWidget *amount_err_label = gtk_label_new("");
    set_widget_css(GTK_WIDGET(amount_err_label), "err_label", "label#err_label{color: red;}");
    GtkWidget *amount_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(amount_entry), "Amount");

    GtkStringList * state_model = gtk_string_list_new((const char *[]){"Payed", "Not payed", NULL});
    GtkWidget *state_dropdown = gtk_drop_down_new(G_LIST_MODEL(state_model), NULL);

    GtkWidget **widgets =  g_new(GtkWidget*, 5);
    widgets[0] = id_consult_entry;
    widgets[1] = amount_entry;
    widgets[2] = state_dropdown;
    widgets[3] = id_consult_err_label;
    widgets[4] = amount_err_label;

    // Create the buttons
    GtkWidget *add_btn = gtk_button_new_with_label("Add");
    GtkWidget *cancel_btn = gtk_button_new_with_label("Cancel");

    // Connect the cancel button to close the window
    g_signal_connect(add_btn, "clicked", G_CALLBACK(g_add_payment), widgets);
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(window_close), GTK_WINDOW(popup_window));
    // g_signal_connect(year_dropdown, "notify::selected", G_CALLBACK(on_year_changed), NULL);

    // Append widgets to the box
    gtk_box_append(GTK_BOX(box), id_consult_err_label);
    gtk_box_append(GTK_BOX(box), id_consult_entry);
    gtk_box_append(GTK_BOX(box), state_dropdown);
    gtk_box_append(GTK_BOX(box), amount_err_label);
    gtk_box_append(GTK_BOX(box), amount_entry);
    gtk_box_append(GTK_BOX(box), add_btn);
    gtk_box_append(GTK_BOX(box), cancel_btn);

    // Show the popup window
    gtk_window_present(GTK_WINDOW(popup_window));
}

void g_delete_payment(GtkButton *btn, gpointer data) {
    int *id_value = data;
    printf("Payment with id: %d have been deleted\n", *id_value);
    deletePayment(*id_value);
}

void delete_payment_popup(GtkButton *btn, gpointer data) {
    int* id = data;
    GtkWidget *popup_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(popup_window), "Confirm deletion");
    gtk_window_set_default_size(GTK_WINDOW(popup_window), 200, 100);

    gtk_window_set_modal(GTK_WINDOW(popup_window), TRUE);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(popup_window), box);
    gtk_widget_set_halign(GTK_WIDGET(box), GTK_ALIGN_CENTER);
    gtk_widget_set_valign(GTK_WIDGET(box), GTK_ALIGN_CENTER);
    set_widget_css(box, "del_box", "box#del_box{padding: 20px;}");

    char msg[100];
    sprintf(msg, "are you sure about deleting this payment (ID: %d)", *id);

    GtkWidget *msg_label = gtk_label_new(msg);
    GtkWidget *del_btn = gtk_button_new_with_label("Delete");
    GtkWidget *cancel_btn = gtk_button_new_with_label("Cancel");

    gtk_box_append(GTK_BOX(box), msg_label);
    gtk_box_append(GTK_BOX(box), del_btn);
    gtk_box_append(GTK_BOX(box), cancel_btn);

    g_signal_connect(del_btn, "clicked", G_CALLBACK(g_delete_payment), data);
    g_signal_connect(del_btn, "clicked", G_CALLBACK(window_close), GTK_WIDGET(popup_window));
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(window_close), GTK_WIDGET(popup_window));

    gtk_window_present(GTK_WINDOW(popup_window));
}

void g_edit_payment(GtkButton *btn, gpointer data) {
    GtkWidget **widgets = data;
    if (!widgets) {
        g_printerr("Error: widgets is NULL\n");
        return;
    }

    //Entries
    GtkEntry *entry_amount = GTK_ENTRY(widgets[0]);
    // Dropdowns
    GtkDropDown *dropdown_state = GTK_DROP_DOWN(widgets[1]);
    //Labels
    GtkLabel *amount_label = GTK_LABEL(widgets[2]);

    int payment_id = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(btn), "paymentID"));
    // Retrieve values from each entry
    double amount = get_double_from_entry(GTK_WIDGET(entry_amount));
    const char* state = get_dropdown_item(dropdown_state);

    int amount_valid = isAmountValid(amount);

    if (!amount_valid) {
        gtk_label_set_text(GTK_LABEL(amount_label), "Invalid amount specified");
    } else {
        gtk_label_set_text(GTK_LABEL(amount_label), "");
    }

    if (amount_valid) {
        Payment *payment = getPayment(payment_id);
        Consult *consult = getConsultation(payment->id_consult);
        payment->amount = amount;
        strcpy(payment->state, state);
        modifyPayment(*payment);
        free(payment);
        free(consult);
        window_close(NULL, GTK_WINDOW(gtk_widget_get_ancestor(GTK_WIDGET(btn), GTK_TYPE_WINDOW)));
    }
}

void edit_payment_popup(GtkButton *btn, gpointer data) {
    const Payment *payment = data;
    // Create the popup window
    GtkWidget *popup_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(popup_window), "Edit Invoice");
    gtk_window_set_default_size(GTK_WINDOW(popup_window), 360, 400);

    gtk_window_set_modal(GTK_WINDOW(popup_window), TRUE);

    // Create a vertical box for layout
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_size_request(box, 360, 400);
    gtk_window_set_child(GTK_WINDOW(popup_window), box);
    gtk_widget_set_halign(GTK_WIDGET(box), GTK_ALIGN_CENTER);
    gtk_widget_set_valign(GTK_WIDGET(box), GTK_ALIGN_CENTER);
    set_widget_css(box, "edit_box", "box#edit_box{padding: 10px;}");
    gtk_window_set_child(GTK_WINDOW(popup_window), box);

    GtkWidget *amount_err_label = gtk_label_new("");
    set_widget_css(GTK_WIDGET(amount_err_label), "amount_err_label", "label#amount_err_label{color: red;}");

    GtkWidget *amount_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(amount_entry), "Amount");
    char amount_text[10];
    sprintf(amount_text, "%.2lf", payment->amount);
    set_text_entry(amount_entry, amount_text);

    GtkWidget *state_err_label = gtk_label_new("");
    set_widget_css(GTK_WIDGET(state_err_label), "state_err_label", "label#state_err_label{color: red;}");

    GtkStringList * state_model = gtk_string_list_new((const char *[]){"Payed", "Not payed", NULL});

    int state_pos = find_payment_state_index(payment->state);
    GtkWidget *state_dropdown = gtk_drop_down_new(G_LIST_MODEL(state_model), NULL);
    gtk_drop_down_set_selected(GTK_DROP_DOWN(state_dropdown),state_pos);

    GtkWidget **widgets =  g_new(GtkWidget*, 3);
    widgets[0] = amount_entry;
    widgets[1] = state_dropdown;
    widgets[2] = amount_err_label;

    // Create the buttons
    GtkWidget *edit_btn = gtk_button_new_with_label("Edit");
    GtkWidget *cancel_btn = gtk_button_new_with_label("Cancel");

    g_object_set_data(G_OBJECT(edit_btn), "paymentID", GINT_TO_POINTER(payment->id));

    // Connect the cancel button to close the window
    g_signal_connect(edit_btn, "clicked", G_CALLBACK(g_edit_payment), widgets);
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(window_close), GTK_WINDOW(popup_window));

    // Append widgets to the box
    gtk_box_append(GTK_BOX(box), amount_err_label);
    gtk_box_append(GTK_BOX(box), amount_entry);
    gtk_box_append(GTK_BOX(box), state_dropdown);
    gtk_box_append(GTK_BOX(box), edit_btn);
    gtk_box_append(GTK_BOX(box), cancel_btn);

    // Show the popup window
    gtk_window_present(GTK_WINDOW(popup_window));
}

void g_update_display_payment(GtkButton *btn, gpointer data) {

    Payment *payments = getPayments();
    int payment_count = getNumbOfPayments();
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
        set_widget_css(GTK_WIDGET(id_payment_label), "header",
        "label#header {"
        "   font-size: 18px;"
        "   border: 1px solid #e8d1c1;"
        "   padding: 10px 30px;"
        "}");

        char id_consult_text[10];
        snprintf(id_consult_text, sizeof(id_consult_text), "%d", payments[i].id_consult);
        GtkWidget *id_consult_label = gtk_label_new(id_consult_text);
        set_widget_css(GTK_WIDGET(id_consult_label), "header",
        "label#header {"
        "   font-size: 18px;"
        "   border: 1px solid #e8d1c1;"
        "   padding: 10px 30px;"
        "}");

        GtkWidget *id_pt_label = gtk_label_new(payments[i].id_pt);
        set_widget_css(GTK_WIDGET(id_pt_label), "header",
        "label#header {"
        "   font-size: 18px;"
        "   border: 1px solid #e8d1c1;"
        "   padding: 10px 30px;"
        "}");

        char fullName_pt[40];
        snprintf(fullName_pt, sizeof(fullName_pt), "%s %s", payments[i].patient_lName, payments[i].patient_fName);
        GtkWidget *name_label = gtk_label_new(fullName_pt);
        set_widget_css(GTK_WIDGET(name_label), "header",
        "label#header {"
        "   font-size: 18px;"
        "   border: 1px solid #e8d1c1;"
        "   padding: 10px 30px;"
        "}");

        char amount[10];
        snprintf(amount, sizeof(amount), "%.2lf", payments[i].amount);
        GtkWidget *amount_label = gtk_label_new(amount);
        set_widget_css(GTK_WIDGET(amount_label), "header",
        "label#header {"
        "   font-size: 18px;"
        "   border: 1px solid #e8d1c1;"
        "   padding: 10px 30px;"
        "}");

        char date[12];
        struct tm tm = *localtime(&payments[i].dateTime);
        snprintf(date, sizeof(date), "%d/%02d/%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
        GtkWidget *date_label = gtk_label_new(date);
        set_widget_css(GTK_WIDGET(date_label), "header",
        "label#header {"
        "   font-size: 18px;"
        "   border: 1px solid #e8d1c1;"
        "   padding: 10px 30px;"
        "}");

        char time[5];
        snprintf(time, sizeof(time), "%dh", tm.tm_hour);
        GtkWidget *hour_label = gtk_label_new(time);
        set_widget_css(GTK_WIDGET(hour_label), "header",
        "label#header {"
        "   font-size: 18px;"
        "   border: 1px solid #e8d1c1;"
        "   padding: 10px 30px;"
        "}");

        GtkWidget *state_label = gtk_label_new(payments[i].state);
        set_widget_css(GTK_WIDGET(state_label), "header",
        "label#header {"
        "   font-size: 18px;"
        "   border: 1px solid #e8d1c1;"
        "   padding: 10px 30px;"
        "}");

        GtkWidget *edit_btn = gtk_button_new_with_label("edit");
        set_widget_css(GTK_WIDGET(edit_btn), "btn",
        "button#btn {"
        "   border: 1px solid #e8d1c1;"
        "   padding: 10px 30px;"
        "   font-size: 18px;"
        "   border-radius: 0px;"
        "   background: transparent;"
        "   box-shadow: none;"
        "}");

        GtkWidget *edit_image = gtk_image_new_from_file("imgs/edit_4.svg");
        gtk_image_set_pixel_size(GTK_IMAGE(edit_image), 25);
        set_widget_css(GTK_WIDGET(edit_image), "edit_image",
        "image#edit_image {"
        "   transition: transform 0.3s;"
        "}"
        "image#edit_image:hover {"
        "   transform: scale(1.1);"
        "   transition: transform 0.3s;"
        "}");

        gtk_button_set_child(GTK_BUTTON(edit_btn), edit_image);

        GtkWidget *delete_btn = gtk_button_new_with_label("delete");
        set_widget_css(GTK_WIDGET(delete_btn), "btn",
        "button#btn {"
        "   border: 1px solid #e8d1c1;"
        "   padding: 10px 30px;"
        "   font-size: 18px;"
        "   border-radius: 0px;"
        "   background: transparent;"
        "   box-shadow: none;"
        "}");

        GtkWidget *delete_image = gtk_image_new_from_file("imgs/delete_4.svg");
        gtk_image_set_pixel_size(GTK_IMAGE(delete_image), 25);
        set_widget_css(GTK_WIDGET(delete_image), "delete_img",
        "image#delete_img {"
        "   transition: transform 0.3s;"
        "}"
        "image#delete_img:hover {"
        "   transform: scale(1.1);"
        "   transition: transform 0.3s;"
        "}");

        gtk_button_set_child(GTK_BUTTON(delete_btn), delete_image);

        if (id_payment_label == NULL) {
            printf("id_payment_label is NULL\n");
        }
        if (id_consult_label == NULL) {
            printf("id_consult_label is NULL\n");
        }
        if (id_pt_label == NULL) {
            printf("id_pt_label is NULL\n");
        }
        if (name_label == NULL) {
            printf("name_label is NULL\n");
        }
        if (amount_label == NULL) {
            printf("amount_label is NULL\n");
        }
        if (date_label == NULL) {
            printf("date_label is NULL\n");
        }
        if (hour_label == NULL) {
            printf("hour_label is NULL\n");
        }
        if (state_label == NULL) {
            printf("state_label is NULL\n");
        }
        if (edit_btn == NULL) {
            printf("edit_btn is NULL\n");
        }
        if (delete_btn == NULL) {
            printf("delete_btn is NULL\n");
        }

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
//===================================================================//
// User Rendezvous functionalities
//===================================================================//
//===================================================================//

void g_request_rdv(GtkButton *btn, gpointer data) {

    char* cin = g_object_get_data(G_OBJECT(btn), "UserID");

    GtkWidget **widgets = data;
    if (!widgets) {
        g_printerr("Error: widgets is NULL\n");
        return;
    }
    //Entries
    GtkDropDown *dropdown_year = GTK_DROP_DOWN(widgets[0]);
    GtkDropDown *dropdown_month = GTK_DROP_DOWN(widgets[1]);
    GtkDropDown *dropdown_day = GTK_DROP_DOWN(widgets[2]);
    GtkDropDown *dropdown_hour = GTK_DROP_DOWN(widgets[3]);
    //Labels
    GtkLabel *label_state = GTK_LABEL(widgets[4]);
    GtkLabel *label_date = GTK_LABEL(widgets[5]);

    // Retrieve values from each entry
    const char* rdv_year = get_dropdown_item(dropdown_year);
    int year = strtol(rdv_year, NULL, 10);
    const char* rdv_month = get_dropdown_item(dropdown_month);
    int month = find_month_index(rdv_month);
    const char* rdv_day = get_dropdown_item(dropdown_day);
    int day = strtol(rdv_day, NULL, 10);
    const char* rdv_hour = get_dropdown_item(dropdown_hour);
    int hour = atoi(rdv_hour);
    const char* rdv_state = gtk_label_get_text(label_state);

    int date_valid = isDateValid(year, month, day);
    int hour_valid = isDateTimeValid(year, month, day, hour);
    printf("hour = %d", hour);

    if (!date_valid) {
        gtk_label_set_text(GTK_LABEL(label_date), "Date chosen is not valid");
    } else {
        if (!hour_valid) {
            gtk_label_set_text(GTK_LABEL(label_date), "Hour chosen is not valid");
        } else {
            gtk_label_set_text(GTK_LABEL(label_date), "");
        }
    }

    if (date_valid && hour_valid) {
        Rendezvous rendezvous;
        rendezvous.id = get_next_valid_RV_id();
        strcpy(rendezvous.id_pt, cin);
        rendezvous.day = day;
        rendezvous.month = month;
        rendezvous.year = year;
        rendezvous.hour = hour;
        strcpy(rendezvous.state, rdv_state);

        int weekend = is_weekend(rendezvous);
        int holiday = is_holiday(rendezvous);
        int full_hour_reserved = isHourFullyReserved(year, month, day, hour);
        int same_user_reserved = isSameUserReserved(cin , year,  month,  day, hour);

        if (holiday) {
            gtk_label_set_text(GTK_LABEL(label_date), "Invalid Rendezvous: it's a holiday");
        } else {
            if (weekend) {
                gtk_label_set_text(GTK_LABEL(label_date), "Invalid Rendezvous: it's weekend");
            } else {
                if (full_hour_reserved) {
                    gtk_label_set_text(GTK_LABEL(label_date), "The selected hour is fully booked. Please choose a different time slot.");
                } else {
                    if (same_user_reserved) {
                        gtk_label_set_text(GTK_LABEL(label_date), "You already reserved this hour. Please choose a different time.");
                    } else {
                        gtk_label_set_text(GTK_LABEL(label_date), "");
                    }
                }
            }
        }
        if (!weekend && !holiday && !full_hour_reserved && !same_user_reserved) {
            addRV(rendezvous);
            window_close(NULL, GTK_WINDOW(gtk_widget_get_ancestor(GTK_WIDGET(btn), GTK_TYPE_WINDOW)));
        }
    }
}

void request_rdv_popup(GtkButton *btn, gpointer data) {
    // Create the popup window
    GtkWidget *popup_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(popup_window), "Request a Rendezvous");
    gtk_window_set_default_size(GTK_WINDOW(popup_window), 300, 600);

    gtk_window_set_modal(GTK_WINDOW(popup_window), TRUE);

    // Create a vertical box for layout
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(popup_window), box);

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

    GtkWidget *state_label = gtk_label_new("Pending");

    GtkWidget **widgets =  g_new(GtkWidget*, 6);
    widgets[0] = year_dropdown;
    widgets[1] = month_dropdown;
    widgets[2] = day_dropdown;
    widgets[3] = hour_dropdown;
    widgets[4] = state_label;
    widgets[5] = date_err_label;

    // Create the buttons
    GtkWidget *add_btn = gtk_button_new_with_label("Request");
    GtkWidget *cancel_btn = gtk_button_new_with_label("Cancel");

    char *cin = data;
    g_object_set_data(G_OBJECT(add_btn), "UserID", cin);

    // Connect the cancel button to close the window
    g_signal_connect(add_btn, "clicked", G_CALLBACK(g_request_rdv), widgets);
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(window_close), GTK_WINDOW(popup_window));

    gtk_box_append(GTK_BOX(box), date_err_label);
    gtk_box_append(GTK_BOX(box), dateBox);
    gtk_box_append(GTK_BOX(box), state_err_label);
    gtk_box_append(GTK_BOX(box), state_label);
    gtk_box_append(GTK_BOX(box), add_btn);
    gtk_box_append(GTK_BOX(box), cancel_btn);

    gtk_window_present(GTK_WINDOW(popup_window));
}

void g_cancel_rdv(GtkButton *btn, gpointer data) {
    int *id = data;
    Rendezvous *rdv = getRV(*id);
    if (rdv == NULL) {
        printf("Rendezvous does not exists\n");
        return;
    }
    strcpy(rdv->state, "Canceled");
    modifyRV(*rdv);
    free(rdv);
}

void cancel_rdv_popup(GtkButton *btn, gpointer data) {
    int* id = data;
    GtkWidget *popup_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(popup_window), "confirmer la suppression ");
    gtk_window_set_default_size(GTK_WINDOW(popup_window), 400, 150);

    gtk_window_set_modal(GTK_WINDOW(popup_window), TRUE);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(popup_window), box);

    char msg[150];
    sprintf(msg, "Are you sure you want to cancel your appointment?\nThe appointment will not be canceled until it is confirmed by an admin.");

    GtkWidget *msg_label = gtk_label_new(msg);
    GtkWidget *del_btn = gtk_button_new_with_label("cancel appointment");
    GtkWidget *cancel_btn = gtk_button_new_with_label("back");

    gtk_box_append(GTK_BOX(box), msg_label);
    gtk_box_append(GTK_BOX(box), del_btn);
    gtk_box_append(GTK_BOX(box), cancel_btn);

    g_signal_connect(del_btn, "clicked", G_CALLBACK(g_cancel_rdv), data);
    g_signal_connect(del_btn, "clicked", G_CALLBACK(window_close), GTK_WIDGET(popup_window));
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(window_close), GTK_WIDGET(popup_window));

    gtk_window_present(GTK_WINDOW(popup_window));
}

void g_reschedule_rdv(GtkButton *btn, gpointer data) {

    char* cin = g_object_get_data(G_OBJECT(btn), "UserID");

    GtkWidget **widgets = data;
    if (!widgets) {
        g_printerr("Error: widgets is NULL\n");
        return;
    }
    //Entries

    GtkDropDown *dropdown_year = GTK_DROP_DOWN(widgets[0]);
    GtkDropDown *dropdown_month = GTK_DROP_DOWN(widgets[1]);
    GtkDropDown *dropdown_day = GTK_DROP_DOWN(widgets[2]);
    GtkDropDown *dropdown_hour = GTK_DROP_DOWN(widgets[3]);
    //Labels
    // GtkLabel *label_state = GTK_LABEL(widgets[4]);
    GtkLabel *label_date = GTK_LABEL(widgets[5]);

    int rdv_id = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(btn), "rdvID"));
    // Retrieve values from each entry
    const char* rdv_year = get_dropdown_item(dropdown_year);
    int year = strtol(rdv_year, NULL, 10);
    const char* rdv_month = get_dropdown_item(dropdown_month);
    int month = find_month_index(rdv_month);
    const char* rdv_day = get_dropdown_item(dropdown_day);
    int day = strtol(rdv_day, NULL, 10);
    const char* rdv_hour = get_dropdown_item(dropdown_hour);
    int hour = atoi(rdv_hour);

    int date_valid = isDateValid(year, month, day);
    int hour_valid = isDateTimeValid(year, month, day, hour);

    if (!date_valid) {
        gtk_label_set_text(GTK_LABEL(label_date), "Date chosen is not valid");
    } else {
        if (!hour_valid) {
            gtk_label_set_text(GTK_LABEL(label_date), "Hour chosen is not valid");
        } else {
            gtk_label_set_text(GTK_LABEL(label_date), "");
        }
    }

    if (date_valid && hour_valid) {
        Rendezvous *rendezvous = getRV(rdv_id);
        rendezvous->day = day;
        rendezvous->month = month;
        rendezvous->year = year;
        rendezvous->hour = hour;
        strcpy(rendezvous->state, "Pending");

        int weekend = is_weekend(*rendezvous);
        int holiday = is_holiday(*rendezvous);
        int full_hour_reserved = isHourFullyReserved(year, month, day, hour);
        int same_user_reserved = isSameUserReserved(rendezvous->id_pt , year,  month,  day, hour);

        if (holiday) {
            gtk_label_set_text(GTK_LABEL(label_date), "Invalid Rendezvous: it's a holiday");
        } else {
            if (weekend) {
                gtk_label_set_text(GTK_LABEL(label_date), "Invalid Rendezvous: it's weekend");
            } else {
                if (full_hour_reserved) {
                    gtk_label_set_text(GTK_LABEL(label_date), "The selected hour is fully booked. Please choose a different time slot.");
                } else {
                    if (same_user_reserved) {
                        gtk_label_set_text(GTK_LABEL(label_date), "You already reserved this hour. Please choose a different time.");
                    } else {
                        gtk_label_set_text(GTK_LABEL(label_date), "");
                    }
                }
            }
        }
        if (!weekend && !holiday && !full_hour_reserved && !same_user_reserved) {
            modifyRV(*rendezvous);
            free(rendezvous);
            window_close(NULL, GTK_WINDOW(gtk_widget_get_ancestor(GTK_WIDGET(btn), GTK_TYPE_WINDOW)));
        }
    }
}

void reschedule_rdv_popup(GtkButton *btn, gpointer data) {

    const Rendezvous *RV = data;
    // Create the popup window
    GtkWidget *popup_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(popup_window), "reschedule rendezvous");
    gtk_window_set_default_size(GTK_WINDOW(popup_window), 300, 600);

    gtk_window_set_modal(GTK_WINDOW(popup_window), TRUE);

    // Create a vertical box for layout
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(popup_window), box);

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

    char state_text[30];
    sprintf(state_text, "%s --> Pending", RV->state);
    GtkWidget *state_label = gtk_label_new(state_text);


    GtkWidget **widgets =  g_new(GtkWidget*, 6);
    widgets[0] = year_dropdown;
    widgets[1] = month_dropdown;
    widgets[2] = day_dropdown;
    widgets[3] = hour_dropdown;
    widgets[4] = state_label;
    widgets[5] = date_err_label;


    // Create the buttons
    GtkWidget *reschedule_btn = gtk_button_new_with_label("Reschedule");
    GtkWidget *cancel_btn = gtk_button_new_with_label("Cancel");

    g_object_set_data(G_OBJECT(reschedule_btn), "rdvID", GINT_TO_POINTER(RV->id));

    // Connect the cancel button to close the window
    g_signal_connect(reschedule_btn, "clicked", G_CALLBACK(g_reschedule_rdv), widgets);
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(window_close), GTK_WINDOW(popup_window));

    // Append widgets to the box
    gtk_box_append(GTK_BOX(box), date_err_label);
    gtk_box_append(GTK_BOX(box), dateBox);
    gtk_box_append(GTK_BOX(box), state_err_label);
    gtk_box_append(GTK_BOX(box), state_label);
    gtk_box_append(GTK_BOX(box), reschedule_btn);
    gtk_box_append(GTK_BOX(box), cancel_btn);

    // Show the popup window
    gtk_window_present(GTK_WINDOW(popup_window));
}

void g_update_user_rdv_display(GtkButton *btn, gpointer data) {

    GtkGrid *grid = GTK_GRID(data);

    GObject **wins = get_windows_instance();
    char *cin = g_object_get_data(G_OBJECT(wins[USER_MENU]), "UserID");

    if (cin == NULL) {
        printf("Error getting user ID\n");
    }

    Rendezvous *RVs = getRVsbyCin(cin);
    int rdv_count = getNumbOfRVsbyCin(cin);

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

        char dateTime[20];
        snprintf(dateTime, sizeof(dateTime), "%d/%d/%d at %dh", RVs[i].year, RVs[i].month, RVs[i].day, RVs[i].hour);
        GtkWidget *dateTime_label = gtk_label_new(dateTime);

        set_widget_css(GTK_WIDGET(dateTime_label), "datetime-label",
        "label#datetime-label {"
        "   font-size: 36px;"
        "}");

        GtkWidget *state_label = gtk_label_new(RVs[i].state);

        GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
        set_widget_css(GTK_WIDGET(box), "box",
        "box#box {"
        "   min-width: 620px;"
        "   margin-top: 10px;"
        "   padding-left: 10px;"
        "   padding-bottom: 5px;"
        "   border: 1px dashed #d8d5f7;"
        "   border-right: none;"
        "}");

        gtk_box_append(GTK_BOX(box), dateTime_label);
        gtk_box_append(GTK_BOX(box), state_label);

        gtk_widget_set_halign(dateTime_label, GTK_ALIGN_START);
        gtk_widget_set_halign(state_label, GTK_ALIGN_START);

        GtkWidget *edit_btn = gtk_button_new_with_label("Edit");
        set_widget_css(GTK_WIDGET(edit_btn), "btn",
        "button#btn {"
        "   border: 1px dashed #d8d5f7;"
        "   border-right: none;"
        "   border-left: none;"
        "   padding: 10px 30px;"
        "   font-size: 18px;"
        "   border-radius: 0px;"
        "   background: transparent;"
        "   box-shadow: none;"
        "   margin-top: 10px;"
        "}");

        GtkWidget *edit_image = gtk_image_new_from_file("imgs/edit_2.svg");
        gtk_image_set_pixel_size(GTK_IMAGE(edit_image), 25);
        set_widget_css(GTK_WIDGET(edit_image), "edit_image",
        "image#edit_image {"
        "   transition: transform 0.3s;"
        "}"
        "image#edit_image:hover {"
        "   transform: scale(1.1);"
        "   transition: transform 0.3s;"
        "}");

        gtk_button_set_child(GTK_BUTTON(edit_btn), edit_image);

        GtkWidget *cancel_btn = gtk_button_new_with_label("Cancel");
        set_widget_css(GTK_WIDGET(cancel_btn), "btn",
        "button#btn {"
        "   border: 1px dashed #d8d5f7;"
        "   border-left: none;"
        "   padding: 10px 30px;"
        "   font-size: 18px;"
        "   border-radius: 0px;"
        "   background: transparent;"
        "   box-shadow: none;"
        "   margin-top: 10px;"
        "}");

        GtkWidget *cancel_image = gtk_image_new_from_file("imgs/delete_2.svg");
        gtk_image_set_pixel_size(GTK_IMAGE(cancel_image), 25);
        set_widget_css(GTK_WIDGET(cancel_image), "delete_img",
        "image#delete_img {"
        "   transition: transform 0.3s;"
        "}"
        "image#delete_img:hover {"
        "   transform: scale(1.1);"
        "   transition: transform 0.3s;"
        "}");

        gtk_button_set_child(GTK_BUTTON(cancel_btn), cancel_image);

        // Attach labels to the grid for each row of patient data
        gtk_grid_attach(grid, box, 0, i + 2, 6, 1);
        gtk_grid_attach(grid, edit_btn, 6, i + 2, 1, 1);
        gtk_grid_attach(grid, cancel_btn, 7, i + 2, 1, 1);

        g_signal_connect(edit_btn, "clicked", G_CALLBACK(reschedule_rdv_popup), &RVs[i]);
        g_signal_connect(cancel_btn, "clicked", G_CALLBACK(cancel_rdv_popup), &RVs[i].id);
    }
}

//===================================================================//
// User Consultation functionalities
//===================================================================//

void show_consult_info_popup(GtkButton *btn, gpointer data) {

    Consult *consult = data;

    GtkWidget *popup_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(popup_window), "consultation info");
    gtk_window_set_default_size(GTK_WINDOW(popup_window), 400, 150);

    gtk_window_set_modal(GTK_WINDOW(popup_window), TRUE);

    GtkWidget *grid = gtk_grid_new();

    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

    GtkWidget *symptoms_header_label = gtk_label_new("Symptoms");
    char symptoms_text[1024];
    sprintf(symptoms_text, "%s", consult->symptoms);
    GtkWidget *symptoms_label = gtk_label_new(symptoms_text);

    GtkWidget *diagnosis_header_label = gtk_label_new("Diagnosis");
    char diagnosis_text[1024];
    sprintf(diagnosis_text, "%s", consult->diagnosis);
    GtkWidget *diagnosis_label = gtk_label_new(diagnosis_text);

    GtkWidget *treatment_plan_header_label = gtk_label_new("Treatment plan");
    char treatment_plan_text[1024];
    sprintf(treatment_plan_text, "%s", consult->treatmentPlan);
    GtkWidget *treatment_plan_label = gtk_label_new(treatment_plan_text);

    GtkWidget *return_btn = gtk_button_new_with_label("Return");

    gtk_grid_attach(GTK_GRID(grid), symptoms_header_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), symptoms_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), diagnosis_header_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), diagnosis_label, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), treatment_plan_header_label, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), treatment_plan_label, 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), return_btn, 0, 6, 1, 1);

    g_signal_connect(return_btn, "clicked", G_CALLBACK(window_close), popup_window);

    gtk_window_set_child(GTK_WINDOW(popup_window), grid);

    gtk_window_present(GTK_WINDOW(popup_window));
}

void g_update_user_consult_display(GtkButton *btn, gpointer data) {

    GtkGrid *grid = GTK_GRID(data);

    GObject **wins = get_windows_instance();
    char *cin = g_object_get_data(G_OBJECT(wins[USER_MENU]), "UserID");

    Consult *consults = getConsultsByCin(cin);
    int consult_count = getNumbOfConsultsByCin(cin);

    int col = 0, row = 2;
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
        char dateTime_text[50];
        time_t dateTime = consults[i].dateTime;
        struct tm consult_dtm = *localtime(&dateTime);
        snprintf(dateTime_text, sizeof(dateTime_text),
            "Consultation on %d/%d/%d at %02d:%02d",
            consult_dtm.tm_year + 1900, consult_dtm.tm_mon + 1, consult_dtm.tm_mday,
            consult_dtm.tm_hour, consult_dtm.tm_min);

        GtkWidget *dateTime_label = gtk_label_new(dateTime_text);
        gtk_widget_set_halign(dateTime_label, GTK_ALIGN_START);

        // CSS for style
        set_widget_css(GTK_WIDGET(dateTime_label), "consult-label",
            "label#consult-label {"
            // "   background: #eee;"
            "   min-width: 620px;"
            "   margin-top: 10px;"
            "   padding-left: 10px;"
            "   font-size: 24px;"
            "   border: 1px dashed #b5e5dc;"
            "   border-right: none;"
            "}");

        GtkWidget *info_btn = gtk_button_new_with_label("More info");
        set_widget_css(GTK_WIDGET(info_btn), "btn",
        "button#btn {"
        "   border: 1px dashed #b5e5dc;"
        "   border-left: none;"
        "   padding: 15px 30px;"
        "   font-size: 18px;"
        "   border-radius: 0px;"
        "   background: transparent;"
        "   box-shadow: none;"
        "   margin-top: 10px;"
        "}");

        GtkWidget *info_image = gtk_image_new_from_file("imgs/delete_2.svg");
        gtk_image_set_pixel_size(GTK_IMAGE(info_image), 25);
        set_widget_css(GTK_WIDGET(info_image), "delete_img",
        "image#delete_img {"
        "   transition: transform 0.3s;"
        "}"
        "image#delete_img:hover {"
        "   transform: scale(1.1);"
        "   transition: transform 0.3s;"
        "}");

        gtk_button_set_child(GTK_BUTTON(info_btn), info_image);

        // Attach widgets to the grid
        gtk_grid_attach(grid, dateTime_label, 0, i + 2, 5, 1);
        gtk_grid_attach(grid, info_btn, 6, i + 2, 2, 1);

        // Connect signals to the button
        g_signal_connect(info_btn, "clicked", G_CALLBACK(show_consult_info_popup), &consults[i]);
    }
}


//===================================================================//
// User payment functionalities
//===================================================================//

void show_bill_info_popup(GtkButton *btn, gpointer data) {
    Payment *payment = data;

    GtkWidget *popup_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(popup_window), "payment info");
    gtk_window_set_default_size(GTK_WINDOW(popup_window), 400, 150);

    gtk_window_set_modal(GTK_WINDOW(popup_window), TRUE);

    GtkWidget *grid = gtk_grid_new();

    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

    GtkWidget *date_header_label = gtk_label_new("Date");

    char date_text[30];
    time_t dateTime = time(&payment->dateTime);
    struct tm payment_dtm = *localtime(&dateTime);
    sprintf(date_text, "%d-%d-%d", payment_dtm.tm_year + 1900, payment_dtm.tm_mon + 1, payment_dtm.tm_mday);
    GtkWidget *date_label = gtk_label_new(date_text);

    GtkWidget *amount_header_label = gtk_label_new("Amount");

    char amount_text[8];
    sprintf(amount_text, "%f", payment->amount);
    GtkWidget *amount_label = gtk_label_new(amount_text);


    GtkWidget *return_btn = gtk_button_new_with_label("Return");

    gtk_grid_attach(GTK_GRID(grid), date_header_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), date_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), amount_header_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), amount_label, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), return_btn, 0, 4, 1, 1);

    g_signal_connect(return_btn, "clicked", G_CALLBACK(window_close), popup_window);

    gtk_window_set_child(GTK_WINDOW(popup_window), grid);

    gtk_window_present(GTK_WINDOW(popup_window));
}

void g_update_user_bill_display(GtkButton *btn, gpointer data) {

    GtkGrid *grid = GTK_GRID(data);
    GObject **wins = get_windows_instance();
    char *cin = g_object_get_data(G_OBJECT(wins[USER_MENU]), "UserID");

    Payment *payment = getPaymentsByCin(cin);
    int bills_count = getNumbOfBillsByCin(cin);

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

    for (int i = 0; i < bills_count; i++) {
        char dateTime_text[50];

        time_t dateTime = payment[i].dateTime;
        struct tm payment_dtm = *localtime(&dateTime);
        snprintf(dateTime_text, sizeof(dateTime_text), "Consultation bill (%d/%d/%d)", payment_dtm.tm_year + 1900, payment_dtm.tm_mon + 1, payment_dtm.tm_mday);

        GtkWidget *dateTime_label = gtk_label_new(dateTime_text);
        gtk_widget_set_halign(dateTime_label, GTK_ALIGN_START);


        // CSS for style
        set_widget_css(GTK_WIDGET(dateTime_label), "consult-label",
            "label#consult-label {"
            // "   background: #eee;"
            "   min-width: 620px;"
            "   margin-top: 10px;"
            "   padding-left: 10px;"
            "   font-size: 24px;"
            "   border: 1px dashed #e8d1c1;"
            "   border-right: none;"
            "}");

        GtkWidget *info_btn = gtk_button_new_with_label("more info");
        set_widget_css(GTK_WIDGET(info_btn), "btn",
        "button#btn {"
        "   border: 1px dashed #e8d1c1;"
        "   border-left: none;"
        "   padding: 15px 30px;"
        "   font-size: 18px;"
        "   border-radius: 0px;"
        "   background: transparent;"
        "   box-shadow: none;"
        "   margin-top: 10px;"
        "}");

        GtkWidget *info_image = gtk_image_new_from_file("imgs/delete_4.svg");
        gtk_image_set_pixel_size(GTK_IMAGE(info_image), 25);
        set_widget_css(GTK_WIDGET(info_image), "delete_img",
        "image#delete_img {"
        "   transition: transform 0.3s;"
        "}"
        "image#delete_img:hover {"
        "   transform: scale(1.1);"
        "   transition: transform 0.3s;"
        "}");

        gtk_button_set_child(GTK_BUTTON(info_btn), info_image);

        // Attach labels to the grid for each row of patient data
        gtk_grid_attach(grid, dateTime_label, 0, i + 2, 4, 1);
        gtk_grid_attach(grid, info_btn, 6, i + 2, 1, 1);

        g_signal_connect(info_btn, "clicked", G_CALLBACK(show_bill_info_popup), &payment[i]);
    }
}

//===================================================================//
// window creation & filling
//===================================================================//

void g_register(GtkButton *btn, gpointer data) {

    GtkBuilder *builder = get_builder_instance();

    GtkWidget **widgets = data;
    if (!widgets) {
        g_printerr("Error: objects is NULL\n");
        return;
    }

    GtkWidget *cin_entry = widgets[0];
    GtkWidget *first_name_entry = widgets[1];
    GtkWidget *last_name_entry = widgets[2];
    GtkWidget *age_entry = widgets[3];
    GtkWidget *address_entry = widgets[4];
    GtkWidget *phone_entry = widgets[5];
    GtkWidget *email_entry = widgets[6];
    GtkWidget *password_entry = widgets[7];
    GtkWidget *confirm_password_entry = widgets[8];

    const char *cin = get_text_from_entry(GTK_WIDGET(cin_entry));
    const char *first_name = get_text_from_entry(GTK_WIDGET(first_name_entry));
    const char *last_name = get_text_from_entry(GTK_WIDGET(last_name_entry));
    const int age = get_int_from_entry(GTK_WIDGET(age_entry));
    const char *address = get_text_from_entry(GTK_WIDGET(address_entry));
    const char *phone = get_text_from_entry(GTK_WIDGET(phone_entry));
    const char *email = get_text_from_entry(GTK_WIDGET(email_entry));
    const char *password = get_text_from_entry(GTK_WIDGET(password_entry));
    const char *confirm_password = get_text_from_entry(GTK_WIDGET(confirm_password_entry));

    GObject *register_cin_label = gtk_builder_get_object(builder, "register_cin_label");
    set_widget_css(GTK_WIDGET(register_cin_label), "register_label", "label#register_label{color:red;}");

    GObject *register_first_name_label = gtk_builder_get_object(builder, "register_first_name_label");
    set_widget_css(GTK_WIDGET(register_first_name_label), "register_label", "label#register_label{color:red;}");

    GObject *register_last_name_label = gtk_builder_get_object(builder, "register_last_name_label");
    set_widget_css(GTK_WIDGET(register_last_name_label), "register_label", "label#register_label{color:red;}");

    GObject *register_age_label = gtk_builder_get_object(builder, "register_age_label");
    set_widget_css(GTK_WIDGET(register_age_label), "register_label", "label#register_label{color:red;}");

    GObject *register_address_label = gtk_builder_get_object(builder, "register_address_label");
    set_widget_css(GTK_WIDGET(register_address_label), "register_label", "label#register_label{color:red;}");

    GObject *register_phone_label = gtk_builder_get_object(builder, "register_phone_label");
    set_widget_css(GTK_WIDGET(register_phone_label), "register_label", "label#register_label{color:red;}");

    GObject *register_email_label = gtk_builder_get_object(builder, "register_email_label");
    set_widget_css(GTK_WIDGET(register_email_label), "register_label", "label#register_label{color:red;}");

    GObject *register_password_label = gtk_builder_get_object(builder, "register_password_label");
    set_widget_css(GTK_WIDGET(register_password_label), "register_label", "label#register_label{color:red;}");

    GObject *register_confirm_password_label = gtk_builder_get_object(builder, "register_confirm_password_label");
    set_widget_css(GTK_WIDGET(register_confirm_password_label), "register_label", "label#register_label{color:red;}");

    int cin_valid = isCinValid(cin);
    int patient_exist = ifPatientExists(cin);
    int first_name_valid = isNameValid(first_name);
    int last_name_valid = isNameValid(last_name);
    int age_valid = isAgeValid(age);
    int address_valid = isTextValid(address);
    int phone_valid = isPhoneValid(phone);
    int email_valid = checkEmail(email);
    int password_valid = checkPassword(password);
    int confirm_password_valid = strcmp(confirm_password, password) == 0;

    if (!cin_valid) {
        gtk_label_set_text(GTK_LABEL(register_cin_label), "Invalid cin");
    } else {
        if (patient_exist) {
            gtk_label_set_text(GTK_LABEL(register_cin_label), "patient already exists");
        } else {
            gtk_label_set_text(GTK_LABEL(register_cin_label), "");
        }
    }

    if (!first_name_valid) {
        gtk_label_set_text(GTK_LABEL(register_first_name_label), "Must not be left empty");
    } else {
        gtk_label_set_text(GTK_LABEL(register_first_name_label), "");
    }

    if (!last_name_valid) {
        gtk_label_set_text(GTK_LABEL(register_last_name_label), "Must not be left empty");
    } else {
        gtk_label_set_text(GTK_LABEL(register_last_name_label), "");
    }

    if (!age_valid) {
        gtk_label_set_text(GTK_LABEL(register_age_label), "Invalid age (must be between 18 and 120)");
    } else {
        gtk_label_set_text(GTK_LABEL(register_age_label), "");
    }

    if (!address_valid) {
        gtk_label_set_text(GTK_LABEL(register_address_label), "Must not be left empty");
    } else {
        gtk_label_set_text(GTK_LABEL(register_address_label), "");
    }

    if (!phone_valid) {
        gtk_label_set_text(GTK_LABEL(register_phone_label), "Invalid phone number (must contain 10 digit)");
    } else {
        gtk_label_set_text(GTK_LABEL(register_phone_label), "");
    }

    if (!email_valid) {
        gtk_label_set_text(GTK_LABEL(register_email_label), "Invalid email (must include '@', a valid domain, and no spaces).");
    } else {
        gtk_label_set_text(GTK_LABEL(register_email_label), "");
    }

    if (!password_valid) {
        gtk_label_set_text(GTK_LABEL(register_password_label), "Invalid password (must contain at least 1 uppercase letter, 1 lowercase letter, 1 number, and 1 special character)");
    } else {
        gtk_label_set_text(GTK_LABEL(register_password_label), "");
    }
    if (!confirm_password_valid) {
        gtk_label_set_text(GTK_LABEL(register_confirm_password_label), "Invalid confirm password");
    } else {
        gtk_label_set_text(GTK_LABEL(register_confirm_password_label), "");
    }

    if (cin_valid && !patient_exist && first_name_valid && last_name_valid && age_valid && address_valid && phone_valid && email_valid &&  password_valid && confirm_password_valid) {
        Login user;
        Patient patient;
        strcpy(patient.cin, cin);
        strcpy(patient.fName, first_name);
        strcpy(patient.lName, last_name);
        patient.age = age;
        strcpy(patient.address, address);
        strcpy(patient.phone, phone);

        strcpy(user.cin, cin);
        strcpy(user.email, email);
        strcpy(user.password, password);
        user.role = USER;

        addPatient(patient);
        addAccount(user);

        switch_to_window(btn, GINT_TO_POINTER(LOGIN));
    }
}

//===================================================================//
// set windows objs
//===================================================================//

// set patient windows objects
void set_patient_win() {

    GtkBuilder *builder = get_builder_instance();

    GObject *admin_pt_window = gtk_builder_get_object(builder, "admin_patient_window");
    if (!admin_pt_window) {
        g_printerr("Failed to get 'admin patient window' from builder UI file\n");
    }
    gtk_window_maximize(GTK_WINDOW(admin_pt_window));

    GObject *scrolled_window = gtk_builder_get_object(builder,"patient_scrolled_window");

    GtkGrid *grid = GTK_GRID(gtk_builder_get_object(builder, "admin_patient_grid"));
    gtk_widget_set_halign(GTK_WIDGET(grid), GTK_ALIGN_CENTER);
    gtk_widget_set_valign(GTK_WIDGET(grid), GTK_ALIGN_START);
    set_widget_css(GTK_WIDGET(grid), "admin_patient_grid",
    "grid#admin_patient_grid {"
        "   min-width: 1000px;"
        "   min-height: 800px;"
        "   margin-top: 50px;"
        "   margin-bottom: 50px;"
    "}");

    // Define grid headers
    GtkWidget *id_header = gtk_label_new("CIN");
    set_widget_css(GTK_WIDGET(id_header), "header",
    "label#header {"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   border: 1px solid #f9d4e4;"
        "   background: #f9e1ec;"
        "   padding: 10px 20px;"
        "   margin-top: 50px;"
        "}");
    GtkWidget *lName_header = gtk_label_new("Last name");
    set_widget_css(GTK_WIDGET(lName_header), "header",
    "label#header {"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   border: 1px solid #f9d4e4;"
        "   background: #f9e1ec;"
        "   padding: 10px 20px;"
        "   margin-top: 50px;"
        "}");
    GtkWidget *fName_header = gtk_label_new("First name");
    set_widget_css(GTK_WIDGET(fName_header), "header",
    "label#header {"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   border: 1px solid #f9d4e4;"
        "   background: #f9e1ec;"
        "   padding: 10px 20px;"
        "   margin-top: 50px;"
        "}");
    GtkWidget *age_header = gtk_label_new("Age");
    set_widget_css(GTK_WIDGET(age_header), "header",
    "label#header {"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   border: 1px solid #f9d4e4;"
        "   background: #f9e1ec;"
        "   padding: 10px 20px;"
        "   margin-top: 50px;"
        "}");
    GtkWidget *addr_header = gtk_label_new("Address");
    set_widget_css(GTK_WIDGET(addr_header), "header",
    "label#header {"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   border: 1px solid #f9d4e4;"
        "   background: #f9e1ec;"
        "   padding: 10px 20px;"
        "   margin-top: 50px;"
        "}");
    GtkWidget *phone_header = gtk_label_new("Phone");
    set_widget_css(GTK_WIDGET(phone_header), "header",
    "label#header {"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   border: 1px solid #f9d4e4;"
        "   background: #f9e1ec;"
        "   padding: 10px 20px;"
        "   margin-top: 50px;"
        "}");

    // Add headers to the first row of the grid
    gtk_grid_attach(grid, id_header, 0, 1, 1, 1);
    gtk_grid_attach(grid, lName_header, 1, 1, 1, 1);
    gtk_grid_attach(grid, fName_header, 2, 1, 1, 1);
    gtk_grid_attach(grid, age_header, 3, 1, 1, 1);
    gtk_grid_attach(grid, addr_header, 4, 1, 1, 1);
    gtk_grid_attach(grid, phone_header, 5, 1, 1, 1);

    g_update_display_patient(NULL, grid);

    // Create and attach buttons after the last row of patient data
    GObject *reload_btn = gtk_builder_get_object(builder, "admin_patient_reload_btn");
    gtk_widget_set_hexpand(GTK_WIDGET(reload_btn), FALSE);
    gtk_widget_set_size_request(GTK_WIDGET(reload_btn), 80, 80);
    set_widget_css(GTK_WIDGET(reload_btn), "btn",
    "button#btn {"
        "   font-size: 18px;"
        "   border: none;"
        "   border-radius: 100px;"
        "   background: transparent;"
        "   box-shadow: none;"
        "   transition: background 0.3s;"
        "}");

    GtkWidget *reload_image = gtk_image_new_from_file("imgs/reload_1.svg");
    gtk_image_set_pixel_size(GTK_IMAGE(reload_image), 50);
    set_widget_css(GTK_WIDGET(reload_image), "reload_img",
    "image#reload_img {"
    "   transition: transform 0.3s;"
    "}"
    "image#reload_img:hover {"
    "   transform: scale(1.1);"
    "   transition: transform 0.3s;"
    "}");
    gtk_button_set_child(GTK_BUTTON(reload_btn), reload_image);
    //----------------------------------------------------
    GObject *add_btn = gtk_builder_get_object(builder, "admin_patient_add_btn");
    gtk_widget_set_hexpand(GTK_WIDGET(add_btn), FALSE);
    gtk_widget_set_size_request(GTK_WIDGET(add_btn), 80, 80);
    set_widget_css(GTK_WIDGET(add_btn), "btn",
    "button#btn {"
        "   font-size: 18px;"
        "   border: none;"
        "   border-radius: 100px;"
        "   background: transparent;"
        "   box-shadow: none;"
        "   transition: background 0.3s;"
        "}");
    GtkWidget *add_image = gtk_image_new_from_file("imgs/add_1.svg");
    gtk_image_set_pixel_size(GTK_IMAGE(add_image), 50);
    set_widget_css(GTK_WIDGET(add_image), "add_img",
    "image#add_img {"
    "   transition: transform 0.3s;"
    "   border-radius: 100px;"
    "}"
    "image#add_img:hover {"
    "   transform: scale(1.1);"
    "   transition: transform 0.3s;"
    "}");
    gtk_button_set_child(GTK_BUTTON(add_btn), add_image);
    //----------------------------------------------------
    GObject *back_btn = gtk_builder_get_object(builder, "admin_patient_back_btn");
    set_widget_css(GTK_WIDGET(back_btn), "btn",
    "button#btn {"
        "   font-size: 18px;"
        "   border-left: 5px solid #f9d4e4;"
        "   border-right: none;"
        "   border-top: none;"
        "   border-bottom: none;"
        "   border-radius: 0px; "
        "   padding: 0px;"
        "   background: transparent;"
        "   box-shadow: none;"
        "}");
    GtkWidget *back_image = gtk_image_new_from_file("imgs/back_arrow_1.svg");
    gtk_image_set_pixel_size(GTK_IMAGE(back_image), 50);
    set_widget_css(GTK_WIDGET(back_image), "back_arrow",
    "image#back_arrow {"
    "   transition: transform 0.3s;"
    "}"
    "image#back_arrow:hover {"
    "   transform: translateX(-10px);"
    "   transition: transform 0.3s;"
    "}");

    gtk_button_set_child(GTK_BUTTON(back_btn), back_image);

    // Connect button signals to their respective callbacks
    g_signal_connect(reload_btn, "clicked", G_CALLBACK(g_update_display_patient), grid);
    g_signal_connect(add_btn,"clicked", G_CALLBACK(add_patient_popup), NULL);
    g_signal_connect(back_btn, "clicked", G_CALLBACK(switch_to_window), GINT_TO_POINTER(ADMIN_MENU));

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), GTK_WIDGET(grid));
    gtk_window_set_child(GTK_WINDOW(admin_pt_window), GTK_WIDGET(scrolled_window));
}

// set rendezvous windows objects
void set_rendezvous_win() {

    GtkBuilder *builder = get_builder_instance();

    GObject *admin_rendezvous_window = gtk_builder_get_object(builder, "admin_rendezvous_window");
    if (!admin_rendezvous_window) {
        g_printerr("Failed to get 'admin rendezvous window' from builder UI file\n");
    }
    gtk_window_maximize(GTK_WINDOW(admin_rendezvous_window));

    GtkScrolledWindow *scrolled_window = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder, "admin_rendezvous_scrolled_window"));

    GtkGrid *grid = GTK_GRID(gtk_builder_get_object(builder, "admin_rendezvous_grid"));
    gtk_widget_set_halign(GTK_WIDGET(grid), GTK_ALIGN_CENTER);
    gtk_widget_set_valign(GTK_WIDGET(grid), GTK_ALIGN_START);
    set_widget_css(GTK_WIDGET(grid), "admin_rendezvous_grid",
    "grid#admin_rendezvous_grid {"
        "   min-width: 1000px;"
        "   min-height: 800px;"
        "   margin-top: 50px;"
        "   margin-bottom: 50px;"
    "}");

    // Define and style headers
    GtkWidget *id_header = gtk_label_new("Rendezvous ID");
    set_widget_css(GTK_WIDGET(id_header), "header",
    "label#header {"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   border: 1px solid #d8d5f7;"
        "   background: #e8e8f9;"
        "   padding: 10px 20px;"
        "   margin-top: 50px;"
        "}");
    GtkWidget *id_pt_header = gtk_label_new("Patient ID");
    set_widget_css(GTK_WIDGET(id_pt_header), "header",
    "label#header {"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   border: 1px solid #d8d5f7;"
        "   background: #e8e8f9;"
        "   padding: 10px 20px;"
        "   margin-top: 50px;"
        "}");
    GtkWidget *name_pt_header = gtk_label_new("Patient Name");
    set_widget_css(GTK_WIDGET(name_pt_header), "header",
    "label#header {"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   border: 1px solid #d8d5f7;"
        "   background: #e8e8f9;"
        "   padding: 10px 20px;"
        "   margin-top: 50px;"
        "}");
    GtkWidget *date_header = gtk_label_new("Date");
    set_widget_css(GTK_WIDGET(date_header), "header",
    "label#header {"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   border: 1px solid #d8d5f7;"
        "   background: #e8e8f9;"
        "   padding: 10px 20px;"
        "   margin-top: 50px;"
        "}");
    GtkWidget *time_header = gtk_label_new("Time");
    set_widget_css(GTK_WIDGET(time_header), "header",
    "label#header {"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   border: 1px solid #d8d5f7;"
        "   background: #e8e8f9;"
        "   padding: 10px 20px;"
        "   margin-top: 50px;"
        "}");
    GtkWidget *state_header = gtk_label_new("State");
    set_widget_css(GTK_WIDGET(state_header), "header",
    "label#header {"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   border: 1px solid #d8d5f7;"
        "   background: #e8e8f9;"
        "   padding: 10px 20px;"
        "   margin-top: 50px;"
        "}");

    gtk_grid_attach(grid, id_header, 0, 1, 1, 1);
    gtk_grid_attach(grid, id_pt_header, 1, 1, 1, 1);
    gtk_grid_attach(grid, name_pt_header, 2, 1, 1, 1);
    gtk_grid_attach(grid, date_header, 3, 1, 1, 1);
    gtk_grid_attach(grid, time_header, 4, 1, 1, 1);
    gtk_grid_attach(grid, state_header, 5, 1, 1, 1);

    g_update_display_rdv(NULL, grid);

    GObject *reload_btn = gtk_builder_get_object(builder, "admin_reload_rendezvous_btn");
    gtk_widget_set_hexpand(GTK_WIDGET(reload_btn), FALSE);
    gtk_widget_set_size_request(GTK_WIDGET(reload_btn), 80, 80);
    set_widget_css(GTK_WIDGET(reload_btn), "btn",
    "button#btn {"
        "   font-size: 18px;"
        "   border: none;"
        "   border-radius: 100px;"
        "   background: transparent;"
        "   box-shadow: none;"
        "   transition: background 0.3s;"
        "}");

    GtkWidget *reload_image = gtk_image_new_from_file("imgs/reload_2.svg");
    gtk_image_set_pixel_size(GTK_IMAGE(reload_image), 50);
    set_widget_css(GTK_WIDGET(reload_image), "reload_img",
    "image#reload_img {"
        "   transition: transform 0.3s;"
        "}"
    "image#reload_img:hover {"
        "   transform: scale(1.1);"
        "   transition: transform 0.3s;"
        "}");
    gtk_button_set_child(GTK_BUTTON(reload_btn), reload_image);
    //----------------------------------------------------
    GObject *add_btn = gtk_builder_get_object(builder, "admin_add_rendezvous_btn");
    gtk_widget_set_hexpand(GTK_WIDGET(add_btn), FALSE);
    gtk_widget_set_size_request(GTK_WIDGET(add_btn), 80, 80);
    set_widget_css(GTK_WIDGET(add_btn), "btn",
    "button#btn {"
        "   font-size: 18px;"
        "   border: none;"
        "   border-radius: 100px;"
        "   background: transparent;"
        "   box-shadow: none;"
        "   transition: background 0.3s;"
        "}");
    GtkWidget *add_image = gtk_image_new_from_file("imgs/add_2.svg");
    gtk_image_set_pixel_size(GTK_IMAGE(add_image), 50);
    set_widget_css(GTK_WIDGET(add_image), "add_img",
    "image#add_img {"
    "   transition: transform 0.3s;"
    "   border-radius: 100px;"
    "}"
    "image#add_img:hover {"
    "   transform: scale(1.1);"
    "   transition: transform 0.3s;"
    "}");
    gtk_button_set_child(GTK_BUTTON(add_btn), add_image);
    //----------------------------------------------------
    GObject *back_btn = gtk_builder_get_object(builder, "admin_back_rendezvous_btn");
    set_widget_css(GTK_WIDGET(back_btn), "btn",
    "button#btn {"
        "   font-size: 18px;"
        "   border-left: 5px solid #c6c5ea;"
        "   border-right: none;"
        "   border-top: none;"
        "   border-bottom: none;"
        "   border-radius: 0px; "
        "   padding: 0px;"
        "   background: transparent;"
        "   box-shadow: none;"
        "}");
    GtkWidget *back_image = gtk_image_new_from_file("imgs/back_arrow_2.svg");
    gtk_image_set_pixel_size(GTK_IMAGE(back_image), 50);
    set_widget_css(GTK_WIDGET(back_image), "back_arrow",
    "image#back_arrow {"
    "   transition: transform 0.3s;"
    "}"
    "image#back_arrow:hover {"
    "   transform: translateX(-10px);"
    "   transition: transform 0.3s;"
    "}");

    gtk_button_set_child(GTK_BUTTON(back_btn), back_image);

    // Connect button signals to their respective callbacks
    g_signal_connect(reload_btn, "clicked", G_CALLBACK(g_update_display_rdv), grid);
    g_signal_connect(add_btn, "clicked", G_CALLBACK(add_rdv_popup), NULL);
    g_signal_connect(back_btn, "clicked", G_CALLBACK(switch_to_window), GINT_TO_POINTER(ADMIN_MENU));

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), GTK_WIDGET(grid));
    gtk_window_set_child(GTK_WINDOW(admin_rendezvous_window), GTK_WIDGET(scrolled_window));
}

// set consultation windows objects
void set_consult_win() {

        GtkBuilder *builder = get_builder_instance();

        GObject *admin_consult_window = gtk_builder_get_object(builder, "admin_consult_window");
        if (!admin_consult_window) {
            g_printerr("Failed to get 'admin consult window' from builder UI file\n");
        }
        gtk_window_maximize(GTK_WINDOW(admin_consult_window));

        GObject *scrolled_window = gtk_builder_get_object(builder, "admin_consult_scrolled_window");

        GtkGrid *grid = GTK_GRID(gtk_builder_get_object(builder, "admin_consult_grid"));
        gtk_widget_set_halign(GTK_WIDGET(grid), GTK_ALIGN_CENTER);
        gtk_widget_set_valign(GTK_WIDGET(grid), GTK_ALIGN_START);
        set_widget_css(GTK_WIDGET(grid), "admin_consult_grid",
        "grid#admin_consult_grid {"
        "   min-width: 1000px;"
        "   min-height: 800px;"
        "   margin-top: 50px;"
        "   margin-bottom: 50px;"
        "}");

        // Define grid headers
        GtkWidget *id_header = gtk_label_new("Consultation ID");
        set_widget_css(GTK_WIDGET(id_header), "header",
        "label#header {"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   border: 1px solid #b5e5dc;"
        "   background: #e4f4f1;"
        "   padding: 10px 20px;"
        "   margin-top: 50px;"
        "}");

        GtkWidget *id_pt_header = gtk_label_new("Patient ID");
        set_widget_css(GTK_WIDGET(id_pt_header), "header",
        "label#header {"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   border: 1px solid #b5e5dc;"
        "   background: #e4f4f1;"
        "   padding: 10px 20px;"
        "   margin-top: 50px;"
        "}");

        GtkWidget *name_pt_header = gtk_label_new("Patient Name");
        set_widget_css(GTK_WIDGET(name_pt_header), "header",
        "label#header {"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   border: 1px solid #b5e5dc;"
        "   background: #e4f4f1;"
        "   padding: 10px 20px;"
        "   margin-top: 50px;"
        "}");

        GtkWidget *symptoms_header = gtk_label_new("Symptoms");
        set_widget_css(GTK_WIDGET(symptoms_header), "header",
        "label#header {"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   border: 1px solid #b5e5dc;"
        "   background: #e4f4f1;"
        "   padding: 10px 20px;"
        "   margin-top: 50px;"
        "}");

        GtkWidget *diagnosis_header = gtk_label_new("Diagnosis");
        set_widget_css(GTK_WIDGET(diagnosis_header), "header",
        "label#header {"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   border: 1px solid #b5e5dc;"
        "   background: #e4f4f1;"
        "   padding: 10px 20px;"
        "   margin-top: 50px;"
        "}");

        GtkWidget *treatmentPlan_header = gtk_label_new("Treatment Plan");
        set_widget_css(GTK_WIDGET(treatmentPlan_header), "header",
        "label#header {"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   border: 1px solid #b5e5dc;"
        "   background: #e4f4f1;"
        "   padding: 10px 20px;"
        "   margin-top: 50px;"
        "}");

        GtkWidget *date_header = gtk_label_new("Date");
        set_widget_css(GTK_WIDGET(date_header), "header",
        "label#header {"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   border: 1px solid #b5e5dc;"
        "   background: #e4f4f1;"
        "   padding: 10px 20px;"
        "   margin-top: 50px;"
        "}");

        GtkWidget *time_header = gtk_label_new("Time");
        set_widget_css(GTK_WIDGET(time_header), "header",
        "label#header {"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   border: 1px solid #b5e5dc;"
        "   background: #e4f4f1;"
        "   padding: 10px 20px;"
        "   margin-top: 50px;"
        "}");

        // Add headers to the first row of the grid
        gtk_grid_attach(grid, id_header, 0, 1, 1, 1);
        gtk_grid_attach(grid, id_pt_header, 1, 1, 1, 1);
        gtk_grid_attach(grid, name_pt_header, 2, 1, 1, 1);
        gtk_grid_attach(grid, symptoms_header, 3, 1, 1, 1);
        gtk_grid_attach(grid, diagnosis_header, 4, 1, 1, 1);
        gtk_grid_attach(grid, treatmentPlan_header, 5, 1, 1, 1);
        gtk_grid_attach(grid, date_header, 6, 1, 1, 1);
        gtk_grid_attach(grid, time_header, 7, 1, 1, 1);

        g_update_display_consult(NULL, grid);

        /// Create and attach buttons after the last row of patient data
        GObject *reload_btn = gtk_builder_get_object(builder, "admin_reload_consult_btn");
        gtk_widget_set_hexpand(GTK_WIDGET(reload_btn), FALSE);
        gtk_widget_set_size_request(GTK_WIDGET(reload_btn), 80, 80);
        set_widget_css(GTK_WIDGET(reload_btn), "btn",
            "button#btn {"
            "   font-size: 18px;"
            "   border: none;"
            "   border-radius: 100px;"
            "   background: transparent;"
            "   box-shadow: none;"
            "   transition: background 0.3s;"
            "}");

        GtkWidget *reload_image = gtk_image_new_from_file("imgs/reload_3.svg");
        gtk_image_set_pixel_size(GTK_IMAGE(reload_image), 50);
        set_widget_css(GTK_WIDGET(reload_image), "reload_img",
            "image#reload_img {"
            "   transition: transform 0.3s;"
            "}"
            "image#reload_img:hover {"
            "   transform: scale(1.1);"
            "   transition: transform 0.3s;"
            "}");
        gtk_button_set_child(GTK_BUTTON(reload_btn), reload_image);

        // ----------------------------------------------------
        GObject *add_btn = gtk_builder_get_object(builder, "admin_add_consult_btn");
        gtk_widget_set_hexpand(GTK_WIDGET(add_btn), FALSE);
        gtk_widget_set_size_request(GTK_WIDGET(add_btn), 80, 80);
        set_widget_css(GTK_WIDGET(add_btn), "btn",
            "button#btn {"
            "   font-size: 18px;"
            "   border: none;"
            "   border-radius: 100px;"
            "   background: transparent;"
            "   box-shadow: none;"
            "   transition: background 0.3s;"
            "}");

        GtkWidget *add_image = gtk_image_new_from_file("imgs/add_3.svg");
        gtk_image_set_pixel_size(GTK_IMAGE(add_image), 50);
        set_widget_css(GTK_WIDGET(add_image), "add_img",
            "image#add_img {"
            "   transition: transform 0.3s;"
            "   border-radius: 100px;"
            "}"
            "image#add_img:hover {"
            "   transform: scale(1.1);"
            "   transition: transform 0.3s;"
            "}");
        gtk_button_set_child(GTK_BUTTON(add_btn), add_image);

        // ----------------------------------------------------
        GObject *back_btn = gtk_builder_get_object(builder, "admin_back_consult_btn");
        set_widget_css(GTK_WIDGET(back_btn), "btn",
            "button#btn {"
            "   font-size: 18px;"
            "   border-left: 5px solid #b5e5dc;"
            "   border-right: none;"
            "   border-top: none;"
            "   border-bottom: none;"
            "   border-radius: 0px; "
            "   padding: 0px;"
            "   background: transparent;"
            "   box-shadow: none;"
            "}");

        GtkWidget *back_image = gtk_image_new_from_file("imgs/back_arrow_3.svg");
        gtk_image_set_pixel_size(GTK_IMAGE(back_image), 50);
        set_widget_css(GTK_WIDGET(back_image), "back_arrow",
            "image#back_arrow {"
            "   transition: transform 0.3s;"
            "}"
            "image#back_arrow:hover {"
            "   transform: translateX(-10px);"
            "   transition: transform 0.3s;"
            "}");

        gtk_button_set_child(GTK_BUTTON(back_btn), back_image);


        // Connect button signals to their respective callbacks
        g_signal_connect(reload_btn, "clicked", G_CALLBACK(g_update_display_consult), grid);
        g_signal_connect(add_btn, "clicked", G_CALLBACK(add_consult_popup), NULL);
        g_signal_connect(back_btn, "clicked", G_CALLBACK(switch_to_window), GINT_TO_POINTER(ADMIN_MENU));

        gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), GTK_WIDGET(grid));
        gtk_window_set_child(GTK_WINDOW(admin_consult_window), GTK_WIDGET(scrolled_window));
}

// set payment windows objects
void set_payment_win() {

    GtkBuilder *builder = get_builder_instance();

    GObject *admin_invoice_window = gtk_builder_get_object(builder, "admin_invoice_window");
    if (!admin_invoice_window) {
        g_printerr("Failed to get 'admin invoice window' from builder UI file\n");
    }
    gtk_window_maximize(GTK_WINDOW(admin_invoice_window));

    GObject *scrolled_window = gtk_builder_get_object(builder, "admin_invoice_scrolled_window");

    GtkGrid *grid = GTK_GRID(gtk_builder_get_object(builder, "admin_invoice_grid"));
    gtk_widget_set_halign(GTK_WIDGET(grid), GTK_ALIGN_CENTER);
    gtk_widget_set_valign(GTK_WIDGET(grid), GTK_ALIGN_START);

    set_widget_css(GTK_WIDGET(grid), "admin_invoice_grid",
        "grid#admin_invoice_grid {"
        "   min-width: 1000px;"
        "   min-height: 800px;"
        "   margin-top: 50px;"
        "   margin-bottom: 50px;"
        "}"
    );

    // Define grid headers
    GtkWidget *id_header = gtk_label_new("Invoice ID");
    set_widget_css(GTK_WIDGET(id_header), "header",
        "label#header {"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   border: 1px solid #e8d1c1;"
        "   background: #f7f0eb;"
        "   padding: 10px 20px;"
        "   margin-top: 50px;"
        "}"
    );

    GtkWidget *id_consult_header = gtk_label_new("Consultation ID");
    set_widget_css(GTK_WIDGET(id_consult_header), "header",
        "label#header {"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   border: 1px solid #e8d1c1;"
        "   background: #f7f0eb;"
        "   padding: 10px 20px;"
        "   margin-top: 50px;"
        "}"
    );

    GtkWidget *id_pt_header = gtk_label_new("Patient ID");
    set_widget_css(GTK_WIDGET(id_pt_header), "header",
        "label#header {"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   border: 1px solid #e8d1c1;"
        "   background: #f7f0eb;"
        "   padding: 10px 20px;"
        "   margin-top: 50px;"
        "}"
    );

    GtkWidget *name_pt_header = gtk_label_new("Patient Name");
    set_widget_css(GTK_WIDGET(name_pt_header), "header",
        "label#header {"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   border: 1px solid #e8d1c1;"
        "   background: #f7f0eb;"
        "   padding: 10px 20px;"
        "   margin-top: 50px;"
        "}"
    );

    GtkWidget *amount_header = gtk_label_new("Amount");
    set_widget_css(GTK_WIDGET(amount_header), "header",
        "label#header {"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   border: 1px solid #e8d1c1;"
        "   background: #f7f0eb;"
        "   padding: 10px 20px;"
        "   margin-top: 50px;"
        "}"
    );

    GtkWidget *date_header = gtk_label_new("Date");
    set_widget_css(GTK_WIDGET(date_header), "header",
        "label#header {"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   border: 1px solid #e8d1c1;"
        "   background: #f7f0eb;"
        "   padding: 10px 20px;"
        "   margin-top: 50px;"
        "}"
    );
    GtkWidget *time_header = gtk_label_new("Time");
    set_widget_css(GTK_WIDGET(time_header), "header",
        "label#header {"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   border: 1px solid #e8d1c1;"
        "   background: #f7f0eb;"
        "   padding: 10px 20px;"
        "   margin-top: 50px;"
        "}"
    );
    GtkWidget *state_header = gtk_label_new("State");
    set_widget_css(GTK_WIDGET(state_header), "header",
        "label#header {"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   border: 1px solid #e8d1c1;"
        "   background: #f7f0eb;"
        "   padding: 10px 20px;"
        "   margin-top: 50px;"
        "}"
    );


    // Add headers to the first row of the grid
    gtk_grid_attach(grid, id_header, 0, 1, 1, 1);
    gtk_grid_attach(grid, id_consult_header, 1, 1, 1, 1);
    gtk_grid_attach(grid, id_pt_header, 2, 1, 1, 1);
    gtk_grid_attach(grid, name_pt_header, 3, 1, 1, 1);
    gtk_grid_attach(grid, amount_header, 4, 1, 1, 1);
    gtk_grid_attach(grid, date_header, 5, 1, 1, 1);
    gtk_grid_attach(grid, time_header, 6, 1, 1, 1);
    gtk_grid_attach(grid, state_header, 7, 1, 1, 1);

    g_update_display_payment(NULL, grid);

    // Create and attach buttons for actions
    GObject *reload_btn = gtk_builder_get_object(builder, "admin_reload_invoice_btn");
    gtk_widget_set_hexpand(GTK_WIDGET(reload_btn), FALSE);
    gtk_widget_set_size_request(GTK_WIDGET(reload_btn), 80, 80);
    set_widget_css(GTK_WIDGET(reload_btn), "btn",
        "button#btn {"
        "   font-size: 18px;"
        "   border: none;"
        "   border-radius: 100px;"
        "   background: transparent;"
        "   box-shadow: none;"
        "   transition: background 0.3s;"
        "}"
    );

    GtkWidget *reload_image = gtk_image_new_from_file("imgs/reload_4.svg");
    gtk_image_set_pixel_size(GTK_IMAGE(reload_image), 50);
    gtk_button_set_child(GTK_BUTTON(reload_btn), reload_image);
    set_widget_css(GTK_WIDGET(reload_image), "reload_image",
            "image#reload_image {"
            "   transition: transform 0.3s;"
            "}"
            "image#reload_image:hover {"
            "   transform: scale(1.1);"
            "   transition: transform 0.3s;"
            "}");
    //-----------------------

    GObject *add_btn = gtk_builder_get_object(builder, "admin_add_invoice_btn");
    gtk_widget_set_hexpand(GTK_WIDGET(add_btn), FALSE);
    gtk_widget_set_size_request(GTK_WIDGET(add_btn), 80, 80);
    set_widget_css(GTK_WIDGET(add_btn), "btn",
        "button#btn {"
        "   font-size: 18px;"
        "   border: none;"
        "   border-radius: 100px;"
        "   background: transparent;"
        "   box-shadow: none;"
        "   transition: background 0.3s;"
        "}"
    );

    GtkWidget *add_image = gtk_image_new_from_file("imgs/add_4.svg");
    gtk_image_set_pixel_size(GTK_IMAGE(add_image), 50);
    gtk_button_set_child(GTK_BUTTON(add_btn), add_image);
    set_widget_css(GTK_WIDGET(add_image), "add_image",
            "image#add_image {"
            "   transition: transform 0.3s;"
            "}"
            "image#add_image:hover {"
            "   transform: scale(1.1);"
            "   transition: transform 0.3s;"
            "}");

    GObject *back_invoice_btn = gtk_builder_get_object(builder, "admin_back_invoice_btn");
    set_widget_css(GTK_WIDGET(back_invoice_btn), "btn",
        "button#btn {"
        "   font-size: 18px;"
        "   border-left: 5px solid #e8d1c1;"
        "   border-right: none;"
        "   border-top: none;"
        "   border-bottom: none;"
        "   border-radius: 0px; "
        "   padding: 0px;"
        "   background: transparent;"
        "   box-shadow: none;"
        "}"
    );

    GtkWidget *back_image = gtk_image_new_from_file("imgs/back_arrow_4.svg");
    gtk_image_set_pixel_size(GTK_IMAGE(back_image), 50);
    gtk_button_set_child(GTK_BUTTON(back_invoice_btn), back_image);
    set_widget_css(GTK_WIDGET(back_image), "back_image",
            "image#back_image {"
            "   transition: transform 0.3s;"
            "}"
            "image#back_image:hover {"
            "   transform: translateX(-10px);"
            "   transition: transform 0.3s;"
            "}");

    // Connect button signals to their respective callbacks
    g_signal_connect(reload_btn, "clicked", G_CALLBACK(g_update_display_payment), grid);
    g_signal_connect(add_btn, "clicked", G_CALLBACK(add_payment_popup), NULL);
    g_signal_connect(back_invoice_btn, "clicked", G_CALLBACK(switch_to_window), GINT_TO_POINTER(ADMIN_MENU));

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), GTK_WIDGET(grid));
    gtk_window_set_child(GTK_WINDOW(admin_invoice_window), GTK_WIDGET(scrolled_window));

}

// set menu windows objects
void set_menu_win_objs() {

    GtkBuilder *builder = get_builder_instance();

    GObject *admin_menu_window = gtk_builder_get_object(builder, "admin_menu_window");
    if (!admin_menu_window) {
        g_printerr("Failed to get 'main_window' from builder UI file\n");
    }
    gtk_window_maximize(GTK_WINDOW(admin_menu_window));
    set_widget_css(GTK_WIDGET(admin_menu_window), "menu_window", "window#menu_window { background: #F2FAFD;}");
    //-------------------------------------

    GObject *menu_grid = gtk_builder_get_object(builder, "admin_menu_grid");
    set_widget_css(GTK_WIDGET(menu_grid), "menu_grid",
    "grid#menu_grid {"
        "   min-width: 800px;"
        "}");
    gtk_widget_set_valign(GTK_WIDGET(menu_grid), GTK_ALIGN_CENTER);
    gtk_widget_set_halign(GTK_WIDGET(menu_grid), GTK_ALIGN_CENTER);
    //-------------------------------------
    //----------Patient section
    //-------------------------------------
    GObject *patient_btn = gtk_builder_get_object(builder, "patient_btn");

    gtk_widget_set_vexpand_set(GTK_WIDGET(patient_btn), TRUE);
    if (patient_btn == NULL) {
        g_printerr("Failed to get 'btn_patient' from builder UI\n");
        return;
    }
    set_widget_css(GTK_WIDGET(patient_btn), "patient_btn",
    "button#patient_btn {"
        "   min-width: 400px;"
        "   min-height: 200px;"
        "   padding: 0px;"
        "   border: 0px;"
        "   border-radius: 10px;"
        "   background: linear-gradient(45deg, rgba(255,255,255,1) 0%, rgba(255,255,255,1) 50%);"
        "   transition: transform 0.3s, background 0.3s;}"
    "#patient_btn:hover {"
        "   background: linear-gradient(45deg, rgba(249,212,228,0.9) 0%, rgba(255,255,255,1) 50%);"
        "   transform: scale(1.05);"
        "   transition: transform 0.3s, background 0.3s;}");
    //-------------------------------------

    GtkWidget *patient_overlay = gtk_overlay_new();
    set_widget_css(GTK_WIDGET(patient_overlay), "patient_overlay","widget#patient_overlay {background: #000;}");
    //-------------------------------------

    GtkWidget *patient_image = gtk_image_new_from_file("imgs/patient_icon.svg");
    gtk_image_set_pixel_size(GTK_IMAGE(patient_image), 50);
    gtk_widget_set_halign(GTK_WIDGET(patient_image), GTK_ALIGN_END);
    gtk_widget_set_valign(GTK_WIDGET(patient_image), GTK_ALIGN_START);
    set_widget_css(GTK_WIDGET(patient_image), "patient_image","image#patient_image {margin-top: 10px; margin-right: 5px;}");
    gtk_overlay_add_overlay(GTK_OVERLAY(patient_overlay), patient_image);
    //-------------------------------------

    GtkWidget *patient_label = gtk_label_new("Patients");
    gtk_widget_set_halign(GTK_WIDGET(patient_label), GTK_ALIGN_START);
    gtk_widget_set_valign(GTK_WIDGET(patient_label), GTK_ALIGN_START);
    set_widget_css(GTK_WIDGET(patient_label), "patient_label","label#patient_label {margin-left: 15px; margin-top: 30px; font-size: 18px;}");
    gtk_overlay_add_overlay(GTK_OVERLAY(patient_overlay), patient_label);
    //-------------------------------------

    GtkWidget *patient_label_2 = gtk_label_new("Manage Patients, view patients\nreports across workspaces.");
    gtk_widget_set_halign(GTK_WIDGET(patient_label_2), GTK_ALIGN_START);
    gtk_widget_set_valign(GTK_WIDGET(patient_label_2), GTK_ALIGN_START);
    set_widget_css(GTK_WIDGET(patient_label_2), "patient_label","label#patient_label {margin-left: 15px ;margin-top: 60px;}");
    gtk_overlay_add_overlay(GTK_OVERLAY(patient_overlay), patient_label_2);
    //-------------------------------------
    GtkWidget *part_of_design_1 = gtk_image_new_from_file("imgs/part_of_design_1.svg");
    gtk_image_set_pixel_size(GTK_IMAGE(part_of_design_1), 50);
    gtk_widget_set_halign(GTK_WIDGET(part_of_design_1), GTK_ALIGN_END);
    gtk_widget_set_valign(GTK_WIDGET(part_of_design_1), GTK_ALIGN_END);
    gtk_overlay_add_overlay(GTK_OVERLAY(patient_overlay), part_of_design_1);

    //-------------------------------------
    gtk_button_set_child(GTK_BUTTON(patient_btn), patient_overlay);

    //-------------------------------------
    //----------Rendezvous section
    //-------------------------------------

    GObject *rdv_btn = gtk_builder_get_object(builder, "rdv_btn");
    gtk_widget_set_vexpand_set(GTK_WIDGET(rdv_btn), TRUE);
    if (rdv_btn == NULL) {
        g_printerr("Failed to get 'btn_rdv' from builder UI\n");
    }
    set_widget_css(GTK_WIDGET(rdv_btn), "rdv_btn",
        "button#rdv_btn {"
        "   min-width: 400px;"
        "   min-height: 200px;"
        "   padding: 0px;"
        "   border: 0px;"
        "   border-radius: 10px;"
        "   background: linear-gradient(45deg, rgba(255,255,255,1) 0%, rgba(255,255,255,1) 50%);"
        "   transition: transform 0.3s, background 0.3s;}"
        "#rdv_btn:hover {"
        "   background: linear-gradient(45deg, rgba(216,213,247,0.9) 0%, rgba(255,255,255,1) 50%);"
        "   transform: scale(1.05);"
        "   transition: transform 0.3s, background 0.3s;}");
    //-------------------------------------

    GtkWidget *rdv_overlay = gtk_overlay_new();
    set_widget_css(GTK_WIDGET(rdv_overlay), "rdv_overlay","widget#rdv_overlay {background: #000;}");
    //-------------------------------------

    GtkWidget *rdv_image = gtk_image_new_from_file("imgs/rendezvous_icon.svg");
    gtk_image_set_pixel_size(GTK_IMAGE(rdv_image), 50);
    gtk_widget_set_halign(GTK_WIDGET(rdv_image), GTK_ALIGN_END);
    gtk_widget_set_valign(GTK_WIDGET(rdv_image), GTK_ALIGN_START);
    set_widget_css(GTK_WIDGET(rdv_image), "rdv_image",
        "image#rdv_image {"
        "   margin-top: 10px;"
        "   margin-right: 5px;}");
    gtk_overlay_add_overlay(GTK_OVERLAY(rdv_overlay), rdv_image);
    //-------------------------------------

    GtkWidget *rdv_label = gtk_label_new("Rendezvous");
    gtk_widget_set_halign(GTK_WIDGET(rdv_label), GTK_ALIGN_START);
    gtk_widget_set_valign(GTK_WIDGET(rdv_label), GTK_ALIGN_START);
    set_widget_css(GTK_WIDGET(rdv_label), "rdv_label","label#rdv_label {margin-left: 15px; margin-top: 30px; font-size: 18px;}");
    gtk_overlay_add_overlay(GTK_OVERLAY(rdv_overlay), rdv_label);
    //-------------------------------------

    GtkWidget *rdv_label_2 = gtk_label_new("Manage rendezvous, view rendezvous\nreports across workspaces.");
    gtk_widget_set_halign(GTK_WIDGET(rdv_label_2), GTK_ALIGN_START);
    gtk_widget_set_valign(GTK_WIDGET(rdv_label_2), GTK_ALIGN_START);
    set_widget_css(GTK_WIDGET(rdv_label_2), "rdv_label_2","label#rdv_label_2 {margin-left: 15px ;margin-top: 60px;}");
    gtk_overlay_add_overlay(GTK_OVERLAY(rdv_overlay), rdv_label_2);
    //-------------------------------------
    GtkWidget *part_of_design_2 = gtk_image_new_from_file("imgs/part_of_design_2.svg");
    gtk_image_set_pixel_size(GTK_IMAGE(part_of_design_2), 50);
    gtk_widget_set_halign(GTK_WIDGET(part_of_design_2), GTK_ALIGN_END);
    gtk_widget_set_valign(GTK_WIDGET(part_of_design_2), GTK_ALIGN_END);
    gtk_overlay_add_overlay(GTK_OVERLAY(rdv_overlay), part_of_design_2);

    //-------------------------------------
    gtk_button_set_child(GTK_BUTTON(rdv_btn), rdv_overlay);

    //-------------------------------------
    //----------consultation section
    //-------------------------------------

    GObject *consult_btn = gtk_builder_get_object(builder, "consult_btn");
    gtk_widget_set_vexpand_set(GTK_WIDGET(consult_btn), TRUE);
    if (consult_btn == NULL) {
        g_printerr("Failed to get 'consult_btn' from builder UI\n");
    }
    set_widget_css(GTK_WIDGET(consult_btn), "consult_btn",
        "button#consult_btn {"
        "   min-width: 400px;"
        "   min-height: 200px;"
        "   padding: 0px;"
        "   border: 0px;"
        "   border-radius: 10px;"
        "   background: linear-gradient(45deg, rgba(255,255,255,1) 0%, rgba(255,255,255,1) 50%);"
        "   transition: transform 0.3s, background 0.3s;}"
        "#consult_btn:hover {"
        "   background: linear-gradient(45deg, rgba(212,244,238,0.9) 0%, rgba(255,255,255,1) 50%);"
        "   transform: scale(1.05);"
        "   transition: transform 0.3s, background 0.3s;}");
    //-------------------------------------

    GtkWidget *consult_overlay = gtk_overlay_new();
    set_widget_css(GTK_WIDGET(consult_overlay), "consult_overlay","widget#consult_overlay {background: #000;}");
    //-------------------------------------

    GtkWidget *consult_image = gtk_image_new_from_file("imgs/consultation_icon.svg");
    gtk_image_set_pixel_size(GTK_IMAGE(consult_image), 50);
    gtk_widget_set_halign(GTK_WIDGET(consult_image), GTK_ALIGN_END);
    gtk_widget_set_valign(GTK_WIDGET(consult_image), GTK_ALIGN_START);
    set_widget_css(GTK_WIDGET(consult_image), "consult_image",
        "image#consult_image {"
        "   margin-top: 10px;"
        "   margin-right: 5px;}");
    gtk_overlay_add_overlay(GTK_OVERLAY(consult_overlay), consult_image);
    //-------------------------------------

    GtkWidget *consult_label = gtk_label_new("Consultations");
    gtk_widget_set_halign(GTK_WIDGET(consult_label), GTK_ALIGN_START);
    gtk_widget_set_valign(GTK_WIDGET(consult_label), GTK_ALIGN_START);
    set_widget_css(GTK_WIDGET(consult_label), "consult_label","label#consult_label {margin-left: 15px; margin-top: 30px; font-size: 18px;}");
    gtk_overlay_add_overlay(GTK_OVERLAY(consult_overlay), consult_label);
    //-------------------------------------

    GtkWidget *consult_label_2 = gtk_label_new("Manage consultations, view consultations\nreports across workspaces.");
    gtk_widget_set_halign(GTK_WIDGET(consult_label_2), GTK_ALIGN_START);
    gtk_widget_set_valign(GTK_WIDGET(consult_label_2), GTK_ALIGN_START);
    set_widget_css(GTK_WIDGET(consult_label_2), "consult_label_2","label#consult_label_2 {margin-left: 15px ;margin-top: 60px;}");
    gtk_overlay_add_overlay(GTK_OVERLAY(consult_overlay), consult_label_2);
    //-------------------------------------
    GtkWidget *part_of_design_3 = gtk_image_new_from_file("imgs/part_of_design_3.svg");
    gtk_image_set_pixel_size(GTK_IMAGE(part_of_design_3), 50);
    gtk_widget_set_halign(GTK_WIDGET(part_of_design_3), GTK_ALIGN_END);
    gtk_widget_set_valign(GTK_WIDGET(part_of_design_3), GTK_ALIGN_END);
    gtk_overlay_add_overlay(GTK_OVERLAY(consult_overlay), part_of_design_3);

    //-------------------------------------
    gtk_button_set_child(GTK_BUTTON(consult_btn), consult_overlay);

    //-------------------------------------
    //----------Invoice section
    //-------------------------------------

    GObject *invoice_btn = gtk_builder_get_object(builder, "invoice_btn");
    gtk_widget_set_vexpand_set(GTK_WIDGET(invoice_btn), TRUE);
    if (invoice_btn == NULL) {
        g_printerr("Failed to get 'invoice_btn' from builder UI\n");
    }
    set_widget_css(GTK_WIDGET(invoice_btn), "invoice_btn",
        "button#invoice_btn {"
        "   min-width: 400px;"
        "   min-height: 200px;"
        "   padding: 0px;"
        "   border: 0px;"
        "   border-radius: 10px;"
        "   background: linear-gradient(45deg, rgba(255,255,255,1) 0%, rgba(255,255,255,1) 50%);"
        "   transition: transform 0.3s, background 0.3s;}"
        "#invoice_btn:hover {"
        "   background: linear-gradient(45deg, rgba(242,227,214,0.9) 0%, rgba(255,255,255,1) 50%);"
        "   transform: scale(1.05);"
        "   transition: transform 0.3s, background 0.3s;}");
    //-------------------------------------

    GtkWidget *invoice_overlay = gtk_overlay_new();
    // set_widget_css(GTK_WIDGET(invoice_overlay), "invoice_overlay", "widget#invoice_overlay {}");
    //-------------------------------------

    GtkWidget *invoice_image = gtk_image_new_from_file("imgs/invoice_icon.svg");
    gtk_image_set_pixel_size(GTK_IMAGE(invoice_image), 50);
    gtk_widget_set_halign(GTK_WIDGET(invoice_image), GTK_ALIGN_END);
    gtk_widget_set_valign(GTK_WIDGET(invoice_image), GTK_ALIGN_START);
    set_widget_css(GTK_WIDGET(invoice_image), "invoice_image",
        "image#invoice_image {"
        "   margin-top: 10px;"
        "   margin-right: 5px;}");
    gtk_overlay_add_overlay(GTK_OVERLAY(invoice_overlay), invoice_image);
    //-------------------------------------

    GtkWidget *invoice_label = gtk_label_new("Invoices");
    gtk_widget_set_halign(GTK_WIDGET(invoice_label), GTK_ALIGN_START);
    gtk_widget_set_valign(GTK_WIDGET(invoice_label), GTK_ALIGN_START);
    set_widget_css(GTK_WIDGET(invoice_label), "invoice_label",
        "label#invoice_label {margin-left: 15px; margin-top: 30px; font-size: 18px;}");
    gtk_overlay_add_overlay(GTK_OVERLAY(invoice_overlay), invoice_label);
    //-------------------------------------

    GtkWidget *invoice_label_2 = gtk_label_new("Manage invoices, view invoice\nreports across workspaces.");
    gtk_widget_set_halign(GTK_WIDGET(invoice_label_2), GTK_ALIGN_START);
    gtk_widget_set_valign(GTK_WIDGET(invoice_label_2), GTK_ALIGN_START);
    set_widget_css(GTK_WIDGET(invoice_label_2), "invoice_label_2",
        "label#invoice_label_2 {margin-left: 15px; margin-top: 60px;}");
    gtk_overlay_add_overlay(GTK_OVERLAY(invoice_overlay), invoice_label_2);
    //-------------------------------------

    GtkWidget *part_of_design_4 = gtk_image_new_from_file("imgs/part_of_design_4.svg");
    gtk_image_set_pixel_size(GTK_IMAGE(part_of_design_4), 50);
    gtk_widget_set_halign(GTK_WIDGET(part_of_design_4), GTK_ALIGN_END);
    gtk_widget_set_valign(GTK_WIDGET(part_of_design_4), GTK_ALIGN_END);
    gtk_overlay_add_overlay(GTK_OVERLAY(invoice_overlay), part_of_design_4);

    //-------------------------------------
    gtk_button_set_child(GTK_BUTTON(invoice_btn), invoice_overlay);

    //------------------------------------
    //------------------------------------
    //------------------------------------

    GObject *logout_btn = gtk_builder_get_object(builder, "logout_btn");
    if (logout_btn == NULL) {
        g_printerr("Failed to get 'btn_logout' from builder UI\n");
    }

    set_widget_css(GTK_WIDGET(logout_btn), "logout_btn",
        "button#logout_btn {"
        "   min-width: 70px;"
        "   min-height: 50px;"
        "   font-size: 14px;"
        "   padding: 0px;"
        "   border: 0px;"
        "   background: linear-gradient(45deg, rgba(255,255,255,1) 0%, rgba(255,255,255,1) 100%);"
        "   transition: background 0.3s;}"
        "#logout_btn:hover {"
        "   background: linear-gradient(45deg, rgba(255,160,160,0.9) 0%, rgba(255,255,255,1) 100%);"
        "   transition: background 0.3s;}");

    set_patient_win();
    set_rendezvous_win();
    set_consult_win();
    set_payment_win();

    g_signal_connect(patient_btn, "clicked", G_CALLBACK(switch_to_window), GINT_TO_POINTER(ADMIN_PATIENT_INFO));
    g_signal_connect(rdv_btn, "clicked", G_CALLBACK(switch_to_window), GINT_TO_POINTER(ADMIN_RENDEZVOUS_INFO));
    g_signal_connect(consult_btn, "clicked", G_CALLBACK(switch_to_window), GINT_TO_POINTER(ADMIN_CONSULT_INFO));
    g_signal_connect(invoice_btn, "clicked", G_CALLBACK(switch_to_window), GINT_TO_POINTER(ADMIN_INVOICE_INFO));
    g_signal_connect(logout_btn, "clicked", G_CALLBACK(switch_to_window), GINT_TO_POINTER(LOGIN));
}

// set user menu windows objects
void set_user_info_objs() {

    GtkBuilder *builder = get_builder_instance();
    GObject **wins = get_windows_instance();

    const char *cin = g_object_get_data(G_OBJECT(wins[USER_MENU]), "UserID");
    if (cin == NULL) {
        g_printerr("Error getting user ID\n");
        return;
    }

    Patient *patient = getPatient(cin);

    if (patient == NULL) {
        g_printerr("Error getting patient\n");
        return;
    }
    GObject *grid = gtk_builder_get_object(builder, "user_info_grid");

    set_widget_css(GTK_WIDGET(grid), "user_info_grid", "grid#user_info_grid{margin: 20px;}");

    // User cin labels
    GObject *user_cin_title_label = gtk_builder_get_object(builder, "user_cin_title_label");
    set_widget_css(GTK_WIDGET(user_cin_title_label), "user_info_label", "label#user_info_label{font-size: 28px;}");

    GObject *user_cin_value_label = gtk_builder_get_object(builder, "user_cin_value_label");
    gtk_label_set_text(GTK_LABEL(user_cin_value_label), cin);
    set_widget_css(GTK_WIDGET(user_cin_value_label), "user_info_label", "label#user_info_label{font-size: 28px;}");

    // User last name labels
    GObject *user_last_name_title_label = gtk_builder_get_object(builder, "user_last_name_title_label");
    set_widget_css(GTK_WIDGET(user_last_name_title_label), "user_info_label", "label#user_info_label{font-size: 28px;}");

    GObject *user_last_name_value_label = gtk_builder_get_object(builder, "user_last_name_value_label");
    gtk_label_set_text(GTK_LABEL(user_last_name_value_label), patient->lName);
    set_widget_css(GTK_WIDGET(user_last_name_value_label), "user_info_label", "label#user_info_label{font-size: 28px;}");

    // User first name labels
    GObject *user_first_name_title_label = gtk_builder_get_object(builder, "user_first_name_title_label");
    set_widget_css(GTK_WIDGET(user_first_name_title_label), "user_info_label", "label#user_info_label{font-size: 28px;}");

    GObject *user_first_name_value_label = gtk_builder_get_object(builder, "user_first_name_value_label");
    gtk_label_set_text(GTK_LABEL(user_first_name_value_label), patient->fName);
    set_widget_css(GTK_WIDGET(user_first_name_value_label), "user_info_label", "label#user_info_label{font-size: 28px;}");

    // User age labels
    GObject *user_age_title_label = gtk_builder_get_object(builder, "user_age_title_label");
    set_widget_css(GTK_WIDGET(user_age_title_label), "user_info_label", "label#user_info_label{font-size: 28px;}");

    GObject *user_age_value_label = gtk_builder_get_object(builder, "user_age_value_label");
    char age_text[4];
    sprintf(age_text, "%d", patient->age);
    gtk_label_set_text(GTK_LABEL(user_age_value_label), age_text);
    set_widget_css(GTK_WIDGET(user_age_value_label), "user_info_label", "label#user_info_label{font-size: 28px;}");

    // User address labels
    GObject *user_address_title_label = gtk_builder_get_object(builder, "user_address_title_label");
    set_widget_css(GTK_WIDGET(user_address_title_label), "user_info_label", "label#user_info_label{font-size: 28px;}");

    GObject *user_address_value_label = gtk_builder_get_object(builder, "user_address_value_label");
    gtk_label_set_text(GTK_LABEL(user_address_value_label), patient->address);
    set_widget_css(GTK_WIDGET(user_address_value_label), "user_info_label", "label#user_info_label{font-size: 28px;}");

    // User phone labels
    GObject *user_phone_title_label = gtk_builder_get_object(builder, "user_phone_title_label");
    set_widget_css(GTK_WIDGET(user_phone_title_label), "user_info_label", "label#user_info_label{font-size: 28px;}");

    GObject *user_phone_value_label = gtk_builder_get_object(builder, "user_phone_value_label");
    gtk_label_set_text(GTK_LABEL(user_phone_value_label), patient->phone);
    set_widget_css(GTK_WIDGET(user_phone_value_label), "user_info_label", "label#user_info_label{font-size: 28px;}");

    GObject *user_return_pt_btn = gtk_builder_get_object(builder, "user_info_back_btn");

    g_signal_connect(user_return_pt_btn, "clicked", G_CALLBACK(switch_to_window), GINT_TO_POINTER(USER_MENU));
}

// set user rendezvous windows objects
void set_user_rendezvous_win() {
    GtkBuilder *builder = get_builder_instance();
    GObject **wins = get_windows_instance();

    GtkWindow *user_rendezvous_window = GTK_WINDOW(gtk_builder_get_object(builder, "user_rendezvous_window"));
    if (!user_rendezvous_window) {
        g_printerr("Failed to get 'user rendezvous window' from builder UI file\n");
        return;
    }
    gtk_window_maximize(user_rendezvous_window);

    GtkScrolledWindow *scrolled_window = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder, "user_rendezvous_scrolled_window"));

    GtkGrid *grid = GTK_GRID(gtk_builder_get_object(builder, "user_rendezvous_grid"));
    gtk_widget_set_halign(GTK_WIDGET(grid), GTK_ALIGN_CENTER);
    gtk_widget_set_valign(GTK_WIDGET(grid), GTK_ALIGN_START);

    set_widget_css(GTK_WIDGET(grid), "user_rendezvous_grid",
    "grid#user_rendezvous_grid {"
        "   min-width: 800px;"
        "   min-height: 600px;"
        "   margin-top: 50px;"
        "   margin-bottom: 50px;"
    "}");

    GtkWidget *empty_label = gtk_label_new("");
    set_widget_css(GTK_WIDGET(empty_label), "empty_label",
    "label#empty_label {"
        "   margin-top: 50px;"
    "}");

    gtk_grid_attach(grid, empty_label, 0, 1, 1, 1);

    // Create and style buttons
    GObject *reload_btn = gtk_builder_get_object(builder, "user_rendezvous_reload_btn");
    gtk_widget_set_size_request(GTK_WIDGET(reload_btn), 80, 80);
    set_widget_css(GTK_WIDGET(reload_btn), "btn",
    "button#btn {"
        "   font-size: 18px;"
        "   border: none;"
        "   border-radius: 100px;"
        "   background: transparent;"
        "   box-shadow: none;"
        "   transition: background 0.3s;"
    "}");

    GtkWidget *reload_image = gtk_image_new_from_file("imgs/reload_2.svg");
    gtk_image_set_pixel_size(GTK_IMAGE(reload_image), 50);
    set_widget_css(GTK_WIDGET(reload_image), "reload_img",
    "image#reload_img {"
        "   transition: transform 0.3s;"
    "}"
    "image#reload_img:hover {"
        "   transform: scale(1.1);"
        "   transition: transform 0.3s;"
    "}");
    gtk_button_set_child(GTK_BUTTON(reload_btn), reload_image);

    GObject *add_btn = gtk_builder_get_object(builder, "user_rendezvous_add_btn");
    gtk_widget_set_size_request(GTK_WIDGET(add_btn), 80, 80);
    set_widget_css(GTK_WIDGET(add_btn), "btn",
    "button#btn {"
        "   font-size: 18px;"
        "   border: none;"
        "   border-radius: 100px;"
        "   background: transparent;"
        "   box-shadow: none;"
        "   transition: background 0.3s;"
    "}");

    GtkWidget *add_image = gtk_image_new_from_file("imgs/add_2.svg");
    gtk_image_set_pixel_size(GTK_IMAGE(add_image), 50);
    set_widget_css(GTK_WIDGET(add_image), "add_img",
    "image#add_img {"
        "   transition: transform 0.3s;"
    "}"
    "image#add_img:hover {"
        "   transform: scale(1.1);"
        "   transition: transform 0.3s;"
    "}");
    gtk_button_set_child(GTK_BUTTON(add_btn), add_image);

    GObject *return_btn = gtk_builder_get_object(builder, "user_rendezvous_return_btn");
    gtk_widget_set_halign(GTK_WIDGET(return_btn), GTK_ALIGN_START);
    set_widget_css(GTK_WIDGET(return_btn), "btn",
    "button#btn {"
        "   font-size: 18px;"
        "   border-left: 5px solid #c6c5ea;"
        "   border-right: none;"
        "   border-top: none;"
        "   border-bottom: none;"
        "   border-radius: 0px;"
        "   padding: 0px 0px 0px 20px;"
        "   background: transparent;"
        "   box-shadow: none;"
    "}");

    GtkWidget *return_image = gtk_image_new_from_file("imgs/back_arrow_2.svg");
    gtk_image_set_pixel_size(GTK_IMAGE(return_image), 50);
    set_widget_css(GTK_WIDGET(return_image), "back_arrow",
    "image#back_arrow {"
        "   transition: transform 0.3s;"
    "}"
    "image#back_arrow:hover {"
        "   transform: translateX(-10px);"
        "   transition: transform 0.3s;"
    "}");
    gtk_button_set_child(GTK_BUTTON(return_btn), return_image);

    // Fetch User ID from window data
    char *cin = g_object_get_data(G_OBJECT(wins[USER_MENU]), "UserID");

    // Update display with user rendezvous data
    g_update_user_rdv_display(NULL, grid);

    // Connect signals to callbacks
    g_signal_connect(reload_btn, "clicked", G_CALLBACK(g_update_user_rdv_display), grid);
    g_signal_connect(add_btn, "clicked", G_CALLBACK(request_rdv_popup), cin);
    g_signal_connect(return_btn, "clicked", G_CALLBACK(switch_to_window), GINT_TO_POINTER(USER_MENU));

    // Attach grid to scrolled window and window
    gtk_scrolled_window_set_child(scrolled_window, GTK_WIDGET(grid));
    gtk_window_set_child(user_rendezvous_window, GTK_WIDGET(scrolled_window));
}

// set user consultation windows objects
void set_user_consult_win() {
    GtkBuilder *builder = get_builder_instance();
    GObject **wins = get_windows_instance();

    GtkWindow *user_consult_window = GTK_WINDOW(gtk_builder_get_object(builder, "user_consult_window"));
    if (!user_consult_window) {
        g_printerr("Failed to get 'user_consult_window' from builder UI file\n");
        return;
    }
    gtk_window_maximize(user_consult_window);

    GtkScrolledWindow *scrolled_window = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder, "user_consult_scrolled_window"));

    GtkGrid *grid = GTK_GRID(gtk_builder_get_object(builder, "user_consult_grid"));
    gtk_widget_set_halign(GTK_WIDGET(grid), GTK_ALIGN_CENTER);
    gtk_widget_set_valign(GTK_WIDGET(grid), GTK_ALIGN_START);

    // Apply CSS styling to the grid
    set_widget_css(GTK_WIDGET(grid), "user_consult_grid",
    "grid#user_consult_grid {"
        "   min-width: 800px;"
        "   margin-top: 50px;"
        "   margin-bottom: 50px;"
    "}");

    GtkWidget *empty_label = gtk_label_new("");
    set_widget_css(GTK_WIDGET(empty_label), "empty_label",
    "label#empty_label {"
        "   margin-top: 50px;"
    "}");

    gtk_grid_attach(grid, empty_label, 0, 1, 1, 1);

    // Create and style buttons
    GObject *reload_btn = gtk_builder_get_object(builder, "user_consult_reload_btn");
    gtk_widget_set_size_request(GTK_WIDGET(reload_btn), 80, 80);
    set_widget_css(GTK_WIDGET(reload_btn), "btn",
    "button#btn {"
        "   font-size: 18px;"
        "   border: none;"
        "   border-radius: 100px;"
        "   background: transparent;"
        "   box-shadow: none;"
        "   transition: background 0.3s;"
    "}");

    GtkWidget *reload_image = gtk_image_new_from_file("imgs/reload_3.svg");
    gtk_image_set_pixel_size(GTK_IMAGE(reload_image), 50);
    set_widget_css(GTK_WIDGET(reload_image), "reload_img",
    "image#reload_img {"
        "   transition: transform 0.3s;"
    "} image#reload_img:hover {"
        "   transform: scale(1.1);"
        "   transition: transform 0.3s;"
    "}");
    gtk_button_set_child(GTK_BUTTON(reload_btn), reload_image);

    GObject *return_btn = gtk_builder_get_object(builder, "user_consult_return_btn");
    gtk_widget_set_halign(GTK_WIDGET(return_btn), GTK_ALIGN_START);
    set_widget_css(GTK_WIDGET(return_btn), "btn",
    "button#btn {"
        "   font-size: 18px;"
        "   border-left: 5px solid #b5e5dc;"
        "   border-right: none;"
        "   border-top: none;"
        "   border-bottom: none;"
        "   border-radius: 0px;"
        "   padding: 0px 0px 0px 20px;"
        "   background: transparent;"
        "   box-shadow: none;"
    "}");

    GtkWidget *return_image = gtk_image_new_from_file("imgs/back_arrow_3.svg");
    gtk_image_set_pixel_size(GTK_IMAGE(return_image), 50);
    set_widget_css(GTK_WIDGET(return_image), "back_arrow",
    "image#back_arrow {"
        "   transition: transform 0.3s;"
    "} image#back_arrow:hover {"
        "   transform: translateX(-10px);"
        "   transition: transform 0.3s;"
    "}");
    gtk_button_set_child(GTK_BUTTON(return_btn), return_image);

    // Fetch User ID from window data
    char *cin = g_object_get_data(G_OBJECT(wins[USER_MENU]), "UserID");
    printf("cin : %s", cin);

    // Update display with user consult data
    g_update_user_consult_display(NULL, grid);

    // Connect signals to callbacks
    g_signal_connect(reload_btn, "clicked", G_CALLBACK(g_update_user_consult_display), grid);
    g_signal_connect(return_btn, "clicked", G_CALLBACK(switch_to_window), GINT_TO_POINTER(USER_MENU));

    // Attach grid to scrolled window and window
    gtk_scrolled_window_set_child(scrolled_window, GTK_WIDGET(grid));
    gtk_window_set_child(user_consult_window, GTK_WIDGET(scrolled_window));
}

// set user facture windows objects
void set_user_facture_win() {

    GtkBuilder* builder = get_builder_instance();

    GtkWindow *user_invoice_window = GTK_WINDOW(gtk_builder_get_object(builder, "user_invoice_window"));
    if (!user_invoice_window) {
        g_printerr("Failed to get 'user invoice window' from builder UI file\n");
        return;
    }
    gtk_window_maximize(user_invoice_window);

    GtkScrolledWindow *scrolled_window = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder, "user_invoice_scrolled_window"));

    GtkGrid *grid = GTK_GRID(gtk_builder_get_object(builder, "user_invoice_grid"));
    gtk_widget_set_halign(GTK_WIDGET(grid), GTK_ALIGN_CENTER);
    gtk_widget_set_valign(GTK_WIDGET(grid), GTK_ALIGN_START);

    set_widget_css(GTK_WIDGET(grid), "user_rendezvous_grid",
    "grid#user_rendezvous_grid {"
        "   min-width: 800px;"
        "   min-height: 600px;"
        "   margin-top: 50px;"
        "   margin-bottom: 50px;"
    "}");

    GtkWidget *empty_label = gtk_label_new("");
    set_widget_css(GTK_WIDGET(empty_label), "empty_label",
    "label#empty_label {"
        "   margin-top: 50px;"
    "}");

    gtk_grid_attach(grid, empty_label, 0, 1, 1, 1);

    // Create and style buttons
    GObject *reload_btn = gtk_builder_get_object(builder, "user_invoice_reload_btn");
    gtk_widget_set_size_request(GTK_WIDGET(reload_btn), 80, 80);
    set_widget_css(GTK_WIDGET(reload_btn), "btn",
    "button#btn {"
        "   font-size: 18px;"
        "   border: none;"
        "   border-radius: 100px;"
        "   background: transparent;"
        "   box-shadow: none;"
        "   transition: background 0.3s;"
    "}");

    GtkWidget *reload_image = gtk_image_new_from_file("imgs/reload_4.svg");
    gtk_image_set_pixel_size(GTK_IMAGE(reload_image), 50);
    set_widget_css(GTK_WIDGET(reload_image), "reload_img",
    "image#reload_img {"
        "   transition: transform 0.3s;"
    "}"
    "image#reload_img:hover {"
        "   transform: scale(1.1);"
        "   transition: transform 0.3s;"
    "}");
    gtk_button_set_child(GTK_BUTTON(reload_btn), reload_image);
    GObject *return_btn = gtk_builder_get_object(builder, "user_invoice_return_btn");
    gtk_widget_set_halign(GTK_WIDGET(return_btn), GTK_ALIGN_START);
    set_widget_css(GTK_WIDGET(return_btn), "btn",
    "button#btn {"
        "   font-size: 18px;"
        "   border-left: 5px solid #e8d1c1;"
        "   border-right: none;"
        "   border-top: none;"
        "   border-bottom: none;"
        "   border-radius: 0px;"
        "   padding: 0px 0px 0px 20px;"
        "   background: transparent;"
        "   box-shadow: none;"
    "}");

    GtkWidget *return_image = gtk_image_new_from_file("imgs/back_arrow_4.svg");
    gtk_image_set_pixel_size(GTK_IMAGE(return_image), 50);
    set_widget_css(GTK_WIDGET(return_image), "back_arrow",
    "image#back_arrow {"
        "   transition: transform 0.3s;"
    "}"
    "image#back_arrow:hover {"
        "   transform: translateX(-10px);"
        "   transition: transform 0.3s;"
    "}");
    gtk_button_set_child(GTK_BUTTON(return_btn), return_image);

    g_update_user_bill_display(NULL, grid);

    // Connect button signals to their respective callbacks
    g_signal_connect(reload_btn, "clicked", G_CALLBACK(g_update_user_bill_display), grid);
    g_signal_connect(return_btn, "clicked", G_CALLBACK(switch_to_window), GINT_TO_POINTER(USER_MENU));

    // Attach grid to scrolled window and window
    gtk_scrolled_window_set_child(scrolled_window, GTK_WIDGET(grid));
    gtk_window_set_child(user_invoice_window, GTK_WIDGET(scrolled_window));
}

// set user menu windows objects
void set_user_menu_objs() {

    GtkBuilder* builder = get_builder_instance();

    GObject *user_menu_window = gtk_builder_get_object(builder, "user_menu_window");
    if (!user_menu_window) {
        g_printerr("Failed to get 'user_menu_window' from builder UI file\n");
    }
    gtk_window_maximize(GTK_WINDOW(user_menu_window));
    set_widget_css(GTK_WIDGET(user_menu_window), "menu_window", "window#menu_window { background: #F2FAFD;}");
    //-------------------------------------

    GObject *menu_grid = gtk_builder_get_object(builder, "user_menu_grid");
    set_widget_css(GTK_WIDGET(menu_grid), "menu_grid",
    "grid#menu_grid {"
        "   min-width: 800px;"
        "}");
    gtk_widget_set_valign(GTK_WIDGET(menu_grid), GTK_ALIGN_CENTER);
    gtk_widget_set_halign(GTK_WIDGET(menu_grid), GTK_ALIGN_CENTER);
    //-------------------------------------
    //----------Patient section
    //-------------------------------------

    GObject *patient_btn = gtk_builder_get_object(builder, "user_info_btn");

    gtk_widget_set_vexpand_set(GTK_WIDGET(patient_btn), TRUE);
    if (patient_btn == NULL) {
        g_printerr("Failed to get 'btn_patient' from builder UI\n");
        return;
    }
    set_widget_css(GTK_WIDGET(patient_btn), "patient_btn",
    "button#patient_btn {"
        "   min-width: 400px;"
        "   min-height: 200px;"
        "   padding: 0px;"
        "   border: 0px;"
        "   border-radius: 10px;"
        "   background: linear-gradient(45deg, rgba(255,255,255,1) 0%, rgba(255,255,255,1) 50%);"
        "   transition: transform 0.3s, background 0.3s;}"
    "#patient_btn:hover {"
        "   background: linear-gradient(45deg, rgba(249,212,228,0.9) 0%, rgba(255,255,255,1) 50%);"
        "   transform: scale(1.05);"
        "   transition: transform 0.3s, background 0.3s;}");
    //-------------------------------------

     GtkWidget *patient_overlay = gtk_overlay_new();
     set_widget_css(GTK_WIDGET(patient_overlay), "patient_overlay","widget#patient_overlay {background: #000;}");
     //-------------------------------------

     GtkWidget *patient_image = gtk_image_new_from_file("imgs/patient_icon.svg");
     gtk_image_set_pixel_size(GTK_IMAGE(patient_image), 50);
     gtk_widget_set_halign(GTK_WIDGET(patient_image), GTK_ALIGN_END);
     gtk_widget_set_valign(GTK_WIDGET(patient_image), GTK_ALIGN_START);
     set_widget_css(GTK_WIDGET(patient_image), "patient_image","image#patient_image {margin-top: 10px; margin-right: 5px;}");
     gtk_overlay_add_overlay(GTK_OVERLAY(patient_overlay), patient_image);
     //-------------------------------------

     GtkWidget *patient_label = gtk_label_new("My information");
     gtk_widget_set_halign(GTK_WIDGET(patient_label), GTK_ALIGN_CENTER);
     gtk_widget_set_valign(GTK_WIDGET(patient_label), GTK_ALIGN_CENTER);
     set_widget_css(GTK_WIDGET(patient_label), "patient_label","label#patient_label {font-size: 24px;}");
     gtk_overlay_add_overlay(GTK_OVERLAY(patient_overlay), patient_label);

     //-------------------------------------
     GtkWidget *part_of_design_1 = gtk_image_new_from_file("imgs/part_of_design_1.svg");
     gtk_image_set_pixel_size(GTK_IMAGE(part_of_design_1), 50);
     gtk_widget_set_halign(GTK_WIDGET(part_of_design_1), GTK_ALIGN_END);
     gtk_widget_set_valign(GTK_WIDGET(part_of_design_1), GTK_ALIGN_END);
     gtk_overlay_add_overlay(GTK_OVERLAY(patient_overlay), part_of_design_1);

     //-------------------------------------
     gtk_button_set_child(GTK_BUTTON(patient_btn), patient_overlay);

    // -------------------------------------
    // ----------Rendezvous section
    // -------------------------------------

     GObject *rdv_btn = gtk_builder_get_object(builder, "user_rdv_btn");

     gtk_widget_set_vexpand_set(GTK_WIDGET(rdv_btn), TRUE);
     if (rdv_btn == NULL) {
         g_printerr("Failed to get 'btn_rdv' from builder UI\n");
     }
     set_widget_css(GTK_WIDGET(rdv_btn), "rdv_btn",
         "button#rdv_btn {"
         "   min-width: 400px;"
         "   min-height: 200px;"
         "   padding: 0px;"
         "   border: 0px;"
         "   border-radius: 10px;"
         "   background: linear-gradient(45deg, rgba(255,255,255,1) 0%, rgba(255,255,255,1) 50%);"
         "   transition: transform 0.3s, background 0.3s;}"
         "#rdv_btn:hover {"
         "   background: linear-gradient(45deg, rgba(216,213,247,0.9) 0%, rgba(255,255,255,1) 50%);"
         "   transform: scale(1.05);"
         "   transition: transform 0.3s, background 0.3s;}");
     //-------------------------------------

     GtkWidget *rdv_overlay = gtk_overlay_new();
     set_widget_css(GTK_WIDGET(rdv_overlay), "rdv_overlay","widget#rdv_overlay {background: #000;}");
     //-------------------------------------

     GtkWidget *rdv_image = gtk_image_new_from_file("imgs/rendezvous_icon.svg");
     gtk_image_set_pixel_size(GTK_IMAGE(rdv_image), 50);
     gtk_widget_set_halign(GTK_WIDGET(rdv_image), GTK_ALIGN_END);
     gtk_widget_set_valign(GTK_WIDGET(rdv_image), GTK_ALIGN_START);
     set_widget_css(GTK_WIDGET(rdv_image), "rdv_image",
         "image#rdv_image {"
         "   margin-top: 10px;"
         "   margin-right: 5px;}");
     gtk_overlay_add_overlay(GTK_OVERLAY(rdv_overlay), rdv_image);
     //-------------------------------------

     GtkWidget *rdv_label = gtk_label_new("My Rendezvous");
     gtk_widget_set_halign(GTK_WIDGET(rdv_label), GTK_ALIGN_CENTER);
     gtk_widget_set_valign(GTK_WIDGET(rdv_label), GTK_ALIGN_CENTER);
     set_widget_css(GTK_WIDGET(rdv_label), "rdv_label","label#rdv_label {font-size: 24px;}");
     gtk_overlay_add_overlay(GTK_OVERLAY(rdv_overlay), rdv_label);

     //-------------------------------------
     GtkWidget *part_of_design_2 = gtk_image_new_from_file("imgs/part_of_design_2.svg");
     gtk_image_set_pixel_size(GTK_IMAGE(part_of_design_2), 50);
     gtk_widget_set_halign(GTK_WIDGET(part_of_design_2), GTK_ALIGN_END);
     gtk_widget_set_valign(GTK_WIDGET(part_of_design_2), GTK_ALIGN_END);
     gtk_overlay_add_overlay(GTK_OVERLAY(rdv_overlay), part_of_design_2);

     //-------------------------------------
     gtk_button_set_child(GTK_BUTTON(rdv_btn), rdv_overlay);

     //-------------------------------------
     //----------consultation section
     //-------------------------------------

     GObject *consult_btn = gtk_builder_get_object(builder, "user_consult_btn");

     gtk_widget_set_vexpand_set(GTK_WIDGET(consult_btn), TRUE);
     if (consult_btn == NULL) {
         g_printerr("Failed to get 'consult_btn' from builder UI\n");
     }
     set_widget_css(GTK_WIDGET(consult_btn), "consult_btn",
         "button#consult_btn {"
         "   min-width: 400px;"
         "   min-height: 200px;"
         "   padding: 0px;"
         "   border: 0px;"
         "   border-radius: 10px;"
         "   background: linear-gradient(45deg, rgba(255,255,255,1) 0%, rgba(255,255,255,1) 50%);"
         "   transition: transform 0.3s, background 0.3s;}"
         "#consult_btn:hover {"
         "   background: linear-gradient(45deg, rgba(212,244,238,0.9) 0%, rgba(255,255,255,1) 50%);"
         "   transform: scale(1.05);"
         "   transition: transform 0.3s, background 0.3s;}");
     //-------------------------------------

     GtkWidget *consult_overlay = gtk_overlay_new();
     set_widget_css(GTK_WIDGET(consult_overlay), "consult_overlay","widget#consult_overlay {background: #000;}");
     //-------------------------------------

     GtkWidget *consult_image = gtk_image_new_from_file("imgs/consultation_icon.svg");
     gtk_image_set_pixel_size(GTK_IMAGE(consult_image), 50);
     gtk_widget_set_halign(GTK_WIDGET(consult_image), GTK_ALIGN_END);
     gtk_widget_set_valign(GTK_WIDGET(consult_image), GTK_ALIGN_START);
     set_widget_css(GTK_WIDGET(consult_image), "consult_image",
         "image#consult_image {"
         "   margin-top: 10px;"
         "   margin-right: 5px;}");
     gtk_overlay_add_overlay(GTK_OVERLAY(consult_overlay), consult_image);
     //-------------------------------------

     GtkWidget *consult_label = gtk_label_new("My Consultations");
     gtk_widget_set_halign(GTK_WIDGET(consult_label), GTK_ALIGN_CENTER);
     gtk_widget_set_valign(GTK_WIDGET(consult_label), GTK_ALIGN_CENTER);
     set_widget_css(GTK_WIDGET(consult_label), "consult_label","label#consult_label {font-size: 24px;}");
     gtk_overlay_add_overlay(GTK_OVERLAY(consult_overlay), consult_label);

     //-------------------------------------
     GtkWidget *part_of_design_3 = gtk_image_new_from_file("imgs/part_of_design_3.svg");
     gtk_image_set_pixel_size(GTK_IMAGE(part_of_design_3), 50);
     gtk_widget_set_halign(GTK_WIDGET(part_of_design_3), GTK_ALIGN_END);
     gtk_widget_set_valign(GTK_WIDGET(part_of_design_3), GTK_ALIGN_END);
     gtk_overlay_add_overlay(GTK_OVERLAY(consult_overlay), part_of_design_3);

     //-------------------------------------
     gtk_button_set_child(GTK_BUTTON(consult_btn), consult_overlay);

     //-------------------------------------
     //----------Invoice section
     //-------------------------------------

     GObject *invoice_btn = gtk_builder_get_object(builder, "user_payment_btn");

     gtk_widget_set_vexpand_set(GTK_WIDGET(invoice_btn), TRUE);
     if (invoice_btn == NULL) {
         g_printerr("Failed to get 'invoice_btn' from builder UI\n");
     }
     set_widget_css(GTK_WIDGET(invoice_btn), "invoice_btn",
         "button#invoice_btn {"
         "   min-width: 400px;"
         "   min-height: 200px;"
         "   padding: 0px;"
         "   border: 0px;"
         "   border-radius: 10px;"
         "   background: linear-gradient(45deg, rgba(255,255,255,1) 0%, rgba(255,255,255,1) 50%);"
         "   transition: transform 0.3s, background 0.3s;}"
         "#invoice_btn:hover {"
         "   background: linear-gradient(45deg, rgba(242,227,214,0.9) 0%, rgba(255,255,255,1) 50%);"
         "   transform: scale(1.05);"
         "   transition: transform 0.3s, background 0.3s;}");
     //-------------------------------------

     GtkWidget *invoice_overlay = gtk_overlay_new();
     //-------------------------------------

     GtkWidget *invoice_image = gtk_image_new_from_file("imgs/invoice_icon.svg");
     gtk_image_set_pixel_size(GTK_IMAGE(invoice_image), 50);
     gtk_widget_set_halign(GTK_WIDGET(invoice_image), GTK_ALIGN_END);
     gtk_widget_set_valign(GTK_WIDGET(invoice_image), GTK_ALIGN_START);
     set_widget_css(GTK_WIDGET(invoice_image), "invoice_image",
         "image#invoice_image {"
         "   margin-top: 10px;"
         "   margin-right: 5px;}");
     gtk_overlay_add_overlay(GTK_OVERLAY(invoice_overlay), invoice_image);
     //-------------------------------------

     GtkWidget *invoice_label = gtk_label_new("Invoices");
     gtk_widget_set_halign(GTK_WIDGET(invoice_label), GTK_ALIGN_CENTER);
     gtk_widget_set_valign(GTK_WIDGET(invoice_label), GTK_ALIGN_CENTER);
     set_widget_css(GTK_WIDGET(invoice_label), "invoice_label",
         "label#invoice_label {font-size: 24px;}");
     gtk_overlay_add_overlay(GTK_OVERLAY(invoice_overlay), invoice_label);

     //-------------------------------------

     GtkWidget *part_of_design_4 = gtk_image_new_from_file("imgs/part_of_design_4.svg");
     gtk_image_set_pixel_size(GTK_IMAGE(part_of_design_4), 50);
     gtk_widget_set_halign(GTK_WIDGET(part_of_design_4), GTK_ALIGN_END);
     gtk_widget_set_valign(GTK_WIDGET(part_of_design_4), GTK_ALIGN_END);
     gtk_overlay_add_overlay(GTK_OVERLAY(invoice_overlay), part_of_design_4);

     //-------------------------------------
     gtk_button_set_child(GTK_BUTTON(invoice_btn), invoice_overlay);

    GObject *logout_btn = gtk_builder_get_object(builder, "user_out_btn");

    set_widget_css(GTK_WIDGET(logout_btn), "logout_btn",
        "button#logout_btn {"
        "   min-width: 70px;"
        "   min-height: 50px;"
        "   font-size: 14px;"
        "   padding: 0px;"
        "   border: 0px;"
        "   background: linear-gradient(45deg, rgba(255,255,255,1) 0%, rgba(255,255,255,1) 100%);"
        "   transition: background 0.3s;}"
        "#logout_btn:hover {"
        "   background: linear-gradient(45deg, rgba(255,160,160,0.9) 0%, rgba(255,255,255,1) 100%);"
        "   transition: background 0.3s;}");

    set_user_info_objs();
    set_user_rendezvous_win();
    set_user_consult_win();
    set_user_facture_win();

    g_signal_connect(patient_btn, "clicked", G_CALLBACK(switch_to_popup_window), GINT_TO_POINTER(USER_INFO));
    g_signal_connect(rdv_btn, "clicked", G_CALLBACK(switch_to_window), GINT_TO_POINTER(USER_RENDEZVOUS_INFO));
    g_signal_connect(consult_btn, "clicked", G_CALLBACK(switch_to_window), GINT_TO_POINTER(USER_CONSULT_INFO));
    g_signal_connect(invoice_btn, "clicked", G_CALLBACK(switch_to_window), GINT_TO_POINTER(USER_INVOICE_INFO));
    g_signal_connect(logout_btn, "clicked", G_CALLBACK(switch_to_window), GINT_TO_POINTER(LOGIN));
}

void g_login(GtkButton *btn, gpointer data) {
    GObject **wins = get_windows_instance();
    GtkWidget **widgets = data;

    if (!widgets) {
        g_printerr("Error: objects is NULL\n");
        return;
    }
    GtkWidget *email_err_label = widgets[0];
    GtkWidget *email_entry = widgets[1];
    GtkWidget *password_err_label = widgets[2];
    GtkWidget *password_entry = widgets[3];

    const char *email = get_text_from_entry(GTK_WIDGET(email_entry));
    const char *password = get_text_from_entry(GTK_WIDGET(password_entry));


    if (!checkEmail(email)) {
       gtk_label_set_text(GTK_LABEL(email_err_label), "Invalid email address (must include '@', a valid domain, and no spaces).");
        return;
    }
    gtk_label_set_text(GTK_LABEL(email_err_label), "");

    if (!checkPassword(password)) {
        gtk_label_set_text(GTK_LABEL(password_err_label), "Invalid password (must contain at least 1 uppercase letter, 1 lowercase letter, 1 number, and 1 special character).");
        return;
    }
    gtk_label_set_text(GTK_LABEL(password_err_label), "");

    Login *user = getAccount(email, password);

    g_object_set_data(G_OBJECT(wins[USER_MENU]), "UserID", user->cin);

    if (user == NULL) {
        gtk_label_set_text(GTK_LABEL(email_err_label), "Invalid email address or password");
        return;
    }
    gtk_label_set_text(GTK_LABEL(email_err_label), "");
    set_text_entry(email_entry, "");
    set_text_entry(password_entry, "");

    switch (checkRole(user)) {
        case ADMIN:
            printf("Admin Successfully logged in\n");
            set_menu_win_objs();
            switch_to_window(btn, GINT_TO_POINTER(ADMIN_MENU));
            break;
        case USER:
            printf("User Successfully logged in\n");
            set_user_menu_objs();
            switch_to_window(btn, GINT_TO_POINTER(USER_MENU));
            break;
    }
}

// set login windows objects
void set_login_win_objs() {

    GtkBuilder *builder = get_builder_instance();

    GObject *main_window = gtk_builder_get_object(builder, "login_window");
    if (!main_window) {
        g_printerr("Failed to get 'main_window' from builder UI file\n");
    }
    gtk_window_maximize(GTK_WINDOW(main_window));
    set_widget_css(GTK_WIDGET(main_window), "login_window", "window#login_window { background: #F2FAFD;}");

    GObject *login_grid = gtk_builder_get_object(builder, "login_grid");
    set_widget_css(GTK_WIDGET(login_grid), "login_grid", "grid#login_grid { min-width: 400px; background: #F2FAFD;}");
    gtk_widget_set_hexpand(GTK_WIDGET(login_grid), TRUE);

    GObject *email_err_label = gtk_builder_get_object(builder, "login_email_err_label");
    if (!email_err_label) {
        g_printerr("Failed to get 'email_err_label' from builder UI file\n");
    }
    gtk_label_set_text(GTK_LABEL(email_err_label), "");
    set_widget_css(GTK_WIDGET(email_err_label), "email_err", "label#email_err{color: red;}");
    gtk_widget_set_hexpand(GTK_WIDGET(email_err_label), TRUE);

    GObject *email_entry = gtk_builder_get_object(builder, "login_email_entry");
    if (!email_entry) {
        g_printerr("Failed to get 'nameEntry' from builder UI file\n");
        return;
    }
    set_text_entry(GTK_WIDGET(email_entry), "");

    GObject *password_err_label = gtk_builder_get_object(builder, "login_password_err_label");
    if (!password_err_label) {
        g_printerr("Failed to get 'password_err_label' from builder UI file\n");
    }
    gtk_label_set_text(GTK_LABEL(password_err_label), "");
    set_widget_css(GTK_WIDGET(password_err_label), "password_err", "label#password_err{color: red;}");

    GObject *password_entry = gtk_builder_get_object(builder, "login_password_entry");
    if (!password_entry) {
        g_printerr("Failed to get 'passwordEntry' from builder UI file\n");
        return;
    }
    set_text_entry(GTK_WIDGET(password_entry), "");

    GObject *login_btn = gtk_builder_get_object(builder, "login_btn");
    if (!login_btn) {
        g_printerr("Failed to get 'login' button from builder UI file\n");
        return;
    }

    GObject *cancel_btn = gtk_builder_get_object(builder, "login_cancel_btn");
    if (!cancel_btn) {
        g_printerr("Failed to get 'cancel' button from builder UI file\n");
        return;
    }


    GtkWidget **widgets =  g_new(GtkWidget*, 4);
    widgets[0] = GTK_WIDGET(email_err_label);
    widgets[1] = GTK_WIDGET(email_entry);
    widgets[2] = GTK_WIDGET(password_err_label);
    widgets[3] = GTK_WIDGET(password_entry);

    g_signal_connect(login_btn, "clicked", G_CALLBACK(g_login), widgets); //<----- will depend on the role of the user
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(switch_to_window), GINT_TO_POINTER(MAIN));

}

// set register windows objects
void set_register_win_objs() {

    GtkBuilder *builder = get_builder_instance();

    GObject *register_window = gtk_builder_get_object(builder, "register_window");
    if (!register_window) {
        g_printerr("Failed to get 'main_window' from builder UI file\n");
    }
    gtk_window_maximize(GTK_WINDOW(register_window));
    set_widget_css(GTK_WIDGET(register_window), "main_window", "window#main_window { background: #F2FAFD;}");


    GObject *grid = gtk_builder_get_object(builder, "register_grid");
    gtk_widget_set_size_request(GTK_WIDGET(grid), 600, 800);

    GObject *register_cin_label = gtk_builder_get_object(builder, "register_cin_label");
    gtk_label_set_text(GTK_LABEL(register_cin_label), "");

    GObject *cin_entry = gtk_builder_get_object(builder, "register_cin_entry");
    set_text_entry(GTK_WIDGET(cin_entry), "");
    if (!cin_entry) {
        g_printerr("Failed to get 'RegisterCinEntry' from builder UI file\n");
        return;
    }

    GObject *register_first_name_label = gtk_builder_get_object(builder, "register_first_name_label");
    gtk_label_set_text(GTK_LABEL(register_first_name_label), "");

    GObject *first_name_entry = gtk_builder_get_object(builder, "register_first_name_entry");
    set_text_entry(GTK_WIDGET(first_name_entry), "");
    if (!first_name_entry) {
        g_printerr("Failed to get 'RegisterCinEntry' from builder UI file\n");
        return;
    }

    GObject *register_last_name_label = gtk_builder_get_object(builder, "register_last_name_label");
    gtk_label_set_text(GTK_LABEL(register_last_name_label), "");

    GObject *last_name_entry = gtk_builder_get_object(builder, "register_last_name_entry");
    set_text_entry(GTK_WIDGET(last_name_entry), "");
    if (!last_name_entry) {
        g_printerr("Failed to get 'RegisterLastNameEntry' from builder UI file\n");
        return;
    }

    GObject *register_age_label = gtk_builder_get_object(builder, "register_age_label");
    gtk_label_set_text(GTK_LABEL(register_age_label), "");

    GObject *age_entry = gtk_builder_get_object(builder, "register_age_entry");
    set_text_entry(GTK_WIDGET(age_entry), "");
    if (!age_entry) {
        g_printerr("Failed to get 'RegisterAgeEntry' from builder UI file\n");
        return;
    }

    GObject *register_address_label = gtk_builder_get_object(builder, "register_address_label");
    gtk_label_set_text(GTK_LABEL(register_address_label), "");

    GObject *address_entry = gtk_builder_get_object(builder, "register_address_entry");
    set_text_entry(GTK_WIDGET(address_entry), "");
    if (!address_entry) {
        g_printerr("Failed to get 'RegisterAgeEntry' from builder UI file\n");
        return;
    }

    GObject *register_phone_label = gtk_builder_get_object(builder, "register_phone_label");
    gtk_label_set_text(GTK_LABEL(register_phone_label), "");

    GObject *phone_entry = gtk_builder_get_object(builder, "register_phone_entry");
    set_text_entry(GTK_WIDGET(phone_entry), "");
    if (!phone_entry) {
        g_printerr("Failed to get 'RegisterAgeEntry' from builder UI file\n");
        return;
    }

    GObject *register_email_label = gtk_builder_get_object(builder, "register_email_label");
    gtk_label_set_text(GTK_LABEL(register_email_label), "");

    GObject *email_entry = gtk_builder_get_object(builder, "register_email_entry");
    set_text_entry(GTK_WIDGET(email_entry), "");
    if (!email_entry) {
        g_printerr("Failed to get 'RegisterAgeEntry' from builder UI file\n");
        return;
    }

    GObject *register_password_label = gtk_builder_get_object(builder, "register_password_label");
    gtk_label_set_text(GTK_LABEL(register_password_label), "");

    GObject *password_entry = gtk_builder_get_object(builder, "register_password_entry");
    set_text_entry(GTK_WIDGET(password_entry), "");
    if (!password_entry) {
        g_printerr("Failed to get 'RegisterEasswordEntry' from builder UI file\n");
        return;
    }

    GObject *register_confirm_password_label = gtk_builder_get_object(builder, "register_confirm_password_label");
    gtk_label_set_text(GTK_LABEL(register_confirm_password_label), "");

    GObject *confirm_password_entry = gtk_builder_get_object(builder, "confirm_password_entry");
    set_text_entry(GTK_WIDGET(confirm_password_entry), "");
    if (!confirm_password_entry) {
        g_printerr("Failed to get 'confirmPasswordEntry' from builder UI file\n");
        return;
    }

    GObject *button_register = gtk_builder_get_object(builder, "register");
    if (!button_register) {
        g_printerr("Failed to get 'login' button from builder UI file\n");
        return;
    }

    GObject *button_cancel = gtk_builder_get_object(builder, "register_cancel_btn");
    if (!button_cancel) {
        g_printerr("Failed to get 'cancel' button from builder UI file\n");
        return;
    }

    GtkWidget **widgets =  g_new(GtkWidget*, 8);
    widgets[0] = GTK_WIDGET(cin_entry);
    widgets[1] = GTK_WIDGET(first_name_entry);
    widgets[2] = GTK_WIDGET(last_name_entry);
    widgets[3] = GTK_WIDGET(age_entry);
    widgets[4] = GTK_WIDGET(address_entry);
    widgets[5] = GTK_WIDGET(phone_entry);
    widgets[6] = GTK_WIDGET(email_entry);
    widgets[7] = GTK_WIDGET(password_entry);
    widgets[8] = GTK_WIDGET(confirm_password_entry);


    g_signal_connect(button_register, "clicked", G_CALLBACK(g_register), widgets);
    g_signal_connect(button_cancel, "clicked", G_CALLBACK(switch_to_window), GINT_TO_POINTER(MAIN));
}

// set main windows objects
void set_main_win_objs() {

    GtkBuilder *builder = get_builder_instance();

    GObject *main_window = gtk_builder_get_object(builder, "main_window");
    if (!main_window) {
        g_printerr("Failed to get 'main_window' from builder UI file\n");
    }
    gtk_window_maximize(GTK_WINDOW(main_window));
    set_widget_css(GTK_WIDGET(main_window), "main_window", "window#main_window { background: #F2FAFD;}");

    GObject *main_grid = gtk_builder_get_object(builder, "main_grid");
    if (!main_grid) {
        g_printerr("Failed to get 'main_grid' from builder UI file\n");
    }

    GObject *main_title_label = gtk_builder_get_object(builder, "main_title_label");
    if (!main_title_label) {
        g_printerr("Failed to get 'main title' label from builder UI file\n");
        return;
    }
    set_widget_css(GTK_WIDGET(main_title_label), "main_title_label", "label#main_title_label { color: #9E999C; font-size: 128px;}");

    GObject *register_btn = gtk_builder_get_object(builder, "main_register_btn");
    if (!register_btn) {
        g_printerr("Failed to get 'Register' button from builder UI file\n");
        return;
    }
    set_widget_css(GTK_WIDGET(register_btn), "register_btn", "button#register_btn { color: #000; background: #AEDFE2; border: #AEDFE2; font-size: 16px; padding: 10px; }"
                                                              "#register_btn:hover {background: #85BDC8;}");

    GObject *login_btn = gtk_builder_get_object(builder, "main_login_btn");
    if (!login_btn) {
        g_printerr("Failed to get 'Login' button from builder UI file\n");
        return;
    }
    set_widget_css(GTK_WIDGET(login_btn), "login_btn", "button#login_btn { color: #000; background: #AEDFE2; border: #AEDFE2; font-size: 16px; padding: 10px;}"
                                                        "#login_btn:hover {background: #85BDC8;}");

    // set_register_win_objs();
    // set_login_win_objs();

    g_signal_connect(register_btn, "clicked", G_CALLBACK(switch_to_window), GINT_TO_POINTER(REGISTER));
    g_signal_connect(register_btn, "clicked", G_CALLBACK(set_register_win_objs), NULL);
    g_signal_connect(login_btn, "clicked", G_CALLBACK(switch_to_window), GINT_TO_POINTER(LOGIN));
    g_signal_connect(login_btn, "clicked", G_CALLBACK(set_login_win_objs), NULL);
}

//=====================================================================
// TESTING

//===================================================================//
void activate(GtkApplication *app, gpointer user_data) {

    GObject **wins = get_windows_instance();

    if (!wins) {
        g_printerr("Failed to initialize windows\n");
        return;
    }
    gtk_window_set_application(GTK_WINDOW(wins[MAIN]), app);
    gtk_window_set_application(GTK_WINDOW(wins[REGISTER]), app);
    gtk_window_set_application(GTK_WINDOW(wins[LOGIN]), app);
    gtk_window_set_application(GTK_WINDOW(wins[ADMIN_MENU]), app);
    gtk_window_set_application(GTK_WINDOW(wins[ADMIN_PATIENT_INFO]), app);
    gtk_window_set_application(GTK_WINDOW(wins[ADMIN_RENDEZVOUS_INFO]), app);
    gtk_window_set_application(GTK_WINDOW(wins[ADMIN_CONSULT_INFO]), app);
    gtk_window_set_application(GTK_WINDOW(wins[ADMIN_INVOICE_INFO]), app);
    gtk_window_set_application(GTK_WINDOW(wins[USER_MENU]), app);
    gtk_window_set_application(GTK_WINDOW(wins[USER_INFO]), app);
    gtk_window_set_application(GTK_WINDOW(wins[USER_RENDEZVOUS_INFO]), app);
    gtk_window_set_application(GTK_WINDOW(wins[USER_CONSULT_INFO]), app);
    gtk_window_set_application(GTK_WINDOW(wins[USER_INVOICE_INFO]), app);

    set_main_win_objs();

    gtk_widget_set_visible(GTK_WIDGET(wins[MAIN]), TRUE);
}

int main(int argc, char *argv[]) {
    #ifdef GTK_SRCDIR
    #endif
    GtkApplication *app = gtk_application_new("org.gtk.medicalOffice", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

