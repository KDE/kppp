################################################################
#
#  kppp rules voor KPN BelBasis abonnement + nummervoordeel per seconde
#
#  Alleen voor lokale gesprekken
#
#  Kosten:
#  Lokaal:      standaardtarief 2.52 cpm=0.042 cps 	08:00-18:59
# 		daltarief     	1.35   cpm=0.0225  cps	19:00-23:59
#		nacht 		0.9   cpm=0.015   cps	00:00-07:59
#  		Zaterdag	0.9   cpm=0.015   cps	00:00-23:59
#		Zondag		0.9   cpm=0.015   cps	00:00-23:59
#
#  Starttarief 			3.73  ct per gesprek
#
#  Gemaakt door: Rinse de Vries <rinse@kde.nl>
#  Datum: 21-11-2002
#
#  Oorspronkelijk script gemaakt door: Michel Weijts <mweijts@yahoo.com>
#  Created on 1 October 2000
#
################################################################

name=Nederlands BelBasis Regio Nummervoordeel per seconde
currency_symbol=euro
currency_position=left
currency_digits=2
per_connection=0.0373
minimum_costs=0.0

default=(0.00042, 1)
on (monday..friday) between (19:00..23:59) use (0.000225, 1)
on (monday..friday) between (00:00..07:59) use (0.00015, 1)
on (saturday) between (00:00..23:59) use (0.00015, 1)
on (sunday) between (00:00..23:59) use (0.00015, 1)
