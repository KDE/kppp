kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Rules/Argentina FILES Argentina_Local.rst Argentina_0610.rst )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA = Argentina_Local.rst Argentina_0610.rst
#
#pkgdir = $(kde_datadir)/kppp/Rules/Argentina
#
#EXTRA_DIST = $(pkg_DATA)
#
#
#
