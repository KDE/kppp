



########### install files ###############

install( FILES Vsnl_local.rst BSNL_Local.rst  	 BSNL_Long_101_To_200.rst  	 BSNL_Medium_51_To_100.rst  DESTINATION ${DATA_INSTALL_DIR}/kppp/Rules/India)




#original Makefile.am contents follow:

#pkg_DATA = Vsnl_local.rst BSNL_Local.rst \
#	 BSNL_Long_101_To_200.rst \
#	 BSNL_Medium_51_To_100.rst
#
#pkgdir = $(kde_datadir)/kppp/Rules/India
#
#EXTRA_DIST = $(pkg_DATA)
