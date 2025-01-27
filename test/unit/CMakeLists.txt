cmake_minimum_required(VERSION 3.10..3.31)
project(inotify-cppUnitTests)

if(POLICY CMP0167)
    # use OLD FindBoost behavior
    cmake_policy(SET CMP0167 OLD)
endif()

###############################################################################
# INOTIFY-CPP
###############################################################################
if(NOT TARGET inotify-cpp::inotify-cpp)
    find_package(inotify-cpp CONFIG REQUIRED)
endif()

###############################################################################
# Thread
###############################################################################
find_package(Threads)
find_package(Boost COMPONENTS unit_test_framework)

if(Boost_FOUND)

    ###############################################################################
    # Test
    ###############################################################################
    add_executable(inotify_unit_test main.cpp NotifierBuilderTests.cpp EventTests.cpp)
    target_link_libraries(inotify_unit_test
        PRIVATE
        inotify-cpp::inotify-cpp
        Boost::unit_test_framework
        ${CMAKE_THREAD_LIBS_INIT})

    add_test(NAME inotify_unit_test COMMAND inotify_unit_test)

else()
    message(WARNING "Boost not found (requires unit_test_framework).  Disabling unit tests.")
endif()
