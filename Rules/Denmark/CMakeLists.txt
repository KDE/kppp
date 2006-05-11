kde4_header()




########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Rules/Denmark FILES 12Move_Analog.rst 12Move_ISDN.rst  	Cybercity_Friabonnement.rst  	Get2net_Betaling.rst Get2net_Gratis.rst  	Mobilix_Wanadoo.rst  	Teledanmark_Basis.rst Teledanmark_Favoritinternet.rst  	Worldonline-Analog.rst Worldonline-ISDN.rst )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA = 12Move_Analog.rst 12Move_ISDN.rst \
#	Cybercity_Friabonnement.rst \
#	Get2net_Betaling.rst Get2net_Gratis.rst \
#	Mobilix_Wanadoo.rst \
#	Teledanmark_Basis.rst Teledanmark_Favoritinternet.rst \
#	Worldonline-Analog.rst Worldonline-ISDN.rst
#
#pkgdir = $(kde_datadir)/kppp/Rules/Denmark
#
#EXTRA_DIST = $(pkg_DATA)
