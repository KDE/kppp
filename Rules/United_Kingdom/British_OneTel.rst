
################################################################
#
# 6 Jan 2001
#
# I TAKE NO RESPONSIBILITY FOR THE VALIDITY AND ACCURACY OF
# THIS INFORMATION, IF YOU ARE SO CONCERNED PLEASE CONTACT
# YOUR OWN TELEPHONE PROVIDER AND CONFIGURE THIS YOURSELF.
# AND DO NOT USE THIS FILE.
#
#
# Alain Trembleau
# <alain@platodesigns.com
#
# OneTel have a flat rate of 1p per minute at all times
# British_OneTel
################################################################


################################################################
#
# NAME OF THE RULESET. This is NEEDED for accounting purposes.
#
################################################################
name=British_OneTel


################################################################
# currency settings
################################################################

# 1 pound sterling = 100 pence sterling
#
# British monetary format examples:
#
#   £1.57 = 1 pound and 57 pence.
#    157p = 1 pound and 57 pence.
#
# defines GBP (Pound Sterling) to be used as currency symbol
# (default = "$")
currency_symbol=£

# Define the position of the currency symbol.
# (default is "right")
currency_position=left

# Define the number of significat digits.
# (default is "2")
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

# minimum costs per connection. If the costs of a phone call
# are less than this value, this value is used instead.
# I haven't checked this, but I believe that there is a minimum
# cost per call of 5p.
minimum_costs=0.05

# This is the default rule which is used when no other rule
# applies. The first component "0.01" is the price of one
# "unit", while "60" is the duration in seconds.
# Therefore the following rule means: "Every 60 seconds 0.01
# Pounds Sterling are added to the bill"
#
# Considering it is a flat rate, that's all that's needed!
default=(0.01, 60)

################################################################
# charge bands
################################################################

# No charge bands are needed since this uses a flat rate.
