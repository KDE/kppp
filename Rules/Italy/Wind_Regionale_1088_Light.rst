################################################################
#
# kppp ruleset for Italy
#
# by Luca Boni (me9139@mclink.it, jungbn@netsacape.net)
#
# Wind_Regionale_1088_Light.rst
#
# Chiamate regionali (verso i prefissi associati al compartimento chiamante).
#
# Tariffe aggiornate a Giugno 2001.
# 
################################################################
##							      ##
## La tariffa prevede uno sconto dopo i primi 4 minuti.       ##
## Nessuno scatto alla risposta.			      ##
##							      ##
## Tutti i prezzi si intendono in £/min IVA compresa.         ##
##							      ##
## Tariffa RIDOTTA: fino a 4 min = £ 174  oltre= £ 130,5      ##
## Tariffa INTERA : fino a 4 min = £ 258  oltre= £ 193,5      ##
##		   					      ##
##		              				      ##
##		    | Ridotta   dalle 19:00 alle 09:00	      ##
## Giorni FERIALI : |					      ##
##		    | Intera    dalle 09:00 alle 19:00	      ##
##							      ##
##			     				      ##
## Giorni FESTIVI : | Ridotta tutto il giorno		      ##
## 	 e SABATO : |					      ##
##							      ##
################################################################


# Nome del ruleset
name=Wind_Regionale_1088_Light

# Impostazioni della valuta
currency_symbol=£.
currency_position=left 
currency_digits=2

# Tariffa di default: ridotta fino a 4 minuti
default=(174.00, 60)

# Giorni Feriali e sabato: ridotta oltre 4 minuti
on (monday..friday) between (19:00..9:00) use (130.50, 60, 240)
on (saturday) between () use (130.50, 60, 240)

# Giorni Feriali: intera
on (monday..friday) between (9:00..19:00) use (258.00, 60)
on (monday..friday) between (9:00..19:00) use (193.50, 60, 240)

# Nelle festivita' si applica la tariffa ridotta
on (01/01) between () use (174.00, 60)
on (01/01) between () use (130.50, 60, 240)
on (01/06) between () use (174.00, 60)
on (01/06) between () use (130.50, 60, 240)
on (04/25) between () use (174.00, 60)
on (04/25) between () use (130.50, 60, 240)
on (05/01) between () use (174.00, 60)
on (05/01) between () use (130.50, 60, 240)
on (08/15) between () use (174.00, 60)
on (08/15) between () use (130.50, 60, 240)
on (11/01) between () use (174.00, 60)
on (11/01) between () use (130.50, 60, 240)
on (12/08) between () use (174.00, 60)
on (12/08) between () use (130.50, 60, 240)
on (12/25) between () use (174.00, 60)
on (12/25) between () use (130.50, 60, 240)
on (easter + 1) between () use (174.00, 60)
on (easter + 1) between () use (130.50, 60, 240)

# Fine
