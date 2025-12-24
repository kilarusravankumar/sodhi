# Set the minimum required version of CMake

cmake_minimum_required(VERSION 3.16)

# Define the project name and version

project(sodhi VERSION 0.0.1)

# Use C++17 standard (a good, modern default)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_PREFIX_PATH "/home/kreten/Qt/6.9.3/gcc_64")

# --- Qt 6 Configuration ---
#
# This tells CMake to automatically run Qt's "Meta-Object Compiler" (moc),
#
# which is necessary for Qt's signals/slots system.

set(CMAKE_AUTOMOC ON)

# This is needed for Qt's UI files (which we'll add later).

set(CMAKE_AUTOUIC ON)

# This is for resource files (like icons).

set(CMAKE_AUTORCC ON)

# Find the Qt 6 package. We specifically need the "Widgets" component.
#
# This is what searches the CMAKE_PREFIX_PATH we set in the terminal.

find_package(Qt6 6.2 REQUIRED COMPONENTS Widgets Network WebSockets Test)

# --- Create a library for our shared code ---
add_library(sodhi_lib STATIC
    src/mainwindow.cpp
    src/landingpage.cpp
    src/register.cpp
    src/login.cpp
    src/user.cpp
    src/dashboard.cpp
    src/roomslayout.cpp
    src/room.cpp
    src/message.cpp
    include/mainwindow.h
    include/landingpage.h
    include/register.h
    include/login.h
    include/user.h
    include/dashboard.h
    include/roomslayout.h
    include/room.h
    include/message.h
)

target_include_directories(sodhi_lib PUBLIC include)

# Link the library against Qt
target_link_libraries(sodhi_lib PRIVATE
    Qt6::Widgets
    Qt6::Network
    Qt6::WebSockets
)


# --- Project Files ---
#
# Create our executable, named "sodhi_client", from main.cpp
#
add_executable(sodhi
    src/main.cpp
)

# --- Link Libraries ---
#
# Link our executable against our shared library.
#
target_link_libraries(sodhi PRIVATE sodhi_lib Qt6::Widgets Qt6::Network Qt6::WebSockets)


# --- Testing Configuration ---

# Enable testing with CTest
enable_testing()

# Create a separate executable for our tests
add_executable(login_register_test
    tests/test_login_register.cpp
)

target_include_directories(login_register_test PUBLIC include)

# Link the test executable to Qt Test and our shared library
target_link_libraries(login_register_test PRIVATE
    Qt6::Test
    sodhi_lib
    Qt6::Widgets
    Qt6::Network
)

# Add the test executable to CTest
add_test(NAME LoginRegisterTest COMMAND login_register_test)
