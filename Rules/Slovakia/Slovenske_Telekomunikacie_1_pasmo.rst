################################################################
# Slovak Telecom rate ruleset
#
# created 98/01/29 by Juraj Bednár <bednar@isternet.sk>
# modified 99/12/9 by Matej Ko¹¹ <koss@miesto.sk>
#
# no extensive testing...
################################################################

name=Slovenske_Telekomunikacie_Mesto
currency_symbol=Sk
currency_position=right 
currency_digits=2
per_connection=0.0
minimum_costs=0.0
default=(2.50, 120)

# (for CITY Tarif)
on (monday..friday)   between (7:00..19:00)  use (2.50, 40)
on (monday..friday)   between (19:00..21:00)  use (2.50, 80)
