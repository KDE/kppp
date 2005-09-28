################################################################
#
# kppp ruleset for Italy
#
# by Giovanni Venturi (jumpyj@tiscali.it)
#
# Cheapnet.rst
#
# CHIAMATE URBANE
#
# Ultimo aggiornamento: 28 Settembre 2005.
# 
#################################################################
##                                                             ##
## Addebito alla risposta di 0,06192 EUR.                      ##
##                                                             ##
## Tutti i prezzi si intendono in EUR/min IVA COMPRESA.        ##
##                                                             ##
## Tariffa RIDOTTA:  fino a 15 min = 0,01092  oltre = 0,00984  ##
## Tariffa INTERA:   fino a 15 min = 0,01896  oltre = 0,01716  ##
##                                                             ##
##                      | intera  dalle 08:00 alle 18:30       ##
## Giorni FERIALI:      | ridotta dalle 18:30 alle 08:00       ##
##                                                             ##
## Giorni FESTIVI:        ridotta tutto il giorno              ##
##                                                             ##
##         Sabato:      | intera  dalle 08:00 alle 13:00       ##
##                      | ridotta dalle 13:00 alle 08:00       ##
##                                                             ##
#################################################################

# Nome del ruleset
name=Cheapnet

# Impostazioni della valuta
currency_symbol=EUR
currency_position=right 
currency_digits=2

# Addebito alla risposta
per_connection=0.06192

# Tariffa ridotta fino a 15 min
default=(0.01092, 60)

# Giorni feriali, sabato e domenica
on (monday..friday) between (18:30..8:00) use (0.00984, 60, 900)
on (monday..friday) between (8:00..18:30) use (0.01896, 60)
on (monday..friday) between (8:00..18:30) use (0.01716, 60, 900)
on (saturday) between (13:00..8:00) use (0.00984, 60, 900)
on (saturday) between (8:00..13:00) use (0.01896, 60)
on (saturday) between (8:00..13:00) use (0.01716, 60, 900)
on (sunday) between () use (0.00984, 60, 900)

# Giorni festivi
on (01/01) between () use (0.01092, 60)
on (01/01) between () use (0.00984, 60, 900)
on (01/06) between () use (0.01092, 60)
on (01/06) between () use (0.00984, 60, 900)
on (04/25) between () use (0.01092, 60)
on (04/25) between () use (0.00984, 60, 900)
on (05/01) between () use (0.01092, 60)
on (05/01) between () use (0.00984, 60, 900)
on (08/15) between () use (0.01092, 60)
on (08/15) between () use (0.00984, 60, 900)
on (11/01) between () use (0.01092, 60)
on (11/01) between () use (0.00984, 60, 900)
on (12/08) between () use (0.01092, 60)
on (12/08) between () use (0.00984, 60, 900)
on (12/25) between () use (0.01092, 60)
on (12/25) between () use (0.00984, 60, 900)
on (12/26) between () use (0.01092, 60)
on (12/26) between () use (0.00984, 60, 900)
on (easter) between () use (0.01092, 60)
on (easter) between () use (0.00984, 60, 900)
on (easter + 1) between () use (0.01092, 60)
on (easter + 1) between () use (0.00984, 60, 900)

# Fine
