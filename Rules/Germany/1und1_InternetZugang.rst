################################################################
#
# This is a rule set for kppp.
# 
# Created by Jesco Topp (jesco@users.sourceforge.net)
#
# Ruleset for 1&1 Internet.Zugang
#
#
################################################################


################################################################
#
# NAME OF THE RULESET. This is NEEDED for accounting purposes.
#
################################################################
name=1und1_InternetZugang

################################################################
# currency settings
################################################################

# defines 'EUR' (EURO) as currency symbol
#
currency_symbol=EUR

# The position of the currency symbol.
#
currency_position=right

# Define the number of significant digits.
# (not absolutely needed, default is "2"
currency_digits=2


################################################################
# connection settings
################################################################

# hourly accounting rules
on (monday..sunday) between (9:00..22:59) use (0.0099, 60)
on (monday..sunday) between (23:00..8:59) use (0.0049, 60)
