################################################################
#
# kppp ruleset for Italy
#
# by Luca Boni (me9139@mclink.it)
#
# Teleconomy_NoStop_Internet.rst
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
##							      ##
## ATTENZIONE:						      ##
## -----------						      ##
## La tariffa prevede 72000 secondi (20 ore) gratis al mese.  ##
## Kppp pero' non e' in grado di tenerne conto.		      ##
## Quindi ai fini del calcolo della spesa, questa tariffa e'  ##
## identica a "Teloconomy24_Internet.rst".		      ##
##							      ##
################################################################


# Nome del ruleset
name=Teleconomy_NoStop_Internet

# Impostazioni della valuta
currency_symbol=EUR
currency_position=right 
currency_digits=2

# Addebito alla risposta
per_connection=0.0619

# Tariffa unica
default=(0.0092, 60)

# Fine
