cmake_minimum_required(VERSION 3.9...3.16.1 FATAL_ERROR)

project(pstreams VERSION "1.0.1.0" DESCRIPTION "C++ utility for simple IOStream-based Inter-Process Communication" HOMEPAGE_URL "http://pstreams.sourceforge.net/" LANGUAGES CXX)

# ==== Define language standard configurations requiring at least c++11 standard
if(CMAKE_CXX_STANDARD EQUAL "98")
    message(FATAL_ERROR "CMAKE_CXX_STANDARD:STRING=98 is not supported.")
endif()

#####
##  Set the default target properties
if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17) # Supported values are ``11``, ``14``, and ``17``.
endif()
if(NOT CMAKE_CXX_STANDARD_REQUIRED)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()
if(NOT CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/bin" CACHE PATH "Install Prefix" FORCE)
endif()

option(BUILD_TEST "Build the test executables" OFF)

add_library(pstreams INTERFACE)
target_sources(pstreams INTERFACE pstream.h)
target_include_directories(pstreams INTERFACE
    $<BUILD_INTERFACE:"${PROJECT_SOURCE_DIR}">
    $<INSTALL_INTERFACE:"${CMAKE_INSTALL_PREFIX}/include/pstream">
)
install(FILES pstream.h DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pstream")

if(BUILD_TEST)
    add_executable(test_minimum test_minimum.cc)
    install(TARGETS test_minimum  ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX} LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX})
    #add_executable(test_pstreams test_pstreams.cc)
    #install(TARGETS test_pstreams  ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX} LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX})
endif()
