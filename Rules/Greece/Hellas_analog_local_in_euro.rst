################################################################
# NOTES:
# You can check for changes in prices at http://www.ote.gr/
# These rules apply to you if you are accessing an ISP without using an EPAK or PEAK
# number (0965-) in the same city as you, from an analog phone (lucky you).
# The costs for an analog local phone-call are:
# 0.03084 euro per connection.
################################################################
# Dimitris Kamenopoulos d.kamenopoulos@mail.ntua.gr
################################################################
# NAME OF THE RULESET. This is NEEDED for accounting purposes.
################################################################
name=Hellas_analog_local_in_euro.rts

################################################################
# currency settings
################################################################

# defines DRS (Hellenic draxmi) to be used as currency
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

# This is charged whenever you connect. If you don't have to
# pay per-connection, use "0" here or comment it out.
per_connection=0.03084

# This is the default rule which is used when no other rule
# applies.
default=(0.0, 0)
