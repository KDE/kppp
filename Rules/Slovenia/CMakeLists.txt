kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Rules/Slovenia FILES omrezje_0880.rst  		omrezje_0889.rst  		stacionarno_omrezje.rst )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA = 	omrezje_0880.rst \
#		omrezje_0889.rst \
#		stacionarno_omrezje.rst
#
#pkgdir = $(kde_datadir)/kppp/Rules/Slovenia
#
#EXTRA_DIST = $(pkg_DATA)
