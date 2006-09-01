################################################################
#
# kppp ruleset for Italy
#
# by Luca Boni (me9139@mclink.it)
#
# Urbane.rst
#
# CHIAMATE URBANE
#
# Ultimo aggiornamento: 7 Marzo 2002.
# 
################################################################
##							      ##
## Nessun addebito alla risposta.		  	      ##
##							      ##
## Tutti i prezzi si intendono in EUR/min IVA COMPRESA.       ##
##							      ##
## Tariffa RIDOTTA:  fino a 3 min = 0,0248   oltre = 0,0198   ##
## Tariffa INTERA:   fino a 3 min = 0,0341   oltre = 0,0273   ##
##							      ##
##			| intera  dalle 09:00 alle 19:00      ##
## Giorni FERIALI:	| ridotta dalle 19:00 alle 09:00      ##
##							      ##
## Giorni FESTIVI:	|  ridotta tutto il giorno	      ##
## 	e  SABATO:	|				      ##
##							      ##
################################################################

# Nome del ruleset
name=Wind_Family_Urbane

# Impostazioni della valuta
currency_symbol=EUR
currency_position=right 
currency_digits=2

# Nessun addebito alla risposta
per_connection=0

# Tariffa ridotta fino a 3 min
default=(0.0248, 60)

# Giorni feriali
on (monday..friday) between (19:00..9:00) use (0.0198, 60, 180)
on (monday..friday) between (9:00..19:00) use (0.0341, 60)
on (monday..friday) between (9:00..19:00) use (0.0273, 60, 180)

# Giorni festivi, sabato e domenica oltre 3 minuti
on (saturday) between () use (0.0198, 60, 180)
on (sunday) between () use (0.0198, 60, 180)
on (01/01) between () use (0.0198, 60, 180)
on (01/06) between () use (0.0198, 60, 180)
on (04/25) between () use (0.0198, 60, 180)
on (05/01) between () use (0.0198, 60, 180)
on (08/15) between () use (0.0198, 60, 180)
on (11/01) between () use (0.0198, 60, 180)
on (12/08) between () use (0.0198, 60, 180)
on (12/25) between () use (0.0198, 60, 180)
on (12/26) between () use (0.0198, 60, 180)
on (easter) between () use (0.0198, 60, 180)
on (easter + 1) between () use (0.0198, 60, 180)

# Fine
