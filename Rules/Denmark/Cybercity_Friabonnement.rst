### INFO #########################################################
# COUNTRY  : Denmark
# ISP      : Cybercity
# PRODUCT  : Friabonnement
# CURRENCY : DKR
# PHONENO  : 1033 ?
# AUTHOR   : Michael Falk Hansen
# SOURCE   : http://www.cybercity.dk/produkter/privat/friabonnement/
#
# Since the ISP market changes fast these rates may be outdated
# May be distributed freely. I take no responsibility for
# the correctness of the information in this file.
##################################################################

  name=DK_CYBERCITY_FRI
  currency_symbol=kr
  currency_position=right 
  currency_digits=2

### RATES ########################################################
# Date:               Septmeber 14, 2002
# Initial expense:    0.25 DKR
# Minimum expense:    0.25 DKR
#
# Peak hours:         0.22 DKR/min      ( 08:00-19:00 on weekdays )
# Off-peak hours:     0.11 DKR/min      ( 19:00-08:00 on weekdays,
#                                         all Saturday and Sunday )
### NOTES ########################################################
# No special rates for national holidays!
##################################################################

# INITIAL EXPENSE
  per_connection=0.25
  minimum_costs=0.25

# OFF-PEAK
  default=(0.0018333, 1)
  default=(0.0018333, 1, 3600)

# PEAK
  on (monday..friday) between (8:00..18:59) use (0.00366667, 1)

