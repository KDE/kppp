################################################################
#
# kppp ruleset for Italy
#
# by Luca Boni (me9139@mclink.it, jungbn@netsacape.net)
#
# Telecom_Formula_Urbana.rst
#
# Sconto per il collegamento ad un ISP con Telecom Italia S.p.A.
#
# Tariffe aggiornate a Giugno 2001.
# 
################################################################
##							      ##
## La tariffa prevede uno sconto dopo i primi 6 minuti,       ##
## e l'addebito di 1 scatto alla risposta di £ 120.	      ##
##							      ##
## Tutti i prezzi si intendono in £/min IVA compresa.         ##
##							      ##
## Tariffa RIDOTTA: fino a 6 min = £ 21,24  oltre= £ 10,44    ##
## Tariffa INTERA : fino a 6 min = £ 36,72  oltre= £ 18,36    ##
##		   					      ##
##		              				      ##
##		    | Ridotta   dalle 18:30 alle 08:00	      ##
## Giorni FERIALI : |					      ##
##		    | Intera    dalle 08:00 alle 18.30	      ##
##							      ##
##			     				      ##
##		    | Ridotta   dalle 13:00 alle 08:00	      ##
## 	   SABATO : |					      ##
##		    | Intera    dalle 08:00 alle 13:00	      ##
##							      ##
## Giorni FESTIVI :   Ridotta   dalle 00:00 alle 24:00	      ##
##							      ##
################################################################


# Nome del ruleset
name=Telecom_Formula_Urbana

# Impostazioni della valuta
currency_symbol=£.
currency_position=left 
currency_digits=2

# Addebito di 1 scatto alla risposta
per_connection=120.00

# Impostazione di default: ridotta fino a 6 min
default=(21.24, 60)

# Giorni Feriali e sabato: ridotta
on (monday..friday) between (18:30..8:00) use (10.44, 60, 360)
on (saturday) between (13:00..8:00) use (10.44, 60, 360)

# Giorni Feriali e sabato: intera
on (monday..friday) between (8:00..18:30) use (36.72, 60)
on (monday..friday) between (8:00..18:30) use (18.36, 60, 360)
on (saturday) between (8:00..13:00) use (36.72, 60)
on (saturday) between (8:00..13:00) use (18.36, 60, 360)

# Nelle festivita' si applica la tariffa ridotta
on (01/01) between () use (21.24, 60)
on (01/01) between () use (10.44, 60, 360)
on (01/06) between () use (21.24, 60)
on (01/06) between () use (10.44, 60, 360)
on (04/25) between () use (21.24, 60)
on (04/25) between () use (10.44, 60, 360)
on (05/01) between () use (21.24, 60)
on (05/01) between () use (10.44, 60, 360)
on (08/15) between () use (21.24, 60)
on (08/15) between () use (10.44, 60, 360)
on (11/01) between () use (21.24, 60)
on (11/01) between () use (10.44, 60, 360)
on (12/08) between () use (21.24, 60)
on (12/08) between () use (10.44, 60, 360)
on (12/25) between () use (21.24, 60)
on (12/25) between () use (10.44, 60, 360)
on (easter + 1) between () use (21.24, 60)
on (easter + 1) between () use (10.44, 60, 360)

# Fine
