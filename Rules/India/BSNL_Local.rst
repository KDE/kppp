#####################################################################
#
# This is the latest rule set for Bharat Sanchar Nigam Limited, 
# It works arround 50 k.m. from any major cities in India.
# This rule set is for M-Call to Internet Service Provider,
# (numbers dialing with the 17XXXX format).   
# This revised tariff is applicable from 1st May 2003.
# This is based on the BSNL Alternate packages and is applicable
# to all the subcatogaries in this package scheme. 
#
# Name  : Thomas Mathew
# email : thomasmathewk@netscape.net
#
#
# NOTE: THIS IS MADE UNDER THE LATEST INDIAN TELECOM TARIFF.
# I AM NOT RESPONSIBLE FOR ANY DAMAGE DUE TO USE OF THIS RULE SET. 
# 
#
# Thanks, Bernd Wuebben
# wuebben@math.cornell.edu / wuebben@kde.org
#####################################################################


#####################################################################
#
# BSNL_local.rst Applicable only arround 50 k.m of any major 
# cities in India.  Numbers dialing with 17XXXX format without 
# any prefix.
#
# Salient features of Revised Telecom Tariff.
# Local call charging 1 unit per 3 minutes during peak hours.
# Peak hours - 06:30AM to 10:30PM.
# Local call charging 1 unit per 10 minutes during Off peak hours.
# Off peak hours - 10:30PM to 06:30AM.
# Off peak rates are applicable for 24 hrs. on National Holidays 
# & Sundays.
# Unit of call charge Rs.1.20 per unit
#####################################################################
name=default

#####################################################################
# Rs.1.00= Paise100
#####################################################################

# defines Rs. (Indian Rupee) to be used as currency
# symbol (default = "$")
currency_symbol=Rs.

# Define the position of the currency symbol.
# (not absolutely needed, default is "right")
currency_position=left

# Define the number of significant digits.
# (not absolutely needed, default is "2"
currency_digits=2



#####################################################################
# connection settings
#####################################################################

# NOTE: rules are applied from top to bottom - the
#       LAST matching rule is the one used for the
#       cost computations.

# This is charged whenever you connect. If you don't have to
# pay per-connection, use "0" here or comment it out.
per_connection=0.0


# minimum costs per connection. If the costs of a phone
# call are less than this value, this value is used instead
minimum_costs=1.20


# You pay 1.20 for the first 180 seconds ( 3 minutes) no matter
# whether you are connected for 1 second or 180 seconds.
# This rule will take priority during the first 180 seconds
# over any other rule, in particular the 'default' rule.
# have a look at costgraphs.gif in the docs directory
# of the kppp distribution for a graphic illustration.
# flat_init_costs=(1.20, 180)

# This is the default rule which is used when no other rule
# applies. The first component "1.20" is the price of one
# "unit", while "180" is the duration in seconds.
# Therefore the following rule means: "Every 180 seconds 1.20 
# Rs. are added to the bill"
# default=(1.20, 180)

#
# more complicated rules:
#

# On peak hours from 06:30AM until 10:30PM the costs
# are 1.20 each 180 seconds.
on (monday..sunday) between (06:30..22:30) use (1.20, 180)
# On Off peak hours from 10:30PM until 06:30AM the costs
# are 1.20 each 600 seconds.
on (monday..sunday) between (22:30..06:30) use (1.20, 600)
# On all the Sundays and the three National Holidays 
# viz. Republic Day (Jan 26th), Independance Day (Aug 15th) 
# and Mahatma Gandhi's Birthday (Oct 2nd)the costs 
# are 1.20 each 600 seconds.
on (sunday, 1/26, 8/15, 10/2) between () use (1.20,600)
#
# NOTE: This rule set is applicable only if your system date and 
# time are correct to your local settings.
# End of Rules.
# Updates to this rule set and suggessions are most welcome.
#####################################################################

