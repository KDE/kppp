################################################################
#  kppp rules voor Onetel Spaarstand Regio abonnement
#  Alleen voor lokale gesprekken
#  Tarieven gedownload van http://www.onetel.nl/tarieven_nationaal.php
#  Laatste wijziging op 24-11-2002
#
#  Kosten:
#  Lokaal:      piektarief	2.4    cpm = 0.04     cps 	08:00-18:59
# 		daltarief     	1.2    cpm = 0.02     cps	19:00-23:59
#		nacht 		0.9    cpm = 0.015    cps	00:00-07:59
#  		Zaterdag	0.9    cpm = 0.015    cps	00:00-23:59
#  		Zondag		0.9    cpm = 0.015    cps	00:00-23:59
#
#  Starttarief 			25  ct per gesprek
#
#  Gemaakt door: Rinse de Vries <rinse@kde.nl>
#  Datum: 21-11-2002
#
#  Oorspronkelijk script gemaakt door: Michel Weijts <mweijts@yahoo.com>
#  Created on 1 October 2000
#
################################################################

name=Onetel Spaarstand Regio
currency_symbol=euro
currency_position=left
currency_digits=2
per_connection=0.25
minimum_costs=0.0

# piektarief
default=(0.0004, 1)

# daltarief
on (monday..friday) between (19:00..23:59) use (0.0002, 1)

# nachttarief
on (monday..friday) between (00:00..07:59) use (0.00015, 1)

# zaterdag en zondag
on (saturday) between (00:00..23:59) use (0.00015, 1)
on (sunday) between (00:00..23:59) use (0.00015, 1)
