################################################################
#
# German Mobilcom Freenet accounting rules
# 
# created 1999/11/27 by Sebastian Linz <Sebastian.Linz@gmx.net>
#
################################################################


################################################################
#
# NAME OF THE RULESET. This is NEEDED for accounting purposes.
#
################################################################

name=Mobilcom_Freenet


################################################################
# currency settings
################################################################

currency_symbol=DM
currency_position=right 
currency_digits=2


################################################################
# connection settings
################################################################

per_connection=0
minimum_costs=0.0
# flat_init_costs=(0.74,180)

# This is the default rule which is used when no other rule
# applies. The first component "0.1" is the price of one
# "unit", while "72" is the duration in seconds.
# Therefore the following rule means: "Every 72 seconds 0.1 
# ATS are added to the bill"
default=(0.05, 60)



