



########### install files ###############

install( FILES BirminghamCable_Local.rst  		BirminghamCable_National.rst  		BirminghamCable_SameTelco.rst  		BritishTelecom_Local.rst  		BritishTelecom_National.rst  		BritishTelecom_Regional.rst  		British_OneTel.rst  		Connaught_Telecom.rst  DESTINATION ${DATA_INSTALL_DIR}/kppp/Rules/United_Kingdom)




#original Makefile.am contents follow:

#pkg_DATA =	BirminghamCable_Local.rst \
#		BirminghamCable_National.rst \
#		BirminghamCable_SameTelco.rst \
#		BritishTelecom_Local.rst \
#		BritishTelecom_National.rst \
#		BritishTelecom_Regional.rst \
#		British_OneTel.rst \
#		Connaught_Telecom.rst
#
#pkgdir = $(kde_datadir)/kppp/Rules/United_Kingdom
#
#EXTRA_DIST = $(pkg_DATA)
