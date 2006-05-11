kde4_header()




########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Rules/Hong_Kong FILES Hong_Kong_Telecom.rst )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA = Hong_Kong_Telecom.rst
#
#pkgdir = $(kde_datadir)/kppp/Rules/Hong_Kong
#
#EXTRA_DIST = $(pkg_DATA)
