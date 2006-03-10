kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Rules/Poland FILES Netia_Lokalne_Niebieska.rst  		Netia_Lokalne_Zielona.rst  		Netia_100km_Niebieska.rst  		Netia_100km_Zielona.rst  		TPSA.rst  		Internetia.rst  		TP_Lokalne.rst  		TP_100km.rst )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA =	Netia_Lokalne_Niebieska.rst \
#		Netia_Lokalne_Zielona.rst \
#		Netia_100km_Niebieska.rst \
#		Netia_100km_Zielona.rst \
#		TPSA.rst \
#		Internetia.rst \
#		TP_Lokalne.rst \
#		TP_100km.rst
#
#pkgdir = $(kde_datadir)/kppp/Rules/Poland
#
#EXTRA_DIST = $(pkg_DATA)
