# Kppp rules voor Priority Telecom nationaal, naar niet Priority-nummers
# Gemaakt door: Rinse de Vries <rinse@kde.nl>
# Datum: 20-11-2002
# Oorspronkelijk script voor Nedpoint gemaakt door:
# Mike Klinkert (michael@cs.vu.nl)
#
# Piektarief:  3 cpm, 08:00-18:59 (maandag t/m vrijdag)
# Daltarief:   1.5 cpm, 19:00-23:59 (maandag t/m vrijdag)
# Nachttarief   1.5 cpm, 00:00-07:59 (maandag t/m vrijdag en weekend)
# Starttarief: 5 cent

name=Priority Telecom Nationaal
currency_symbol=euro
currency_position=left
currency_digits=2
per_connection=0.05
minimum_costs=0.0

# Piektarief (0.03 / 60 = 0.0005)
on (monday..friday) between (8:00..18:59) use (0.0005, 1)

# Daltarief (0,015 / 60 = 0.00025)
on (monday..friday) between (19:00..23:59) use (0.00025, 1)


# Nachttarief (0.015 / 60 = 0.00025)
default=(0.00015, 1)
# Kerstmis
# on (12/25..12/26, 12/31) between () use (0.00025, 1)
# Nieuwjaar
# on (01/01) between () use (0.00025, 1)
# Koninginnedag
# on (04/30) between () use (0.00025, 1)
# Pasen
on (easter) between () use (0.00025, 1)
# Pinksteren
on (easter+56) between () use (0.00025, 1)
