



########### install files ###############

install( FILES Brasil.rst  	SaoPaulo.rst  	Brasil_Ligbr.rst  	Rio_de_Janeiro.rst  DESTINATION ${DATA_INSTALL_DIR}/kppp/Rules/Brasil)




#original Makefile.am contents follow:

#pkg_DATA = Brasil.rst \
#	SaoPaulo.rst \
#	Brasil_Ligbr.rst \
#	Rio_de_Janeiro.rst
#
#pkgdir = $(kde_datadir)/kppp/Rules/Brasil
#
#EXTRA_DIST = $(pkg_DATA)
