
add_executable(${GAME_NAME} src/Entry.cpp
        src/Game.cpp
        src/Game.h
        src/Player.cpp
        src/Player.h
        src/RayMarch.cpp
        src/RayMarch.h
        src/PBR.cpp
        src/PBR.h)


target_include_directories(${GAME_NAME} PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
set_target_properties(${GAME_NAME} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
set_target_properties(${GAME_NAME} PROPERTIES RELEASE_POSTFIX ${CMAKE_RELEASE_POSTFIX})

target_link_libraries(${GAME_NAME} PRIVATE Society compiler_flags ${PACKAGE_LIBRARIES})

#
#install(
#        DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources/"
#        DESTINATION bin
#)

#add_custom_command(TARGET ${GAME_NAME} PRE_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_directory
#        ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${GAME_NAME}>/resources)


#include("cmake/findSociety.cmake")
#
#
#include("/Users/peterfarber/Workspace/C++/CMake/fetch_glad.cmake")
#include("/Users/peterfarber/Workspace/C++/CMake/fetch_glfw.cmake")
#include("/Users/peterfarber/Workspace/C++/CMake/fetch_glm.cmake")
#include("/Users/peterfarber/Workspace/C++/CMake/fetch_spdlog.cmake")
