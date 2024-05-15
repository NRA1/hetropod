add_library(hetropod SHARED
        Hetropod.h
        Hetropod.cpp
        HetropodState.cpp
        HetropodState.h
        Alphabet.cpp
        Alphabet.h
        Keyboard.cpp
        Keyboard.h
        Numerals.cpp
        Numerals.h)
target_link_libraries(hetropod PRIVATE Fcitx5::Core)
set_target_properties(hetropod PROPERTIES PREFIX "")
install(TARGETS hetropod DESTINATION "${FCITX_INSTALL_LIBDIR}/fcitx5")

configure_file(hetropod-addon.conf.in hetropod-addon.conf)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/hetropod-addon.conf" RENAME hetropod.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/addon")

install(FILES "hetropod.conf.in" RENAME hetropod.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")