cmake_minimum_required(VERSION 3.0)
project(devpotdr)
set(PROJECT_VERSION "0.0.1")
include(GNUInstallDirs)

set(PYTHONPATH "" CACHE STRING "Instalation path of the Python package")

find_package(PythonLibs REQUIRED)
add_library(devpotdr SHARED src/drvwrapper.cpp
            src/register.cpp
            src/potdr.cpp
            src/potdrtimebase.cpp
            src/potdrlaserctrl.cpp
            src/potdraccumulatorcore.cpp
            src/potdrspi.cpp
            src/potdrpowermeter.cpp
            src/potdrdacsource.cpp
            src/potdrfanctrl.cpp
            src/potdrsequencercore.cpp)
target_include_directories(devpotdr PUBLIC include ${PYTHON_INCLUDE_DIRS})
target_compile_options(devpotdr PUBLIC -Wall -Wextra -O3)
set_target_properties( devpotdr PROPERTIES PREFIX "lib" SOVERSION "${PROJECT_VERSION}")
install(TARGETS devpotdr LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})


### If pybind is available, generate the python binding
set(PYTHON_MODULE_EXTENSION ".so" CACHE INTERNAL "Cross python lib extension")
find_package(pybind11)

if(pybind11_FOUND)
if ("${PYTHONPATH}" STREQUAL "")
        message(FATAL_ERROR "You must set PYTHONPATH variable")
endif()
pybind11_add_module(pypotdr py/bindings.cpp)
target_compile_options(pypotdr PUBLIC -DPYTHON_BINDINGS)
target_link_libraries(pypotdr PUBLIC devpotdr)
install(TARGETS pypotdr LIBRARY DESTINATION ${PYTHONPATH})
endif()

add_executable(test-basic test/basic.cpp)
target_link_libraries(test-basic PUBLIC devpotdr)
install(TARGETS test-basic)
