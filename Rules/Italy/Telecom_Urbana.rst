################################################################
# kppp ruleset for Italy
# 
# Telecom Italia S.p.A.
# Tariffa: urbana 
#
# by Andrea Botti (siesta@libero.it)
################################################################

name=Italy_Telecom_urbana_2
currency_symbol=L.
currency_position=left 
currency_digits=0

# Al 03/04/2000  

# La chiamata urbana comporta un importo iniziale di L. 100 + IVA, addebitati
# all'istante della risposta.
per_connection=120

# Tariffa		costo per i primi 15'		costo dopo 15'
#-------------------------------------------------------------------------
# Ordinaria		L. 36,72 per minuto 		L. 33,12 per minuto 
# Ridotta		L. 21,24   "			L. 19,08    "

# Tariffe ridotte // The minimum charge rate 
default=(.354, 1)
on () between () use (.318, 1, 900)

# Settimane normali // Normal weeks
# Tariffa ordinaria per i primi 15' //The  normal charge rate for the first 15 min.
on (monday..friday) between (8:00..18:30) use (.612, 1)
on (saturday) between (8:00..13:00) use (.612, 1)
# Tariffa ordinaria dopo i primi 15' //The normal charge rate after the first 15 min.
on (monday..friday) between (8:00..18:30) use (.552, 1, 900)
on (saturday) between (8:00..13:00) use (.552, 1, 900)

# Nelle festivita' (Natale, S.Stefano, Capodanno, Pasquetta, ecc) si applica
# la tariffa minima // On bank holidays (Christmas Day, Boxing Day, New Year's 
# Day, etc.) the minimum charge rate applies 
on (01/01) between () use (.354, 1)
on (01/01) between () use (.318, 1, 900)
on (01/06) between () use (.354, 1)
on (01/06) between () use (.318, 1, 900)
on (04/25) between () use (.354, 1)
on (04/25) between () use (.318, 1, 900)
on (05/01) between () use (.354, 1)
on (05/01) between () use (.318, 1, 900)
on (08/15) between () use (.354, 1)
on (08/15) between () use (.318, 1, 900)
on (11/01) between () use (.354, 1)
on (11/01) between () use (.318, 1, 900)
on (12/08) between () use (.354, 1)
on (12/08) between () use (.318, 1, 900)
on (12/25) between () use (.354, 1)
on (12/25) between () use (.318, 1, 900)
on (12/25) between () use (.354, 1)
on (12/25) between () use (.318, 1, 900)
on (easter + 1) between () use (.354, 1)
on (easter + 1) between () use (.318, 1, 900)

# Nota: IVA inclusa // Note: VAT is included 

#End file

