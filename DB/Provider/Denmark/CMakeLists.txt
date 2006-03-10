kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Provider/Denmark FILES .directory Get2Net )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA = .directory Get2Net
#
#pkgdir = $(kde_datadir)/kppp/Provider/Denmark
#
#EXTRA_DIST = $(pkg_DATA)
