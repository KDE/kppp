################################################################
#
# NAME OF THE RULESET. This is NEEDED for accounting purposes.
#
# Prepared by Ales.Kosir@hermes.si, 
# based on data, provided by Srdjan Cvjetovic
#
# Calls from stationary phone network 
# to the Moj Net ISP on number 0889.
#
################################################################
name=moj_net.rst

################################################################
# currency settings
################################################################

currency_symbol=SIT
currency_position=right 
currency_digits=2

################################################################
# connection settings
################################################################

per_connection=0.0
minimum_costs=4.11


# same as above. You must use 24 hour notation, or the accounting
# will not work correctly. (Example: write 15:00 for 3 pm)

on (monday..saturday) between (0:00..6:59) use (4.11, 197.28)
on (monday..saturday) between (7:00..18:59) use (4.11, 98.64)
on (monday..saturday) between (19:00..23:59) use (4.11, 197.28)
on (sunday) between () use (4.11, 197.28)

# Drzavni prazniki:
# 1.1., 2.1 - Novo leto
# 8.2.      - Slovenski kulturni praznik
# Easter    - Velika noc
# Easter Monday - Velikonocni ponedeljek
# 27.4.     - Dan upora proti okupatorju
# 1.5.,2.5. - Praznik dela
#           - Binkosti
# 25.6.     - Dan drzavnosti
# 15.8.     - Marijino vnebovzetje
# 31.10.    - Dan reformacije
# 1.11.     - Dan spomina na mrtve
# 25.12.    - Bozic
# 26.12.    - Dan samostojnosti
on (01/01, 01/02, 02/08, easter, easter+1, 04/26, 05/01, 05/02, easter+50, 06/25, 08/15, 10/31, 11/01, 12/25, 12/26) between () use (4.11, 197.28)




