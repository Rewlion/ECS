cmake_minimum_required(VERSION 3.19)

project(ECS VERSION 0.1
        DESCRIPTION "<=*_*=>"
        LANGUAGES CXX)

file(GLOB_RECURSE ECS_SOURCES "./*.cpp" "./*.cc" "./*.h")

add_library(ECS_LIB ${ECS_SOURCES})

set_target_properties(ECS_LIB PROPERTIES 
                        CXX_STANDARD 17
                        CXX_STANDARD_REQUIRED YES
                        CXX_EXTENSIONS NO )
