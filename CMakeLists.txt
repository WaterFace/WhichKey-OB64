cmake_minimum_required(VERSION 3.21.0)
project(
  WhichKey-OB64
  VERSION 1.2.0
  DESCRIPTION
    "OBSE64 plugin to display the key name instead of lock level when appropriate"
  LANGUAGES CXX)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/plugin.rc.in
               ${CMAKE_CURRENT_BINARY_DIR}/plugin.rc @ONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/plugin.cpp.in
               ${CMAKE_CURRENT_BINARY_DIR}/plugin.cpp @ONLY)

set(REX_OPTION_INI
    ON
    CACHE INTERNAL "Enables ini config support for REX." FORCE)
add_subdirectory(lib/commonlibob64)

find_path(SIMPLEINI_INCLUDE_DIR "SimpleIni.h")
add_library(
  simpleini INTERFACE ${SIMPLEINI_INCLUDE_DIR}/SimpleIni.h
  ${SIMPLEINI_INCLUDE_DIR}/ConvertUTF.c ${SIMPLEINI_INCLUDE_DIR}/ConvertUTF.h)

set(SOURCES
    ${CMAKE_CURRENT_BINARY_DIR}/plugin.rc
    ${CMAKE_CURRENT_BINARY_DIR}/plugin.cpp src/main.cpp src/types.h
    src/config.h src/which_key.h)
add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.h)
target_link_libraries(${PROJECT_NAME} PRIVATE CommonLibOB64)
target_link_libraries(${PROJECT_NAME} PRIVATE simpleini)

if(CMAKE_EXPORT_COMPILE_COMMANDS)
  execute_process(
    COMMAND
      ${CMAKE_COMMAND} -E create_symlink
      ${CMAKE_BINARY_DIR}/compile_commands.json
      ${CMAKE_SOURCE_DIR}/compile_commands.json)
endif()

add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND
    ${CMAKE_COMMAND} -E make_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/output/OblivionRemastered/Binaries/Win64/OBSE/Plugins/"
)
add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND
    ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}>
    "${CMAKE_CURRENT_SOURCE_DIR}/output/OblivionRemastered/Binaries/Win64/OBSE/Plugins/"
)

if(${CMAKE_BUILD_TYPE} STREQUAL Debug OR ${CMAKE_BUILD_TYPE} STREQUAL
                                         RelWithDebInfo)
  add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND
      ${CMAKE_COMMAND} -E copy $<TARGET_PDB_FILE:${PROJECT_NAME}>
      "${CMAKE_CURRENT_SOURCE_DIR}/output/OblivionRemastered/Binaries/Win64/OBSE/Plugins/"
  )
endif()
