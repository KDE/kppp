kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Rules/South_Africa FILES South_Africa_local.rst  South_Africa_long_distance.rst )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA = South_Africa_local.rst  South_Africa_long_distance.rst
#
#pkgdir = $(kde_datadir)/kppp/Rules/South_Africa
#
#EXTRA_DIST = $(pkg_DATA)
