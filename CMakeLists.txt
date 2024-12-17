cmake_minimum_required(VERSION 3.10)
project(medical_office C)

# Set C standard to C11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

# Define source files
set(SOURCE_FILES
        main.c
        consultation.h consultation.c
        cfile.h cfile.c
        patient.h patient.c
        rendezvous.h
        rendezvous.c
        payment.c
        payment.h
        login.c
        login.h
        builder.c
        builder.h
)

# Create executable
add_executable(medical_office ${SOURCE_FILES})

# Find GTK4 using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk4)

# Check if GTK4 was found
if (NOT GTK_FOUND)
    message(FATAL_ERROR "GTK4 not found. Please install it.")
endif()

# Link GTK libraries and include directories
target_include_directories(medical_office PRIVATE ${GTK_INCLUDE_DIRS})
target_link_libraries(medical_office PRIVATE ${GTK_LIBRARIES})

# Additional settings for UCRT (Windows with MinGW)
if (WIN32)
    target_link_options(medical_office PRIVATE -Wl,--enable-auto-import)
endif()
