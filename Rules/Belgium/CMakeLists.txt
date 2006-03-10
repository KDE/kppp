kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Rules/Belgium FILES Belgium_interzonal.rst Belgium_zonal.rst  	Belgium_internet_frank.rst Belgium_internet_euro.rst )

kde4_footer()



#original Makefile.am contents follow:

#rules_DATA = Belgium_interzonal.rst Belgium_zonal.rst \
#	Belgium_internet_frank.rst Belgium_internet_euro.rst
#
#rulesdir = $(kde_datadir)/kppp/Rules/Belgium
#
#EXTRA_DIST = $(rules_DATA)
