################################################################
#
#  Sunrise Select Internet
#  Created by Daniel Brönnimann (dbroenni@g26.ethz.ch) Nov 19. 1998
#  Changed by Philipp Gressly (phi@gressly.ch) Sep. 16. 2000
#
################################################################

name=Sunrise_Select_Internet

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

# Normaltarif, Niedertarif tarif normal
on (monday..friday) between (8:00..23:00) use (0.0004639, 1)

# "Nationale Feiertage"
# easter+39 = Ascension Day (Auffahrt), easter+50 = Whit Monday (Pfingstmontag)
on (01/01, 02/01, easter-2, easter+1) between (8:00..23:00) use (0.0004639, 1)
on (easter+39, easter+50) between (8:00..23:00) use(0.0004639, 1)
on (01/08, 12/25, 12/26) between (8:00..23:00) use (0.0004639, 1)
