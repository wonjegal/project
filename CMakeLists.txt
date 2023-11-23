cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 17)

project(GPTNotice)

add_subdirectory(myhtml)
add_subdirectory(src)
add_subdirectory(openai-cpp)

target_link_libraries(GPTNotice myhtml)
target_link_libraries(GPTNotice curl)
target_include_directories(GPTNotice
PUBLIC
        ${CMAKE_SOURCE_DIR}/myhtml/include
        ${CMAKE_SOURCE_DIR}/openai-cpp/include/openai
)
