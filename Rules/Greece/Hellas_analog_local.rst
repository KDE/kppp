################################################################
# NOTES: 
# These rules were made at December 4th, 1999 (4/12/99)
# These rules are valid since November 22nd, 1999 (22/11/99)
# You can check for changes in prices at http://www.ote.gr/
# These rules apply to you if you are accessing an ISP without using an EPAK or PEAK
# number (0965-) in the same city as you, from an analog phone.
# The costs for an analog local phone-call are:
# 1 Unit (7.0+18%VAT=8.26Drs) per connection.
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
name=Hellas_analog_local.rts

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

# This is charged whenever you connect. If you don't have to
# pay per-connection, use "0" here or comment it out.
per_connection=8.26

# This is the default rule which is used when no other rule
# applies. The first component "8.26" is the price of one
# "unit", while "60" is the duration in seconds.
# Therefore the following rule means: "Every 210 seconds 8.26
# DRS are added to the bill"

default=(0.0, 0)
