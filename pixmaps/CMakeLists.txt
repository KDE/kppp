



########### install files ###############

install( FILES folder.png phone.png  		dock_both.png dock_left.png dock_none.png dock_right.png  		modemboth.png modemleft.png modemnone.png modemright.png  DESTINATION ${DATA_INSTALL_DIR}/kppp/pics)




#original Makefile.am contents follow:

#
## add here all files
## leaving out kppplogo.png until it gets used
#emo_DATA = 	folder.png phone.png \
#		dock_both.png dock_left.png dock_none.png dock_right.png \
#		modemboth.png modemleft.png modemnone.png modemright.png
#
#emodir = $(kde_datadir)/kppp/pics
#
#EXTRA_DIST = $(emo_DATA)
