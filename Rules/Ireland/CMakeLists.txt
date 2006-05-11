kde4_header()




########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Rules/Ireland FILES Eircom_Internet.rst  		Eircom_Local.rst  		Eircom_National.rst  		Eircom_Special.rst )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA =    	Eircom_Internet.rst \
#		Eircom_Local.rst \
#		Eircom_National.rst \
#		Eircom_Special.rst
#
#pkgdir = $(kde_datadir)/kppp/Rules/Ireland
#
#EXTRA_DIST = $(pkg_DATA)
