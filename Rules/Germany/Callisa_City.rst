################################################################
#
# German Callisa City accounting rules
# created 2001-10-13 Herbert Nachbagauer (herby74@freemail.de)
# 
# NOTE: Please check at http://www.callisa.de/callisacities.php3
#       if the rate applies to your city.
#
################################################################
#
# name of ruleset
#
################################################################

name=Callisa_City

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

default=(0.0244,60)

on (monday..friday) between (09:00..18:00) use (0.0244,60)

on (monday..friday) between (18:00..24:00) use (0.0189,60)

on (monday..friday) between (00:00..09:00) use (0.0189,60)

on (saturday..sunday) between() use (0.0189,60)

on (01/01, easter-2, easter, easter+1, 05/01, easter+39, easter+49, easter+50, 10/03, 12/25, 12/26) between () use (0.0189, 60)

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
