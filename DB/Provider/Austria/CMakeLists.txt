kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Provider/Austria/ FILES .directory Simon%032Media )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA = .directory Simon%032Media
#
#pkgdir = $(kde_datadir)/kppp/Provider/Austria/
#
#EXTRA_DIST = $(pkg_DATA)
