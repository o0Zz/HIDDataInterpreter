cmake_minimum_required(VERSION 3.14)
project(HIDDataInterpreterTests)

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.11.0  # Adjust this to the version you need
)

# Set to OFF to skip fetching and use system-wide installed gtest
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Prevent GoogleTest from overriding our compiler/linker options
set(gtest_disable_pthreads ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing() #to discover tests in test explorer 

add_executable(HIDDataInterpreterTests test_double_joystick.cpp) # add this executable

target_link_libraries(HIDDataInterpreterTests PRIVATE gtest) # link google test to this executable
target_link_libraries(HIDDataInterpreterLib PRIVATE gtest) # link google test to this executable

include(GoogleTest)
gtest_discover_tests(HIDDataInterpreterTests) # discovers tests by asking the compiled test executable to enumerate its tests