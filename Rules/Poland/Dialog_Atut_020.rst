##################################################################
# Koszty polaczen z Internetem w sieci Dialog - Atut dla 0-20
# utor: Robert Monczkowski <robert@monczkowski.prv.pl>
# Aktualizacje: http://www.monczkowski.prv.pl
# Koszt impulsu nie zawiera 7% VAT
##################################################################

name=Dialog_Atut_020
currency_symbol=PLN
currency_position=right
currency_digits=2
per_connection=0.0
minimum_costs=0.0

# Impuls w godzinach 18:00-7:59 naliczany jest co 6 minut
on (monday..friday) between (18:00..7:59) use (0.58, 360)
# Impuls w sobote i niedziele naliczany jest co 6 minut przez cala dobe
on (saturday..sunday) between () use (0.58, 360)
# Swieta
on (01/01, 05/01, 05/03, 08/15, 11/01, 11/11, 12/25, 12/26) between () use (0.58, 360)
# Impuls w godzinach 8:00-17:59 naliczany jest co 3 minuty
default=(0.58, 180)



