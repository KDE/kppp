################################################################
# NOTES: 
# These rules were made at May  25th, 2001 (25/5/2001)
# These rules are valid since March, 2001 
# You can check for changes in prices at http://www.ote.gr/
# These rules apply to you if you are accessing an ISP using an EPAK or PEAK
# number (0965-) in the same city as you.
# H an o ISP vrisketai stin prwtevousa tou Nomou sas (plin
# Aitwloakarnanias kai Argolidas)  
# The costs for an EPAK local phone-call are:
# 1 Unit (0.03084 euro) every 315 sec.
# Exception: 22:00 - 08:00 :1 Unit every 630 sec  .
# prices are in euro. I assumed
# 10.5 drs = 0.03084 though 10.5drs = 0.030843.... euro
# this leads to an underestimation of the costs by less than
# 1 Euro per year, say 50 Hrs of dialup usage per week.
# email me if you think something should be corrected.
# creator:
# Dimitris Kamenopoulos, d.kamenopoulos@mail.ntua.gr
# studying Electr. and Computer Engineering in National Technical
# University of Athens
################################################################
# NAME OF THE RULESET. This is NEEDED for accounting purposes.
################################################################
name=Hellas_EPAK_local_in_euro.rts

################################################################
# currency settings
################################################################

# defines euro to be used as currency
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
# applies. The first component "0.03084" is the price of one
# "unit", while "315" is the duration in seconds.
# Therefore the following rule means: "Every 315 seconds 0.03084
# euro is added to the bill"
default=(0.03084, 315)

#
# more complicated rules:
#

# "on monday until sunday from 22:00 until 08:00 the costs
# are half as above"
on (monday..sunday) between (22:00..08:00) use (0.03084, 630)
