################################################################
#
# kppp ruleset for Italy
#
# by Luca Boni (me9139@mclink.it, jungbn@netsacape.net)
#
# TAT_Interdistrettuale_30+.rst
#
#
# Tariffe aggiornate a Giugno 2001.
# 
################################################################
##							      ##
## Addebito di 1 scatto alla risposta di £ 152,4.  	      ##
##							      ##
## Tutti i prezzi si intendono in £/min IVA COMPRESA.         ##
##							      ##
## Tariffa RIDOTTA:	£ 175,80 al minuto		      ##
## Tariffa INTERA:	£ 318,60 al minuto		      ##
##							      ##
##			| intera  dalle 08:00 alle 18:30      ##
## Giorni FERIALI:	| ridotta dalle 18:30 alle 08:00      ##
##							      ##
## Giorni FESTIVI:	  ridotta tutto il giorno	      ##
##							      ##
## 	   Sabato:	| intera  dalle 08:00 alle 13:00      ##
##                      | ridotta dalle 13:00 alle 08:00      ##
##							      ##
################################################################


# Nome ruleset
name=TAT_Interdistrettuale_30+

# Impostazione valuta
currency_symbol=£.
currency_position=left 
currency_digits=2

# Addebito di 1 scatto alla risposta
per_connection=152.40

# Impostazione di default: tariffa ridotta
default=(175.80, 60)

# Giorni Feriali e sabato: intera
on (monday..friday) between (8:00..18:30) use (318.60, 60)
on (saturday) between (8:00..13:00) use (318.60, 60)

# Nelle festivita' si applica la tariffa ridotta
on (01/01) between () use (175.80, 60)
on (01/06) between () use (175.80, 60)
on (04/25) between () use (175.80, 60)
on (05/01) between () use (175.80, 60)
on (08/15) between () use (175.80, 60)
on (11/01) between () use (175.80, 60)
on (12/08) between () use (175.80, 60)
on (12/25) between () use (175.80, 60)
on (easter + 1) between () use (175.80, 60)

# Fine
