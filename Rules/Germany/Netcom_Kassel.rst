########################################################
# Netcom-Kassel (Hercules L-XL) Hotline-Tel:9202020
#                                  internet:9202000
#
# createt on 13.05.2001 by Stefan Zintel
# <zintel@netcomcity.de>
#
########################################################

name=Netcom-Kassel
currency_symbol=DM
currency_position=right
currency_digits=2
per_connection=0.0
minimum_costs=0.0
# Standardtarif ist 3 Pf pro Minute
default=(0.0005, 1)
# Taeglich zwischen 18 Uhr und 19 Uhr ist es Kostenlos
on () between (18:00..19:00) use (0.0, 2)

