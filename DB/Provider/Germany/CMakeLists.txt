kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Provider/Germany FILES .directory CityWeb Netsurf  	FH%032Rhein%032Sieg%032%040Informatik%041 MUC%046DE )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA = .directory CityWeb Netsurf \
#	FH%032Rhein%032Sieg%032%040Informatik%041 MUC%046DE
#
#pkgdir = $(kde_datadir)/kppp/Provider/Germany
#
#EXTRA_DIST = $(pkg_DATA)
