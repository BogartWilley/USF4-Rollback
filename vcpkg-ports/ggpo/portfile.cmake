vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO pond3r/ggpo
    REF 7ddadef85
    SHA512 7c5134917d22fa4a9343ffa821b258e8a2b297d301e411618c855b8122d58890a77e3c5a248bc882bbdde36da5f230a41af2c0e172acdc47efbc22390f1d492b
    PATCHES
        "install-cmake-export.patch"
)

vcpkg_cmake_configure(
    SOURCE_PATH ${SOURCE_PATH}
    OPTIONS
        ${FEATURE_OPTIONS}
        -DGGPO_BUILD_SDK=ON
)

vcpkg_cmake_install()
vcpkg_cmake_config_fixup()
vcpkg_fixup_pkgconfig()
vcpkg_copy_pdbs()

file(REMOVE_RECURSE ${CURRENT_PACKAGES_DIR}/debug/include)

# Handle copyright
file(INSTALL ${SOURCE_PATH}/LICENSE DESTINATION ${CURRENT_PACKAGES_DIR}/share/${PORT} RENAME copyright)
