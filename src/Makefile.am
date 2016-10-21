# author: Nader Khammassi
# bugreport to : nader.khammassi@gmail.com

## place generated object files (.o) into the same directory as their source
## files, in order to avoid collisions when non-recursive make is used.
AUTOMAKE_OPTIONS = foreign subdir-objects

#SUBDIRS = tools

## additional flags to pass to aclocal when it is invoked automatically at
## make time. The ${ACLOCAL_FLAGS} variable is picked up from the environment
## to provide a way for the user to supply additional arguments.
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} 

CXX=g++
# AM_CPPFLAGS=-std=c++11 -I. -I ./xpu-0.1.5/ -Wfatal-errors -DCG_BC -DQX_SPARSE_MV_MUL -DXPU_TIMER -D__BUILTIN_LINALG__ -mavx -msse4.2 -mavx2 -mfma -O3 # -ggdb -fno-inline-functions # -O3 
AM_CPPFLAGS=-std=c++11 -I. -I ./xpu-0.1.5/ -Wfatal-errors -DCG_BC -DQX_SPARSE_MV_MUL -DXPU_TIMER -D__BUILTIN_LINALG__ -msse4.2 -mavx2 -mfma -DUSE_OPENMP -O3 -fopenmp -g

XPU=-I ./xpu-0.1.5/
FLAGS = --pedantic -std=c++11 -O3 -I. -I ./xpu-0.1.5/ -Wfatal-errors -DCG_BC -DQX_SPARSE_MV_MUL -DXPU_TIMER -fopenmp # $(XPU) -Wall 
LDFLAGS = -lm -fopenmp

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
bin_PROGRAMS = qx_simulator  qx_simulation_server # ninja_star_simulator 
qx_simulator_SOURCES = ./tools/simulator.cc
# ninja_star_simulator_SOURCES = ./tools/ninja_star_simulator.cc
qx_simulation_server_SOURCES = ./server/qx_server.cc



