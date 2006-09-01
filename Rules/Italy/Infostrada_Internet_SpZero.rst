################################################################
#
# kppp ruleset for Italy
#
# by Luca Boni (me9139@mclink.it)
#
# Infostrada_Internet_SpZero.rst
#
# INTERNET 
# Collegamenti tramite numero 1055-421010
#
# Ultimo aggiornamento: 7 Marzo 2002.
# 
################################################################
##							      ##
## Addebito alla risposta di 0,0620 EUR.		      ##
##							      ##
## Tutti i prezzi si intendono in EUR/min IVA compresa.       ##
##							      ##
##							      ##
## 		Tariffa UNICA : 0,0095 EUR   		      ##
##		   					      ##
##		              				      ##
## Tutti i giorni :	dalle 00:00 alle 24:00	              ##
##							      ##
##							      ##
################################################################


# Nome del ruleset
name=Infostrada_Internet_SpZero

# Impostazioni della valuta
currency_symbol=EUR
currency_position=right 
currency_digits=2

# Addebito alla risposta
per_connection=0.0620

# Tutti i giorni a tutte le ore
default=(0.0095, 60)

# Fine
