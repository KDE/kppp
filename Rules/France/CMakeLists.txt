kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Rules/France FILES France_Telecom_Local.rst France_Telecom_Internet.rst France_Telecom_National.rst      Cegetel_Local.rst Cegetel_National.rst Le_9_Local.rst Le_9_National.rst Tele2_Local.rst Tele2_National.rst      OneTel.rst Wanadoo_Free.rst )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA = France_Telecom_Local.rst France_Telecom_Internet.rst France_Telecom_National.rst \
#    Cegetel_Local.rst Cegetel_National.rst Le_9_Local.rst Le_9_National.rst Tele2_Local.rst Tele2_National.rst \
#    OneTel.rst Wanadoo_Free.rst
#
#pkgdir = $(kde_datadir)/kppp/Rules/France
#
#EXTRA_DIST = $(pkg_DATA)
