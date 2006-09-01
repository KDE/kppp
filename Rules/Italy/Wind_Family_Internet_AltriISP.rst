################################################################
#
# kppp ruleset for Italy
#
# by Luca Boni (me9139@mclink.it)
#
# Internet_AltriISP.rst
#
# Chiamate dirette ad un ISP diverso da Wind.
#
# Ultimo aggiornamento: 7 Marzo 2002.
# 
################################################################
##							      ##
## Nessun addebito alla risposta.		  	      ##
##							      ##
## Tutti i prezzi si intendono in EUR/min IVA COMPRESA.       ##
##							      ##
## 		Tariffa RIDOTTA:    0,0248 EUR		      ##
## 		Tariffa INTERA :    0,0341 EUR		      ##
##							      ##
##			| intera  dalle 09:00 alle 19:00      ##
## Giorni FERIALI:	| ridotta dalle 19:00 alle 09:00      ##
##							      ##
## Giorni FESTIVI:	|  ridotta tutto il giorno	      ##
## 	e  SABATO:	|				      ##
##							      ##
################################################################

# Nome del ruleset
name=Wind_Family_Internet_AltriISP

# Impostazioni della valuta
currency_symbol=EUR
currency_position=right 
currency_digits=2

# Nessun addebito alla risposta
per_connection=0

# Tariffa ridotta
default=(0.0248, 60)

# Giorni feriali
on (monday..friday) between (9:00..19:00) use (0.0341, 60)

# Giorni festivi
on (01/01) between () use (0.0248, 60)
on (01/06) between () use (0.0248, 60)
on (04/25) between () use (0.0248, 60)
on (05/01) between () use (0.0248, 60)
on (08/15) between () use (0.0248, 60)
on (11/01) between () use (0.0248, 60)
on (12/08) between () use (0.0248, 60)
on (12/25) between () use (0.0248, 60)
on (12/26) between () use (0.0248, 60)
on (easter) between () use (0.0248, 60)
on (easter + 1) between () use (0.0248, 60)

# Fine
