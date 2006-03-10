kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


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
