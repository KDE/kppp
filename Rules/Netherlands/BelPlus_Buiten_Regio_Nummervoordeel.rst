################################################################
#
#  kppp rules voor KPN Belplus abonnement per seconde + nummervoordeel
#
#  Alleen voor buiten de regio gesprekken
#  Laatste wijziging: 09-07-2003
#
#  Kosten:
#  standaardtarief doordeweeks 	3.51  cpm = 0.000585          euro ps 08:00-18:59
#  's-avonds en in weekend  	1,66  cpm = 0.0002766666667   euro ps
#
#  Starttarief 			4.14  ct per gesprek #
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
per_connection=0.0414
minimum_costs=0.0

default=(0.0002766666667, 1)
on (monday..friday) between (08:00..18:59) use (0.000585, 1)

