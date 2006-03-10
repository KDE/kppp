kde4_header()

add_subdirectory( Provider ) 

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Provider FILES  )

kde4_footer()



#original Makefile.am contents follow:

#SUBDIRS = Provider
#
#pkgdir = $(kde_datadir)/kppp/Provider
