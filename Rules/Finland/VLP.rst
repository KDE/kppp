# VLP:n hinnat 2.1.2003 VLP:n asiakaspalvelun mukaan
# Mikko Korhonen mjkorhon@cc.hut.fi
################################################################


################################################################
#
# NAME OF THE RULESET. This is NEEDED for accounting purposes.
#
################################################################
name=VLP

################################################################
# currency settings
################################################################

# this is the euro sign in UTF-8
currency_symbol=¤

# Define the position of the currency symbol.
# (not absolutely needed, default is "right")
currency_position=right

# Define the number of significant digits.
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
per_connection=0.0821


# minimum costs per per connection. If the costs of a phone
# call are less than this value, this value is used instead
#minimum_costs=0.0


# You pay .74 for the first 180 seconds ( 3 minutes) no matter
# whether you are connected for 1 second or 180 seconds.
# This rule will take priority during the first 180 seconds
# over any other rule, in particular the 'default' rule.
# have a look at costgraphs.gif in the docs directory
# of the kppp distribution for a graphic illustration.
#flat_init_costs=(0.74,180)

# This is the default rule which is used when no other rule
# applies. The first component "0.1" is the price of one
# "unit", while "72" is the duration in seconds.
# Therefore the following rule means: "Every 72 seconds 0.1
# ATS are added to the bill"
# cost is 1.34 cents per minute with accuracy of 1 second
# 8 significant digits should be enough
default=(0.00022333333, 1)

