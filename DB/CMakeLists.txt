kde4_header()

add_subdirectory( Provider ) 




########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Provider FILES  )

kde4_footer()



#original Makefile.am contents follow:

#SUBDIRS = Provider
#
#pkgdir = $(kde_datadir)/kppp/Provider
