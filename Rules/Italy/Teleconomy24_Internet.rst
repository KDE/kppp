################################################################
#
# kppp ruleset for Italy
#
# by Luca Boni (me9139@mclink.it)
#
# Teleconomy24_Internet.rst
#
# Chiamate dirette ad un ISP qualsiasi (con numero appartenente
# allo stesso distretto telefonico del chiamante o ad addebito
# ripartito del tipo 848 anche su linea ISDN o offerta BB.B).
#
# Ultimo aggiornamento: 7 Marzo 2002.
# 
################################################################
##							      ##
## Addebito alla risposta di 0,0619 EUR.	  	      ##
##							      ##
## Tutti i prezzi si intendono in EUR/min IVA COMPRESA.       ##
##							      ##
## Tariffa unica:  0,0092 tutti i giorni a tutte le ore	      ##
##							      ##
################################################################


# Nome del ruleset
name=Teleconomy24_Internet

# Impostazioni della valuta
currency_symbol=EUR
currency_position=right 
currency_digits=2

# Addebito alla risposta
per_connection=0.0619

# Tariffa unica
default=(0.0092, 60)

# Fine
