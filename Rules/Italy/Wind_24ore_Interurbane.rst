################################################################
#
# kppp ruleset for Italy
#
# by Luca Boni (me9139@mclink.it)
#
# Interurbane.rst
#
# CHIAMATE INTERURBANE
#
# Ultimo aggiornamento: 7 Marzo 2002.
# 
################################################################
##							      ##
## Nessun addebito alla risposta.		  	      ##
##							      ##
## Tutti i prezzi si intendono in EUR/min IVA COMPRESA.       ##
##							      ##
## Tariffa unica:  fino a 3 min = 0,1116  oltre = 0,0107      ##
##		   Tutti i giorni a tutte le ore              ##
##							      ##
################################################################


# Nome del ruleset
name=24ore_Interurbane

# Impostazioni della valuta
currency_symbol=EUR
currency_position=right 
currency_digits=2

# Nessun addebito alla risposta
per_connection=0

# Tariffa unica
default=(0.1116, 60)
default=(0.0107, 60, 180)

# Fine
