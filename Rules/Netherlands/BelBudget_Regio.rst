################################################################
#
#  kppp rules voor KPN BelBudget abonnement
#
#  Alleen voor lokale gesprekken
#
#  Kosten:
#  Lokaal:      standaardtarief 18.5 cpm=0.308 cps 	08:00-18:59
# 		daltarief     	10   cpm=0.166 cps	19:00-23:59
#		nacht 		6.7  cpm=0.111 cps	00:00-07:59
#               Zaterdag	6.7  cpm=0.111 cps	00:00-23:59
#		Zondag		6.7  cpm=0.111 cps	00:00-23:59
#
#  Starttarief 			10  ct per gesprek
#
#
#  Michel Weijts
#  mweijts@yahoo.com
#
#  Created on Monday 12 June 2000 
#
################################################################

name=Nederlands BelBudget Lokaal
currency_symbol=Hfl
currency_position=left
currency_digits=2
per_connection=0.10
minimum_costs=0.0

default=(0.00308, 1)
on (monday..friday) between (19:00..23:59) use (0.00166, 1)
on (monday..friday) between (00:00..07:59) use (0.00111, 1)
on (saturday) between (00:00..23:59) use (0.00111, 1)
on (sunday) between (00:00..23:59) use (0.00111, 1)
