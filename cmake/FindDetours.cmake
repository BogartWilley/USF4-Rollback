include(FindPackageHandleStandardArgs)

set(DETOURS_DIR CACHE PATH "Location of a checkout of Detours.")

if(NOT DETOURS_DIR)
	set(DETOURS_DIR "${PROJECT_SOURCE_DIR}/../Detours/")
endif()

if(EXISTS "${DETOURS_DIR}/lib.X86/detours.lib")
	set(DETOURS_FOUND True)
	add_library(Detours STATIC IMPORTED)
	set_target_properties(Detours PROPERTIES IMPORTED_LOCATION "${DETOURS_DIR}/lib.X86/detours.lib")
	target_include_directories(Detours INTERFACE "${DETOURS_DIR}/include/")
else()
	set(DETOURS_FOUND False)
endif()