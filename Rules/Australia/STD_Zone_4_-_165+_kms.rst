################################################################
#
# Australian STD Zone 4 - Telstra
#
# created by Collin Baillie (gvlink@tropinet.com) 16 June 1999
#
################################################################
name=STD_(165+km)
currency_symbol=$
currency_position=left
currency_digits=2

################################################################
# connection settings
################################################################

per_connection=0.15
default=(0.28, 60)

# more complicated rules:
#
on (monday..friday) between (07:00..12:59) use (0.28, 60)
on (monday..friday) between (13:00..18:59) use (0.25, 60)
on (monday..friday) between (19:00..06:59) use (0.14, 60)
on (saturday..sunday) between () use (0.14, 60)

# NOTE: The Australian carrier Telstra, actually has a special tarrif during
# weekdays (Monday - Friday), from 7pm - 12 midnight, during which the normal
# tarrif applies up to, but not exceeding $3.00. This feature doesn't seem
# to be implimented with kppp yet.
