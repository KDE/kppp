



########### install files ###############

install( FILES Belgium_interzonal.rst Belgium_zonal.rst  	Belgium_internet_frank.rst Belgium_internet_euro.rst  DESTINATION ${DATA_INSTALL_DIR}/kppp/Rules/Belgium)




#original Makefile.am contents follow:

#rules_DATA = Belgium_interzonal.rst Belgium_zonal.rst \
#	Belgium_internet_frank.rst Belgium_internet_euro.rst
#
#rulesdir = $(kde_datadir)/kppp/Rules/Belgium
#
#EXTRA_DIST = $(rules_DATA)
