################################################################
#
#  kppp rules voor Tele2 Preselect Buiten Regio abonnement
#
#  Alleen voor buiten de regio gesprekken
#
#  Tarieven gedownload van http://www.tele2.nl/chap02/c020201.html
#  Laatste wijziging op 24-11-2002
#
#  Kosten:
#  piektarief doordeweeks 	3.8  cpm = 0.0633333   cps 08:00-18:59
#  's-avonds en in weekend  	1.7  cpm = 0.0283333   cps
# 
#  Starttarief 			4.9  ct per gesprek
#
#
#  Gemaakt door: Rinse de Vries <rinse@kde.nl>
#  Datum: 21-11-2002
#
#  Oorspronkelijk script gemaakt door: Michel Weijts <mweijts@yahoo.com>
#  Created on 1 October 2000
#
################################################################

name=Tele2 Preselect Buiten Regio
currency_symbol=euro
currency_position=left
currency_digits=2
per_connection=0.049
minimum_costs=0.0

#daltarief
default=(0.000283333, 1)

#piektarief
on (monday..friday) between (08:00..18:59) use (0.000633333, 1)

