######################################################################
#
#  kppp rules voor KPN BelPlus abonnement+ nummervoordeel per seconde
#
#  Alleen voor lokale gesprekken
#
#  Kosten:
#  Lokaal:      standaardtarief 4.995 cpm=0.0825 cps	08:00-18:59
# 		daltarief     	2.25  cpm=0.0375 cps	19:00-23:59
#		nacht 		1.8   cpm=0.03   cps	00:00-07:59 
#  		Zaterdag	1.8   cpm=0.03	 cps	00:00-23:59 
#		Zondag		1.8   cpm=0.03   cps	00:00-23:59
#
#  Starttarief 			9  ct per gesprek
#
#
#  Michel Weijts
#  mweijts@yahoo.com
#
#  Created on 1 October 2000 
#
######################################################################

name=Nederlands BelPlus Lokaal
currency_symbol=Hfl
currency_position=left
currency_digits=2
per_connection=0.09
minimum_costs=0.0

default=(0.0008325, 1)
on (monday..friday) between (19:00..23:59) use (0.000375, 1)
on (monday..friday) between (00:00..07:59) use (0.0003, 1)
on (saturday) between (00:00..23:59) use (0.0003, 1)
on (sunday) between (00:00..23:59) use (0.0003, 1)
