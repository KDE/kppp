################################################################
# Kppp phone cost ruleset for local calls in Israel (Sicha BeinIronit)
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
name=Israel_Interurban

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

default=(0.26, 60)

# Minimum costs for a call are 23.1 Agorot. If the costs of a
# call are less than this value, this value is used instead
minimum_costs=0.231


# On weekdays from 8:00 to 18:00, and on fridays from
# 8:00 to 13:00 costs are 26 Agorot per minute
on (sunday..thursday) between (8:00..18:00) use (0.26, 60)
on (friday) between (8:00..13:00) use (0.26, 60)

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

# Holidays left in 2000 are:
# Independence day (May 10), Lag Ba'Omer (May 23), Shavuot (June 9)
on (05/09, 05/22, 06/08) between (13:00..23:59) use (0.015, 60)
on (05/10, 05/23, 06/09) between () use (0.015, 60)
# Rosh HaShana (September 30), Yom Kippur (October 9), Sukkot (October 14, 21)
on (09/30, 10/09, 10/14, 10/21) between (13:00..23:59) use (0.015, 60)
on (09/30, 10/09, 10/14, 10/21) between () use (0.015, 60)
