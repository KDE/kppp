#################################################################################
# AddCom accounting rules
# http://www.CallOkay.net
#
# derived 26/09/99 by Frank Meier <Frank.Meier@deDanaan.de>
#         from CityCall by Stefan Markgraf and Stefan Troeger
#
#################################################################################

name=CallOkay.net
currency_symbol=DM
currency_position=right 
currency_digits=3
per_connection=0.0
minimum_costs=0.0
default=(0.039, 1)

# Montag bis Freitag
on (monday..sunday) between (8:00..7:59)   use (0.049, 1)

# Bundeseinheitliche Feiertage, Heiligabend und Silvester
on (01/01, easter-2, easter, easter+1, 05/01, easter+39, easter+49, easter+50,10/03, 12/24, 12/25, 12/26, 12/31) between (8:00..7:59) use (0.049, 1)

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

# Zusaetzliche Tage mit Feiertagstarif:
# 12/24     Heiligabend
# 12/31     Silvester
