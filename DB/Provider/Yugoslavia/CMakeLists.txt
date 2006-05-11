kde4_header()




########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kppp/Provider/Yugoslavia FILES 041Net CG.Berane.yu CG.HercegNovi.yu CG.Podgorica.yu  		DrenikNet EUnetCacak EUnetPristina InfoSKY Sezampro  		CG.BijeloPolje.CG.yu CG.Kotor.yu CG.Tivat.yu  		EUnet@Full EUnetKragujevac VeratNet  		BeoTelNet CG.Budva.yu CG.Niksic.yu CG.Ulcinj.yu  		EUnet@Lite EUnetNis EUnetSombor PTT YUBCnet  		CG.Bar.yu CG.Cetinje.yu CG.Pljevlja.yu CG.yu  		TippNet SuOnline  		EUnetBeograd EUnetNoviSad EUnetSubotica SCnet )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA =	041Net CG.Berane.yu CG.HercegNovi.yu CG.Podgorica.yu \
#		DrenikNet EUnetCacak EUnetPristina InfoSKY Sezampro \
#		CG.BijeloPolje.CG.yu CG.Kotor.yu CG.Tivat.yu \
#		EUnet@Full EUnetKragujevac VeratNet \
#		BeoTelNet CG.Budva.yu CG.Niksic.yu CG.Ulcinj.yu \
#		EUnet@Lite EUnetNis EUnetSombor PTT YUBCnet \
#		CG.Bar.yu CG.Cetinje.yu CG.Pljevlja.yu CG.yu \
#		TippNet SuOnline \
#		EUnetBeograd EUnetNoviSad EUnetSubotica SCnet
#
#pkgdir = $(kde_datadir)/kppp/Provider/Yugoslavia
#
#EXTRA_DIST = $(pkg_DATA)
