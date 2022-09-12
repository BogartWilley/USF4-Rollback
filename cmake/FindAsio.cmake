include(FindPackageHandleStandardArgs)

set(ASIO_DIR CACHE PATH "Location of a checkout of Asio.")

if(NOT ASIO_DIR)
	set(ASIO_DIR "${PROJECT_SOURCE_DIR}/../asio/")
endif()

if(EXISTS "${ASIO_DIR}/include/asio.hpp")
	set(ASIO_FOUND True)
	add_library(Asio INTERFACE IMPORTED)
    target_include_directories(Asio INTERFACE "${ASIO_DIR}/include/")
else()
	set(ASIO_FOUND False)
endif()