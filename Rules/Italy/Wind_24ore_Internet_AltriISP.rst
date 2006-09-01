################################################################
#
# kppp ruleset for Italy
#
# by Luca Boni (me9139@mclink.it)
#
# Internet_InWind.rst
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
## Tariffa unica:  0,0341  tutti i giorni a tutte le ore      ##
##							      ##
################################################################


# Nome del ruleset
name=24ore_Internet_AltriISP

# Impostazioni della valuta
currency_symbol=EUR
currency_position=right 
currency_digits=2

# Nessun addebito alla risposta
per_connection=0

# Tariffa unica
default=(0.0341, 60)

# Fine
