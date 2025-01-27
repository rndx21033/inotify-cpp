cmake_minimum_required(VERSION 3.10..3.31)
project(inotify-cpp)

if(POLICY CMP0167)
    # use OLD FindBoost behavior
    cmake_policy(SET CMP0167 OLD)
endif()

option(INOTIFY_BUILD_EXAMPLE "Build inotify-cpp example program" ON)
option(INOTIFY_BUILD_TEST "Build inotify-cpp unittest program" ON)
option(BUILD_SHARED_LIBS "Build inotify-cpp as a shared library" ON)
option(BUILD_STATIC_LIBS "Build inotify-cpp as a static library" OFF)

# boost is required for boost::bimap
# optionally, unit_test_framework is also needed for unit tests
find_package(Boost 1.54.0 REQUIRED)

add_subdirectory(src)

if(INOTIFY_BUILD_EXAMPLE)
    add_subdirectory(example)
endif()

if(INOTIFY_BUILD_TEST)
    enable_testing()
    add_subdirectory(test)
endif()

message(STATUS "")
message(STATUS "")
message(STATUS "${PROJECT_NAME} configuration summary:")
message(STATUS "")
message(STATUS "  CMake build type ................ : ${CMAKE_BUILD_TYPE}")
message(STATUS "  Build shared libs  .............. : ${BUILD_SHARED_LIBS}")
message(STATUS "  Build static libs  .............. : ${BUILD_STATIC_LIBS}")
message(STATUS "  Build example  .................. : ${INOTIFY_BUILD_EXAMPLE}")
message(STATUS "  Build test ...................... : ${INOTIFY_BUILD_TEST}")
message(STATUS "  Build c++ standard .............. : ${CMAKE_CXX_STANDARD}")
message(STATUS "")
message(STATUS " Dependencies:")
message(STATUS "  Boost version.................... : ${Boost_VERSION}")
message(STATUS "  Boost root....................... : ${Boost_DIR}")
message(STATUS "")
