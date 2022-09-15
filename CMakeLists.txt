# CMake file mostly written from this example:
# https://cliutils.gitlab.io/modern-cmake/chapters/packages/ROOT.html
# https://cliutils.gitlab.io/modern-cmake/examples/root-dict/

# Assert CMake version
cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

# This is your project statement. You should always list languages
project(root-eclipse LANGUAGES CXX)

# ROOT 6.10+ supports config file discovery
#find_package(ROOT REQUIRED COMPONENTS RooFit RooFitCore Gui)

# TODO: try just this
find_package(ROOT CONFIG REQUIRED)

# For setting project in IDE via CMake generators
include_directories(${ROOT_INCLUDE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/src)

# First find all headers in project
# file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
                  ${PROJECT_SOURCE_DIR}/src/*.hpp
                  ${PROJECT_SOURCE_DIR}/src/*.hxx
                  ${PROJECT_SOURCE_DIR}/src/*.hh
                  ${PROJECT_SOURCE_DIR}/src/*.h)
message(STATUS "Found list of all header files: ${HEADERS}")

# Find LinkDef.h file
file(GLOB_RECURSE LINKDEFH ${CMAKE_CURRENT_SOURCE_DIR}/*.hxx ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/*.hh ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
list(FILTER LINKDEFH INCLUDE REGEX "[lL]ink[dD]ef")
message(STATUS "Found LinkDef file: ${LINKDEFH}")

# Exclude Linkdef from other headers
list(REMOVE_ITEM HEADERS ${LINKDEFH})
message(STATUS "List of header files only: ${HEADERS}")

# TARGET: generate the dictionary (extra C++ file containing custom ROOT methods)
set(DICTIONARY_TARGET ${PROJECT_NAME}-dict)
root_generate_dictionary(${DICTIONARY_TARGET} ${HEADERS} LINKDEF ${LINKDEFH})

# Tip: name of the generated dictionary is same as target name with .cxx extension: ${DICTIONARY_TARGET}.cxx
set(DICTIONARY ${CMAKE_CURRENT_BINARY_DIR}/${DICTIONARY_TARGET})
message(STATUS "Created dictionary file: ${DICTIONARY}")

# Find sources in project
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
                  ${PROJECT_SOURCE_DIR}/src/*.cpp
                  ${PROJECT_SOURCE_DIR}/src/*.cxx
                  ${PROJECT_SOURCE_DIR}/src/*.cc
                  ${PROJECT_SOURCE_DIR}/src/*.c)
message(STATUS "Found list of source files: ${SOURCES}")

# TARGET: create shared library
set(SHARED_LIB_TARGET ${PROJECT_NAME}-shared)
add_library(${SHARED_LIB_TARGET} SHARED)

# Specify sources that shared library target should be built from. Dictionary file goes last
target_sources(${SHARED_LIB_TARGET} PUBLIC ${SOURCES} ${DICTIONARY})

# Compose list of ROOT libraries with "ROOT::" prefix - need to link them to the shared library
set(LIB_NAMES "")
FOREACH(X IN LISTS ROOT_LIBRARIES)
  get_filename_component(FILENAME ${X} NAME)
  string(REGEX REPLACE "lib([a-zA-Z0-9]+).so" "ROOT::\\1" FILENAME2 ${FILENAME})
  list(APPEND LIB_NAMES "${FILENAME2}")
ENDFOREACH()

# Manually append extra ROOT libraries (why missing?)
list(APPEND LIB_NAMES "ROOT::Gui")
list(APPEND LIB_NAMES "ROOT::RooFit")
list(APPEND LIB_NAMES "ROOT::RooFitCore")
list(APPEND LIB_NAMES "ROOT::Html")
list(APPEND LIB_NAMES "ROOT::Minuit")
list(APPEND LIB_NAMES "ROOT::Fumili")
message(STATUS "Found ROOT libraries: ${LIB_NAMES}")

# Link found libraries to the project shared library
target_link_libraries(${SHARED_LIB_TARGET} PUBLIC ${LIB_NAMES})
message(STATUS "Created shared library target.")

# TARGET: executable
set(EXECUTABLE_TARGET ${PROJECT_NAME})
add_executable(${EXECUTABLE_TARGET} ${PROJECT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(${EXECUTABLE_TARGET} ${SHARED_LIB_TARGET})
