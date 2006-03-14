kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )

add_subdirectory(Argentina)
add_subdirectory(Australia)
add_subdirectory(Austria)
add_subdirectory(Bangladesh)
add_subdirectory(Belgium)
add_subdirectory(Bosnia_and_Herzegovina)
add_subdirectory(Brasil)
add_subdirectory(Croatia)
add_subdirectory(Czechia)
add_subdirectory(Denmark)
add_subdirectory(Estonia)
add_subdirectory(Finland)
add_subdirectory(France)
add_subdirectory(Germany)
add_subdirectory(Greece)
add_subdirectory(HongKong)
add_subdirectory(Hungary)
add_subdirectory(Iceland)
add_subdirectory(India)
add_subdirectory(Indonesia)
add_subdirectory(Ireland)
add_subdirectory(Israel)
add_subdirectory(Italy)
add_subdirectory(Jamaica)
add_subdirectory(Japan)
add_subdirectory(Kazakhstan)
add_subdirectory(Luxembourg)
add_subdirectory(Macedonia)
add_subdirectory(Malaysia)
add_subdirectory(Netherlands)
add_subdirectory(Norway)
add_subdirectory(Poland)
add_subdirectory(Portugal)
add_subdirectory(Romania)
add_subdirectory(Russia)
add_subdirectory(Singapore)
add_subdirectory(Slovakia)
add_subdirectory(Slovenia)
add_subdirectory(SouthAfrica)
add_subdirectory(SouthAfrika)
add_subdirectory(Spain)
add_subdirectory(Sweden)
add_subdirectory(Switzerland)
add_subdirectory(Turkey)
add_subdirectory(Ukraine)
add_subdirectory(United_Kingdom)
add_subdirectory(Uruguay)
add_subdirectory(Yugoslavia)

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
