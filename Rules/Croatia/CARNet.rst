################################################################
#
# NAME OF THE RULESET. This is NEEDED for accounting purposes.
# Updated by zapov (zapov at yahoo com)
#
################################################################
name=CARNet.rst

################################################################
# currency settings
################################################################

currency_symbol=HRK
currency_position=right
currency_digits=2

################################################################
# connection settings
################################################################

per_connection=0.0
minimum_costs=0.0

# You pay .74 for the first 180 secons ( 3minutes) no matter
# whether you are connected for 1 second or 180 seconds.
# This rule will take priority during the first 180 seconds
# over any other rule, in particular the 'default' rule.
# have a look at costgraphs.gif in the docs directory
# of the kppp distribution for a graphic illustration.

flat_init_costs=(0.061, 60)

# This is the default rule which is used when no other rule
# applies. The first component "0.1" is the price of one
# "unit", while "72" is the duration in seconds.
# Therefore the following rule means: "Every 72 seconds 0.1
# ATS are added to the bill"
default=(0.061, 60)

# more complicated rules:

# "on monday until sunday from 12:00 am until 11:59 pm the costs
# are 0.2 each 72 seconds"
#on () between () use (0.2, 2)

# same as above. You must use 24 hour notation, or the accounting
# will not work correctly. (Example: write 15:00 for 3 pm)
on (monday..saturday) between (0:00..6:59) use (0.0305, 60)
on (monday..saturday) between (7:00..18:59) use (0.061, 60)
on (monday..saturday) between (19:00..23:59) use (0.0305, 60)
on (sunday) between () use (0.0305, 60)

# Sluzbeni praznici, najjeftinija tarifa
# 1.1. Nova Godina
# Easter - Uskrs
# 6.1. - Tri kralja
# 1.5. - Medjunarodni praznik rada
# Easter+60 - Tijelovo
# 22.6. - Dan antifasisticke borbe
# 25.6. - Dan drzavnosti
# 5.8. - Dan domovinske zahvalnosti
# 15.8. - Velika Gospa
# 8.10. - Dan neovisnosti
# 1.11. - Svi sveti
# 25.12. - Bozic
# 26.12. - Dan iza :)
on (01/01, 01/06, easter+1, 05/1, easter+60, 06/22, 06/25, 08/05, 08/15, 11/01, 12/25, 12/26) between () use (0.0305, 60)
