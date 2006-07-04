



########### install files ###############

install( FILES PT_Local.rst PT_Local_Ilhas.rst PT_Regional.rst  	PT_Regional_Ilhas.rst PT_YesNET.rst PT_YesNET_Ilhas.rst  DESTINATION ${DATA_INSTALL_DIR}/kppp/Rules/Portugal)




#original Makefile.am contents follow:

#pkg_DATA = PT_Local.rst PT_Local_Ilhas.rst PT_Regional.rst \
#	PT_Regional_Ilhas.rst PT_YesNET.rst PT_YesNET_Ilhas.rst
#
#pkgdir = $(kde_datadir)/kppp/Rules/Portugal
#
#EXTRA_DIST = $(pkg_DATA)
