################################################################
#
#  Sunrise Internet
#  Created by Nicolas Boichat (nicolas@boichat.ch)
#  Feb 10 2000
#
################################################################

name=Sunrise_Internet

################################################################
# currency settings
################################################################

currency_symbol=CHF
currency_position=right

currency_digits=2

################################################################
# connection settings
################################################################

per_connection=0.0
minimum_costs=0.0

# Nachttarif, tarif de nuit
default=(0.00023056, 1)

# Niedertarif, tarif reduit
on (monday..sunday) between (8:00..23:00) use (0.00046111, 1)
