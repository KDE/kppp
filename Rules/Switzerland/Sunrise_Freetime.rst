################################################################
#
#  Sunrise Freetime (Unbeschr�nkt surfen mit 30.-/Monat)
#  Created by Gregor Zumstein (zumstein@ssd.ethz.ch)
#  Sep 29 2000
#
################################################################

name=Sunrise_Freetime

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
default=(0, 1)

# Niedertarif, tarif r�duit
# +30 Fr. Basisgeb�hr pro Monat, tarif de base par mois
on (monday..friday) between (8:00..22:59) use (0.00046111, 1)

on (01/01, 01/02, easter-2, easter+1, easter+39, easter+50, 08/01) between (0:00..23:59) use (0, 1)

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