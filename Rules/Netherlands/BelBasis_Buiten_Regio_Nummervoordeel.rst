################################################################
#
#  kppp rules voor KPN BelBasis abonnement per seconde + nummervoordeel
#
#  Alleen voor buiten de regio gesprekken
#
#  Kosten:
#  standaardtarief doordeweeks 	3.83  cpm=0.000638333   cps 08:00-18:59
#  's-avonds en in weekend  	1.81 cpm=0.000301667 cps
# 
#  Starttarief 			4.5  ct per gesprek #
#
#  Gemaakt door: Rinse de Vries <rinse@kde.nl>
#  Datum: 21-11-2002
#
#  Oorspronkelijk script gemaakt door: Michel Weijts <mweijts@yahoo.com>
#  Created on 1 October 2000
#
################################################################

name=Nederlands BelBasis Buiten Regio Nummervoordeel
currency_symbol=euro
currency_position=left
currency_digits=2
per_connection=0.045
minimum_costs=0.0

default=(0.000301667, 1)
on (monday..friday) between (08:00..18:59) use (0.000638333, 1)

