### INFO #########################################################
# COUNTRY  : Denmark
# ISP      : Teledanmark
# PRODUCT  : Basis
# CURRENCY : DKR
# PHONENO  : 16101
# AUTHOR   : Peter Thorsager <thorsager@post.tele.dk>
# SOURCE   : http://www.opasia.dk/kundeservice/abonnement/info_basis.shtml
#
# Since the ISP market changes fast these rates may be outdated
# May be distributed freely. I take no responsibility for
# the correctness of the information in this file.
##################################################################

  name=DK_TDK_BASIS
  currency_symbol=kr
  currency_position=right 
  currency_digits=2

### RATES ########################################################
# Date:               February 21st - 2000
# Initial expense:    0.25 DKR
# Minimum expense:    0.00 DKR
#
# Peak hours:         0.29 DKR/min      ( 08:00-19:30 on weekdays + Saturday )
# Off-peak hours:     0.145 DKR/min     ( 19:30-08:00 on weekdays + Saturdays,
#                                         00:00-24:00 on Sundays )
#
### NOTES ########################################################
# No special rates for national holidays!
##################################################################

# INITIAL EXPENSE
  per_connection=0.25
  minimum_costs=0.00

# OFF-PEAK
  default=(0.002417, 1)

# PEAK
  on (monday..saturday) between (8:00..19:29) use (0.004833, 1)

