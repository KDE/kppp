################################################################
#
#  kppp rules voor OneTel Spaarstand Buiten Regio abonnement
#
#  Alleen voor buiten de regio gesprekken
#
#  Tarieven gedownload van http://www.onetel.nl/tarieven_nationaal.php
#  Laatste wijziging op 24-11-2002
#
#  Kosten:
#  piektarief doordeweeks 	4.1  cpm = 0.0683333   cps 08:00-18:59
#  's-avonds en in weekend  	1.8  cpm = 0.03        cps
# 
#  Starttarief 			25  ct per gesprek
#
#
#  Gemaakt door: Rinse de Vries <rinse@kde.nl>
#  Datum: 21-11-2002
#
#  Oorspronkelijk script gemaakt door: Michel Weijts <mweijts@yahoo.com>
#  Created on 1 October 2000
#
################################################################

name=OneTel Toets 1658 Buiten Regio
currency_symbol=euro
currency_position=left
currency_digits=2
per_connection=0.25
minimum_costs=0.0

#daltarief
default=(0.0003, 1)

#piektarief
on (monday..friday) between (08:00..18:59) use (0.000683333, 1)

