################################################################
#
#  kppp rules voor KPN Belplus abonnement per seconde
#
#  Alleen voor buiten de regio gesprekken
#  Laatste wijziging: 09-07-2003
#
#  Kosten:
#  standaardtarief doordeweeks 	4.39 cpm= 0.07316666667  cps 08:00-18:59
#  daltarief 'savonds		2,08 cpm= 0.03466666667 cps 19.00-23.59
#  's-nachts en in weekend  	2,08 cpm=  0.03466666667  cps
#
#  Starttarief 		0,0518  euro per gesprek
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
per_connection=0.0518
minimum_costs=0.0

default=(0.03466666667, 1)
on (monday..friday) between (08:00..18:59) use (0.07316666667, 1)
on (monday..friday) between (19:00..23:59) use (0.03466666667, 1)
