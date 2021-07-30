get_filename_component(FOLDER_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)

find_package(Threads REQUIRED)

add_executable(01_future 01_future.cpp)

target_link_libraries(01_future Threads::Threads)

install(
    TARGETS
    01_future
    DESTINATION 
    ${FOLDER_NAME}
)