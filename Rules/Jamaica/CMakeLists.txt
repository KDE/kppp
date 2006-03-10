kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Rules/Jamaica FILES CWJ_InterParish.rst  		CWJ_Local.rst   )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA =	CWJ_InterParish.rst \
#		CWJ_Local.rst  
#
#pkgdir = $(kde_datadir)/kppp/Rules/Jamaica
#
#EXTRA_DIST = $(pkg_DATA)
