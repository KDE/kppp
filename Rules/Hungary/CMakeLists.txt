kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Rules/Hungary FILES Local.rst  		Local_MATAV.rst  		LocalTop_MATAV.rst  		PapaTel.rst )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA = 	Local.rst \
#		Local_MATAV.rst \
#		LocalTop_MATAV.rst \
#		PapaTel.rst
#
#pkgdir = $(kde_datadir)/kppp/Rules/Hungary
#
#EXTRA_DIST = $(pkg_DATA)
