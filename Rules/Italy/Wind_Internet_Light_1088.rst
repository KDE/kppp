################################################################
#
# kppp ruleset for Italy
#
# by Luca Boni (me9139@mclink.it, jungbn@netsacape.net)
#
# Wind_Internet_Light_1088.rst
#
# Nelle localita' dove e' attivo il servizio locale
# (urbane e fuoriporta) e dove ci sono i POP InWind.
#
# Tariffe aggiornate a Giugno 2001.
# 
################################################################
##							      ##
## Nessuno scatto alla risposta.		  	      ##
##							      ##
## Tutti i prezzi si intendono in £/min IVA COMPRESA.         ##
##							      ##
## Tariffa RIDOTTA:	£ 12 al minuto			      ##
## Tariffa INTERA:	£ 24 al minuto			      ##
##							      ##
##			| intera  dalle 09:00 alle 19:00      ##
## Giorni FERIALI:	| ridotta dalle 19:00 alle 09:00      ##
##							      ##
## Giorni FESTIVI:	| ridotta tutto il giorno	      ##
##	 e Sabato:	|				      ##
##							      ##
################################################################


# Nome ruleset
name=Wind_Internet_Light_1088

# Impostazione valuta
currency_symbol=£.
currency_position=left 
currency_digits=2

# Tariffa di default: ridotta
default=(12.00, 60)

# Giorni Feriali: intera
on (monday..friday) between (9:00..19:00) use (24.00, 60)

# Nelle festivita' si applica la tariffa ridotta
on (01/01) between () use (12.00, 60)
on (01/06) between () use (12.00, 60)
on (04/25) between () use (12.00, 60)
on (05/01) between () use (12.00, 60)
on (08/15) between () use (12.00, 60)
on (11/01) between () use (12.00, 60)
on (12/08) between () use (12.00, 60)
on (12/25) between () use (12.00, 60)
on (easter + 1) between () use (12.00, 60)

# Fine
