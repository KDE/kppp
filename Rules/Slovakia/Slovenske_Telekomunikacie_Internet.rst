################################################################
# Slovak Telecom rate ruleset
#
# created 01/08/21 by Jan Gaspar <jano_gaspar@yahoo.com>
#
# no extensive testing...
################################################################

name=Slovenske_Telekomunikacie_Internet
currency_symbol=Sk
currency_position=right
currency_digits=2
per_connection=1.476
minimum_costs=0.0
default=(1.476, 240)

on (monday..friday)   between (7:00..19:00)  use (1.476, 90)
on (saturday..sunday) between ()  use (1.476, 290)

