# Kppp rules voor Priority Telecom Regionaal, naar niet Priority-nummers
# Gemaakt door: Rinse de Vries <rinse@kde.nl>
# Datum: 20-11-2002
# Oorspronkelijk script voor Nedpoint gemaakt door:
# Mike Klinkert (michael@cs.vu.nl)
#
# Piektarief:  5,40 cpm, 08:00-18:00 (maandag t/m vrijdag)
# Daltarief:   2,70 cpm, 18:00-08:00 (maandag t/m vrijdag en in het weekend)
# Starttarief: 10 cent

name=Priority Telecom Regionaal
currency_symbol=euro
currency_position=left
currency_digits=2
per_connection=0.035
minimum_costs=0.0

# Piektarief (0.054 / 60)
on (monday..friday) between (8:00..18:00) use (0.0009, 1)

# Daltarief (0.027 / 60)
default=(0.00045, 1)
# Kerstmis
on (12/25..12/26, 12/31) between () use (0.00045, 1)
# Nieuwjaar
on (01/01) between () use (0.00045, 1)
# Koninginnedag
on (04/30) between () use (0.00045, 1)
# Pasen
on (easter) between () use (0.00045,1)
# Pinksteren
on (easter+56) between () use (0.00045,1)
