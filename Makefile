# Generated automatically from Makefile.in by configure.
#
#                           kppp Makefile
#
#  Copyright 1997 Bernd Johannes Wuebben, wuebben@math.cornell.edu
#
#  $Id$
#


CXXFLAGS= -O2 -Wall -I/usr/local/kde/include -I/usr/local/qt/include -I/usr/X11R6/include -DHAVE_CONFIG_H  -I.. -I.
LDFLAGS	= -s -L/usr/local/kde/lib -L/usr/local/qt/lib -L/usr/X11R6/lib  -lkdecore  -lqt \
	  -lXext -lX11  -lkdeui -lkdecore
CXX     = g++
MOC   	= /usr/local/qt/bin/moc

############# INSTALL #################

INSTALL = /usr/bin/ginstall -c -m 0755
INSTALL_DATA = /usr/bin/ginstall -c -m 0644
INSTALL_DIR = /usr/bin/ginstall -c -d -m 0755
INSTALL_DIR_PRIVATE = /usr/bin/ginstall -c -d -m 0700

prefix = /usr/local/kde
KDEINC = $(prefix)/include
BINDIR = $(prefix)/bin
LIBDIR = $(prefix)/lib
PICSDIR = $(prefix)/share/apps/kppp/pics/
ICONSDIR = $(prefix)/share/icons
DOCSDIR = $(prefix)/share/doc/HTML/en/kppp
APPLNKDIR = $(prefix)/share/applnk/Internet
RULESDIR = $(prefix)/share/apps/kppp/Rules

########### File Formats ###########

# Uncoment this is you want kppp to give you more 
DEFS =  #-DMY_DEBUG 

.SUFFIXES: .cpp

.cpp.o:
	$(CXX) $(DEFS) -c $(CXXFLAGS) $<

%.moc: %.h
	$(MOC) $< -o $@

########## Files #####################

SOURCES =	accounts.cpp \
		connect.cpp \
		conwindow.cpp \
		debug.cpp \
		edit.cpp \
		general.cpp \
		iplined.cpp \
		main.cpp \
		modemcmds.cpp \
		pppdargs.cpp \
		pppdata.cpp \
		scriptedit.cpp\
		modeminfo.cpp\
		pppstatdlg.cpp\
		pppstats.cpp\
		miniterm.cpp\
		kintedit.cpp \
		accounting.cpp \
		acctselect.cpp\
		ruleset.cpp \
		homedir.cpp\
		progress.cpp

HEADERS =		accounts.h \
		config.h \
		connect.h \
		conwindow.h \
		debug.h \
		edit.h \
		general.h \
		iplined.h \
		main.h \
		modemcmds.h \
		pppdargs.h \
		pppdata.h \
		scriptedit.h\
		modeminfo.h\
		pppstatdlg.h\
		miniterm.h\
		kintedit.h \
		accounting.h \
		acctselect.h\
		ruleset.h \
		homedir.h\
		progress.h

OBJECTS =		accounts.o \
		connect.o \
		conwindow.o \
		debug.o \
		edit.o \
		general.o \
		iplined.o \
		main.o \
		modemcmds.o \
		pppdargs.o \
		pppdata.o \
		scriptedit.o\
		modeminfo.o\
		pppstatdlg.o\
		pppstats.o\
		miniterm.o\
		kintedit.o\
		accounting.o\
		acctselect.o\
		ruleset.o\
		homedir.o\
		progress.o\
		moc_accounts.o \
		moc_connect.o \
		moc_conwindow.o \
		moc_debug.o \
		moc_edit.o \
		moc_general.o \
		moc_main.o \
		moc_modemcmds.o \
		moc_pppdargs.o \
		moc_scriptedit.o\
		moc_modeminfo.o\
		moc_pppstatdlg.o\
		moc_miniterm.o\
		moc_kintedit.o \
		moc_accounting.o\
		moc_acctselect.o\
		moc_progress.o

SRCMETA =	moc_accounts.cpp \
		moc_connect.cpp \
		moc_conwindow.cpp \
		moc_debug.cpp \
		moc_edit.cpp \
		moc_general.cpp \
		moc_main.cpp \
		moc_modemcmds.cpp \
		moc_pppdargs.cpp \
		moc_scriptedit.cpp\
		moc_modeminfo.cpp\
		moc_pppstatdlg.cpp\
		moc_miniterm.cpp\
		moc_kintedit.cpp \
		moc_accounting.cpp\
		moc_acctselect.cpp\
		moc_progress.cpp

TARGET	=	kppp


######### build rules ################

all: $(TARGET)

Makefile: Makefile.in
	cd .. && CONFIG_FILES=kppp/$@ CONFIG_HEADERS= $(SHELL) ./config.status

$(TARGET): $(SRCMETA) $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

depend:
	@makedepend $(SOURCES) 2> /dev/null

showfiles:
	@echo $(SOURCES) $(HEADERS) Makefile

clean:
	rm -f *.o *.bak *~ *% #*
	rm -f $(SRCMETA) $(TARGET)

distclean: clean
	rm -f Makefile

install: $(TARGET)
	$(INSTALL_DIR) $(BINDIR)
	$(INSTALL) $(TARGET) $(BINDIR)
	$(INSTALL_DIR) $(DOCSDIR)
	$(INSTALL_DATA) docs/k* $(DOCSDIR)
	$(INSTALL_DATA) docs/hayes.html $(DOCSDIR)
	$(INSTALL_DIR) $(PICSDIR)
	$(INSTALL_DATA) ./pixmaps/*.xpm $(PICSDIR)
	$(INSTALL_DIR) $(ICONSDIR)
	$(INSTALL_DATA) ./pixmaps/kppp.xpm $(ICONSDIR)
	$(INSTALL_DIR) $(APPLNKDIR)	
	$(INSTALL_DATA) Kppp.kdelnk $(APPLNKDIR)
	$(INSTALL_DIR) $(RULESDIR)
	$(INSTALL_DIR) $(RULESDIR)/Austria
	$(INSTALL_DIR) $(RULESDIR)/Austria/Business_1
	$(INSTALL_DATA) ./Rules/Austria/Business_1/*.rst $(RULESDIR)/Austria/Business_1
	$(INSTALL_DIR) $(RULESDIR)/Austria/Business_2
	$(INSTALL_DATA) ./Rules/Austria/Business_2/*.rst $(RULESDIR)/Austria/Business_2
	$(INSTALL_DIR) $(RULESDIR)/Austria/Minimum
	$(INSTALL_DATA) ./Rules/Austria/Minimum/*.rst $(RULESDIR)/Austria/Minimum
	$(INSTALL_DIR) $(RULESDIR)/Austria/Standard
	$(INSTALL_DATA) ./Rules/Austria/Standard/*.rst $(RULESDIR)/Austria/Standard
	$(INSTALL_DIR) $(RULESDIR)/England
	$(INSTALL_DATA) ./Rules/England/*.rst $(RULESDIR)/England
	$(INSTALL_DIR) $(RULESDIR)/France
	$(INSTALL_DATA) ./Rules/France/*.rst $(RULESDIR)/France
	$(INSTALL_DIR) $(RULESDIR)/Germany
	$(INSTALL_DATA) ./Rules/Germany/*.rst $(RULESDIR)/Germany
	$(INSTALL_DIR) $(RULESDIR)/Greece
	$(INSTALL_DATA) ./Rules/Greece/*.rst $(RULESDIR)/Greece
	$(INSTALL_DIR) $(RULESDIR)/Hungary
	$(INSTALL_DATA) ./Rules/Hungary/*.rst $(RULESDIR)/Hungary
	$(INSTALL_DIR) $(RULESDIR)/Italy
	$(INSTALL_DATA) ./Rules/Italy/*.rst $(RULESDIR)/Italy
	$(INSTALL_DIR) $(RULESDIR)/Netherlands
	$(INSTALL_DATA) ./Rules/Netherlands/*.rst $(RULESDIR)/Netherlands
	$(INSTALL_DIR) $(RULESDIR)/Norway
	$(INSTALL_DATA) ./Rules/Norway/*.rst $(RULESDIR)/Norway
	$(INSTALL_DIR) $(RULESDIR)/Poland
	$(INSTALL_DATA) ./Rules/Poland/*.rst $(RULESDIR)/Poland
	$(INSTALL_DIR) $(RULESDIR)/Portugal
	$(INSTALL_DATA) ./Rules/Portugal/*.rst $(RULESDIR)/Portugal
	$(INSTALL_DIR) $(RULESDIR)/Spain
	$(INSTALL_DATA) ./Rules/Spain/*.rst $(RULESDIR)/Spain
	$(INSTALL_DIR) $(RULESDIR)/Sweden
	$(INSTALL_DATA) ./Rules/Sweden/*.rst $(RULESDIR)/Sweden
	$(INSTALL_DATA) ./Rules/TEMPLATE $(DOCSDIR)
	chmod +s $(BINDIR)/kppp
	@echo ""
	@echo "Dont't forget to send me a phone tarif rules file for your"
	@echo "telecom company/country/region. Thanks."
	@echo ""
	@echo "I am mainting an archive on the kppp page at"
	@echo "http://math.cornell.edu/~wuebben/kde.html"
	@echo ""
	@echo "Happy Dialing! -- Bernd Wuebben"
	@echo ""

uninstall:
	rm -f $(BINDIR)/$(TARGET)
#	rm -f $(DOCSDIR)/kppp.html
#	rm -f $(DOCSDIR)/kppp.gif
#	rm -f $(APPSDIR)/Applications/kppp.kdelnk



moc_modemcmds.o: moc_modemcmds.cpp \
		modemcmds.h \
		pppdata.h \
		config.h

moc_pppdargs.o: moc_pppdargs.cpp \
		pppdargs.h \
		pppdata.h \
		config.h

moc_pppstatdlg.o: moc_pppstatdlg.cpp\
		pppstatdlg.h

moc_scriptedit.o: moc_scriptedit.cpp \
		scriptedit.h \

moc_miniterm.cpp : miniterm.cpp miniterm.h
	$(MOC) miniterm.h -o moc_miniterm.cpp

moc_pppstatdlg.cpp : pppstatdlg.cpp pppstatdlg.h
	$(MOC) pppstatdlg.h -o moc_pppstatdlg.cpp

moc_accounts.cpp: accounts.h
	$(MOC) accounts.h -o moc_accounts.cpp

moc_connect.cpp: connect.h
	$(MOC) connect.h -o moc_connect.cpp

moc_conwindow.cpp: conwindow.h
	$(MOC) conwindow.h -o moc_conwindow.cpp

moc_debug.cpp: debug.h
	$(MOC) debug.h -o moc_debug.cpp

moc_edit.cpp: edit.h
	$(MOC) edit.h -o moc_edit.cpp

moc_general.cpp: general.h
	$(MOC) general.h -o moc_general.cpp

moc_main.cpp: main.h
	$(MOC) main.h -o moc_main.cpp

moc_modemcmds.cpp: modemcmds.h
	$(MOC) modemcmds.h -o moc_modemcmds.cpp

moc_pppdargs.cpp: pppdargs.h
	$(MOC) pppdargs.h -o moc_pppdargs.cpp

moc_scriptedit.cpp: scriptedit.h
	$(MOC) scriptedit.h -o moc_scriptedit.cpp

moc_modeminfo.cpp: modeminfo.h
	$(MOC) modeminfo.h -o moc_modeminfo.cpp

moc_kintedit.cpp: kintedit.h
	$(MOC) kintedit.h -o moc_kintedit.cpp

moc_accounting.cpp: accounting.h
	$(MOC) accounting.h -o moc_accounting.cpp

moc_acctselect.cpp: acctselect.h
	$(MOC) acctselect.h -o moc_acctselect.cpp

moc_progress.cpp: progress.h
	$(MOC) progress.h -o moc_progress.cpp
