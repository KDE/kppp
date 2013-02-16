project(kppp)

if(NOT INSIDE_KDENETWORK)
    message("Not building inside KDENetwork, loading KDE CMake Macros.")

    find_package(KDE4 REQUIRED)

    include(KDE4Defaults)
    include(MacroLibrary)

    include(CheckIncludeFile)
    include(CheckIncludeFiles)
    include(CheckSymbolExists)
    include(CheckFunctionExists)
    include(CheckLibraryExists)
    include(CheckPrototypeExists)
    include(CheckTypeSize)

    set(CMAKE_REQUIRED_DEFINITIONS ${_KDE4_PLATFORM_DEFINITIONS})
    if(WIN32)
       set(CMAKE_REQUIRED_LIBRARIES ${KDEWIN32_LIBRARIES})
       set(CMAKE_REQUIRED_INCLUDES  ${KDEWIN32_INCLUDES})
    endif(WIN32)
    add_definitions(${QT_DEFINITIONS} ${QT_QTDBUS_DEFINITIONS} ${KDE4_DEFINITIONS})
    add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
    include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES})
endif(NOT INSIDE_KDENETWORK)

check_include_files(sys/param.h HAVE_SYS_PARAM_H)
check_include_files(net/if_ppp.h HAVE_NET_IF_PPP_H)
check_include_files(linux/if_ppp.h HAVE_LINUX_IF_PPP_H)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config-kppp.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-kppp.h )


add_subdirectory( pixmaps ) 
add_subdirectory( icons ) 
add_subdirectory( logview ) 
add_subdirectory( Rules ) 
add_subdirectory( DB ) 

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
install( FILES org.kde.kppp.xml DESTINATION ${DBUS_INTERFACES_INSTALL_DIR} ) 
install( PROGRAMS Kppp.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR})

if (NOT INSIDE_KDENETWORK)
    macro_display_feature_log()
endif ()
