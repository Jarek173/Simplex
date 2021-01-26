project(simplex)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(simplex
               main.cpp
               simplex.cpp
               simplex.h)

