cmake_minimum_required(VERSION 3.12)

project(SofaMiscTopology_test)

set(HEADER_FILES)
set(SOURCE_FILES ../../empty.cpp)

list(APPEND SOURCE_FILES
TopologicalChangeProcessor_test.cpp
)

add_definitions("-DSOFAMISCTOPOLOGY_TEST_SCENES_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/scenes\"")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} SofaGTestMain SofaTest)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
