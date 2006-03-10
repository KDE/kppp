kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Provider/Norway FILES .directory Institutt%032for%032informatikk BGNett )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA = .directory Institutt%032for%032informatikk BGNett
#
#pkgdir = $(kde_datadir)/kppp/Provider/Norway
#
#EXTRA_DIST = $(pkg_DATA)
