################################################################
#
# kppp ruleset for Italy
#
# by Luca Boni (me9139@mclink.it, jungbn@netsacape.net)
#
# Infostrada_Libero@Sogno.rst
#
# Navigazione internet con ISP Infostrada a costo ZERO
# dalle 18:30 alle 8:00.
#
# Tariffe aggiornate a Giugno 2001.
# 
################################################################
##							      ##
## Addebito di 1 scatto (£ 120) alla risposta solo nel	      ##
## periodo in cui si applica la tariffa intera.		      ##
##							      ##
## Tutti i prezzi si intendono in £/min IVA compresa.         ##
##							      ##
## 		Tariffa RIDOTTA: £ 0		 	      ##
## 		Tariffa INTERA : £ 33,6	      		      ##
##		   					      ##
##		              				      ##
##		    | Ridotta   dalle 18:30 alle 08:00	      ##
## Giorni FERIALI : |					      ##
##		    | Intera    dalle 08:00 alle 18:30	      ##
##							      ##
##			     				      ##
## Giorni FESTIVI : | Ridotta tutto il giorno		      ##
## 	 e SABATO : |					      ##
##							      ##
################################################################


# Nome del ruleset
name=Infostrada_Libero@Sogno

# Impostazioni della valuta
currency_symbol=£.
currency_position=left 
currency_digits=2

# Tariffa di default: ridotta
default=(00.00, 60)

# Serve a simulare lo scatto alla risposta ;-)
# per la tariffa intera
on (monday..friday) between (8:00..18:30) use (120.00, 1)

# Giorni Feriali: intera
on (monday..friday) between (8:00..18:30) use (33.60, 60, 1)

# Nelle festivita' si applica la tariffa ridotta
on (01/01) between () use (00.00, 60)
on (01/06) between () use (00.00, 60)
on (04/25) between () use (00.00, 60)
on (05/01) between () use (00.00, 60)
on (08/15) between () use (00.00, 60)
on (11/01) between () use (00.00, 60)
on (12/08) between () use (00.00, 60)
on (12/25) between () use (00.00, 60)
on (easter + 1) between () use (00.00, 60)

# Fine
