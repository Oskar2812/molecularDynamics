cmake_minimum_required(VERSION 3.5.0)
project(MolecularDynamics VERSION 0.1.0 LANGUAGES C)

include_directories(include)
include_directories(raylib/include)

set(SOURCES
    src/Cell.c 
    src/Particle.c
    src/Simulation.c 
    src/Vector.c
    src/Game.c
    src/Histogram.c
    src/Assets.c)

set(HEADERS 
    include/Cell.h 
    include/Particle.h
    include/Simulation.h 
    include/Vector.h
    include/Game.h
    include/Histogram.h
    include/Assets.h)

link_directories(raylib-5.0_linux_amd64/lib)

add_executable(MolecularDynamics src/main.c ${SOURCES} ${HEADERS})
add_executable(Benchmarking test/benchmark.c ${SOURCES} ${HEADERS})

add_compile_options(-Wall -Werror -fopenmp)

find_package(OpenMP REQUIRED)

if(OpenMP_C_FOUND)
    target_link_libraries(MolecularDynamics PUBLIC OpenMP::OpenMP_C m raylib)
    target_link_libraries(Benchmarking PUBLIC OpenMP::OpenMP_C  m raylib)
endif()


