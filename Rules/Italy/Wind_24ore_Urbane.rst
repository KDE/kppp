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
## Tariffa unica:  fino a 3 min = 0,0341  oltre = 0,0273      ##
##		   Tutti i giorni a tutte le ore              ##
##							      ##
################################################################


# Nome del ruleset
name=24ore_Urbane

# Impostazioni della valuta
currency_symbol=EUR
currency_position=right 
currency_digits=2

# Nessun addebito alla risposta
per_connection=0

# Tariffa unica
default=(0.0341, 60)
default=(0.0273, 60, 180)

# Fine
