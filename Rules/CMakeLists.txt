kde4_header()

message(STATUS "${CMAKE_CURRENT_SOURCE_DIR}: skipped subdir $(AUTODIRS)")
include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Rules FILES TEMPLATE )

kde4_footer()



#original Makefile.am contents follow:

#SUBDIRS = $(AUTODIRS)
#
#pkg_DATA = TEMPLATE
#pkg_SCRIPTS = checkrules
#
#pkgdir = $(kde_datadir)/kppp/Rules
#
#EXTRA_DIST = $(pkg_DATA)
#
