#################################################################
# Danish rate ruleset for Mobilix wanadoo, Danmark              #
# Created 00.03.19 by Frank M.G. Jørgensen<gajda@iname.com>     # 
# Since everything in this world changes quite frequently these #
# rates may be outdated.                                        # 
# You may use and distribute this ruleset freely, but I take no #
# responsibility for the correctness of the information in this #
# file.                                                         #
# In other words: If it breaks, you get to keep the pieces.     #
#################################################################

name=Mobilix_Wanadoo
currency_symbol=kr
currency_position=right 
currency_digits=2

#################
# Mobilix Internet Prices Danmark
# Initial expense: 0.25 DKR
# rates at off-peak hours: 0.1 kr/min - reduced to 0.08 kr/min after 1hour 
# rates at reduced peak hours: 0.18 kr/min
# rates at peak hours: 0.27 kr/min
# rates at night: 0.07 kr/min
# Reduced peak hours is 16:00 - 19:00 monday to friday &  8:00 - 19:00onsaturdays 
# Peak hours is 8:00 - 19:00 monday to friday 
# Night is 24:00 - 8:00
# Off-peak hours is everything else, and on national holidays
#################



# Initial expense is 0.25 DKR
per_connection=0.25
minimum_costs=0.0

# Off-peak: 0.1 kr/min and 0.08kr/min +1 hour
default=(0.001667, 1)
default=(0.001333, 1, 3600)

# Peak hours: 0.27 kr/min
on (monday..friday) between (8:00..15:59) use (0.004500, 1)

# Reduced peak hours: 0.18 kr/min
on (monday..friday) between (16:00..18:59) use (0.003000, 1)
on (saturday..saturday) between (8:00..18:59) use (0.003000, 1)

# Night rates: 0.07 kr/min
on (monday..sunday) between (0:00..7:59) use (0.001167, 1)

# National Holidays ( like off-peak hours )

# Newyear
on (01/01) between (8:00..23:59) use (0.001667, 1)
on (01/01) between (8:00..23:59) use (0.001333, 1, 3600)

# "Skærtorsdag"
on (easter-3) between (8:00..23:59) use (0.001667, 1)
on (easter-3) between (8:00..23:59) use (0.001333, 1, 3600)

# "Langfredag"
on (easter-2) between (8:00..23:59) use (0.001667, 1)
on (easter-2) between (8:00..23:59) use (0.001333, 1, 3600)

# "2. påskedag"
on (easter+1) between (8:00..23:59) use (0.001667, 1)
on (easter+1) between (8:00..23:59) use (0.001333, 1, 3600)

# "Bededag"
on (easter+26) between (8:00..23:59) use (0.001667, 1)
on (easter+26) between (8:00..23:59) use (0.001333, 1, 3600)

# "Kristi himmelfart"
on (easter+39) between (8:00..23:59) use (0.001667, 1)
on (easter+39) between (8:00..23:59) use (0.001333, 1, 3600)

# "2. pinsedag"
on (easter+50) between (8:00..23:59) use (0.001667, 1)
on (easter+50) between (8:00..23:59) use (0.001333, 1, 3600)

# Christmas day
on (12/25) between (8:00..23:59) use (0.001667, 1)
on (12/25) between (8:00..23:59) use (0.001333, 1, 3600)

# "Boxing day"
on (12/26) between (8:00..23:59) use (0.001667, 1)
on (12/26) between (8:00..23:59) use (0.001333, 1, 3600)
