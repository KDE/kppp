################################################################
#
#  kppp rules voor KPN BelBasis abonnement per seconde
#
#  Alleen voor buiten de regio gesprekken
#
#  Kosten:
#  standaardtarief doordeweeks 	4.25 cpm=.0708333   cps 08:00-18:59
#  daltarief 'savonds		2.01 cpm= 0.335 cps 19.00-23.59
#  's-nachts en in weekend  	4.44 cpm=0.074   cps
#
#  Starttarief 		5  ct per gesprek
#
#  Gemaakt door: Rinse de Vries <rinse@kde.nl>
#  Datum: 21-11-2002
#
#  Oorspronkelijk script gemaakt door: Michel Weijts <mweijts@yahoo.com>
#  Created on 1 October 2000
#
################################################################

name=Nederlands BelBasis Buiten Regio
currency_symbol=euro
currency_position=left
currency_digits=2
per_connection=0.05
minimum_costs=0.0

default=(0.000335, 1)
on (monday..friday) between (08:00..18:59) use (0.000708333, 1)
on (monday..friday) between (19:00..23:59) use (0.000335, 1)
