kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Rules/Russia FILES TEMPLATE.ru mtu-intel_standart.rst )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA = TEMPLATE.ru mtu-intel_standart.rst
#
#pkgdir = $(kde_datadir)/kppp/Rules/Russia
#
#EXTRA_DIST = $(pkg_DATA)
