include(FindPackageHandleStandardArgs)

set(DEARIMGUI_DIR CACHE PATH "Location of a checkout of dear-imgui.")

if(NOT DEARIMGUI_DIR)
	set(DEARIMGUI_DIR "${PROJECT_SOURCE_DIR}/../imgui/")
endif()

if(EXISTS "${DEARIMGUI_DIR}/imgui.h")
	set(DEARIMGUI_FOUND True)

	add_library(DearImgui STATIC
        "${DEARIMGUI_DIR}/imgui_demo.cpp"
        "${DEARIMGUI_DIR}/imgui_draw.cpp"        
        "${DEARIMGUI_DIR}/imgui.cpp"
        "${DEARIMGUI_DIR}/imgui_widgets.cpp"
    )
    target_include_directories(DearImgui PUBLIC "${DEARIMGUI_DIR}")

    add_library(DearImgui_Win32 STATIC
        "${DEARIMGUI_DIR}/examples/imgui_impl_win32.cpp"
        "${DEARIMGUI_DIR}/examples/imgui_impl_win32.h"
    )
    target_include_directories(DearImgui_Win32 PUBLIC "${DEARIMGUI_DIR}/examples")
    target_link_libraries(DearImgui_Win32 DearImgui)

    add_library(DearImgui_DX9 STATIC "${DEARIMGUI_DIR}/examples/imgui_impl_dx9.cpp")
    target_include_directories(DearImgui_DX9 PUBLIC "${DEARIMGUI_DIR}/examples")
    target_link_libraries(DearImgui_DX9 DearImgui)
else()
	set(DEARIMGUI_FOUND False)
endif()