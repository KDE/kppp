######################################################################
#
#  kppp rules voor KPN BelPlus abonnement+ nummervoordeel per seconde
#
#  Alleen voor lokale gesprekken
#
#  Kosten:
#  Lokaal:      standaardtarief 5.04 cpm=0.084  cps	08:00-20:00
# 		daltarief     	2.25 cpm=0.0375 cps	20:00-23:59
#		daltarief nacht 2.25 cpm=0,0375 cps	00:00-07:59
#               nacht 		1.8  cpm=0.03   cps	00:00-07:59 (1)
#  		Zaterdag	2.25 cpm=0.0375 cps	00:00-23:59
#  		Zaterdag	1.8  cpm=0.03	cps	00:00-23:59 (1)
#		Zondag		1.8  cpm=0.03   cps	00:00-23:59
#
#  Starttarief 			10  ct per gesprek
#
#  NOTE: Tot nadere Kennisgeving geldt (1)
#
#  Michel Weijts
#  mweijts@yahoo.com
#
#  Created on Monday 12 June 2000 
#
######################################################################

name=Nederlands BelPlus Lokaal
currency_symbol=Hfl
currency_position=left
currency_digits=2
per_connection=0.10
minimum_costs=0.0

default=(0.00084, 1)
on (monday..friday) between (20:00..23:59) use (0.000375, 1)
on (monday..friday) between (00:00..07:59) use (0.0003, 1)
# on (monday..friday) between (00:00..07:59) use (0.000375, 1)
on (saturday) between (00:00..23:59) use (0.0003, 1)
# on (saturday) between (00:00..23:59) use (0.000375, 1)
on (sunday) between (00:00..23:59) use (0.0003, 1)
