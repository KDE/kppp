kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Rules/India FILES Vsnl_local.rst BSNL_Local.rst  	 BSNL_Long_101_To_200.rst  	 BSNL_Medium_51_To_100.rst )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA = Vsnl_local.rst BSNL_Local.rst \
#	 BSNL_Long_101_To_200.rst \
#	 BSNL_Medium_51_To_100.rst
#
#pkgdir = $(kde_datadir)/kppp/Rules/India
#
#EXTRA_DIST = $(pkg_DATA)
