kde4_header()

add_subdirectory( Business_1 ) 
add_subdirectory( Business_2 ) 
add_subdirectory( Minimum ) 
add_subdirectory( Standard ) 

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############


kde4_footer()



#original Makefile.am contents follow:

#SUBDIRS =	Business_1 \
#		Business_2 \
#		Minimum \
#		Standard
#
##		Business_3 
