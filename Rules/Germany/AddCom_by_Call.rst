################################################################
#
# created 99/09/27 by M.Jerger
# email:micha@mjerger.de
#
################################################################


################################################################
#
# NAME OF THE RULESET. This is NEEDED for accounting purposes.
#
################################################################
name=AddCom by Call


################################################################
# currency settings
################################################################

currency_symbol=DM
currency_position=right 
currency_digits=2


################################################################
# connection settings
################################################################

per_connection=0.0
minimum_costs=0.0
default=(0.059, 60)
on () between (21:00..08:59) use (0.039, 60)
on (saturday..sunday, 01/01, easter-2, easter, easter+1, 05/01, easter+39, easter+49, easter+50, 10/03, 12/25, 12/26) between (09:00..20:59) use (0.055, 60)
on (saturday..sunday, 01/01, easter-2, easter, easter+1, 05/01, easter+39, easter+49, easter+50, 10/03, 12/25, 12/26) between (21:00..08:59) use ( 0.039, 60)


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

