################################################################
#
#  kppp rules voor KPN BelBudget abonnement
#
#  Alleen voor lokale gesprekken
#
#  Kosten:
#  Lokaal:      standaardtarief 9.18  cpm=0.153 cps 	08:00-18:59
# 		daltarief     	4.54  cpm=0.0756667 cps	19:00-23:59
#		nacht 		3.28  cpm=0.0546667 cps	00:00-07:59
#               Zaterdag	3.28  cpm=0.0546667 cps	00:00-23:59
#		Zondag		3.28  cpm=0.0546667 cps	00:00-23:59
#
#  Starttarief 			4.14  ct per gesprek
#
#
#  Gemaakt door: Rinse de Vries <rinse@kde.nl>
#  Datum: 21-11-2002
#
#  Oorspronkelijk script gemaakt door: Michel Weijts <mweijts@yahoo.com>
#  Created on 1 October 2000
#
################################################################

name=Nederlands BelBudget Regio
currency_symbol=euro
currency_position=left
currency_digits=2
per_connection=0.0414
minimum_costs=0.0

default=(0.00153, 1)
on (monday..friday) between (19:00..23:59) use (0.000756667, 1)
on (monday..friday) between (00:00..07:59) use (0.000546667, 1)
on (saturday) between (00:00..23:59) use (0.000546667, 1)
on (sunday) between (00:00..23:59) use (0.000546667, 1)
