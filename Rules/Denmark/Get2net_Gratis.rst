### INFO #########################################################
# COUNTRY  : Denmark
# ISP      : Get2net
# PRODUCT  : Gratis
# CURRENCY : DKR
# PHONENO  : 1001 ?
# AUTHOR   : Peter Thorsager <thorsager@post.tele.dk>
# SOURCE   : http://www.get2net.dk/get2net/plsql/PackageSupport.PageSupportMain?i_Function=DISPLAYPAGE&i_ID=takster-gratis
#
# Since the ISP market changes fast these rates may be outdated
# May be distributed freely. I take no responsibility for
# the correctness of the information in this file.
##################################################################

  name=DK_GET2NET_GRATIS
  currency_symbol=kr
  currency_position=right 
  currency_digits=2

### RATES ########################################################
# Date:               February 21st - 2000
# Initial expense:    0.25 DKR
# Minimum expense:    0.00 DKR
#
# Peak hours:         0.27 DKR/min      ( 08:00-19:00 on weekdays )
# Off-peak hours:     0.135 DKR/min     ( 19:00-08:00 on weekdays,
#                                         all Saturday and Sunday )
# Time rebate:        Off-peak rates are reduced to 0.10DKR after 1 hour
#
### NOTES ########################################################
# (not included in cost-calculations!)
# Dialup attempts shorter than 20s are free (not for ISDN)
#
### NOTES ########################################################
# No special rates for national holidays!
##################################################################

# INITIAL EXPENSE
  per_connection=0.25
  minimum_costs=0.00

# OFF-PEAK
  default=(0.002250, 1)
  default=(0.001667, 1, 3600)

# PEAK
  on (monday..friday) between (8:00..18:59) use (0.004500, 1)

