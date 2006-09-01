################################################################
#
# kppp ruleset for Italy
#
# by Luca Boni (me9139@mclink.it, jungbn@netsacape.net)
#
# Telecom_Interurbane_Fino15Km.rst
#
# CHIAMATE INTERURBANE ENTRO 15 KM
#
# Ultimo aggiornamento: 7 Marzo 2002.
# 
################################################################
##							      ##
## Addebito alla risposta di 0,0787 EUR.	  	      ##
##							      ##
## Tutti i prezzi si intendono in EUR/min IVA COMPRESA.       ##
##							      ##
## 		Tariffa RIDOTTA:   0,0310 EUR   	      ##
##		Tariffa INTERA :   0,0619 EUR		      ##
##							      ##
##		  	| intera  dalle 08:00 alle 18:30      ##
## Giorni FERIALI:	| ridotta dalle 18:30 alle 08:00      ##
##							      ##
## Giorni FESTIVI:	|  ridotta tutto il giorno	      ##
##	 e SABATO:	|				      ##
##							      ##
################################################################


# Nome ruleset
name=Telecom_Interurbane_Fino15Km

# Impostazione valuta
currency_symbol=EUR
currency_position=right 
currency_digits=2

# Addebito alla risposta
per_connection=0.0787

# Giorni feriali, sabato e domenica a tariffa ridotta
default=(0.0310, 60)

# Giorni feriali a tariffa intera
on (monday..friday) between (8:00..18:30) use (0.0619, 60)

# Giorni festivi a tariffa ridotta
on (01/01) between () use (0.0310, 60)
on (01/06) between () use (0.0310, 60)
on (04/25) between () use (0.0310, 60)
on (05/01) between () use (0.0310, 60)
on (08/15) between () use (0.0310, 60)
on (11/01) between () use (0.0310, 60)
on (12/08) between () use (0.0310, 60)
on (12/25) between () use (0.0310, 60)
on (12/26) between () use (0.0310, 60)
on (easter) between () use (0.0310, 60)
on (easter + 1) between () use (0.0310, 60)

# Fine
