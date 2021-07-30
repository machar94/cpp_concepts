get_filename_component(FOLDER_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)

add_executable(01_sfinae 01_sfinae.cpp)
add_executable(02_sfinae 02_sfinae.cpp)

install(
    TARGETS 
    01_sfinae
    02_sfinae
    DESTINATION 
    ${FOLDER_NAME}
)