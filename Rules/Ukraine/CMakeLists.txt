kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Rules/Ukraine FILES TEMPLATE.uk Utel_Unet.rst IPTelecom_hourly.rst NuVse_hourly.rst )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA = TEMPLATE.uk Utel_Unet.rst IPTelecom_hourly.rst NuVse_hourly.rst
#
#pkgdir = $(kde_datadir)/kppp/Rules/Ukraine
#
#EXTRA_DIST = $(pkg_DATA)
