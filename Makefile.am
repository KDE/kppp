# Yow Emacs, this is a -*- makefile -*-

# if you use a variable *dir and have *_DATA, it will be installed by
# make install
xdg_apps_DATA = Kppp.desktop

EXTRA_DIST = $(xdg_apps_DATA)

# set the include path for X, qt and KDE
INCLUDES= $(all_includes)
# claim, which subdirectories you want to install
SUBDIRS = pixmaps icons logview Rules DB

# This one gets installed 
bin_PROGRAMS = 	kppp

# Which sources should be compiled for kppp.
kppp_SOURCES	= kpppwidget.cpp \
		general.cpp \
		accounts.cpp \
		connect.cpp \
		conwindow.cpp \
		debug.cpp \
		edit.cpp \
		iplined.cpp \
		main.cpp \
                modem.cpp \
		modemcmds.cpp \
		pppdargs.cpp \
		pppdata.cpp \
		scriptedit.cpp\
		pwentry.cpp \
		modeminfo.cpp\
		pppstatdlg.cpp\
		pppstats.cpp\
		miniterm.cpp\
		accounting.cpp \
		acctselect.cpp\
		ruleset.cpp \
		docking.cpp \
		runtests.cpp \
		loginterm.cpp \
		ppplog.cpp \
		newwidget.cpp \
		requester.cpp \
		opener.cpp \
		modemdb.cpp \
		utils.cpp \
		providerdb.cpp \
		modems.cpp \
		KPPPIface.skel

# the library search path
kppp_LDFLAGS = $(all_libraries) $(KDE_RPATH)

# the libraries to link against. Be aware of the order. First the libraries,
# that depend on the following ones.
kppp_LDADD   =       -lm $(LIB_KIO) $(LIB_KDEUI)

# this option you can leave out. Just, if you use "make dist", you need it
noinst_HEADERS = kpppwidget.h \
		accounts.h \
		kpppconfig.h \
		connect.h \
		conwindow.h \
		debug.h \
		edit.h \
		general.h \
		iplined.h \
		main.h \
                modem.h \
		modemcmds.h \
		pppdargs.h \
		pppdata.h \
		scriptedit.h\
		pwentry.h \
		modeminfo.h\
		pppstatdlg.h\
		miniterm.h\
		accounting.h \
		acctselect.h\
		ruleset.h \
		docking.h \
		runtests.h \
		loginterm.h \
		auth.h \
		version.h \
		macros.h \
		ppplog.h \
		newwidget.h \
		modemdb.h \
		utils.h \
		providerdb.h \
		modems.h \
		KPPPIface.h

METASOURCES = accounting.moc \
	accounts.moc \
	acctselect.moc \
	connect.moc \
	conwindow.moc \
	debug.moc \
	docking.moc \
	edit.moc \
	general.moc \
	loginterm.moc \
	kpppwidget.moc \
	miniterm.moc \
	modem.moc \
	modemcmds.moc \
	modemdb.moc \
	modeminfo.moc \
	pppdargs.moc \
	pppstatdlg.moc \
	pppstats.moc \
	pwentry.moc \
	scriptedit.moc \
	providerdb.moc \
	modems.moc

messages: 
	$(XGETTEXT) *.cpp -o $(podir)/kppp.pot

# just install datas here. Use install-exec-data for scripts and etc.
# the binary itself is already installed from automake
# use mkinstalldirs, not "install -d"
# don't install a list of file. Just one file per install.
# if you have more of them, create a subdirectory with an extra Makefile 
install-data-local:
	@echo ""
	@echo "Don't forget to send me a phone tarif rules file for your"
	@echo "telecom company/country/region. Thanks."
	@echo ""
	@echo "We will be maintaining an archive on the kppp page at"
	@echo "http://devel-home.kde.org/~kppp/index.html"
	@echo "WARNING: kppp is by default installed with SETUID bit on!"
	@echo  "If you are concerned about the SETUID bit, create a modem"
	@echo "group instead."
	@echo ""
	@echo "Happy Dialing! -- the KPPP team"
	@echo ""

# kppp wants to be installed suid root
install-exec-hook:
	@(chown root $(DESTDIR)/$(bindir)/kppp && chmod 4755 $(DESTDIR)/$(bindir)/kppp) || echo "Was not able to make kppp setuid root"

# remove ALL you have installed in install-data-local or install-exec-local
#uninstall-local:
#	@(cd Rules && $(MAKE) uninstall)

#distclean-local:
#	@(cd Rules && $(MAKE) distclean)

