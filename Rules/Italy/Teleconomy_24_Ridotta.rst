################################################################
#
# kppp ruleset for Italy
#
# by Luca Boni (me9139@mclink.it, jungbn@netsacape.net)
#
# Teleconomy_24.rst
#
# Sconto per il collegamento ad un ISP con Telecom Italia S.p.A.
#
# Tariffe aggiornate a Giugno 2001.
# 
################################################################
##							      ##
## Addebito di 1 scatto alla risposta di £ 120.  	      ##
##							      ##
## Tutti i prezzi si intendono in £/min IVA COMPRESA.         ##
##							      ##
## Tariffa:  £ 18 sempre (Feriali, Festivi e Sabato)	      ##
##							      ##
################################################################


# Nome del ruleset
name=Teleconomy_24_Ridotta

# Impostazioni della valuta
currency_symbol=£.
currency_position=left 
currency_digits=2

# Addebito di 1 scatto alla risposta
per_connection=120.00

# Impostazione di default: tariffa unica
default=(18.00, 60)

# Fine
