################################################################
#
#  kppp rules voor KPN BelBasis abonnement per seconde
#
#  Alleen voor lokale gesprekken
#
#  Kosten:
#  Lokaal:      standaardtarief 5.55 cpm=0.0925 cps 	08:00-18:59
# 		daltarief     	3    cpm=0.05   cps	19:00-23:59
#		nacht 		2    cpm=0.0333 cps	00:00-07:59 
#  		Zaterdag	2    cpm=0.0333 cps	00:00-23:59
#  		Zondag		2    cpm=0.0333 cps	00:00-23:59
#
#  Starttarief 			10  ct per gesprek
#
#
#  Michel Weijts
#  mweijts@yahoo.com
#
#  Created on 1 October 2000 
#
################################################################

name=Nederlands BelBasis Lokaal
currency_symbol=Hfl
currency_position=left
currency_digits=2
per_connection=0.10
minimum_costs=0.0

default=(0.000925, 1)
on (monday..friday) between (19:00..23:59) use (0.0005, 1)
on (monday..friday) between (00:00..07:59) use (0.000333, 1)
on (saturday) between (00:00..23:59) use (0.000333, 1)
on (sunday) between (00:00..23:59) use (0.000333, 1)
