kde4_header()




########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Provider/France FILES .directory ClubInternetFull  )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA = .directory ClubInternetFull 
#
#pkgdir = $(kde_datadir)/kppp/Provider/France
#
#EXTRA_DIST = $(pkg_DATA)
