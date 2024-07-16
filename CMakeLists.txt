cmake_minimum_required (VERSION 3.20)

project (
    sf4e
    VERSION 0.0.1
    DESCRIPTION "A process-inspection and modification tool for the Steam release of Ultra Street Fighter 4."
    HOMEPAGE_URL "https://gitlab.com/adanducci/sf4e"
)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_compile_definitions(WIN32_LEAN_AND_MEAN)

find_library(LIB_DETOURS "detours")
find_package(GameNetworkingSockets CONFIG REQUIRED)
find_package(ggpo CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_path(VALVEFILEVDF_INCLUDE_DIRS "vdf_parser.hpp")

add_library(Dimps STATIC
    "src/Dimps/Dimps.hxx"
    "src/Dimps/Dimps.cxx"
    "src/Dimps/Dimps__Eva.hxx"
    "src/Dimps/Dimps__Eva.cxx"
    "src/Dimps/Dimps__Event.hxx"
    "src/Dimps/Dimps__Event.cxx"
    "src/Dimps/Dimps__Game.hxx"
    "src/Dimps/Dimps__Game.cxx"
    "src/Dimps/Dimps__Game__Battle.hxx"
    "src/Dimps/Dimps__Game__Battle.cxx"
    "src/Dimps/Dimps__Game__Battle__Action.hxx"
    "src/Dimps/Dimps__Game__Battle__Action.cxx"
    "src/Dimps/Dimps__Game__Battle__Camera.hxx"
    "src/Dimps/Dimps__Game__Battle__Camera.cxx"
    "src/Dimps/Dimps__Game__Battle__Chara.hxx"
    "src/Dimps/Dimps__Game__Battle__Chara.cxx"
    "src/Dimps/Dimps__Game__Battle__Command.hxx"
    "src/Dimps/Dimps__Game__Battle__Command.cxx"
    "src/Dimps/Dimps__Game__Battle__Effect.hxx"
    "src/Dimps/Dimps__Game__Battle__Effect.cxx"
    "src/Dimps/Dimps__Game__Battle__Hud.hxx"
    "src/Dimps/Dimps__Game__Battle__Hud.cxx"
    "src/Dimps/Dimps__Game__Battle__System.hxx"
    "src/Dimps/Dimps__Game__Battle__System.cxx"
    "src/Dimps/Dimps__Game__Battle__Training.hxx"
    "src/Dimps/Dimps__Game__Battle__Training.cxx"
    "src/Dimps/Dimps__Game__Battle__Vfx.hxx"
    "src/Dimps/Dimps__Game__Battle__Vfx.cxx"
    "src/Dimps/Dimps__GameEvents.hxx"
    "src/Dimps/Dimps__GameEvents.cxx"
    "src/Dimps/Dimps__Math.hxx"
    "src/Dimps/Dimps__Math.cxx"
    "src/Dimps/Dimps__Pad.hxx"
    "src/Dimps/Dimps__Pad.cxx"
    "src/Dimps/Dimps__Platform.hxx"
    "src/Dimps/Dimps__Platform.cxx"
    "src/Dimps/Dimps__UserApp.hxx"
    "src/Dimps/Dimps__UserApp.cxx"
)
target_link_libraries(
    Dimps
    PUBLIC nlohmann_json::nlohmann_json
)

add_library(sf4e STATIC
    "src/sf4e/sf4e.hxx"
    "src/sf4e/sf4e.cxx"
    "src/sf4e/sf4e__Event.hxx"
    "src/sf4e/sf4e__Event.cxx"
    "src/sf4e/sf4e__Game.hxx"
    "src/sf4e/sf4e__Game.cxx"
    "src/sf4e/sf4e__Game__Battle.hxx"
    "src/sf4e/sf4e__Game__Battle.cxx"
    "src/sf4e/sf4e__Game__Battle__Effect.hxx"
    "src/sf4e/sf4e__Game__Battle__Effect.cxx"
    "src/sf4e/sf4e__Game__Battle__Hud.hxx"
    "src/sf4e/sf4e__Game__Battle__Hud.cxx"
    "src/sf4e/sf4e__Game__Battle__System.hxx"
    "src/sf4e/sf4e__Game__Battle__System.cxx"
    "src/sf4e/sf4e__Game__Battle__Vfx.hxx"
    "src/sf4e/sf4e__Game__Battle__Vfx.cxx"
    "src/sf4e/sf4e__GameEvents.hxx"
    "src/sf4e/sf4e__GameEvents.cxx"
    "src/sf4e/sf4e__Pad.hxx"
    "src/sf4e/sf4e__Pad.cxx"
    "src/sf4e/sf4e__Platform.hxx"
    "src/sf4e/sf4e__Platform.cxx"
    "src/sf4e/sf4e__UserApp.hxx"
    "src/sf4e/sf4e__UserApp.cxx"
)
target_link_libraries(
    sf4e
    PRIVATE ${LIB_DETOURS} Session "Kernel32" "ws2_32" "Mswsock" "AdvApi32" "Shlwapi" "Bcrypt"
    PUBLIC nlohmann_json::nlohmann_json spdlog::spdlog GameNetworkingSockets::shared ggpo::GGPO
)

add_library(Session STATIC
    "src/session/sf4e__SessionClient.hxx"
    "src/session/sf4e__SessionClient.cxx"
    "src/session/sf4e__SessionProtocol.hxx"
    "src/session/sf4e__SessionProtocol.cxx"
    "src/session/sf4e__SessionServer.hxx"
    "src/session/sf4e__SessionServer.cxx"
)
target_link_libraries(
    Session
    PRIVATE Dimps
    PUBLIC nlohmann_json::nlohmann_json spdlog::spdlog GameNetworkingSockets::shared ggpo::GGPO
)

add_executable(Launcher WIN32 src/launcher/launcher.cxx)
target_include_directories(Launcher PRIVATE ${VALVEFILEVDF_INCLUDE_DIRS})
target_link_libraries(Launcher PRIVATE ${LIB_DETOURS} spdlog::spdlog "shlwapi" "pathcch")

add_library(Overlay STATIC src/overlay/overlay.cxx src/overlay/overlay.h)
target_link_libraries(Overlay PRIVATE "Xinput9_1_0" imgui::imgui ggpo::GGPO spdlog::spdlog Dimps sf4e Session)

add_library(Sidecar SHARED src/sidecar/sidecar.cxx)
target_include_directories(Sidecar PRIVATE ${VALVEFILEVDF_INCLUDE_DIRS})
target_link_libraries(Sidecar PRIVATE ${LIB_DETOURS} Dimps sf4e Overlay)

target_compile_definitions(Launcher PRIVATE -D_UNICODE -DUNICODE)

install(
  TARGETS Launcher Sidecar
  RUNTIME DESTINATION "."
)

set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR "The ${PROJECT_NAME} Dev Team")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_NAME} ${PROJECT_DESCRIPTION}")
