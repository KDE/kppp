################################################################
#
#  kppp rules voor KPN BelPlus abonnement per seconde + nummervoordeel
#
#  Alleen voor buiten de regio gesprekken
#
#  Kosten:
#  standaardtarief doordeweeks 	8.47  cpm=0.142   cps 08:00-18:59
#  's-avonds en in weekend  	2.997 cpm=0.0499 cps
# 
#  Starttarief 			9  ct per gesprek #
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
per_connection=0.09
minimum_costs=0.0

default=(0.000499, 1)
on (monday..friday) between (08:00..18:59) use (0.00142, 1)

