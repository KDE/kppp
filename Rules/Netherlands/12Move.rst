################################################################
#
#  kppp rules voor 12Move
# informatie gehaald van http://www.12move.nl/content/article/376150.htm
# laatste bijwerking op 26-11-2002
#
#  Alleen voor lokale gesprekken
#
#  Kosten:
#  Lokaal:      standaardtarief 3.08  cpm = 0.0513333 cps 	08:00-18:59
# 		daltarief     	1.65  cpm = 0.0275    cps	19:00-23:59
#		nacht 		1.09  cpm = 0.0181667 cps	00:00-07:59
#               Zaterdag	1.09  cpm = 0.0181667 cps	00:00-23:59
#		Zondag		1.09  cpm = 0.0181667 cps	00:00-23:59
#
#  Starttarief 			4.14  ct per gesprek
#
#
#  Gemaakt door: Rinse de Vries <rinse@kde.nl>
#  Datum: 21-11-2002
#
#  Oorspronkelijk script gemaakt door: Michel Weijts <mweijts@yahoo.com>
#  Created on 1 October 2000
#
################################################################

name=Nederlands BelBudget Regio
currency_symbol=euro
currency_position=left
currency_digits=2
per_connection=0.0414
minimum_costs=0.0

# standaardtarief:
default=(0.000513333, 1)
# daltarief
on (monday..friday) between (19:00..23:59) use (0.000275, 1)

# nachttarief
on (monday..friday) between (00:00..07:59) use (0.000181667, 1)

# zaterdag en zondag
on (saturday) between (00:00..23:59) use (0.000181667, 1)
on (sunday) between (00:00..23:59) use (0.000181667, 1)
