kde4_header()




########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Rules/Austria/Minimum FILES Local.rst  		Long_Distance.rst  		Online.rst )

kde4_footer()



#original Makefile.am contents follow:

#rules_DATA =	Local.rst \
#		Long_Distance.rst \
#		Online.rst
#
#rulesdir = $(kde_datadir)/kppp/Rules/Austria/Minimum
#
#EXTRA_DIST = $(rules_DATA)
