
cmake_minimum_required(VERSION 3.23)

project(SimpleOnlineChat)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c*")

add_executable(${PROJECT_NAME} ${SOURCES})
        
target_link_libraries(${PROJECT_NAME} PUBLIC ${Boost_LIBRARIES})