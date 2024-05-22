cmake_minimum_required(VERSION 3.9)

project(DFN LANGUAGES CXX C VERSION 1.0.0)

# Set C++ Standard
################################################################################
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# IMPOSE WARNINGS ON DEBUG
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -pedantic-errors")

# IMPOSE CXX FLAGS FOR WINDOWS
if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wa,-mbig-obj")
endif (WIN32)

# Add dependencies
################################################################################
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH};${CMAKE_CURRENT_SOURCE_DIR}/../externals/Main_Install/eigen3)
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH};${CMAKE_CURRENT_SOURCE_DIR}/../externals/Main_Install/googletest)

## Eigen3
find_package(Eigen3 CONFIG REQUIRED)
list(APPEND DFN_LINKED_LIBRARIES PUBLIC Eigen3::Eigen)

## Threads
find_package(Threads REQUIRED)
list(APPEND DFN_LINKED_LIBRARIES PRIVATE Threads::Threads)

## GTest
find_package(GTest REQUIRED)

list(APPEND DFN_LINKED_LIBRARIES PRIVATE GTest::gtest)
list(APPEND DFN_LINKED_LIBRARIES PRIVATE GTest::gtest_main)
list(APPEND DFN_LINKED_LIBRARIES PRIVATE GTest::gmock)
list(APPEND DFN_LINKED_LIBRARIES PRIVATE GTest::gmock_main)

# Insert Sources
################################################################################

add_subdirectory(src)
list(APPEND DFN_sources ${src_sources})
list(APPEND DFN_headers ${src_headers})
list(APPEND DFN_includes ${src_includes})


add_subdirectory(TestingParaview/Code/src)
list(APPEND DFN_sources ${src_paraview_sources})
list(APPEND DFN_headers ${src_paraview_headers})
list(APPEND DFN_includes ${src_paraview_includes})

add_subdirectory(src_test)
list(APPEND DFN_TEST_sources ${src_test_sources})
list(APPEND DFN_TEST_headers ${src_test_headers})
list(APPEND DFN_TEST_includes ${src_test_includes})

# Create executable
################################################################################
add_executable(${CMAKE_PROJECT_NAME} main.cpp ${${CMAKE_PROJECT_NAME}_sources} ${${CMAKE_PROJECT_NAME}_headers})

add_executable(${CMAKE_PROJECT_NAME}_TEST main_test.cpp ${${CMAKE_PROJECT_NAME}_sources} ${${CMAKE_PROJECT_NAME}_headers} ${${CMAKE_PROJECT_NAME}_TEST_headers} ${${CMAKE_PROJECT_NAME}_TEST_sources})



file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/DFN/FR3_data.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/DFN/FR10_data.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/DFN/FR50_data.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/DFN/FR82_data.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/DFN/FR200_data.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/DFN/FR362_data.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${${CMAKE_PROJECT_NAME}_includes})
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC ${${CMAKE_PROJECT_NAME}_LINKED_LIBRARIES})
target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC -fPIC)


target_include_directories(${CMAKE_PROJECT_NAME}_TEST PUBLIC ${${CMAKE_PROJECT_NAME}_includes} ${${CMAKE_PROJECT_NAME}_TEST_includes})
target_link_libraries(${CMAKE_PROJECT_NAME}_TEST PUBLIC ${${CMAKE_PROJECT_NAME}_LINKED_LIBRARIES})
target_compile_options(${CMAKE_PROJECT_NAME}_TEST PUBLIC -fPIC)
