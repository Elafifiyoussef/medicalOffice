# CMake generated Testfile for 
# Source directory: C:/Users/asus/Documents/medical office/libs/cJSON
# Build directory: C:/Users/asus/Documents/medical office/cmake-build-debug/libs/cJSON
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(cJSON_test "C:/Users/asus/Documents/medical office/cmake-build-debug/libs/cJSON/cJSON_test")
set_tests_properties(cJSON_test PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/asus/Documents/medical office/libs/cJSON/CMakeLists.txt;248;add_test;C:/Users/asus/Documents/medical office/libs/cJSON/CMakeLists.txt;0;")
subdirs("tests")
subdirs("fuzzing")
