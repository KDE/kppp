################################################################
#
# German Freenet accounting rules
# created 2000-11-24 by thomas holst (tho), holstt@thoserve.de
# up-to-date 2000-11-01 by volker schlechter, schlecht@selk-stuttgart.de
# up-to-date 2002-02-11 by Friedemann Schorer, friedemann.schorer@gmx.de
#
################################################################
#
# name of ruleset
#
################################################################

name=Freenet

################################################################
#
# currency settings
#
################################################################

currency_symbol=¤
currency_position=left
currency_digits=2

################################################################
#
# connection settings
#
################################################################

#per_connection=0.06
#minimum_costs=0.0

# Default - falls mal was nicht klappt, wird sicherheitshalber
# der teuerste Tarif genommen
default=(0.0145,60)

on (monday..sunday) between (22:00..09:00) use (0.0089,60)
on (monday..sunday) between (09:00..18:00) use (0.0145,60)
on (monday..sunday) between (18:00..22:00) use (0.0115,60)


# on (01/01, easter-2, easter, easter+1, 05/01, easter+39, easter+49, easter+50, 10/03, 12/25, 12/26) between () use (0.025, 60)

# Bundeseinheitliche Feiertage:
# 01/01     Neujahr
# easter-2  Karfreitag
# easter    Ostersonntag
# easter+1  Ostermontag
# 05/01     Maifeiertag Tag der Arbeit
# easter+39 Christi Himmelfahrt
# easter+49 Pfingstsonntag
# easter+50 Pfingstmontag
# 10/03     Tag der Deutschen Einheit
# 12/25     1. Weihnachtstag
# 12/26     2. Weihnachtstag
