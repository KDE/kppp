### INFO #########################################################
# COUNTRY  : Denmark
# ISP      : Teledanmark
# PRODUCT  : Favoritinternet
# CURRENCY : DKR
# PHONENO  : 16110
# AUTHOR   : Peter Thorsager <thorsager@post.tele.dk>
# SOURCE   : http://www.favoritinternet.dk
#
# Since the ISP market changes fast these rates may be outdated
# May be distributed freely. I take no responsibility for
# the correctness of the information in this file.
##################################################################

  name=DK_TDK_FAVORIT
  currency_symbol=kr
  currency_position=right 
  currency_digits=2

### RATES ########################################################
# Date:               February 21st - 2000
# Initial expense:    0.25 DKR
# Minimum expense:    0.00 DKR
#
# Peak hours:         0.28 DKR/min      ( 08:00-16:00 on weekdays )
# Reduced peak hours: 0.20 DKR/min      ( 16:00-19:30 on weekdays,
#                                         08:00-16:00 on Saturdays )
# Off-peak hours:     0.10 DKR/min      ( 19:30-08:00 on weekdays,
#                                         16:00-00:00 on Saturdays,
#                                         00:00-24:00 on Sundays and national holidays )
# Time rebate:        Off-peak rates are reduced to 0.08DKR after 1 hour
##################################################################

# INITIAL EXPENSE
  per_connection=0.25
  minimum_costs=0.00

# OFF-PEAK and OFF-PEAK +1 hour
  default=(0.001667, 1)
  default=(0.001333, 1, 3600)

# PEAK
  on (monday..friday) between (8:00..15:59) use (0.004667, 1)

# REDUCED PEAK
  on (monday..friday) between (16:00..19:29) use (0.003333, 1)
  on (saturday..saturday) between (8:00..15:59) use (0.003333, 1)

# NATIONAL HOLIDAYS ( like off-peak hours )
# Newyear
  on (01/01) between () use (0.001667, 1)
  on (01/01) between () use (0.001333, 1, 3600)

# "Skærtorsdag"
  on (easter-3) between () use (0.001667, 1)
  on (easter-3) between () use (0.001333, 1, 3600)

# "Langfredag"
  on (easter-2) between () use (0.001667, 1)
  on (easter-2) between () use (0.001333, 1, 3600)

# "2. påskedag"
  on (easter+1) between () use (0.001667, 1)
  on (easter+1) between () use (0.001333, 1, 3600)

# "St. bededag"
  on (easter+26) between () use (0.001667, 1)
  on (easter+26) between () use (0.001333, 1, 3600)

# "Kristi himmelfart"
  on (easter+39) between () use (0.001667, 1)
  on (easter+39) between () use (0.001333, 1, 3600)

# "2. pinsedag"
  on (easter+50) between () use (0.001667, 1)
  on (easter+50) between () use (0.001333, 1, 3600)

# Christmas day
  on (12/25) between () use (0.001667, 1)
  on (12/25) between () use (0.001333, 1, 3600)

# 2.nd christmas day
  on (12/26) between () use (0.001667, 1)
  on (12/26) between () use (0.001333, 1, 3600)

