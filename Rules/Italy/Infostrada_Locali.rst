################################################################
#
# kppp ruleset for Italy
#
# by Luca Boni (me9139@mclink.it, jungbn@netsacape.net)
#
# Infostrada_Locali.rst
#
# Tariffa per il collegamento ad un numero di ugual prefisso.
#
# Tariffe aggiornate a Giugno 2001.
# 
################################################################
##							      ##
## Addebito di 1 scatto alla risposta di £ 120.  	      ##
## 	     						      ##
## Tutti i prezzi si intendono in £/min IVA COMPRESA.         ##
##							      ##
##							      ##
## 		Tariffa RIDOTTA: £ 18			      ##
## 		Tariffa INTERA : £ 33,6			      ##
##		   					      ##
##		              | dalle 00:00 alle 08:00	      ##
##		    | Ridotta | dalle 18:30 alle 24:00	      ##
## Giorni FERIALI : |					      ##
##		    | Intera    dalle 08:00 alle 18:30	      ##
##							      ##
##			          			      ##
## Giorni FESTIVI : | Ridotta 	dalle 00:00 alle 24:00        ##
##	 e SABATO : | 	      	      			      ##
##							      ##
################################################################


# Nome del ruleset
name=Infostrada_Locali

# Impostazioni della valuta
currency_symbol=£.
currency_position=left 
currency_digits=2

# Addebito di 1 scatto alla risposta
per_connection=120.00

# Impostazione di default: giorno feriale a tariffa intera
default=(33.60, 60)

# Giorni feriali: tariffa ridotta
on (monday..friday) between (0:00..8:00) use (18.00, 60)
on (monday..friday) between (18:30..23:59) use (18.00, 60)

# Giorni festivi e sabato: tariffa ridotta
on (saturday) between () use (18.00, 60)
on (01/01) between () use (18.00, 60)
on (01/06) between () use (18.00, 60)
on (04/25) between () use (18.00, 60)
on (05/01) between () use (18.00, 60)
on (08/15) between () use (18.00, 60)
on (11/01) between () use (18.00, 60)
on (12/08) between () use (18.00, 60)
on (12/25) between () use (18.00, 60)
on (easter + 1) between () use (18.00, 60)

# Fine
