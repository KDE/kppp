### INFO #########################################################
# COUNTRY  : Denmark
# ISP      : Mobilix
# PRODUCT  : Wanadoo
# CURRENCY : DKR
# PHONENO  : 1029 ?
# AUTHOR   : Peter Thorsager <thorsager@post.tele.dk>
# SOURCE   : http://www.wanadoo.dk/
#
# Since the ISP market changes fast these rates may be outdated
# May be distributed freely. I take no responsibility for
# the correctness of the information in this file.
##################################################################

  name=DK_MOBILIX_WANADOO
  currency_symbol=kr
  currency_position=right 
  currency_digits=2

### RATES ########################################################
# Date:               February 21st - 2000
# Initial expense:    0.25 DKR
# Minimum expense:    0.00 DKR
#
# Peak hours:         0.27 DKR/min      ( 08:00-16:00 on weekdays )
# Reduced peak hours: 0.18 DKR/min      ( 16:00-19:00 on weekdays,
#                                         08:00-19:00 on Saturdays )
# Off-peak hours:     0.10 DKR/min      ( 19:00-00:00 on weekdays + Saturday
#                                         08:00-00:00 on Sundays )
# Night hours:        0.07 DKR/min      ( 00:00-08:00 all days )
# Time rebate:        Off-peak rates are reduced to 0.08DKR after 1 hour
#
### NOTES ########################################################
# No special rates for national holidays!
##################################################################

# INITIAL EXPENSE
  per_connection=0.25
  minimum_costs=0.00

# OFF-PEAK and OFF-PEAK +1 hour
  default=(0.001667, 1)
  default=(0.001333, 1, 3600)

# PEAK
  on (monday..friday) between (8:00..15:59) use (0.004500, 1)

# REDUCED PEAK
  on (monday..friday) between (16:00..18:59) use (0.003000, 1)
  on (saturday..saturday) between (8:00..18:59) use (0.003000, 1)

# NIGHT
  on (monday..sunday) between (00:00..07:59) use (0.001167,1)

