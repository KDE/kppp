project(kppp)

check_include_files(sys/param.h HAVE_SYS_PARAM_H)
check_include_files(net/if_ppp.h HAVE_NET_IF_PPP_H)
check_include_files(linux/if_ppp.h HAVE_LINUX_IF_PPP_H)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config-kppp.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-kppp.h )


add_subdirectory( pixmaps ) 
add_subdirectory( icons ) 
add_subdirectory( logview ) 
add_subdirectory( Rules ) 
add_subdirectory( DB ) 


add_definitions ( -DQT3_SUPPORT -DQT3_SUPPORT_WARNINGS)

########### next target ###############

set(kppp_SRCS 
   kpppwidget.cpp 
   general.cpp 
   accounts.cpp 
   connect.cpp 
   conwindow.cpp 
   debug.cpp 
   edit.cpp 
   iplined.cpp 
   main.cpp 
   modem.cpp 
   modemcmds.cpp 
   pppdargs.cpp 
   pppdata.cpp 
   scriptedit.cpp 
   pwentry.cpp 
   modeminfo.cpp 
   pppstatdlg.cpp 
   pppstats.cpp 
   miniterm.cpp 
   accounting.cpp 
   acctselect.cpp 
   ruleset.cpp 
   docking.cpp 
   runtests.cpp 
   loginterm.cpp 
   ppplog.cpp 
   newwidget.cpp 
   requester.cpp 
   opener.cpp 
   modemdb.cpp 
   utils.cpp 
   providerdb.cpp 
   modems.cpp )

qt4_add_dbus_interfaces(kppp_SRCS org.kde.kppp.xml)
qt4_add_dbus_adaptor(kppp_SRCS org.kde.kppp.xml kpppwidget.h KPPPWidget)

kde4_add_executable(kppp ${kppp_SRCS})

target_link_libraries(kppp  ${KDE4_KDE3SUPPORT_LIBS} m )

if(KDE4_ENABLE_FPIE)
    macro_add_compile_flags(kppp ${KDE4_CXX_FPIE_FLAGS})
    macro_add_link_flags(kppp ${KDE4_PIE_LDFLAGS})
endif(KDE4_ENABLE_FPIE)


#TODO add message about install kppp as setuid 
MESSAGE(STATUS "Warning: kppp use setuid")
install(TARGETS kppp PERMISSIONS SETUID OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############
install( FILES org.kde.kppp.xml DESTINATION ${DBUS_INTERFACES_DIR} ) 
install( FILES Kppp.desktop  DESTINATION ${XDG_APPS_DIR})

