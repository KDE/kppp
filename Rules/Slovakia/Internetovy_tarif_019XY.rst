################################################################
# Slovak Telecom rate ruleset for  019XY numbers
#
# created 00/02/27 by carcass <carcass@gmx.net>
#
# DISCLAIMER: Use at your own risk		;)
################################################################

name=Internetovy_tarif_019XY
currency_symbol=Sk
currency_position=right
currency_digits=2
per_connection=0.0
minimum_costs=0.0
default=(2.50, 300)

on (monday..friday)   between (7:00..19:00)     use (2.50, 150)
on (monday..friday)   between (21:00..5:00)     use (2.50, 480)
on (saturday..sunday)    between ()    use (2.50, 480)

on (1/1, 1/6, 5/1, 5/8, 7/5, 8/29)  between ()  use (2.50, 480)
on (9/1, 9/15, 11/1, 12/24..12/26)  between ()  use (2.50, 480)

on (easter)   between ()    use (2.50, 480)
