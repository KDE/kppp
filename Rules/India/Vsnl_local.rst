################################################################
# India's Vsnl Accounting rules (for local phone dial up connections).
# Created 18th June, 1999 by Madhusudan Singh (kchhabra@ch1.dot.net.in)
# Valid till : next second or next great civilization (as your judgement
# thinks !!!)
################################################################
#DISCLAIMER:
#There is no guarantee that these rules are valid for all users.
#They work for me and I can't care more. In case, you find another rule
#set appropriate to your region, please mail it to wuebben@kde.org
#I do not accept any responsibility for any damage/inconvenience caused by
#the use of these rules. Use at your own risk.
################################################################
#
# NAME OF THE RULESET. This is NEEDED for accounting purposes.
#
################################################################
name=vsnl

################################################################
# currency settings
################################################################
#defines Indian rupees as the currency.
# Re 1/- = 100 paise.
#Plural for Re. is Rs. 
#Paise is the lowest unit of coinage (Though it is hard to find any change 
#in the market, I think that we should believe the GOI that it is really the lowest
#monetary unit)
# symbol (not absolutely needed, default = "Rs")
currency_symbol=Rs.

# Define the position of the currency symbol.
# (not absolutely needed, default is "right")
currency_position=left 

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
per_connection=0.0


# minimum costs per per connection. If the costs of a phone
# call are less than this value, this value is used instead
minimum_costs=0.0


#flat_init_costs=(1.25,120)

# This is the default rule which is used when no other rule
# applies. The first component "1.40" is the price of one
# "unit", while "180" is the duration in seconds.
# Therefore the following rule means: "Every 180 seconds 1.40 
# Rupees are added to the bill"
default=(1.40, 180)

# This is a major simplification.
# Local phone call rates vary with the number of local calls made.
# I am assuming the worst case scenario.
# Upto 150 phone calls : Free !
# 151-500 phone calls : 0.80
# 501-1000 phone calls : 1.00
# 1001-2000 phone calls : 1.25
# >2000 phone calls : 1.40

#I have not attempted to code this as it is possible that you are using 
# your phone line for ordinary voice calls/fax as well. In that case, it would
# really be fajool di gall to code all that. 

#This rule set also does not do justice to those unlucky ones amongst us who have to 
#dial an outstation STD number to get to the nearest VSNL/DOT node. For them : aape karo !
#Ate navin .rst file Wuebben nu ghall deo. (wuebben@kde.org).
#Rabb Rakha.
#
# more complicated rules:
#

# The "on()" rules above all relates to current time only. You can also
# make a rule depend on the number of seconds you have been connected
# by specifying this time as a third argument to "use()".
# For instance, let's say normal rate in the evening is 0.20 per minute,
# and it drops by 20% after one hour of connect time. This can be modelled
# like:

#on () between (19:30..08:00) use (0.20, 60)
#on () between (19:30..08:00) use (0.16, 60, 3600)

# Note that these rules, just like other rules, are sensitive to the 
# order in which they appear.

