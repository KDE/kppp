################################################################
#
# kppp ruleset for Italy
#
# by Luca Boni (me9139@mclink.it, jungbn@netsacape.net)
#
# Infostrada_Nazionali.rst
#
# Chiamate verso localita' situate in una regione diversa da 
# quella del chiamante.
#
# Tariffe aggiornate a Giugno 2001.
# 
################################################################
##							      ##
## Addebito di 1 scatto alla risposta di £ 150.		      ##
##							      ##
## Tutti i prezzi si intendono in £/min IVA compresa.         ##
##							      ##
## 		Tariffa  RIDOTTA: £ 60		 	      ##
## 		Tariffa  INTERA : £ 150	      		      ##
##		Tariffa di PUNTA: £ 222			      ##
##		   					      ##
##		              				      ##
##		    | Ridotta   dalle 22:00 alle 08:00	      ##
## Giorni FERIALI : | Punta	dalle 08:00 alle 18:30	      ##
##		    | Intera    dalle 18:30 alle 22:00	      ##
##							      ##
##			     				      ##
## Giorni FESTIVI : | Ridotta   dalle 22:00 alle 08:00        ##
## 	 e SABATO : | Intera	dalle 08:00 alle 22:00	      ##
##							      ##
################################################################


# Nome del ruleset
name=Infostrada_Nazionali

# Impostazioni della valuta
currency_symbol=£.
currency_position=left 
currency_digits=2

# Addebito di 1 scatto alla risposta
per_connection=150.00

# Tariffa di default: ridotta
default=(60.00, 60)

# Giorni Feriali: tariffa di punta e intera
on (monday..friday) between (8:00..18:30) use (222.00, 60)
on (monday..friday) between (18:30..22:00) use (150.00, 60)

# Nelle festivita' e il sabato: intera
on (saturday) between (8:00..22:00) use (150.00, 60)
on (01/01) between (8:00..22:00) use (150.00, 60)
on (01/06) between (8:00..22:00) use (150.00, 60)
on (04/25) between (8:00..22:00) use (150.00, 60)
on (05/01) between (8:00..22:00) use (150.00, 60)
on (08/15) between (8:00..22:00) use (150.00, 60)
on (11/01) between (8:00..22:00) use (150.00, 60)
on (12/08) between (8:00..22:00) use (150.00, 60)
on (12/25) between (8:00..22:00) use (150.00, 60)
on (easter + 1) between (8:00..22:00) use (150.00, 60)

# Fine
