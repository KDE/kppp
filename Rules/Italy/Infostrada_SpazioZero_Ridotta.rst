################################################################
#
# kppp ruleset for Italy
#
# by Luca Boni (me9139@mclink.it, jungbn@netsacape.net)
#
# Infostrada_SpazioZero_Ridotta.rst
#
# Tariffa SpazioZero scontata per il collegamento ad un ISP Infostrada.
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
name=Infostrada_SpazioZero_Ridotta

# Impostazioni della valuta
currency_symbol=£.
currency_position=left 
currency_digits=2

# Addebito di 1 scatto alla risposta
per_connection=120.00

# Impostazione di default: tariffa unica
default=(18.00, 60)

# Fine
