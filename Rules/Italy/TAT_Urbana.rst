################################################################
#
# kppp ruleset for Italy
#
# by Luca Boni (me9139@mclink.it, jungbn@netsacape.net)
#
# TAT_Urbana.rst
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
## Tariffa RIDOTTA:  fino a 15 min = £ 21,24  oltre = £ 19,08 ##
## Tariffa INTERA:   fino a 15 min = £ 36,72  oltre = £ 33,12 ##
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

# Nome del ruleset
name=TAT_Urbana

# Impostazioni della valuta
currency_symbol=£.
currency_position=left 
currency_digits=2

# Addebito di 1 scatto alla risposta
per_connection=120.00

# Impostazione di default: tariffa ridotta fino a 15 min
default=(21.24, 60)

# Giorni Feriali, sabato e domenica
on (monday..friday) between (18:30..8:00) use (19.08, 60, 900)
on (monday..friday) between (8:00..18:30) use (36.72, 60)
on (monday..friday) between (8:00..18:30) use (33.12, 60, 900)
on (saturday) between (13:00..8:00) use (19.08, 60, 900)
on (saturday) between (8:00..13:00) use (36.72, 60)
on (saturday) between (8:00..13:00) use (33.12, 60, 900)
on (sunday) between () use (19.08, 60, 900)

# Giorni Festivi
on (01/01) between () use (21.24, 60)
on (01/01) between () use (19.08, 60, 900)
on (01/06) between () use (21.24, 60)
on (01/06) between () use (19.08, 60, 900)
on (04/25) between () use (21.24, 60)
on (04/25) between () use (19.08, 60, 900)
on (05/01) between () use (21.24, 60)
on (05/01) between () use (19.08, 60, 900)
on (08/15) between () use (21.24, 60)
on (08/15) between () use (19.08, 60, 900)
on (11/01) between () use (21.24, 60)
on (11/01) between () use (19.08, 60, 900)
on (12/08) between () use (21.24, 60)
on (12/08) between () use (19.08, 60, 900)
on (12/25) between () use (21.24, 60)
on (12/25) between () use (19.08, 60, 900)
on (12/26) between () use (21.24, 60)
on (12/26) between () use (19.08, 60, 900)
on (easter + 1) between () use (21.24, 60)
on (easter + 1) between () use (19.08, 60, 900)

# Fine
