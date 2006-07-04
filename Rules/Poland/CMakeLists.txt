



########### install files ###############

install( FILES Netia_Lokalne_Niebieska.rst  		Netia_Lokalne_Zielona.rst  		Netia_100km_Niebieska.rst  		Netia_100km_Zielona.rst  		TPSA.rst  		Internetia.rst  		TP_Lokalne.rst  		TP_100km.rst  DESTINATION ${DATA_INSTALL_DIR}/kppp/Rules/Poland)




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
