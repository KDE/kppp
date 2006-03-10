kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Provider/Czech_Republic FILES .directory  	AICOM ARsystem ASYS ApexNet  	BohemiaNet Brailcom  	CITYNET Contactel  	ES-servis Econnect  	Falco_computer Fortech  	HP-NET  	INTERNET_OnLine INTERNEXT IQNET  	KPNQuest  	M-soft  	Nextra  	ProfiNet  	SeverNET  	Video_OnLine Volny )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA = .directory \
#	AICOM ARsystem ASYS ApexNet \
#	BohemiaNet Brailcom \
#	CITYNET Contactel \
#	ES-servis Econnect \
#	Falco_computer Fortech \
#	HP-NET \
#	INTERNET_OnLine INTERNEXT IQNET \
#	KPNQuest \
#	M-soft \
#	Nextra \
#	ProfiNet \
#	SeverNET \
#	Video_OnLine Volny
#
#pkgdir = $(kde_datadir)/kppp/Provider/Czech_Republic
#
#EXTRA_DIST = $(pkg_DATA)
