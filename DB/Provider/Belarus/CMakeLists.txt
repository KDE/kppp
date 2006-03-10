kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Provider/Belarus FILES .directory AtlantTelecom )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA = .directory AtlantTelecom
#
#pkgdir = $(kde_datadir)/kppp/Provider/Belarus
#
#EXTRA_DIST = $(pkg_DATA)
#
