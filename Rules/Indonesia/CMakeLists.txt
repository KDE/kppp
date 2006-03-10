kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Rules/Indonesia FILES Lokal_1_Metropolitan.rst  		Lokal_non_metropolitan.rst  		Lokal_2_Metropolitan.rst )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA =	Lokal_1_Metropolitan.rst \
#		Lokal_non_metropolitan.rst \
#		Lokal_2_Metropolitan.rst
#
#pkgdir = $(kde_datadir)/kppp/Rules/Indonesia
#
#EXTRA_DIST = $(pkg_DATA)
