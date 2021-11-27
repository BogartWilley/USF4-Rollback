cmake_minimum_required (VERSION 3.10)
project (
    SF4X
    VERSION 0.0.1
    DESCRIPTION "A process-inspection and modification tool for the Steam release of Ultra Street Fighter 4."
    HOMEPAGE_URL "https://gitlab.com/adanducci/sf4x"
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package(DearImgui 1 REQUIRED)
find_package(Detours 4 REQUIRED)
find_package(ValveFileVDF 1 REQUIRED)

add_library(Game STATIC
    "src/game/Dimps.hxx"
    "src/game/Dimps.cxx"
    "src/game/Dimps__Game.hxx"
    "src/game/Dimps__Game.cxx"
    "src/game/Dimps__Game__Battle.hxx"
    "src/game/Dimps__Game__Battle.cxx"
    "src/game/Dimps__Game__Battle__Chara.hxx"
    "src/game/Dimps__Game__Battle__Chara.cxx"
    "src/game/Dimps__Game__Battle__Chara__Actor.hxx"
    "src/game/Dimps__Game__Battle__Chara__Actor.cxx"
    "src/game/Dimps__Game__Battle__Chara__Unit.hxx"
    "src/game/Dimps__Game__Battle__Chara__Unit.cxx"
    "src/game/Dimps__Game__Battle__System.hxx"
    "src/game/Dimps__Game__Battle__System.cxx"
    "src/game/Dimps__Math.hxx"
    "src/game/Dimps__Math.cxx"
    "src/game/Dimps__Platform.hxx"
    "src/game/Dimps__Platform.cxx"
    "src/game/Dimps__Platform__D3D.hxx"
    "src/game/Dimps__Platform__D3D.cxx"
    "src/game/Dimps__Platform__Main.hxx"
    "src/game/Dimps__Platform__Main.cxx"

    "src/game/sf4x.hxx"
    "src/game/sf4x.cxx"
    "src/game/sf4x__Platform.hxx"
    "src/game/sf4x__Platform.cxx"
    "src/game/sf4x__Platform__D3D.hxx"
    "src/game/sf4x__Platform__D3D.cxx"
    "src/game/sf4x__Platform__Main.hxx"
    "src/game/sf4x__Platform__Main.cxx"
)
target_link_libraries(Game PUBLIC Detours "ws2_32" "Mswsock" "AdvApi32")

add_executable(Launcher WIN32
    
    
    src/launcher/launcher.cxx
)
target_link_libraries(Launcher
	PRIVATE Detours
	PRIVATE ValveFileVDF
	PUBLIC Sidecar
    PUBLIC "shlwapi"
)

add_library(Overlay STATIC src/overlay/overlay.cxx src/overlay/overlay.h)
target_link_libraries(Overlay PUBLIC "Xinput9_1_0" DearImgui DearImgui_Win32 DearImgui_DX9 Game)

add_library(Sidecar SHARED src/sidecar/sidecar.cxx)
target_link_libraries(Sidecar PRIVATE Detours Game Overlay ValveFileVDF)

target_compile_definitions(Launcher PRIVATE -D_UNICODE -DUNICODE)
target_compile_definitions(DearImgui_Win32 PRIVATE IMGUI_IMPL_WIN32_DISABLE_LINKING_XINPUT=1)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

install(
  TARGETS Launcher Game Overlay Sidecar DearImgui DearImgui_Win32 DearImgui_DX9
  CONFIGURATIONS Debug
  RUNTIME DESTINATION "."
)
install(
  TARGETS Launcher Game Overlay Sidecar DearImgui DearImgui_Win32 DearImgui_DX9
  CONFIGURATIONS Release
  RUNTIME DESTINATION "."
)


set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR "The ${PROJECT_NAME} Dev Team")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_NAME} ${PROJECT_DESCRIPTION}")
