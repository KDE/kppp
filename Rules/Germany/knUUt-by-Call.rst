
################################################################
#German UUNET - Internet By Call accounting rules
#
#UUNET Deutschland GmbH
#
#knUUt-by-Call
# 
#created 99/10/17 by Reinhard Kunert <reinhard.kunert@gmx.de>
#costs based on "www.knuut.de/call" Sunday 99/10/17
################################################################

name=knUUt-by-Call
currency_symbol=DM
currency_position=right 
currency_digits=2
per_connection=0.0
minimum_costs=0.0
default=(0.055, 60)

# Abendtarif
on (monday..sunday) between (21:00..8:59) use (0.049, 60)
