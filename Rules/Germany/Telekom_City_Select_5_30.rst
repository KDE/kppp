#################################################################################
# German Telekom accounting rules
#
# CityCall Select 5/30
#
# based on "Telekom_City.rst"
# created  97/07/18 by Stefan Markgraf <S_Markgraf@hotmail.com>
# modified 98/05/01 by Stefan Troeger  <stefan.troeger@wirtschaft.tu-chemnitz.de>
# modified to Select 5/30  99/10/17 by Reinhard Kunert<reinhard.kunert@gmx.de> 
#################################################################################

name=German_Telekom_City_Call_Select_5_30
currency_symbol=DM
currency_position=right 
currency_digits=2
per_connection=0.0
minimum_costs=0.0
default=(0.084, 90)

# Montag bis Freitag
on (monday..friday) between (5:00..8:59)   use (0.084, 150)
on (monday..friday) between (18:00..20:59) use (0.084, 150) 
on (monday..friday) between (21:00..4:59)  use (0.084, 240)

# Sonnabend, Sonntag
on (saturday..sunday) between (5:00..20:59) use (0.084, 150)
on (saturday..sunday) between (21:00..4:59) use (0.084, 240)

# Bundeseinheitliche Feiertage, Heiligabend und Silvester
on (01/01, easter-2, easter, easter+1, 05/01, easter+39, easter+49, easter+50, 10/03, 12/24, 12/25, 12/26, 12/31) between (5:00..20:59) use (0.121, 150)
on (01/01, easter-2, easter, easter+1, 05/01, easter+39, easter+49, easter+50, 10/03, 12/24, 12/25, 12/26, 12/31) between (21:00..4:59) use (0.121, 240)

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
