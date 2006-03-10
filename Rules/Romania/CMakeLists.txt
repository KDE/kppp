kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Rules/Romania FILES Romtelecom_Acces_Special_Internet.rst             Romtelecom_Interjudetean.rst             Romtelecom_Local.rst             RomTelecom_GSM.rst             Zapp-Mobile.rst Easynet.rst )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA = Romtelecom_Acces_Special_Internet.rst \
#           Romtelecom_Interjudetean.rst \
#           Romtelecom_Local.rst \
#           RomTelecom_GSM.rst \
#           Zapp-Mobile.rst Easynet.rst
#
#pkgdir = $(kde_datadir)/kppp/Rules/Romania
#
#EXTRA_DIST = $(pkg_DATA)
