################################################################
#
# kppp ruleset for Italy
#
# by Pino Toscano (toscano.pino@tiscali.it)
#
# Atlanet_Internet.rst
#
# COLLEGAMENTI AD INTERNET
#
# Ultimo aggiornamento: 16 Luglio 2003.
# 
################################################################
##							      ##
## Addebito alla risposta di 0 EUR.(!!)	  	      ##
##							      ##
## Tutti i prezzi si intendono in EUR/min IVA COMPRESA.       ##
##							      ##
## Tariffa RIDOTTA:  0,01 EUR   ##
## Tariffa INTERA:   0,017 EUR  ##
##							      ##
##			| intera  dalle 08:00 alle 18:30      ##
## Giorni FERIALI:	| ridotta dalle 18:30 alle 08:00      ##
##							      ##
## Sabato e FESTIVI:	  ridotta tutto il giorno    ##
##							      ##
################################################################

# Nome del ruleset
name=Atlanet_Internet

# Impostazioni della valuta
currency_symbol=EUR
currency_position=right 
currency_digits=2

# Addebito alla risposta
per_connection=0.0

# Tariffa ridotta
default=(0.01, 60)

# Giorni feriali, sabato e domenica
on (monday..friday) between (8:00..18:30) use (0.017, 60)

# Giorni festivi
on (01/01) between () use (0.01, 60)
on (01/06) between () use (0.01, 60)
on (04/25) between () use (0.01, 60)
on (05/01) between () use (0.01, 60)
on (08/15) between () use (0.01, 60)
on (11/01) between () use (0.01, 60)
on (12/08) between () use (0.01, 60)
on (12/25) between () use (0.01, 60)
on (12/26) between () use (0.01, 60)
on (easter) between () use (0.01, 60)
on (easter + 1) between () use (0.01, 60)

# Fine
