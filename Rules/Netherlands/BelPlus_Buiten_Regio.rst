################################################################
#
#  kppp rules voor KPN BelPlus abonnement per seconde
#
#  Alleen voor buiten de regio gesprekken
#
#  Kosten:
#  standaardtarief doordeweeks 	9.44 cpm=0.157   cps 08:00-18:59
#  's-avonds en in weekend  	3.33 cpm=0.055   cps
# 
#  Starttarief 			10  ct per gesprek #
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

default=(0.00055, 1)
on (monday..friday) between (08:00..18:59) use (0.00157, 1)

