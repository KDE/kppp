################################################################
#
# For Hungarian text scroll down
# Internet calls in the areacode 89 in Hungary
# Phone number: 0651389301
# Valid by 1st March 2001
# By Miklos Pataki <pataki.miklos.1@freemail.hu> from Hungary, Papa
# Date: 10th Feb 2002
#
# If you find some bugs or if you have any questions
# please feel free to send me an e-mail
#
################################################################
name=PapaTel
################################################################
# currency settings
################################################################

currency_symbol=Ft
currency_position=right
currency_digits=2

################################################################
# connection settings
################################################################

per_connection=0.0
minimum_costs=0.0

default=(0.17, 1)

on (monday..friday) between (18:00..6:59) use (0.05, 1)
on (saturday..sunday) between () use (0.05, 1)

#public holidays
on (01/01, 03/15, 05/01, 08/20, 10/23, 11/01, 12/25, 12/26) between () use (0.05, 1)
on (easter+1) between () use (0.05, 1)
on (easter+50) between () use (0.05, 1)

################################################################
#
# Internet hivasok a 89-es korzetben Magyarorszagon (PapaTel korzete)
# Behivoszam: 0651389301
# Ervenyes: 2001. marc. 1-tol
# Keszitette: Pataki Miklos <pataki.miklos.1@freemail.hu> Papan
# Datum: 2002. feb. 10.
#
# Ha hibat eszlel vagy kerdese van
# kerem irjon nekem egy e-mail -t
#
################################################################