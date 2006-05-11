kde4_header()




########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Provider/Portugal FILES .directory OniNet Clix Netc )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA = .directory OniNet Clix Netc
#
#pkgdir = $(kde_datadir)/kppp/Provider/Portugal
#
#EXTRA_DIST = $(pkg_DATA)
