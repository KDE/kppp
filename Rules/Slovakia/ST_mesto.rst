################################################################
# Slovak Telecom rate ruleset
#
# created 98/01/29 by Juraj Bednár <bednar@isternet.sk>
#
# no extensive testing...
# updated 00/03/04 by carcass <carcass@gmx.net>
################################################################

name=ST_mesto
currency_symbol=Sk
currency_position=right 
currency_digits=2
per_connection=0.0
minimum_costs=0.0
default=(2.50, 120)

on (monday..friday)   between (19:00..7:00)  use (2.50, 240)
on (saturday..sunday)   between ()   use (2.50, 360)

on (1/1, 1/6, 5/1, 5/8, 7/5, 8/29)  between ()  use (2.50, 360)
on (9/1, 9/15, 11/1, 12/24..12/26)  between ()  use (2.50, 360)

on (easter)   between ()    use (2.50, 360)
