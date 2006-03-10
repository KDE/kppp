kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Provider/United_Kingdom FILES .directory FreeServe Demon%032Green%0322120666 Demon%032Purple%0322121666  	Demon%032Red%0320798666  	UK%032Free%032Software%032Network%032ISDN UKPOST%032ISDN  	UK%032Free%032Software%032Network%032Modem UKPOST%032Modem )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA = .directory FreeServe Demon%032Green%0322120666 Demon%032Purple%0322121666 \
#	Demon%032Red%0320798666 \
#	UK%032Free%032Software%032Network%032ISDN UKPOST%032ISDN \
#	UK%032Free%032Software%032Network%032Modem UKPOST%032Modem
#
#pkgdir = $(kde_datadir)/kppp/Provider/United_Kingdom
#
#EXTRA_DIST = $(pkg_DATA)
