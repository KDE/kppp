################################################################
# NOTES: 
# These rules were made at December 4th, 1999 (4/12/99)
# These rules are valid since November 22nd, 1999 (22/11/99)
# You can check for changes in prices at http://www.ote.gr/
# These rules apply to you if you are accessing an ISP using an EPAK or PEAK
# number (0965-) in a city that is up to 45 km from your own, or in the same perfecture.
# The costs for an EPAK Zone 1 long distance phone-call are:
# 1 Unit (7.0+18%VAT=8.26Drs) every 1 minute (60secs).
# Exception1: 22:00 - 08:00 :1 Unit every 7 minutes (420secs).
# Exception2: Sunday 22:00 - 08:00 :1 Unit every 2 minutes (120secs).
################################################################
#                             Achilleas Kotsis 
# Electrical and Computer Engineering, National Technical University of Athens
#           Webpage: http://www.cc.ece.ntua.gr/~akots/
#           E-mail : mailto:achille@freemail.gr IRCnick: Achille
#           PGP Key: http://www.cc.ece.ntua.gr/~akots/pgp.txt
################################################################
#                    Koussouris Sotiris a.k.a Broxopoios
# Electrical and Computer Engineering, National Technical University of Athens
# root@sound.gr
# http://broxopoios.cjb.net/
################################################################
# NAME OF THE RULESET. This is NEEDED for accounting purposes.
################################################################
name=Hellas_EPAK_Zone1.rts

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
# applies. The first component "8.26" is the price of one
# "unit", while "60" is the duration in seconds.
# Therefore the following rule means: "Every 60 seconds 8.26
# DRS are added to the bill"
default=(8.26, 60)

#
# more complicated rules:
#

# "on monday until sunday from 22:00 until 08:00 the costs
# are 8.26 each 420 seconds"
on (monday..sunday) between (22:00..08:00) use (8.26, 420)
# "on sunday from 08:00 until 22:00 the costs
# are 8.26 each 120 seconds"
on (sunday) between (22:00..08:00) use (8.26, 120)
