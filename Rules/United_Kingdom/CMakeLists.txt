kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Rules/United_Kingdom FILES BirminghamCable_Local.rst  		BirminghamCable_National.rst  		BirminghamCable_SameTelco.rst  		BritishTelecom_Local.rst  		BritishTelecom_National.rst  		BritishTelecom_Regional.rst  		British_OneTel.rst  		Connaught_Telecom.rst )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA =	BirminghamCable_Local.rst \
#		BirminghamCable_National.rst \
#		BirminghamCable_SameTelco.rst \
#		BritishTelecom_Local.rst \
#		BritishTelecom_National.rst \
#		BritishTelecom_Regional.rst \
#		British_OneTel.rst \
#		Connaught_Telecom.rst
#
#pkgdir = $(kde_datadir)/kppp/Rules/United_Kingdom
#
#EXTRA_DIST = $(pkg_DATA)
