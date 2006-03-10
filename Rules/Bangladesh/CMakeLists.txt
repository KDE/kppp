kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Rules/Bangladesh FILES ATT00007.rst  		ATT00010.rst )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA = 	ATT00007.rst \
#		ATT00010.rst
#
#pkgdir = $(kde_datadir)/kppp/Rules/Bangladesh
#
#EXTRA_DIST = $(pkg_DATA)
