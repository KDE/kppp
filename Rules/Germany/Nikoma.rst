######################################################################
# Nikoma Internet by Call Ruleset
# Gebuehren: 08:00 - 01:00 4.79Pf/min, 01:00 - 08:00 2.99Pf/min
# created on 18.10.1999 by Thomas Escher
# <thomas-escher@notesdev.de>
# Please visit my Linux Homepage: http://www.linux-power.notrix.de
#######################################################################

name=Nikoma
currency_symbol=DM
currency_position=right 
currency_digits=2
per_connection=0.0
minimum_costs=0.0

# default cost auf Maximum setzen
default=(0.000793, 1)

# Deutschlandweiter Tarif
on (monday..sunday)   between (1:00..7:59)  use (0.000498, 1)
on (monday..sunday)   between (8:00..0:59)  use (0.000793,  1)

# Bundesweite Feiertage:
# 01/01     Neujahr
# easter-2  Karfreitag
# easter    Ostersonntag
# easter+1  Ostermontag
# 05/01     Tag der Arbeit
# easter+39 Christi Himmelfahrt
# easter+49 Pfingstsonntag
# easter+50 Pfingstmontag
# 10/03     Tag der Deutschen Einheit
# 12/25     1. Weihnachtstag
# 12/26     2. Weihnachtstag
