################################################################
#  kppp rules voor Tele2 Extra Regio abonnement
#  Alleen voor lokale gesprekken
#  Tarieven gedownload van http://www.tele2.nl/chap02/c020201.html
#  Laatste wijziging op 24-11-2002
#
#  Kosten:
#  Lokaal:      piektarief	2.25    cpm = 0.0375 cps 	08:00-18:59
# 		daltarief     	1.17    cpm = 0.0195 cps	19:00-23:59
#		nacht 		0.81    cpm = 0.0135 cps	00:00-07:59
#  		Zaterdag	0.81    cpm = 0.0135 cps	00:00-23:59
#  		Zondag		0.81    cpm = 0.0135 cps	00:00-23:59
#
#  Starttarief 			3.51  ct per gesprek
#
#  Gemaakt door: Rinse de Vries <rinse@kde.nl>
#  Datum: 21-11-2002
#
#  Oorspronkelijk script gemaakt door: Michel Weijts <mweijts@yahoo.com>
#  Created on 1 October 2000
#
################################################################

name=Tele2 Extra 1602 Regio
currency_symbol=euro
currency_position=left
currency_digits=2
per_connection=0.0351
minimum_costs=0.0

# piektarief
default=(0.000375, 1)

# daltarief
on (monday..friday) between (19:00..23:59) use (0.000195, 1)

# nachttarief
on (monday..friday) between (00:00..07:59) use (0.000135, 1)

# zaterdag en zondag
on (saturday) between (00:00..23:59) use (0.000135, 1)
on (sunday) between (00:00..23:59) use (0.000135, 1)
