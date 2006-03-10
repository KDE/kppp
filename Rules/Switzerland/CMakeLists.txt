kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Rules/Switzerland FILES Swisscom_Local.rst  		Swisscom_Remote.rst  		Swisscom_Surf.rst  		Sunrise_Freetime.rst  		Sunrise_Select_Internet.rst  		Sunrise_Local.rst )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA =	Swisscom_Local.rst \
#		Swisscom_Remote.rst \
#		Swisscom_Surf.rst \
#		Sunrise_Freetime.rst \
#		Sunrise_Select_Internet.rst \
#		Sunrise_Local.rst
#
#pkgdir = $(kde_datadir)/kppp/Rules/Switzerland
#
#EXTRA_DIST = $(pkg_DATA)
