################################################################
#
#  kppp rules voor Planet Internet per seconde
# http://web.planet.nl/klantenservice/helpdesk/inbelpunten.html#3
# tarieven vanaf 1 januari 2003
# laatste bijwerking op 28-11-2003
#
#  Alleen voor lokale gesprekken
#
#  Kosten:
#  Lokaal:      standaardtarief 2,74    cpm = 0.0456667 cps	08:00-18:59
# 		daltarief     	1.47    cpm = 0.0245     cps	19:00-23:59
#		nacht 		0.98    cpm = 0.0163333 cps	00:00-07:59
#  		Zaterdag	0.98    cpm = 0.0163333 cps	00:00-23:59
#  		Zondag		0.98    cpm = 0.0163333 cps	00:00-23:59
#
#  Starttarief 			4.05 ct per gesprek
#
#  Gemaakt door: Rinse de Vries <rinse@kde.nl>
#  Datum: 21-11-2002
#
#  Oorspronkelijk script gemaakt door: Michel Weijts <mweijts@yahoo.com>
#  Created on 1 October 2000
#
################################################################

name=Nederlands BelBasis Regio
currency_symbol=euro
currency_position=left
currency_digits=2
per_connection=0.0405
minimum_costs=0.0

# standaardtarief
default=(0.000456667, 1)
# daltarief
on (monday..friday) between (19:00..23:59) use (0.000245, 1)
# nachttarief
on (monday..friday) between (00:00..07:59) use (0.000163333, 1)
on (saturday) between (00:00..23:59) use (0.000163333, 1)
on (sunday) between (00:00..23:59) use (0.000163333, 1)
