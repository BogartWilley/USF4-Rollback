cmake_minimum_required (VERSION 3.20)
set(VCPKG_OVERLAY_PORTS "${CMAKE_SOURCE_DIR}/vcpkg-ports")
set(VCPKG_OVERLAY_TRIPLETS "${CMAKE_SOURCE_DIR}/vcpkg-triplets")
set(VCPKG_TARGET_TRIPLET "x86-windows-wchar-filenames")

project (
    sf4e
    VERSION 0.0.1
    DESCRIPTION "A process-inspection and modification tool for the Steam release of Ultra Street Fighter 4."
    HOMEPAGE_URL "https://gitlab.com/adanducci/sf4e"
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_compile_definitions(WIN32_LEAN_AND_MEAN)

find_library(LIB_DETOURS "detours")
find_package(GameNetworkingSockets CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_path(VALVEFILEVDF_INCLUDE_DIRS "vdf_parser.hpp")

add_library(Game STATIC
    "src/game/Dimps.hxx"
    "src/game/Dimps.cxx"
    "src/game/Dimps__Eva.hxx"
    "src/game/Dimps__Eva.cxx"
    "src/game/Dimps__Event.hxx"
    "src/game/Dimps__Event.cxx"
    "src/game/Dimps__Game.hxx"
    "src/game/Dimps__Game.cxx"

    "src/game/Dimps__Game__Battle.hxx"
    "src/game/Dimps__Game__Battle.cxx"
    "src/game/Dimps__Game__Battle__Action.hxx"
    "src/game/Dimps__Game__Battle__Action.cxx"
    "src/game/Dimps__Game__Battle__Camera.hxx"
    "src/game/Dimps__Game__Battle__Camera.cxx"
    "src/game/Dimps__Game__Battle__Chara.hxx"
    "src/game/Dimps__Game__Battle__Chara.cxx"
    "src/game/Dimps__Game__Battle__Chara__Actor.hxx"
    "src/game/Dimps__Game__Battle__Chara__Actor.cxx"
    "src/game/Dimps__Game__Battle__Chara__Unit.hxx"
    "src/game/Dimps__Game__Battle__Chara__Unit.cxx"
    "src/game/Dimps__Game__Battle__Command.hxx"
    "src/game/Dimps__Game__Battle__Command.cxx"
    "src/game/Dimps__Game__Battle__Command__CommandImpl.hxx"
    "src/game/Dimps__Game__Battle__Command__CommandImpl.cxx"
    "src/game/Dimps__Game__Battle__Command__Unit.hxx"
    "src/game/Dimps__Game__Battle__Command__Unit.cxx"
    "src/game/Dimps__Game__Battle__Effect.hxx"
    "src/game/Dimps__Game__Battle__Effect.cxx"
    "src/game/Dimps__Game__Battle__GameManager.hxx"
    "src/game/Dimps__Game__Battle__GameManager.cxx"
    "src/game/Dimps__Game__Battle__Hud.hxx"
    "src/game/Dimps__Game__Battle__Hud.cxx"
    "src/game/Dimps__Game__Battle__System.hxx"
    "src/game/Dimps__Game__Battle__System.cxx"
    "src/game/Dimps__Game__Battle__Training.hxx"
    "src/game/Dimps__Game__Battle__Training.cxx"
    "src/game/Dimps__Game__Battle__Vfx.hxx"
    "src/game/Dimps__Game__Battle__Vfx.cxx"

    "src/game/Dimps__GameEvents.hxx"
    "src/game/Dimps__GameEvents.cxx"
    "src/game/Dimps__Math.hxx"
    "src/game/Dimps__Math.cxx"
    "src/game/Dimps__Pad.hxx"
    "src/game/Dimps__Pad.cxx"
    "src/game/Dimps__Platform.hxx"
    "src/game/Dimps__Platform.cxx"
    "src/game/Dimps__UserApp.hxx"
    "src/game/Dimps__UserApp.cxx"

    "src/game/sf4e.hxx"
    "src/game/sf4e.cxx"
    "src/game/sf4e__Event.hxx"
    "src/game/sf4e__Event.cxx"
    "src/game/sf4e__Game.hxx"
    "src/game/sf4e__Game.cxx"
    "src/game/sf4e__Game__Battle__Action.hxx"
    "src/game/sf4e__Game__Battle__Action.cxx"
    "src/game/sf4e__Game__Battle__Effect.hxx"
    "src/game/sf4e__Game__Battle__Effect.cxx"
    "src/game/sf4e__Game__Battle__System.hxx"
    "src/game/sf4e__Game__Battle__System.cxx"
    "src/game/sf4e__Game__Battle__Vfx.hxx"
    "src/game/sf4e__Game__Battle__Vfx.cxx"
    "src/game/sf4e__GameEvents.hxx"
    "src/game/sf4e__GameEvents.cxx"
    "src/game/sf4e__Pad.hxx"
    "src/game/sf4e__Pad.cxx"
    "src/game/sf4e__Platform.hxx"
    "src/game/sf4e__Platform.cxx"
    "src/game/sf4e__SessionClient.hxx"
    "src/game/sf4e__SessionClient.cxx"
    "src/game/sf4e__SessionProtocol.hxx"
    "src/game/sf4e__SessionServer.hxx"
    "src/game/sf4e__SessionServer.cxx"
    "src/game/sf4e__UserApp.hxx"
    "src/game/sf4e__UserApp.cxx"
)
target_link_libraries(
    Game
    PRIVATE ${LIB_DETOURS} "ws2_32" "Mswsock" "AdvApi32" "Shlwapi"
    PUBLIC nlohmann_json::nlohmann_json spdlog::spdlog GameNetworkingSockets::shared
)

add_executable(Launcher WIN32 src/launcher/launcher.cxx)
target_include_directories(Launcher PRIVATE ${VALVEFILEVDF_INCLUDE_DIRS})
target_link_libraries(Launcher PRIVATE ${LIB_DETOURS} "shlwapi")

add_library(Overlay STATIC src/overlay/overlay.cxx src/overlay/overlay.h)
target_link_libraries(Overlay PRIVATE "Xinput9_1_0" imgui::imgui Game)

add_library(Sidecar SHARED src/sidecar/sidecar.cxx)
target_include_directories(Sidecar PRIVATE ${VALVEFILEVDF_INCLUDE_DIRS})
target_link_libraries(Sidecar PRIVATE ${LIB_DETOURS} Game Overlay)

target_compile_definitions(Launcher PRIVATE -D_UNICODE -DUNICODE)

install(
  TARGETS Launcher Sidecar
  CONFIGURATIONS Debug
  RUNTIME DESTINATION "."
)
install(
  TARGETS Launcher Sidecar
  CONFIGURATIONS Release
  RUNTIME DESTINATION "."
)

set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR "The ${PROJECT_NAME} Dev Team")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_NAME} ${PROJECT_DESCRIPTION}")
