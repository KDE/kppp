################################################################
#
# Addcom rules
#
# created on 99/10/14, Holger Witthaut <Holger@tapage.de>
#
################################################################

name=addcom
currency_symbol=DM
currency_position=right 
currency_digits=2
per_connection=0.0
minimum_costs=0.0
default=(0.039, 60)

# Montag bis Freitag
on (monday..friday) between (09:00..20:59) use (0.059, 60)
on (monday..friday) between (21:00..08:59) use (0.039, 60)

# Samstag und Sonntag
on (saturday..sunday) between (09:00..20:59) use (0.055, 60)
on (saturday..sunday) between (21:00..08:59) use (0.039, 60)
