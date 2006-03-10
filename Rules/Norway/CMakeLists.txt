kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Rules/Norway FILES Local_Area.rst  		Long_Distance.rst  		Netcom.rst )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA =	Local_Area.rst \
#		Long_Distance.rst \
#		Netcom.rst
#
#pkgdir = $(kde_datadir)/kppp/Rules/Norway
#
#EXTRA_DIST = $(pkg_DATA)
