
########### next target ###############

set(kppplogview_SRCS 
   main.cpp 
   loginfo.cpp 
   log.cpp 
   monthly.cpp 
   export.cpp )

kde4_add_executable(kppplogview ${kppplogview_SRCS})

target_link_libraries(kppplogview  ${KDE4_KDE3SUPPORT_LIBS} m )

install(TARGETS kppplogview  ${INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( PROGRAMS kppplogview.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR})
