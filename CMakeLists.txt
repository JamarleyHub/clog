cmake_minimum_required(VERSION 3.30)
project(clog VERSION 1.6.3 LANGUAGES C)

set(CMAKE_C_STANDARD 17)

add_library(clog SHARED src/clog.c)

# Set the output name of the library
set_target_properties(clog PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        OUTPUT_NAME "clog"
        PUBLIC_HEADER "src/clog.h")

# Add include directories for the library
target_include_directories(clog PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>)

# Install the library, headers, and CMake configuration
include(GNUInstallDirs)
install(TARGETS clog
        EXPORT clogTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Export the targets
install(EXPORT clogTargets
        FILE clogTargets.cmake
        NAMESPACE clog::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/clog)

# Generate and install CMake config files
include(CMakePackageConfigHelpers)
configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/clogConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/clogConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/clog)

write_basic_package_version_file(
        "clogConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/clogConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/clogConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/clog)

add_subdirectory(src)

add_executable(test-log example/test-log.c)

target_link_libraries(test-log PRIVATE clog)