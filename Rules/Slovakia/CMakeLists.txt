kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Rules/Slovakia FILES Internetovy_tarif_019XY.rst  	ST_medzimesto.rst  	ST_mesto.rst )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA = Internetovy_tarif_019XY.rst \
#	ST_medzimesto.rst \
#	ST_mesto.rst
#
#pkgdir = $(kde_datadir)/kppp/Rules/Slovakia
#
#EXTRA_DIST = $(pkg_DATA)
