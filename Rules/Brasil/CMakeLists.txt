kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Rules/Brasil FILES Brasil.rst  	SaoPaulo.rst  	Brasil_Ligbr.rst  	Rio_de_Janeiro.rst )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA = Brasil.rst \
#	SaoPaulo.rst \
#	Brasil_Ligbr.rst \
#	Rio_de_Janeiro.rst
#
#pkgdir = $(kde_datadir)/kppp/Rules/Brasil
#
#EXTRA_DIST = $(pkg_DATA)
