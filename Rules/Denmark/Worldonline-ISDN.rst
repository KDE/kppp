### INFO #########################################################
# COUNTRY  : Denmark
# ISP      : WorldOnline
# PRODUCT  : World Access (ISDN)
# CURRENCY : DKR
# PHONENO  : 1049 3883 3883
# AUTHOR   : Peter Thorsager <thorsager@post.tele.dk>
# SOURCE   : http://www.worldonline.dk/home_nyeminutpriser.html
#
# Since the ISP market changes fast these rates may be outdated
# May be distributed freely. I take no responsibility for
# the correctness of the information in this file.
##################################################################

  name=DK_WORLDONLINE_ISDN
  currency_symbol=kr
  currency_position=right 
  currency_digits=2

### RATES ########################################################
# Date:               February 21st - 2000
# Initial expense:    0.10 DKR
# Minimum expense:    0.00 DKR
# Additional timeexpense (3sec) = 0.011kr (max)
#
# Peak hours:         0.22 DKR/min      ( 06:00-18:59 on weekdays )
# Off-peak hours:     0.11 DKR/min      ( 19:00-23:59 on weekdays,
#                                         06:00-23:59 on Saturdays and Sundays )
# Night rates:        0.06 DKR/min      ( 00:00-05:59 all days )
# Time rebate:        all rates are reduced to 50% after 1 hour
#
# NOTE: No special rates for national holidays!
### REBATES ######################################################
# (not included in cost-calculations!) 
# Volume Rebate:      250-500 DKR = 5%
#                        +500 DKR = 10%
# Students:           Members of World Student Club receives a 10%
#                     discount on all minute rates
#
### NOTES ########################################################
# No special rates for national holidays!
##################################################################


# INITIAL EXPENSE
  per_connection=0.10
  minimum_costs=0.011

# OFF-PEAK and OFFPEAK +1 hour
  default=(0.001833, 1)
  default=(0.000917, 1, 3600)

# PEAK and PEAK +1 hour
  on (monday..friday) between (6:00..18:59) use (0.003667, 1)
  on (monday..friday) between (6:00..18:59) use (0.003667, 1, 3600)

# NIGHT and NIGHT +1 hour
  on (monday..sunday) between (00:00..05:59) use (0.001000, 1)
  on (monday..sunday) between (00:00..05:59) use (0.000500, 1, 3600)

