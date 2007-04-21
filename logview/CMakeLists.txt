
add_definitions ( -DQT3_SUPPORT -DQT3_SUPPORT_WARNINGS)

########### next target ###############

set(kppplogview_SRCS 
   main.cpp 
   loginfo.cpp 
   log.cpp 
   monthly.cpp 
   export.cpp )

kde4_automoc(${kppplogview_SRCS})

kde4_add_executable(kppplogview ${kppplogview_SRCS})

target_link_libraries(kppplogview  ${KDE4_KDE3SUPPORT_LIBS} m )

install(TARGETS kppplogview  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES kppplogview.desktop  DESTINATION ${XDG_APPS_DIR})
