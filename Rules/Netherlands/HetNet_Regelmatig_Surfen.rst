################################################################
#
#  kppp rules voor HetNet Regelmatig Surfen
#  HetNet Regelmatig Surfen is 20% goekoper dan KPN BelBasis.
#
#  Alleen voor lokale gesprekken
#
#  Kosten KPN BelBasis Regio:						-20%:
#  Lokaal:      standaardtarief 2,80 cpm=0.0466667 cps 	08:00-18:59	0.0373334 cps
# 		daltarief     	1.5    cpm=0.025   cps	19:00-23:59	0.02	  cps
#		nacht 		1    cpm=0.0166667 cps	00:00-07:59	0.0133334 cps
#  		Zaterdag	1    cpm=0.0166667 cps	00:00-23:59	0.0133334 cps
#  		Zondag		1    cpm=0.0166667 cps	00:00-23:59	0.0133334 cps
#
#  Starttarief 			4.14  ct per gesprek
#
#  Gemaakt door: Rinse de Vries <rinse@kde.nl>
#  Datum: 21-11-2002
#
#  Oorspronkelijk script gemaakt door: Michel Weijts <mweijts@yahoo.com>
#  Created on 1 October 2000
#
################################################################

name=HetNet Regelmatig Surfen
currency_symbol=euro
currency_position=left
currency_digits=2
per_connection=0.0414
minimum_costs=0.0

# standaardtarief
default=(0.000373334, 1)
# daltarief
on (monday..friday) between (19:00..23:59) use (0.0002, 1)
# nachttarief
on (monday..friday) between (00:00..07:59) use (0.000133334, 1)
# zaterdag en zondag
on (saturday) between (00:00..23:59) use (0.000133334, 1)
on (sunday) between (00:00..23:59) use (0.000133334, 1)
