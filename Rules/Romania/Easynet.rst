################################################################
# Ruleset for Easynet (inclusiv TVA)                           #
# written by Sorin Batariuc <sorin@bonbon.net>                 #
# actualizat la 20 Martie 2005, pentru convorbiri 89(3)0123    #
################################################################

name=Easynet
currency_symbol=Euro
currency_position=right
currency_digits=4
per_connection=0.0
minimum_costs=0.01428
default=(0.022372, 60)

# TARIFUL I
on (monday..sunday) between (08:00..19:59) use (0.022372,60)

# TARIFUL II
on (monday..sunday) between (20:00..07:59) use (0.01428,60)

