###############################################################
# Czech Telecom Internet 2001 rate ruleset
#
# created 2001/03/12 by Jiri Vasina <jvasina@med.muni.cz>
################################################################

name=Czech_Telecom_Internet_2001
currency_symbol=Kc
currency_position=right 
currency_digits=2
per_connection=0.0
minimum_costs=0.0

default=(2.60, 120)

on (monday..friday) between (7:00..16:59) use (2.60, 180, 360)
on (monday..friday) between (17:00..18:59) use (2.60, 390, 120)
on (monday..friday) between (19:00..6:59) use (2.60, 290)
on (monday..friday) between (19:00..6:59) use (2.60, 750, 290)
on (saturday..sunday) between () use (2.60, 290)
on (saturday..sunday) between () use (2.60, 750, 290)
