################################################################
# NOTES: 
# This rules apply to you if you are accessing an ISP  not using an EPAK or PEAK
# number (0965-), in the same city as you.
# The costs for a local city phone-call are:1 Unit(6.0+18%FPA=7.08Drs)
# every 1 minute (60secs), and between 10:00pm and 08:00am 1 Unit every
# 3 minutes (180secs).
################################################################
#                             Achilleas Kotsis 
# Electrical and Computer Engineering, National Technical University of Athens
#           Webpage: http://www.chat.net.gr/~achille/
#           E-mail : mailto:achille@freemail.gr IRCnick: Achille
#           PGP Key: http://www.chat.net.gr/~achille/pgp.txt
################################################################
# NAME OF THE RULESET. This is NEEDED for accounting purposes.
################################################################
name=Hellas_local.rts

################################################################
# currency settings
################################################################

# defines DRS (Hellenic draxmi) to be used as currency
# symbol (not absolutely needed, default = "$")
currency_symbol=Drs

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
# applies. The first component "7.08" is the price of one
# "unit", while "60" is the duration in seconds.
# Therefore the following rule means: "Every 60 seconds 7.08 
# DRS are added to the bill"
default=(7.08, 60)

#
# more complicated rules:
#

# "on monday until sunday from 22:00 pm until 08:00 pm the costs
# are 7.08 each 180 seconds"
on (monday..sunday) between (22:00..08:00) use (7.08, 180)
