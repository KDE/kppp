kde4_header()




########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Provider/Ukraine FILES .directory Adamant IPTelecom NuVse )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA = .directory Adamant IPTelecom NuVse
#
#pkgdir = $(kde_datadir)/kppp/Provider/Ukraine
#
#EXTRA_DIST = $(pkg_DATA)
