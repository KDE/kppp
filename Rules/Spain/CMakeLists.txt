kde4_header()




########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Rules/Spain FILES Infovia.rst Telefonica_Interprovincial.rst  	Infovia_IVA.rst Telefonica_Interprovincial_IVA.rst  	Telefonica_Metropolitana.rst Telefonica_Metropolitana_IVA.rst  	Telefonica_Nacional.rst Telefonica_Provincial.rst  	Telefonica_Provincial_IVA.rst Telefonica_Local.rst )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA = Infovia.rst Telefonica_Interprovincial.rst \
#	Infovia_IVA.rst Telefonica_Interprovincial_IVA.rst \
#	Telefonica_Metropolitana.rst Telefonica_Metropolitana_IVA.rst \
#	Telefonica_Nacional.rst Telefonica_Provincial.rst \
#	Telefonica_Provincial_IVA.rst Telefonica_Local.rst
#
#pkgdir = $(kde_datadir)/kppp/Rules/Spain
#
#EXTRA_DIST = $(pkg_DATA)
