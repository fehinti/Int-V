cmake_minimum_required(VERSION 3.5)
project(basic_agent_ST)

set(CMAKE_CXX_STANDARD 11)
# If you are on mac x86
#set(CMAKE_OSX_ARCHITECTURES x86_64)

include(lib/basic.cmake)

include_directories(
        matlab/
        matlab/primitives/
        lib
        log
        PathPlanning
)

link_directories(
        lib
)

# ------------------------------- basic agent ------------------------------------
add_executable(basic_agent
        log/screen_print.c
        log/logvars.cc
        matlab/primitives/rt_nonfinite.cpp
        matlab/primitives/primitives.cpp
        starting_point.cc
        PathPlanning/HLPlanner.cpp
)


target_link_libraries(basic_agent agent_lib ${lib_ws32})

target_compile_options(basic_agent
        PUBLIC -DNOT_INLINED -DSERVER
        )

set_target_properties(basic_agent PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${AGENT_FOLDER})
# ------------------------------------------------------------------------------------
