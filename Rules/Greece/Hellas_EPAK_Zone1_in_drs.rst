################################################################
# NOTES:
# These rules were made at May 25th, 2001
# These rules are valid since March, 2001
# You can check for changes in prices at http://www.ote.gr/
# These rules apply to you if you are accessing an ISP using an EPAK or PEAK
# number (0965-) in a city that is up to 45 km from your own, or in the same perfecture.
# The costs for an EPAK Zone 1 long distance phone-call are:
# 1 Unit (10.5) every 1 minute (60secs).
# Exception1: 22:00 - 08:00 :1 Unit every 630 sec.
# mail d.kamenopoulos@mail.ntua.gr for comments.
################################################################
# NAME OF THE RULESET. This is NEEDED for accounting purposes.
################################################################
name=Hellas_EPAK_Zone1_in_drs.rts

################################################################
# currency settings
################################################################

# defines DRS (Hellenic draxmi) to be used as currency
# symbol (not absolutely needed, default = "$")
currency_symbol=drs

# Define the position of the currency symbol.
# (not absolutely needed, default is "right")
currency_position=right

# Define the number of significat digits.
# (not absolutely needed, default is "2"
currency_digits=1

################################################################
# connection settings
################################################################

# NOTE: rules are applied from top to bottom - the
#       LAST matching rule is the one used for the
#       cost computations.

# This is the default rule which is used when no other rule
# applies. The first component "10.5" is the price of one
# "unit", while "60" is the duration in seconds.
# Therefore the following rule means: "Every 60 seconds 10.5
# DRS are added to the bill"
default=(10.5, 60)

#
# more complicated rules:
#

# "on monday until sunday from 22:00 until 08:00 the costs
# are 10.5 drs each 630 seconds"
on (monday..sunday) between (22:00..08:00) use (10.5, 630)
# "on sunday from 08:00 until 22:00 the costs
# are 10.5 each 630 seconds"
on (sunday) between (22:00..08:00) use (10.5, 630)
