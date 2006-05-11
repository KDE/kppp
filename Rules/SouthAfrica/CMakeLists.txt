kde4_header()




########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Rules/South_Africa FILES South_Africa_local.rst  South_Africa_long_distance.rst )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA = South_Africa_local.rst  South_Africa_long_distance.rst
#
#pkgdir = $(kde_datadir)/kppp/Rules/South_Africa
#
#EXTRA_DIST = $(pkg_DATA)
