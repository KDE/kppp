# Calls outside own area code
# Created by J. Wezel (jwezel@access.ch)
# Sun Sep 14 01:30:33 1997
# Changed by Daniel Brönnimann (dbreonni@g26.ethz.ch)
# Thu Nov 19 1998
# Changed by Fritz Zaucker (zaucker@ee.ethz.ch)
# Sun Oct 24 1999
# Valid from 1-Oct-1999

name=Swisscom_Remote

currency_symbol=CHF

currency_position=left
currency_digits=2
per_connection=0.00
minimum_costs=0.00
default=(0.10, 24)

# Special dates:
# easter -  2 days (Karfreitag/ ??? )
# easter + 39 days (Auffahrt/ ??? )
# easter + 50 days (Pfingstmontag/ Pentecost Monday )
# 08/01 (August 1st, Swiss National Holiday)

# Note: the previous version of this rule set erroneously had
#        02/01 instead of 01/02 (January 2nd)
#  and  01/08 instead of 08/01 (August 1st)

# Normal tarif
on (monday..friday) between (8:00..17:00) use (0.10, 24)

# Low tarif
on (monday..friday) between (6:00..8:00) use (0.10, 48)
on (monday..friday) between (17:00..22:00) use (0.10, 48)
on (saturday..sunday) between (6:00..22:00) use (0.10, 48)

on (01/01, 01/02, easter-2, easter+1) between (6:00..22:00) use (0.10, 48)
on (easter+39, easter+50, 08/01, 12/25, 12/26) between (6:00..22:00) use (0.10, 48)

# Night tarif
on (monday..sunday) between (22:00..0:00) use (0.10, 96)
on (monday..sunday) between (0:00..6:00) use (0.10, 96)
