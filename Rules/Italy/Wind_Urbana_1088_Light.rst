################################################################
# kppp ruleset for Italy
#
# Wind con piano telefonico '1088 light'
# Tariffa: urbana
#
# by Giovanni Venturi (jumpyj@tiscali.it)
################################################################

# Nome del ruleset
name=Wind_1088_light

# Impostazioni della valuta
currency_symbol=EUR
currency_position=right
currency_digits=2



################################################################
# connection settings
#
# The cost is ¤ 0,3719 every hour of connection.
# Il costo e` ¤ 0,3719 ogni ora di connessione.
################################################################

# Nessun addebito alla risposta
per_connection=0.0
minimum_costs=0.0

default=(0.0001, 1)


# Il costo e` di ¤ 0,7437 l'ora nei giorni feriali
on (monday..friday) between (09:00..19:00) use (0.0002, 1)

# Giorni festivi, sabato e domenica
on (saturday) between () use (0.0001, 1)
on (sunday) between () use (0.0001, 1)
on (01/01) between () use (0.0001, 1)
on (01/06) between () use (0.0001, 1)
on (04/25) between () use (0.0001, 1)
on (05/01) between () use (0.0001, 1)
on (08/15) between () use (0.0001, 1)
on (11/01) between () use (0.0001, 1)
on (12/08) between () use (0.0001, 1)
on (12/25) between () use (0.0001, 1)
on (12/26) between () use (0.0001, 1)
on (easter) between () use (0.0001, 1)
on (easter + 1) between () use (0.0001, 1)
# NOTA: IVA inclusa

# End of file
