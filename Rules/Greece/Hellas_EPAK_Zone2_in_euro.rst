################################################################
# NOTES:
# these rules were made at May 25th 2001 (25/2/2001)
# they are valid since March 2001
# prices are in Euro. 10.5 drs = 0.03084 euro
################################################################
# Dimitris Kamenopoulos, el97146@mail.ntua.gr
################################################################
# NAME OF THE RULESET. This is NEEDED for accounting purposes.
################################################################
name=Hellas_EPAK_Zone2_in_euro.rts

################################################################
# currency settings
################################################################

# defines Euro to be used as currency
# symbol (not absolutely needed, default = "$")
currency_symbol=euro

# Define the position of the currency symbol.
# (not absolutely needed, default is "right")
currency_position=right

# Define the number of significat digits.
# (not absolutely needed, default is "2"
currency_digits=2

################################################################
# connection settings
################################################################

# NOTE: rules are applied from top to bottom - the
#       LAST matching rule is the one used for the
#       cost computations.

# This is the default rule which is used when no other rule
# applies. The first component "10.5" is the price of one
# "unit", while "0.03084" is the duration in seconds.
# Therefore the following rule means: "Every 22.5 seconds 0.03084
# euro are added to the bill"
default=(0.03084, 22.5)

#
# more complicated rules:
#

# "on monday until sunday from 22:00 until 08:00 the costs
# are 0.03084 each 630 seconds"
on (monday..sunday) between (22:00..08:00) use (0.03084, 630)

