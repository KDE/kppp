################################################################
#
# Prices valid on 1st April 2001
#
# I am not responsible for the accuracy of this information.  If
# you know this file to be out of date or inaccurate, please
# modify it.
#
# Jonathan Melhuish <jonathanmelhuish@email.com>
#
# Some comments copied from BT ruleset by Darryl L Miles
# <dlm@g7led.demon.co.uk>
#
# This ruleset covers numbers covered by the Connaught Telecom
# 'Internet' tarif - ie. 0845 numbers.
#
# All prices are *exclusive* of V.A.T.
#
################################################################


################################################################
#
# NAME OF THE RULESET. This is NEEDED for accounting purposes.
#
################################################################
name=Connaught_Connaught_Telecom


################################################################
# currency settings
################################################################

# 1 pound sterling = 100 pence sterling
# 1 pence sterling is our lowest unit of coinage, but
#  I suspect telephone companys may remember the
#  fractions of pence for billing.
#
# Our monetry format:
#
# e.g. £1.57 = 1 pound and 57 pence.
# e.g. 157p = 1 pound and 57 pence.
#
# defines UKP (PoundSterling) to be used as currency
# symbol (not absolutely needed, default = "$")
currency_symbol=£

# Define the position of the currency symbol.
# (not absolutely needed, default is "right")
currency_position=left

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
per_connection=0.0

# minimum costs per per connection. If the costs of a phone
# call are less than this value, this value is used instead
minimum_costs=0.01

# This is the default rule which is used when no other rule
# applies. The first component "0.04" is the price of one
# "unit", while "60" is the duration in seconds.
# Therefore the following rule means: "Every 60 seconds 0.04
# PoundsSterling are added to the bill"
#
# I've simply taken the worst case price for this, peak time
#  calls.  Just in case I miss a period of time out.
default=(0.04, 60)

################################################################
# charge bands
################################################################

# Charge:	3.3p/min (peak time)
# Unit Time:	per second
#
on (monday..friday) between (8:00..17:59) use (0.00055, 1)

# Change:	1.7p/min (off-peak time)
# Unit Time:	per second
#
on (monday..friday) between (18:00..7:59) use (0.0002, 1)

# Charge:	0.8p/min (weekend rate)
# Unit Time:	per second
#
on (saturday..sunday) between () use (0.0001, 1)
