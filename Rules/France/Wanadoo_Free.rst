################################################################
#
#  kppp rules voor het Wanadoo Free abonnement per minuut
#
#  Alleen voor lokale gesprekken
#
#  Kosten:
#  Lokaal:      standaardtarief 3.25 eurocpm=0.0542 eurocps 	08:00-18:59
# 		daltarief     	1,77   eurocpm=0.0295   eurocps	19:00-23:59
#		nacht 		1,29    eurocpm=0.0215 eurocps	00:00-07:59
#  		Zaterdag	1,29    eurocpm=0.0215 eurocps	00:00-23:59
#  		Zondag		1,29    eurocpm=0.0215 eurocps	00:00-23:59
#
#  Starttarief 			3,5  euroct per gesprek
#
#
#  Eelco van Kuik
#  kppp-accounting@vankuik.com
#
#  Created on 23 june 2002
#
################################################################

name=Wanadoo Free
currency_symbol=EUR
currency_position=left
currency_digits=2
per_connection=0.0350
minimum_costs=0.0

default=(0.000542, 1)
on (monday..friday) between (19:00..23:59) use (0.000295, 1)
on (monday..friday) between (00:00..07:59) use (0.000215, 1)
on (saturday) between (00:00..23:59) use (0.000215, 1)
on (sunday) between (00:00..23:59) use (0.000215, 1)
