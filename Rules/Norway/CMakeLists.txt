kde4_header()




########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Rules/Norway FILES Local_Area.rst  		Long_Distance.rst  		Netcom.rst )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA =	Local_Area.rst \
#		Long_Distance.rst \
#		Netcom.rst
#
#pkgdir = $(kde_datadir)/kppp/Rules/Norway
#
#EXTRA_DIST = $(pkg_DATA)
