kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Rules/Greece FILES Hellas_digital_local_in_euro.rst  	Hellas_EPAK_Zone1_in_euro.rst  	Hellas_EPAK_Zone2_in_euro.rst  	Hellas_EPAK_local_in_euro.rst  	Hellas_analog_local_in_euro.rst )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA = Hellas_digital_local_in_euro.rst \
#	Hellas_EPAK_Zone1_in_euro.rst \
#	Hellas_EPAK_Zone2_in_euro.rst \
#	Hellas_EPAK_local_in_euro.rst \
#	Hellas_analog_local_in_euro.rst
#
#pkgdir = $(kde_datadir)/kppp/Rules/Greece
#
#EXTRA_DIST = $(pkg_DATA)
