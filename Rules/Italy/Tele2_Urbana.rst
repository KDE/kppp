################################################################
#
# kppp ruleset for Italy
#
# by Luca Boni (me9139@mclink.it, jungbn@netsacape.net)
#
# Tele2_Urbana.rst
#
#
# Tariffe aggiornate a Giugno 2001.
# 
################################################################
##							      ##
## Addebito di 1 scatto alla risposta di £ 120.  	      ##
##							      ##
## Tutti i prezzi si intendono in £/min IVA COMPRESA.         ##
##							      ##
## 		Tariffa RIDOTTA:  £ 17			      ##
## 		Tariffa INTERA :  £ 30			      ##
##							      ##
##			| intera  dalle 08:00 alle 18:30      ##
## Giorni FERIALI:	| ridotta dalle 18:30 alle 08:00      ##
##							      ##
## Giorni FESTIVI:	| ridotta tutto il giorno	      ##
##	 e Sabato:	|				      ##
##							      ##
################################################################


# Nome ruleset
name=Tele2_Urbana

# Impostazione valuta
currency_symbol=£.
currency_position=left 
currency_digits=2

# Addebito di 1 scatto alla risposta
per_connection=120.00

# Impostazione di default: tariffa ridotta
default=(17.00, 60)

# Giorni Feriali: intera
on (monday..friday) between (8:00..18:30) use (30.00, 60)

# Nelle festivita' si applica la tariffa ridotta
on (01/01) between () use (17.00, 60)
on (01/06) between () use (17.00, 60)
on (04/25) between () use (17.00, 60)
on (05/01) between () use (17.00, 60)
on (08/15) between () use (17.00, 60)
on (11/01) between () use (17.00, 60)
on (12/08) between () use (17.00, 60)
on (12/25) between () use (17.00, 60)
on (easter + 1) between () use (17.00, 60)

# Fine
