#/**********************************************************\ 
#Original Author: Richard Bateman (taxilian)
#
#Created:    Nov 20, 2009
#License:    Dual license model; choose one of two:
#            New BSD License
#            http://www.opensource.org/licenses/bsd-license.php
#            - or -
#            GNU Lesser General Public License, version 2.1
#            http://www.gnu.org/licenses/lgpl-2.1.html
#            
#Copyright 2009 PacketPass, Inc and the Firebreath development team
#\**********************************************************/

# Written to work with cmake 2.6
cmake_minimum_required (VERSION 2.6)
set (CMAKE_BACKWARDS_COMPATIBILITY 2.6)

project(mbedx509)

get_target_property(library_target_exists mbedx509 TYPE)
if (library_target_exists)
    set(TARGET_ALREADY_EXISTS 1)
else()
    set(TARGET_ALREADY_EXISTS 0)
endif()

include_directories (
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../include
)

file (GLOB SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    *.c
    *.h
    ../include/mbedtls/*.h
)

if (NOT TARGET_ALREADY_EXISTS)
    add_library (mbedx509 STATIC ${SOURCES})
    set_target_properties(mbedx509 PROPERTIES FOLDER "Libraries")
    target_link_libraries(mbedx509 mbedcrypto)
    if (WIN32)
		target_link_libraries(mbedx509 ws2_32)
	endif()
endif()

if (FIREBREATH)
    append_firebreath_link_library(mbedx509)

    append_firebreath_include_dir(${CMAKE_CURRENT_SOURCE_DIR}/../include)

    export_project_dependencies()
endif()
