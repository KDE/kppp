################################################################
#
# German Callino Surf Basic accounting rules
# created 2000-11-24 by thomas holst (tho), holstt@thoserve.de
#
################################################################
#
# name of ruleset
#
################################################################

name=Callino_Surf_Basic

################################################################
#
# currency settings
#
################################################################

currency_symbol=DM
currency_position=right
currency_digits=2

################################################################
#
# connection settings
#
################################################################

per_connection=0.06
minimum_costs=0.0
default=(0.029,60)

on (monday..friday) between (22:00..08:00) use (0.025,60)

on (saturday..sunday) between() use (0.025,60)

on (01/01, easter-2, easter, easter+1, 05/01, easter+39, easter+49, easter+50, 10/03, 12/25, 12/26) between () use (0.025, 60)

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
