################################################################
#
#  kppp rules voor Freeler Voordelig per seconde
#  http://www.freeler.nl/service/tarieven.html
#  bijgewerkt op 30-11-2002
#  Alleen voor lokale gesprekken
#  Let op! bedragen in euro per seconde!!
#  Kosten:
#  Lokaal:      piektarief 	0.0196 euro pm =  0.00032667 euro ps  08:00-18:59
# 		daltarief     	0.0105 euro pm =  0.000175   euro ps  19:00-23:59
#		nacht 		0.0070 euro pm =  0.00011667 euro ps  00:00-07:59
#  		Zaterdag	0.0070 euro pm =  0.00011667 euro ps  00:00-23:59
#  		Zondag		0.0070 euro pm =  0.00011667 euro ps  00:00-23:59
#
#  Starttarief 			0.0290 euro per gesprek
#
#  Gemaakt door: Rinse de Vries <rinse@kde.nl>
#  Datum: 21-11-2002
#
#  Oorspronkelijk script gemaakt door: Michel Weijts <mweijts@yahoo.com>
#  Created on 1 October 2000
#
################################################################

name=Freeler Voordelig
currency_symbol=euro
currency_position=left
currency_digits=2
per_connection=0.0290
minimum_costs=0.0

# piektarief
default=(0.00032667, 1)
# daltarief
on (monday..friday) between (19:00..23:59) use (0.000175, 1)
# nachttarief
on (monday..friday) between (00:00..07:59) use (0.00011667, 1)
# zaterdag en zondag
on (saturday) between (00:00..23:59) use (0.00011667, 1)
on (sunday) between (00:00..23:59) use (0.00011667, 1)
