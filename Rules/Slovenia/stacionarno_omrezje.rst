################################################################
#
# NAME OF THE RULESET. This is NEEDED for accounting purposes.
#
# Prepared by Ales.Kosir@hermes.si, 
# based on data, provided by Srdjan Cvjetovic
#
# This accounting information includes all calls from stationary 
# phone network to the numbers starting with 01, 02, 03 (except 031), 
# 04 (except 040 in 041), 05, and 07.
#
################################################################
name=stacionarno_omrezje.rst

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

on (monday..saturday) between (0:00..6:59) use (4.11, 120)
on (monday..saturday) between (7:00..18:59) use (4.11, 60)
on (monday..saturday) between (19:00..23:59) use (4.11, 120)
on (sunday) between () use (4.11, 120)

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
on (01/01, 01/02, 02/08, easter, easter+1, 04/26, 05/01, 05/02, easter+50, 06/25, 08/15, 10/31, 11/01, 12/25, 12/26) between () use (4.11, 120)




