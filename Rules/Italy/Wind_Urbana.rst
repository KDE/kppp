################################################################
#
# kppp ruleset for Italy
#
# by Luca Boni (me9139@mclink.it, jungbn@netsacape.net)
#
# Wind_Urbana.rst
#
# Chiamate urbane (con lo stesso prefisso del chiamante).
#
# Tariffe aggiornate a Giugno 2001.
# 
################################################################
##							      ##
## Nessuno scatto alla risposta.		  	      ##
## 	     						      ##
## Tutti i prezzi si intendono in £/min IVA COMPRESA.         ##
##							      ##
##							      ##
## 		Tariffa RIDOTTA: £ 66			      ##
##		   					      ##
## Giorni FERIALI : |					      ##
## 	  FESTIVI : | ridotta tutto il giorno		      ##
##	 e SABATO : | 	      	      			      ##
##							      ##
################################################################


# Nome del ruleset
name=Wind_Urbana

# Impostazioni della valuta
currency_symbol=£.
currency_position=left 
currency_digits=2

# Impostazione di default: tariffa unica ridotta
default=(66.00, 60)

# Fine
