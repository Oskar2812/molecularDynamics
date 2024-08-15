cmake_minimum_required(VERSION 3.5.0)
project(MolecularDynamics VERSION 0.1.0 LANGUAGES C)

include_directories(include)

set(SOURCES
    src/Cell.c 
    src/Particle.c
    src/Simulation.c 
    src/Vector2.c)

set(HEADERS 
    include/Cell.h 
    include/Particle.h
    include/Simulation.h 
    include/Vector2.h)

add_executable(MolecularDynamics src/main.c ${SOURCES} ${HEADERS})

target_link_libraries(MolecularDynamics m)

