################################################################
#
#  kppp rules voor KPN BelBudget abonnement per seconde
#
#  Alleen voor buiten de regio gesprekken
#
#  Kosten:
#  standaardtarief doordeweeks 	21,25 cpm=0.354167   cps 08:00-18:59
#  's-avonds en in weekend  	10,06 cpm=0.167667   cps
# 
#  Starttarief 			4.9  ct per gesprek #
#
#  Gemaakt door: Rinse de Vries <rinse@kde.nl>
#  Datum: 21-11-2002
#
#  Oorspronkelijk script gemaakt door: Michel Weijts <mweijts@yahoo.com>
#  Created on 1 October 2000
#
################################################################

name=Nederlands Belbudget Buiten Regio
currency_symbol=euro
currency_position=left
currency_digits=2
per_connection=0.049
minimum_costs=0.0

default=(0.00167667, 1)
on (monday..friday) between (08:00..18:59) use (0.00354167, 1)

