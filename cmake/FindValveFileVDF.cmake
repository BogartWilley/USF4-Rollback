include(FindPackageHandleStandardArgs)

set(VALVEFILEVDF_DIR CACHE PATH "Location of a checkout of ValveFileVDF.")

if(NOT VALVEFILEVDF_DIR)
	set(VALVEFILEVDF_DIR "${PROJECT_SOURCE_DIR}/../ValveFileVDF/")
endif()

if(EXISTS "${VALVEFILEVDF_DIR}/vdf_parser.hpp")
	set(VALVEFILEVDF_FOUND True)
	add_library(ValveFileVDF INTERFACE IMPORTED)
	target_include_directories(ValveFileVDF INTERFACE "${VALVEFILEVDF_DIR}/")
else()
	set(VALVEFILEVDF_FOUND False)
endif()