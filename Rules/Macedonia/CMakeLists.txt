kde4_header()




########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Rules/Macedonia FILES Macedonia_GenericISP_interurban.rst  	Macedonia_GenericISP_local.rst  	Macedonia_MTnet.rst )

kde4_footer()



#original Makefile.am contents follow:

#emo_DATA = Macedonia_GenericISP_interurban.rst \
#	Macedonia_GenericISP_local.rst \
#	Macedonia_MTnet.rst
#
#emodir = $(kde_datadir)/kppp/Rules/Macedonia
#
#EXTRA_DIST = $(emo_DATA)
