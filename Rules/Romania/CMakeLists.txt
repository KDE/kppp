



########### install files ###############

install( FILES Romtelecom_Acces_Special_Internet.rst             Romtelecom_Interjudetean.rst             Romtelecom_Local.rst             RomTelecom_GSM.rst             Zapp-Mobile.rst Easynet.rst  DESTINATION ${DATA_INSTALL_DIR}/kppp/Rules/Romania)




#original Makefile.am contents follow:

#pkg_DATA = Romtelecom_Acces_Special_Internet.rst \
#           Romtelecom_Interjudetean.rst \
#           Romtelecom_Local.rst \
#           RomTelecom_GSM.rst \
#           Zapp-Mobile.rst Easynet.rst
#
#pkgdir = $(kde_datadir)/kppp/Rules/Romania
#
#EXTRA_DIST = $(pkg_DATA)
