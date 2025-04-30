cmake_minimum_required(VERSION 3.10)
project(solver_project)

# Set C++ standard to C++20.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add the custom stl.
add_library(stl INTERFACE)
target_include_directories(stl
  INTERFACE
    ${CMAKE_SOURCE_DIR}/stl
)

# Add the executable.
add_executable(server 
  src/solver.cpp
  src/util.cpp
  server.cpp
)

# Link libraries.
find_package(Threads REQUIRED)
target_link_libraries(server PRIVATE Threads::Threads stl)
