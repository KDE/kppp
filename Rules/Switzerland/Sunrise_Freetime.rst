################################################################
#
#  Sunrise Freetime (Unbeschränkt surfen mit 30.-/Monat)
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

# Niedertarif, tarif réduit
# +30 Fr. Basisgebühr pro Monat, tarif de base par mois
on (monday..friday) between (8:00..22:59) use (0.00046111, 1)

on (01/01, 01/02, easter-2, easter+1, easter+39, easter+50, 08/01) between (0:00..23:59) use (0, 1)

# Feiertage / Jours fériés:
# 01/01       Neujahr / Nouvel an
# 01/02       2. Januar / 2 janvier
# easter-2    Karfreitag / Vendredi saint
# easter+1    Ostermontag / Lundi de pâques
# easter+39   Auffahrt / Ascenscion
# easter+50   Pfingstmontag / Lundi de pentecôte
# 08/01       Nationalfeiertag / Fête nationale
# 12/25       Weihnachten / Noël
# 12/26       Stephanstag / 2ème jour de noël