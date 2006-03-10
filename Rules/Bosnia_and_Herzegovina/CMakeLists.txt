kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Rules/Bosnia_and_Herzegovina FILES BiHnet_-_home_-_bez_impulsa.rst  	BiHnet_-_student_-_bez_impulsa.rst          SmartNet_PERSONAL_bez_impulsa.rst   	samo_impulsi.rst )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA = BiHnet_-_home_-_bez_impulsa.rst \
#	BiHnet_-_student_-_bez_impulsa.rst \
#        SmartNet_PERSONAL_bez_impulsa.rst  \
#	samo_impulsi.rst
#
#pkgdir = $(kde_datadir)/kppp/Rules/Bosnia_and_Herzegovina
#
#EXTRA_DIST = $(pkg_DATA)
