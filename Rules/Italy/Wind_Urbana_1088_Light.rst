################################################################
# kppp ruleset for Italy
#
# Wind con piano telefonico '1088 light'
# Tariffa: urbana
#
# by Filippo Gioachin (gioachin@ieee.org)
################################################################


name=Wind_1088_light

currency_symbol=L.
currency_position=left
currency_digits=0



################################################################
# connection settings
#
# The cost is L. 720 every hour of connection.
# Il costo e` L. 720 ogni ora di connessione.
################################################################


per_connection=0.0
minimum_costs=0.0

default=(0.2, 1)


# Il costo e` di L. 1440 l'ora nei giorni feriali
on (monday..friday) between (09:00..19:00) use (0.4, 1)

# Il costo e` di L. 720 l'ora nelle festivita`
on (01/01, 01/06, 04/25, 05/01, 08/15) between () use (0.2, 1)
on (11/01, 12/08, 12/25, 12/26) between () use (0.2, 1)

# NOTA: IVA inclusa

# End of file
