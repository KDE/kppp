################################################################
#
#  Sunrise Select Internet
#  Created by Daniel Br�nnimann (dbroenni@g26.ethz.ch) Nov 19. 1998
#  Changed by Philipp Gressly (phi@gressly.ch) Sep. 16. 2000
#  Changed by Gregor Zumstein (zumstein@ssd.ethz.ch) Sep 29 2000
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
on (monday..sunday) between (8:00..22:59) use (0.0004639, 1)

# "Nationale Feiertage"
# easter+39 = Ascension Day (Auffahrt), easter+50 = Whit Monday (Pfingstmontag)
on (01/01, 01/02, easter-2, easter+1) between (8:00..22:59) use (0.0004639, 1)
on (easter+39, easter+50) between (8:00..22:59) use(0.0004639, 1)
on (08/01, 12/25, 12/26) between (8:00..22:59) use (0.0004639, 1)


# Feiertage / Jours f�ri�s:
# 01/01       Neujahr / Nouvel an
# 01/02       2. Januar / 2 janvier
# easter-2    Karfreitag / Vendredi saint
# easter+1    Ostermontag / Lundi de p�ques
# easter+39   Auffahrt / Ascenscion
# easter+50   Pfingstmontag / Lundi de pentec�te
# 08/01       Nationalfeiertag / F�te nationale
# 12/25       Weihnachten / No�l
# 12/26       Stephanstag / 2�me jour de no�l
