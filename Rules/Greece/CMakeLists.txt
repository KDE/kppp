kde4_header()




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
