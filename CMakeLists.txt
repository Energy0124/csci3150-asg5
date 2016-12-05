cmake_minimum_required(VERSION 3.6)
project(csci3150_asg5)

enable_language(C)

set(CMAKE_C_STANDARD 99)

#[[set(SOURCE_FILES
    manager.c
    manager.h)]]

set(SOURCE_FILES_TEST
    manager.c
    manager.h
    tester.c)

#add_executable(csci3150_asg5 ${SOURCE_FILES})
add_executable(csci3150_asg5_test ${SOURCE_FILES_TEST})

#target_link_libraries(csci3150_asg5 m)
target_link_libraries(csci3150_asg5_test m)
target_link_libraries(csci3150_asg5_test cunit)