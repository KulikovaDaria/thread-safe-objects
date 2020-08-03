cmake_minimum_required(VERSION 3.6)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

project(threadsafe_objects)

include_directories(.)

add_subdirectory(avl-tree)
add_subdirectory(vector-stack-queue)