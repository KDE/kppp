##########################################################################
# Planet-Interkom - Internet by Call accounting rules
#
# created 00/07/01 by Andreas Nestler <andreas.nestler@kdt.de>
#   look at http://www.planet-interkom.de
# modified
#
##########################################################################

name=Planet-Interkom Internet by call
currency_symbol=DM
currency_position=right 
currency_digits=2

################################################################
# connection settings
################################################################


# My_definitions for Planet-Interkom

per_connection=0.0
minimum_costs=0.0
default=(0.032, 60)
on (monday..friday) between (8:00..18:00) use (0.032, 60)
on (monday..friday) between (18:00..8:00) use (0.025, 60)
on (saturday..sunday) between () use (0.025, 60)
on (12/25..12/26) between () use (0.025, 60)
on (easter) between () use (0.025, 60)
on (easter+50) between () use (0.025, 60)

