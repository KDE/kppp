kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Rules/Malaysia FILES malaysia.rst TMNet_Jaring.rst )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA = malaysia.rst TMNet_Jaring.rst
#
#pkgdir = $(kde_datadir)/kppp/Rules/Malaysia
#
#EXTRA_DIST = $(pkg_DATA)
