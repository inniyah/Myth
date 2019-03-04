# -*- mode: CMake; tab-width: 2; indent-tabs-mode: nil; -*-
cmake_minimum_required(VERSION 3.5)
project(OctaCore)

include(FindPkgConfig)

# Build options.
option(OCTACORE_BUILD_CLIENT "Build the game client" ON)
option(OCTACORE_BUILD_SERVER "Build the game server" ON)

# Generate compile commands (compile_commands.json) for clang tooling etc.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

pkg_search_module(ZLIB REQUIRED zlib)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(ENET REQUIRED libenet)

if(OCTACORE_BUILD_CLIENT)
  # Add OpenGL.
  find_package(OpenGL REQUIRED)

  # Platform specific libraries.
  if(UNIX AND NOT APPLE)
    set(PLATFORM_LIBRARIES rt)
  endif()
endif()

# Additional compiler flags.
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  # Silence warnings from Clang.
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-exception-spec")
endif()

if(OCTACORE_BUILD_CLIENT)
  # Client executable.
  set(CLIENT_SOURCES
      shared/crypto.cc
      shared/geom.cc
      shared/glemu.cc
      shared/stream.cc
      shared/tools.cc
      shared/zip.cc
      engine/aa.cc
      engine/bih.cc
      engine/blend.cc
      engine/client.cc
      engine/command.cc
      engine/console.cc
      engine/dynlight.cc
      engine/grass.cc
      engine/light.cc
      engine/main.cc
      engine/material.cc
      engine/menus.cc
      engine/movie.cc
      engine/normal.cc
      engine/octa.cc
      engine/octaedit.cc
      engine/octarender.cc
      engine/physics.cc
      engine/pvs.cc
      engine/rendergl.cc
      engine/renderlights.cc
      engine/rendermodel.cc
      engine/renderparticles.cc
      engine/rendersky.cc
      engine/rendertext.cc
      engine/renderva.cc
      engine/server.cc
      engine/serverbrowser.cc
      engine/shader.cc
      engine/sound.cc
      engine/stain.cc
      engine/texture.cc
      engine/ui.cc
      engine/water.cc
      engine/world.cc
      engine/worldio.cc
      game/ai.cc
      game/client.cc
      game/entities.cc
      game/game.cc
      game/render.cc
      game/scoreboard.cc
      game/server.cc
      game/waypoint.cc
      game/weapon.cc
    )
  set(CLIENT_INCLUDES
      ${CMAKE_CURRENT_SOURCE_DIR}/shared
      )
  set(CLIENT_LIBRARIES
      ${ENET_LIBRARIES}
      ${OPENGL_LIBRARIES}
      ${ZLIB_LIBRARIES}
      ${PLATFORM_LIBRARIES}
      ${SDL2_LIBRARIES}
      SDL2_image
      SDL2_mixer
      )
  add_executable(native_client ${CLIENT_SOURCES})
  target_include_directories(native_client SYSTEM PUBLIC
    ${ENET_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
    )
  target_include_directories(native_client PRIVATE ${CLIENT_INCLUDES})
  target_link_libraries(native_client ${CLIENT_LIBRARIES})

  # Client launcher.
  if(UNIX)
    configure_file("scripts/runclient.sh.in" "${CMAKE_CURRENT_BINARY_DIR}/runclient.sh" @ONLY)
  endif()
endif()

if(OCTACORE_BUILD_SERVER)
  # Server executable.
  set(SERVER_SOURCES
      shared/crypto.cc
      shared/stream.cc
      shared/tools.cc
      engine/command.cc
      engine/server.cc
      engine/worldio.cc
      game/entities.cc
      game/server.cc
      )
  set(SERVER_INCLUDES
      ${CMAKE_CURRENT_SOURCE_DIR}/shared
      )
  set(SERVER_LIBRARIES
      ${ENET_LIBRARIES}
      ${ZLIB_LIBRARIES}
      )
  add_executable(native_server ${SERVER_SOURCES})
  target_compile_definitions(native_server PRIVATE -DSTANDALONE)
  target_include_directories(native_server SYSTEM PUBLIC
    ${ENET_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
    )
  target_include_directories(native_server PRIVATE ${SERVER_INCLUDES})
  target_link_libraries(native_server ${SERVER_LIBRARIES})

  # Master executable.
  set(MASTER_SOURCES
      shared/crypto.cc
      shared/stream.cc
      shared/tools.cc
      engine/command.cc
      engine/master.cc
      )
  set(MASTER_INCLUDES
      ${CMAKE_CURRENT_SOURCE_DIR}/shared
      )
  set(MASTER_LIBRARIES
      ${ENET_LIBRARIES}
      ${ZLIB_LIBRARIES}
      )
  add_executable(native_master ${MASTER_SOURCES})
  target_compile_definitions(native_master PRIVATE -DSTANDALONE)
  target_include_directories(native_master SYSTEM PUBLIC
    ${ENET_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
    )
  target_include_directories(native_master PRIVATE ${MASTER_INCLUDES})
  target_link_libraries(native_master ${MASTER_LIBRARIES})
endif()
