################################################################
#
#  kppp rules voor InterNLnet
# gegevens van http://www.internl.net/telefoontarieven/
# laatste wijziging op 27-11-2002
#
#  Alleen voor lokale gesprekken
# Let op!! InterNLnet rekent in euro's per uur!!
#
#  Kosten InterNLnet:
#  Lokaal:      standaardtarief 1.25 euro pu = 0.000347222 euro ps	08:00-18:59
# 		daltarief     	0.60 euro pu = 0.000166667 euro ps	19:00-23:59
#		nacht 		0.45 euro pu = 0.000125    euro ps	00:00-07:59
#  		Zaterdag	0.45 euro pu = 0.000125    euro ps	00:00-23:59
#  		Zondag		0.45 euro pu = 0.000125    euro ps	00:00-23:59
#
#  Starttarief 			0,03 euro per geslaagde verbinding
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
per_connection=0.0
minimum_costs=0.0
flat_init_costs=(0.03, 5)

# standaardtarief
default=(0.000347222, 1)
# daltarief
on (monday..friday) between (19:00..23:59) use (0.000166667, 1)
# nachttarief
on (monday..friday) between (00:00..07:59) use (0.000125, 1)
# zaterdag en zondag
on (saturday) between (00:00..23:59) use (0.000125, 1)
on (sunday) between (00:00..23:59) use (0.000125, 1)
