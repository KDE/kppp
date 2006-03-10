kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Rules/Sweden FILES ACN.rst Abonnera_com.rst  	CNEAB-Route66.rst Crossnet-Affinity.rst Glocalnet.rst Gts.rst  	Home_se.rst Nemtel.rst Plusenergi.rst RSLCom.rst  	Rix_Telecom.rst Supertel.rst Svensk_Telekom.rst  	Tele1_Europe.rst Tele2.rst Tele8.rst Teleman.rst  	Telenordia.rst Telerian.rst Telia.rst Telia_Telebonus1.rst  	Telia_Telebonus2.rst Telitel.rst Transnet.rst  	Universal_Telecom.rst Utfors.rst Vattenfall.rst Tiscali.rst )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA = ACN.rst Abonnera_com.rst \
#	CNEAB-Route66.rst Crossnet-Affinity.rst Glocalnet.rst Gts.rst \
#	Home_se.rst Nemtel.rst Plusenergi.rst RSLCom.rst \
#	Rix_Telecom.rst Supertel.rst Svensk_Telekom.rst \
#	Tele1_Europe.rst Tele2.rst Tele8.rst Teleman.rst \
#	Telenordia.rst Telerian.rst Telia.rst Telia_Telebonus1.rst \
#	Telia_Telebonus2.rst Telitel.rst Transnet.rst \
#	Universal_Telecom.rst Utfors.rst Vattenfall.rst Tiscali.rst
#
#pkgdir = $(kde_datadir)/kppp/Rules/Sweden
#
#EXTRA_DIST = $(pkg_DATA)
