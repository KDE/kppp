kde4_header()




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
