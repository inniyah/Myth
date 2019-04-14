# -*- mode: CMake; tab-width: 2; indent-tabs-mode: nil; -*-
cmake_minimum_required(VERSION 3.5)
project(OctaCore)

# Build options.
option(LAMIAE_BUILD_CLIENT "Build the game client" ON)
option(LAMIAE_BUILD_RPGGAME "Build the rpggame client" ON)
option(LAMIAE_BUILD_SERVER "Build the game server" ON)

find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" ${DOXYGEN_FOUND})

# Generate compile commands (compile_commands.json) for clang tooling etc.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig)

pkg_search_module(ZLIB REQUIRED zlib)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(ENET REQUIRED libenet)

if(LAMIAE_BUILD_CLIENT OR LAMIAE_BUILD_RPGGAME)
  # Add OpenGL.
  pkg_search_module(OPENGL REQUIRED gl)

  # Platform specific libraries.
  if(UNIX AND NOT APPLE)
    set(PLATFORM_LIBRARIES rt)
  endif()
endif()

# Additional compiler flags.
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  # Silence warnings from Clang.
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wno-missing-exception-spec")
endif()

if(LAMIAE_BUILD_CLIENT)
  # Client executable.
  set(CLIENT_SOURCES
      shared/crypto.cpp
      shared/geom.cpp
      shared/glemu.cpp
      shared/stream.cpp
      shared/tools.cpp
      shared/zip.cpp
      engine/aa.cpp
      engine/bih.cpp
      engine/blend.cpp
      engine/client.cpp
      engine/command.cpp
      engine/console.cpp
      engine/dynlight.cpp
      engine/grass.cpp
      engine/light.cpp
      engine/main.cpp
      engine/material.cpp
      engine/movie.cpp
      engine/normal.cpp
      engine/octa.cpp
      engine/octaedit.cpp
      engine/octarender.cpp
      engine/physics.cpp
      engine/pvs.cpp
      engine/rendergl.cpp
      engine/renderlights.cpp
      engine/rendermodel.cpp
      engine/renderparticles.cpp
      engine/rendersky.cpp
      engine/rendertext.cpp
      engine/renderva.cpp
      engine/server.cpp
      engine/serverbrowser.cpp
      engine/shader.cpp
      engine/sound.cpp
      engine/stain.cpp
      engine/texture.cpp
      engine/ui.cpp
      engine/water.cpp
      engine/world.cpp
      engine/worldio.cpp
      game/ai.cpp
      game/client.cpp
      game/entities.cpp
      game/game.cpp
      game/render.cpp
      game/scoreboard.cpp
      game/server.cpp
      game/waypoint.cpp
      game/weapon.cpp
    )
  set(CLIENT_INCLUDES
      ${CMAKE_CURRENT_SOURCE_DIR}/shared
      )
  set(CLIENT_LIBRARIES
      ${PLATFORM_LIBRARIES}
      ${ENET_LIBRARIES}
      ${ZLIB_LIBRARIES}
      ${OPENGL_LIBRARIES}
      ${SDL2_LIBRARIES}
      SDL2_image
      SDL2_mixer
      )
  add_executable(native_client ${CLIENT_SOURCES})
  target_compile_options(native_client PUBLIC
    ${ENET_CFLAGS_OTHER}
    ${ZLIB_CFLAGS_OTHER}
    ${OPENGL_CFLAGS_OTHER}
    ${SDL2_CFLAGS_OTHER}
    )
  target_include_directories(native_client SYSTEM PUBLIC
    ${ENET_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS}
    )
  target_include_directories(native_client PRIVATE ${CLIENT_INCLUDES})
  target_link_libraries(native_client ${CLIENT_LIBRARIES})

  # Client launcher.
  if(UNIX)
    configure_file("scripts/runclient.sh.in" "${CMAKE_CURRENT_BINARY_DIR}/runclient.sh" @ONLY)
  endif()
endif()

if(LAMIAE_BUILD_RPGGAME)
  set(RPGGAME_SOURCES
      shared/crypto.cpp
      shared/geom.cpp
      shared/glemu.cpp
      shared/stream.cpp
      shared/tools.cpp
      shared/zip.cpp
      engine/aa.cpp
      engine/bih.cpp
      engine/blend.cpp
      engine/client.cpp
      engine/command.cpp
      engine/console.cpp
      engine/dynlight.cpp
      engine/grass.cpp
      engine/light.cpp
      engine/main.cpp
      engine/material.cpp
      engine/movie.cpp
      engine/normal.cpp
      engine/octa.cpp
      engine/octaedit.cpp
      engine/octarender.cpp
      engine/pvs.cpp
      engine/physics.cpp
      engine/rendergl.cpp
      engine/renderlights.cpp
      engine/rendermodel.cpp
      engine/renderparticles.cpp
      engine/rendersky.cpp
      engine/rendertext.cpp
      engine/renderva.cpp
      engine/server.cpp
      engine/serverbrowser.cpp
      engine/shader.cpp
      engine/stain.cpp
      engine/sound.cpp
      engine/texture.cpp
      engine/ui.cpp
      engine/water.cpp
      engine/world.cpp
      engine/worldio.cpp
      rpggame/rpg.cpp
      rpggame/rpgaction.cpp
      rpggame/rpgai.cpp
      rpggame/rpgcamera.cpp
      rpggame/rpgchar.cpp
      rpggame/rpgconfig.cpp
      rpggame/rpgcontainer.cpp
      rpggame/rpgeffect.cpp
      rpggame/rpgentities.cpp
      rpggame/rpggui.cpp
      rpggame/rpghud.cpp
      rpggame/rpgio.cpp
      rpggame/rpgitem.cpp
      rpggame/rpgobstacle.cpp
      rpggame/rpgplatform.cpp
      rpggame/rpgproj.cpp
      rpggame/rpgscript.cpp
      rpggame/rpgstats.cpp
      rpggame/rpgstatus.cpp
      rpggame/rpgstubs.cpp
      rpggame/rpgrender.cpp
      rpggame/rpgreserved.cpp
      rpggame/rpgtest.cpp
      rpggame/rpgtrigger.cpp
      rpggame/waypoint.cpp
    )
  set(RPGGAME_INCLUDES
      ${CMAKE_CURRENT_SOURCE_DIR}/shared
      ${CMAKE_CURRENT_SOURCE_DIR}/engine
      )
  set(RPGGAME_LIBRARIES
      ${PLATFORM_LIBRARIES}
      ${ENET_LIBRARIES}
      ${ZLIB_LIBRARIES}
      ${OPENGL_LIBRARIES}
      ${SDL2_LIBRARIES}
      SDL2_image
      SDL2_mixer
      )
  add_executable(native_rpggame ${RPGGAME_SOURCES})
  target_compile_options(native_rpggame PUBLIC
    ${ENET_CFLAGS_OTHER}
    ${ZLIB_CFLAGS_OTHER}
    ${OPENGL_CFLAGS_OTHER}
    ${SDL2_CFLAGS_OTHER}
    )
  target_include_directories(native_rpggame SYSTEM PUBLIC
    ${ENET_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS}
    )
  target_include_directories(native_rpggame PRIVATE ${RPGGAME_INCLUDES})
  target_link_libraries(native_rpggame ${RPGGAME_LIBRARIES})
endif()

if(LAMIAE_BUILD_SERVER)
  # Server executable.
  set(SERVER_SOURCES
      shared/crypto.cpp
      shared/stream.cpp
      shared/tools.cpp
      engine/command.cpp
      engine/server.cpp
      engine/worldio.cpp
      game/entities.cpp
      game/server.cpp
      )
  set(SERVER_INCLUDES
      ${CMAKE_CURRENT_SOURCE_DIR}/shared
      )
  set(SERVER_LIBRARIES
      ${ENET_LIBRARIES}
      ${ZLIB_LIBRARIES}
      )
  add_executable(native_server ${SERVER_SOURCES})
  target_compile_definitions(native_server PRIVATE
    -DSTANDALONE
    )
  target_compile_options(native_client PUBLIC
    ${ENET_CFLAGS_OTHER}
    ${ZLIB_CFLAGS_OTHER}
    )
  target_include_directories(native_server SYSTEM PUBLIC
    ${ENET_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
    )
  target_include_directories(native_server PRIVATE ${SERVER_INCLUDES})
  target_link_libraries(native_server ${SERVER_LIBRARIES})

  # Master executable.
  set(MASTER_SOURCES
      shared/crypto.cpp
      shared/stream.cpp
      shared/tools.cpp
      engine/command.cpp
      engine/master.cpp
      )
  set(MASTER_INCLUDES
      ${CMAKE_CURRENT_SOURCE_DIR}/shared
      )
  set(MASTER_LIBRARIES
      ${ENET_LIBRARIES}
      ${ZLIB_LIBRARIES}
      )
  add_executable(native_master ${MASTER_SOURCES})
  target_compile_definitions(native_master PRIVATE
    -DSTANDALONE
    )
  target_compile_options(native_client PUBLIC
    ${ENET_CFLAGS_OTHER}
    ${ZLIB_CFLAGS_OTHER}
    )
  target_include_directories(native_master SYSTEM PUBLIC
    ${ENET_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
    )
  target_include_directories(native_master PRIVATE ${MASTER_INCLUDES})
  target_link_libraries(native_master ${MASTER_LIBRARIES})
endif()

if(BUILD_DOCUMENTATION)
    if(NOT DOXYGEN_FOUND)
         message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()

    set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${doxyfile_in} ${doxyfile} @ONLY)

    message("Doxygen build started.")

    add_custom_target(doc
                      COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
                      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                      COMMENT "Generating API documentation with Doxygen"
                      VERBATIM)

    #install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/doc)
endif()
