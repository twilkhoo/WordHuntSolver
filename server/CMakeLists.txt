cmake_minimum_required(VERSION 3.10)
project(solver_project)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the executable
add_executable(solver 
  solver.cpp
  util.cpp
  server.cpp
)

# Link pthread library
find_package(Threads REQUIRED)
target_link_libraries(solver PRIVATE Threads::Threads)
