kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Rules/Kazakhstan FILES Akparat_Sprint.rst )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA = Akparat_Sprint.rst
#
#pkgdir = $(kde_datadir)/kppp/Rules/Kazakhstan
#
#EXTRA_DIST = $(pkg_DATA)
