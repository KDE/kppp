#################################################################################
# German Nikoma Study & Surf - Internet by Call accounting rules
#
# Study & Surf is especially for students
#
#
# created 99/10/14 by Claus Atzenbeck <claus.atzenbeck@stud.uni-regensburg.de>
#   based on costs of 99/10/14 
#   look at http://www.nikoma.de/
#
# This file is based on German_Arcor_Internet_by_Call
#################################################################################

name=German_Nikoma_Study_and_Surf
currency_symbol=DM
currency_position=right 
currency_digits=2
per_connection=0.0
minimum_costs=0.0
default=(0.0007983333333333, 1)

# Montag bis Sonntag
on (monday..sunday) between (8:00..20:59)  use (0.0007983333333333, 1)
on (monday..sunday) between (21:00..0:59)  use (0.0006483333333333, 1) 
on (monday..sunday) between (1:00..7:59)  use (0.0004983333333333, 1) 

# Bundeseinheitliche Feiertage (kein Unterschied zu Werktagen)
#on (01/01, easter-2, easter, easter+1, 05/01, easter+39, easter+49, easter+50, 10/03, 12/25, 12/26) between (0:00..23:59) use (0.10, 60)

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
