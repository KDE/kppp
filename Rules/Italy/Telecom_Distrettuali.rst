###############################################################
#
# kppp ruleset for Italy
# by Riccardo Rencinai (riccardo.rencinai@prometeo.lett.unisi.it)
#
# Telecom_Distrettuali.rst 
#
# Comunicazioni distrettuali / Calls within the same call area between
# different urban networks.
#
# Le comunicazioni telefoniche distrettuali sono quelle che avvengono
# tra due numeri con lo stesso prefisso fra reti urbane diverse
#
# 'District' call rates apply when both the caller and the receiver
# have the same area code but belong to different urban networks.
#
################################################################


# Nome del ruleset // Ruleset name
name=Telecom_Distrettuali

# Impostazioni della valuta // Currency settings
currency_symbol=L.
currency_position=left 
currency_digits=0

# Settimane normali // Normal weeks 
per_connection=152.4
default=(0.424, 1)
on (monday..friday) between (8:00..18:30) use (0.846, 1)
on (saturday) between (8:00..13:00) use (0.846, 1)

# Nelle festivita' (Natale, S.Stefano, Capodanno, Pasquetta, ecc) si applica
# la tariffa minima // On bank holidays (Christmas Day, Boxing Day, New Year's 
# Day, etc.) the minimum charge rate applies 
on (01/01) between () use (0.424, 1)
on (01/06) between () use (0.424, 1)
on (04/25) between () use (0.424, 1)
on (05/01) between () use (0.424, 1)
on (08/15) between () use (0.424, 1)
on (11/01) between () use (0.424, 1)
on (12/08) between () use (0.424, 1)
on (12/25) between () use (0.424, 1)
on (12/26) between () use (0.424, 1)
on (easter + 1) between () use (0.424, 1)

# Nota: IVA inclusa // Note: VAT is included 

# Fine // End of file









