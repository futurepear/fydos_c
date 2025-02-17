﻿# CMakeList.txt : CMake project for networkTest, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17) 
# Make sure the compiler supports the specified C++ standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project (fydos)

set(CMAKE_AUTORCC ON)

find_package(fmt CONFIG REQUIRED)
find_package(GameNetworkingSockets)
find_package(glfw3 3.4 REQUIRED) 
find_package(glad)

set(STB_INCLUDE_DIR "C:/OpenGL/stb")
include_directories(${STB_INCLUDE_DIR})

set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/ico/fydos.rc")


#include_directories(src);

# Add source to this project's executable.
add_executable (fydos "fydos.cpp" "fydos.h" "game/renderer/renderer.cpp" "game/renderer/renderer.h" "game/game.cpp" "game/game.h"  ${app_icon_resource_windows} "game/input/processInput.cpp" "game/renderer/glHelper.cpp" "game/renderer/glHelper.h" "game/renderer/util/glmath.h" "game/renderer/util/glmath.cpp" "game/physics/vector.h" "game/physics/transform.h" "game/physics/AABB.h" "game/physics/body.hpp" "game/physics/physics.h" "game/world/chunk.h" "game/constants.h" "game/world/entities/player.h" "game/world/entities/player.cpp" "game/renderer/objects/devRenderChunk.h" "game/input/inputMap.h" "game/physics/collisions.cpp" "game/world/chunk.cpp" "game/world/tiles/tile.h" "game/world/tiles/tile.cpp" "game/world/chunkManager.h" "game/world/terrain/generation.h"  "game/world/terrain/generation.cpp" "game/systems/inventory.h" "game/systems/inventory.cpp" "game/systems/animation.hpp" "game/world/entities/weapon.h"   "game/systems/registry.hpp" "game/systems/dictionary.h"  "game/systems/dictionary.cpp" "game/systems/utility.h" "game/systems/utility.cpp" "game/systems/crafting.h" "game/systems/crafting.cpp" "game/gui/core.h" "game/gui/core.cpp" "game/renderer/gui/renderer.h" "game/renderer/gui/renderer.cpp" "game/gui/components.h" "game/gui/components.cpp" "game/gui/events.h" "game/world/tiles/crafter.h" "game/world/tiles/core.h" "game/world/tiles/crafter.cpp" "game/world/tiles/core.cpp" "game/world/entities/controllers.h" "game/world/entities/controllers.cpp" "game/world/entities/entity.h" "game/world/entities/entity.cpp" "game/world/entities/item.h" "game/world/entities/weapon.cpp" "game/world/tiles/chests.h" "game/world/tiles/chests.cpp" "game/gui/styling/measurements.h" "game/gui/styling/measurements.cpp" "game/gui/styling/styles.h" "game/gui/element.h" "game/gui/element.cpp" "game/renderer/text/textRenderer.h" "game/renderer/text/textRenderer.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET fydos PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
target_link_libraries(fydos PRIVATE fmt::fmt)
target_link_libraries(fydos PRIVATE GameNetworkingSockets::shared)
target_link_libraries(fydos PRIVATE glfw)
target_link_libraries(fydos PRIVATE glad::glad)
target_include_directories(fydos PRIVATE ${STB_INCLUDE_DIR})