##############################################################
# Finnish rate ruleset for calls with Vaasan L‰‰nin Puhelin
# Created 2001-05-07 by Heikki Viitala  <heikki.viitala@nic.fi>
# May be distributed freely. I take no responsibility for
# the correctness of the information in this file.
##############################################################

name=Vaasan_LP
currency_symbol=FIM
currency_position=right
currency_digits=2

# This is one of the simpliest rules

# N‰in lasketaan kustannus (minun k‰sitykseni mukaan)
# Avausmaksu veloitetaan heti yhteyden muodostuttua, 0.488FIM
# Aikaveloitus on koko ajan sama, 0.078FIM/MIN, laskenta
#  ilmeisesti sekuntiperusteinen, 0.0013FIM/SEK

per_connection=0.488
minimum_costs=0.0

default=(0.0013, 1)
