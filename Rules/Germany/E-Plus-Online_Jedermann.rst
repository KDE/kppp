################################################################
#
# E-Plus-Online accounting rules for everybody
# although without a handy!!
#  
# created 02/03/31 by Robert Klier<Robert.Klier@eplus-online.de>
#
################################################################
#
#
# Name:
#
name=E-Plus-Online_für_Jedermann
#
#
# Währungseinstellungen
#
currency_symbol=EUR
currency_position=right 
currency_digits=4
#
#
# Kosten pro Verbindung
#
per_connection=0.0
minimum_costs=0.0
#
#
# Kosten pro Zeit                                   EUR/Min
#   Mo. - Fr. 9 -18 Uhr                              0,0175
#   übrige Zeit sowie an bundeseinh. Feiertagen      0,0125
#
default=(0.002083, 10)
on (monday..friday) between (9:00..18:00) use (0.002917, 10)
on (01/01, easter-2, easter, easter+1, 05/01, easter+39, easter+49, easter+50, 10/03, 12/24..12/26, 12/31) between (0:00..23:59) use (0.002083, 10)
#
#
#
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
#
# Zusaetzliche Tage mit Feiertagstarif:
# 12/24     Heiligabend
# 12/31     Silvester






