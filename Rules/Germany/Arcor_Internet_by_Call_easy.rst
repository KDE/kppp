################################################################
#
# kppp ruleset for Arcor-Internet by Call easy accounts
# created 01/04/01 by Harald Bongartz <Harald@Bongartz.cx>
# modified 02/01/26 by Alexander Heide <alexander.heide@gmx.net>
#
################################################################


################################################################
#
# NAME OF THE RULESET. This is NEEDED for accounting purposes.
#
################################################################
name=Arcor-Internet by Call easy


################################################################
# currency settings
################################################################

currency_symbol=¤
currency_position=right 
currency_digits=2


################################################################
# connection settings
################################################################

per_connection=0.0
minimum_costs=0.0
default=(0.0148, 60)
on () between (18:00..08:59) use (0.0097, 60)
on (saturday..sunday, 01/01, easter-2, easter, easter+1, 05/01, easter+39, easter+49, easter+50, 10/03, 12/25, 12/26) between () use ( 0.0097, 60)


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



