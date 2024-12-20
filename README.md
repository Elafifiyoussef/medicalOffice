# MedNova
MedNova is a comprehensive medical office management system designed to streamline operations for doctors and users. It simplifies scheduling, record management, and communication within a secure and user-friendly platform.

## Table of Contents
Features.
Installation.
Usage.
Screenshots.
Technologies Used.

## User Functionalities:
Appointment Management: Book, cancel, and modify appointments (subject to rules like changes before 23:59 on the previous day).
Record Access: View personal appointment details and updates from the medical office.
Admin Functionalities:

Patient Record Management: Create, update, and organize patient records securely.
Appointment Oversight: Manage and oversee all appointments, ensuring availability and smooth scheduling.
System Configuration: Set up user accounts and customize system settings.
Secure Platform: Data is handled with robust security to maintain confidentiality and integrity.

Streamlined Interface: Designed for ease of use for both medical staff and users.

## Installation
### Step 1: Install MSYS2
- Download the MSYS2 installer from msys2.org.
- Run the installer and follow the installation instructions.
- Open the MSYS2 terminal and update the package database and core system packages:
```bash
pacman -Syu
```
- Close and reopen the terminal, then update everything:
```bash
pacman -Syu
```
### Step 2: Install GTK and Required Tools
Install the necessary packages for GTK development:
- Install the GTK library:
```bash
pacman -S mingw-w64-x86_64-gtk4
```
- Install the MinGW toolchain and other essential tools:
```bash
pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake mingw-w64-x86_64-meson
```
- Install pkgconf (used for package configuration):
```bash
pacman -S mingw-w64-x86_64-pkgconf
```
- Install the GLib library (a prerequisite for GTK):
```bash
pacman -S mingw-w64-x86_64-glib2
```

### Step 3: Set Up the Environment
1- Open Environment Variable Settings
- Press Win + S, type Environment Variables, and select Edit the system environment variables.
- In the System Properties window, click the Environment Variables button.
2- Add MinGW Bin Directory to PATH
- Under System Variables, find and select the Path variable, then click Edit.
- Click New and add the path to the MinGW bin directory. For MSYS2, this is typically:
```makefile
C:\msys64\mingw64\bin
```
### Steps 4: to Execute the Program:
1- Locate the .exe File
- Open the cmake-build-debug directory. This is usually in the root folder of your project.
- Look for a file with the .exe extension (e.g., gtk_app.exe).
2- Double-Click to Run
- Double-click the .exe file to execute the program.
- The GTK application window should open, displaying your user interface.
3- Troubleshooting
- If the `.exe` file doesn’t run, ensure all required GTK runtime libraries are in your PATH (see Step 3: Set Up the Environment).
- If you encounter errors related to missing DLLs, confirm that the `mingw64/bin` directory is correctly added to your environment variables.

## Usage
For Users:
- Log in: Access your personalized dashboard by logging in securely with your credentials.
- Manage Appointments: Easily book new appointments or modify existing ones based on availability and office policies.
- Consultations: View past and upcoming consultations, keep track of your medical history, and access detailed notes from each session.
- Invoices: View, download, or request invoices for consultations, appointments, or other services provided at the medical office.
- Track Your Appointments: View upcoming appointments and receive reminders for better planning and time management.

For Admins:
- Admin Dashboard: A centralized interface to manage patient records, appointments, and system settings.
- Patient Records Management: Add, update, or delete patient information to ensure accuracy and up-to-date records.
- Appointment Oversight: Monitor and manage the appointment schedule, resolving conflicts or adjusting timings as necessary to accommodate patient needs.

## Screenshots

## Technologies Used
GTK: The project utilizes GTK (GIMP Toolkit) for creating the graphical user interface, providing a robust and flexible framework for building cross-platform applications.
C Programming Language: All core functionalities and application logic have been implemented using the C programming language, ensuring performance and efficiency.
CSS for Styling: Custom styling of the application's interface has been achieved using CSS, allowing for a modern and visually appealing user experience.



