################################################################
# Kppp phone cost ruleset for local calls in Israel (Sicha Mekomit)
#
# Created on April 20, 2000 by Meni Livne <meni@mail.com>
#
# I TAKE NO RESPONSIBILITY FOR THE VALIDITY AND ACCURACY OF
# THIS INFORMATION. IF YOU ARE SO CONCERNED PLEASE CONTACT
# YOUR TELEPHONE PROVIDER, CONFIGURE THIS YOURSELF AND DO
# NOT USE THIS FILE.
#
# Rates valid from May 1, 2000 
################################################################


################################################################
#
# NAME OF THE RULESET. This is NEEDED for accounting purposes.
#
################################################################
name=Israel_Local

################################################################
# currency settings
################################################################

# Define NIS (New Israeli Shekel) to be used as currency symbol
currency_symbol=NIS

# Define the position of the currency symbol.
currency_position=left

# Define the number of significant digits.
currency_digits=2


################################################################
# connection settings
################################################################

# NOTE: rules are applied from top to bottom - the
#       LAST matching rule is the one used for the
#       cost computations.

default=(0.09, 60) 

# Minimum costs for a call are 23.1 Agorot. If the costs of a
# call are less than this value, this value is used instead
minimum_costs=0.231


# On weekdays from 8:00 to 18:00, and on fridays from
# 8:00 to 13:00 costs are 9 Agorot per minute
on (sunday..thursday) between (8:00..18:00) use (0.09, 60)
on (friday) between (8:00..13:00) use (0.09, 60)

# On weekdays from 18:00 to 22:00
# costs are 4.5 Agorot per minute
on (sunday..thursday) between (8:00..18:00) use (0.045, 60)

# On weekdays from 22:00 to 8:00, on fridays from 13:00,
# and on saturdays costs are 1.5 Agorot per minute
on (sunday..thursday) between (8:00..18:00) use (0.015, 60)
on (friday) between (13:00..23:59) use (0.015, 60)
on (saturday) between () use (0.015, 60)


# On holiday eves from 13:00 and on holidays
# costs are 1.5 Agorot per minute

# Holidays for 2001:
# Passover (8,14/4) Independence day (26/4), Lag Ba'Omer (11/5), Shavuot (28/5)
on (04/07, 04/13, 04/25, 05/10, 05/27) between (13:00..23:59) use (0.015, 60)
on (04/08, 04/14, 04/26, 05/11, 05/28) between () use (0.015, 60)
# Rosh HaShana (18-19/9), Yom Kippur (27/9)
on (09/17, 09/26) between (13:00..23:59) use (0.015, 60)
on (09/18, 08/19, 09/27) between () use (0.015, 60)
