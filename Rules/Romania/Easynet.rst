################################################################
# Ruleset for Easynet (inclusiv TVA)                           #
# written by Sorin Batariuc <sorin@bonbon.net>                 #
# actualizat la 5 Ianuarie 2004 (pentru convorbiri 89(3)0123   #
################################################################

name=Easynet
currency_symbol=Euro
currency_position=right
currency_digits=4
per_connection=0.0
minimum_costs=0.00476
default=(0.009401, 30)

# TARIFUL I
on (monday..sunday) between (08:00..21:59) use (0.009401,30)

# TARIFUL II
on (monday..sunday) between (22:00..07:59) use (0.00476,30)

