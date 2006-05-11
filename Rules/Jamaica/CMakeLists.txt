kde4_header()




########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Rules/Jamaica FILES CWJ_InterParish.rst  		CWJ_Local.rst   )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA =	CWJ_InterParish.rst \
#		CWJ_Local.rst  
#
#pkgdir = $(kde_datadir)/kppp/Rules/Jamaica
#
#EXTRA_DIST = $(pkg_DATA)
