################################################################
# $Id$
# $Source$
#
# RULES FOR KPPP WITH SLOVENIAN PHONE NETWORK 0880 (ISP PROVIDER).
#
# 2002-02-02:
# Updated by roman.maurer@amis.net,
# based on data provided by Primoz Hrvatin.
#
# 2000-09-25:
# Prepared by Ales.Kosir@hermes.si, 
# based on data provided by Srdjan Cvjetovic.
#
# Calls from stationary phone network to the ISP numbers 0880:
# A user pays to the phone company the fixed price for connection, and 
# the ISP will charge you per second for the duration of the call.
# The cost is 2.04 SIT per minute on normal rate,
# and 1.02 SIT on discounted. 
#
################################################################
name=omrezje_0880.rst

################################################################
# currency settings
################################################################

# NOTE: rules are applied from top to bottom - the
#       LAST matching rule is the one used for the
#       cost computations.

currency_symbol=SIT
currency_position=right 
currency_digits=2

################################################################
# connection settings
################################################################

# This is charged whenever you connect. The amount is payed to the 
# phone company.

per_connection=6.30

# "on monday until sunday the costs
# are 2.04 on normal and 1.02 on discounted per each 60 seconds"

on (monday..saturday) between (0:00..6:59)   use (1.02, 60)
on (monday..saturday) between (7:00..18:59)  use (2.04, 60)
on (monday..saturday) between (19:00..23:59) use (1.02, 60)
on (sunday) between () use (1.02, 60)

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
on (01/01, 01/02, 02/08, easter, easter+1, 04/26, 05/01, 05/02, easter+50, 06/25, 08/15, 10/31, 11/01, 12/25, 12/26) between () use (1.02, 60)
