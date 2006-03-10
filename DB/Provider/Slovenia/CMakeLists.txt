kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Provider/Slovenia FILES AmisNet Arnes Kiss MojNet SiOL Volja )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA =	AmisNet Arnes Kiss MojNet SiOL Volja
#
#pkgdir = $(kde_datadir)/kppp/Provider/Slovenia
#
#EXTRA_DIST = $(pkg_DATA)
