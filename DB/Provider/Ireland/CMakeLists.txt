kde4_header()




########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Provider/Irland FILES .directory Eircom  UTV  UVT IOL )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA = .directory Eircom  UTV  UVT IOL
#
#pkgdir = $(kde_datadir)/kppp/Provider/Irland
#
#EXTRA_DIST = $(pkg_DATA)
