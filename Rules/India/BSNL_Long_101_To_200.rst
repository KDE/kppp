################################################################
#
# This is the latest rule set for Bharat Sanchar Nigam Limited, 
# It works between 101k.m. to 200k.m. from any major cities in 
# India. This revised tariff is applicable from 01-02-2001.  
#
# Name  : Rex
# email : rex@caltiger.com
#
#
# NOTE: THIS IS MADE UNDER LATEST INDIAN TELECOM TARIFF. I AM 
# NOT RESPONSIBLE FOR ANY DAMAGE DUE TO USE OF THIS RULE SET. 
# 
#
# Thanks, Bernd Wuebben
# wuebben@math.cornell.edu / wuebben@kde.org
################################################################


################################################################
#
# BSNL_Long_101_To_200.rst Applicable between 101 k.m to 200k.m.
# from any major cities in India.
#
# Salient features of Revised Telecom Tariff.
# Medium-long distance (101k.m. - 200k.m.) call charging 1 unit 
# per 30 seconds.
# Unit of call charge Rs.1.20 per unit.
# for all types of charges  : Service Tax @ 5% to be paid extra.
################################################################
name=default

################################################################
# Rs.1.00= Paise100
################################################################

# defines ATS (Austrian Schilling) to be used as currency
# symbol (not absolutely needed, default = "$")
currency_symbol=Rs.

# Define the position of the currency symbol.
# (not absolutely needed, default is "right")
currency_position=left

# Define the number of significant digits.
# (not absolutely needed, default is "2"
currency_digits=2



################################################################
# connection settings
################################################################

# NOTE: rules are applied from top to bottom - the
#       LAST matching rule is the one used for the
#       cost computations.

# This is charged whenever you connect. If you don't have to
# pay per-connection, use "0" here or comment it out.
per_connection=0.0


# minimum costs per per connection. If the costs of a phone
# call are less than this value, this value is used instead
minimum_costs=0.0


# You pay .74 for the first 180 seconds ( 3 minutes) no matter
# whether you are connected for 1 second or 180 seconds.
# This rule will take priority during the first 180 seconds
# over any other rule, in particular the 'default' rule.
# have a look at costgraphs.gif in the docs directory
# of the kppp distribution for a graphic illustration.
flat_init_costs=(1.26, 30)

# This is the default rule which is used when no other rule
# applies. The first component "0.1" is the price of one
# "unit", while "72" is the duration in seconds.
# Therefore the following rule means: "Every 72 seconds 0.1 
# ATS are added to the bill"
default=(1.26, 30)

#
# more complicated rules:
#
