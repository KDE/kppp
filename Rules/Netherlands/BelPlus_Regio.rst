################################################################
#
#  kppp rules voor KPN BelPlus abonnement per seconde
# Laatste wijziging 09-07-2003
#
#  Alleen voor lokale gesprekken
#
#  Kosten:
#  Lokaal:      standaardtarief 2,89 cpm=0.0007583333333 euro ps 08:00-18:59
# 		daltarief     	1,55 cpm=0.0002583333333 euro ps 19:00-23:59
#		nacht 		1,04 cpm=0.0001733333333 euro ps 00:00-07:59
#  		Zaterdag	1,04 cpm=0.0001733333333 euro ps 00:00-23:59
#		Zondag		1,04 cpm=0.0001733333333 euro ps 00:00-23:59
#
#  Starttarief 			4.27  ct per gesprek
#
#  
#  Gemaakt door: Rinse de Vries <rinse@kde.nl>
#  Datum: 21-11-2002
#
#  Oorspronkelijk script gemaakt door: Michel Weijts <mweijts@yahoo.com>
#  Created on 1 October 2000
#
################################################################

name=Nederlands BelPlus Regio
currency_symbol=euro
currency_position=left
currency_digits=2
per_connection=0.0427
minimum_costs=0.0

default=(0.0007583333333, 1)
on (monday..friday) between (19:00..23:59) use (0.0002583333333, 1)
on (monday..friday) between (00:00..07:59) use (0.0001733333333, 1)
on (saturday) between (00:00..23:59) use (0.0001733333333, 1)
on (sunday) between (00:00..23:59) use (0.0001733333333, 1)
