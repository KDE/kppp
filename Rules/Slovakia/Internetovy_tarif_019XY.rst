################################################################
# Slovak Telecom rate ruleset for  019XY numbers
#
# created 00/02/27 by carcass <carcass@gmx.net>
# modified by Juraj Bednar <juraj@bednar.sk>
# contributed by maddan
#
# DISCLAIMER: Use at your own risk		;)
################################################################

name=Internetovy_tarif_019XY
currency_symbol=Sk
currency_position=right
currency_digits=2
per_connection=1.476
minimum_costs=0.0
default=(1.476, 240)

on (monday..friday)   between (7:00..19:00)     use (1.476, 150)
on (monday..friday)   between (21:00..5:00)     use (1.476, 240)
on (saturday..sunday)    between ()    use (1.476,290)

on (1/1, 1/6, 5/1, 5/8, 7/5, 8/29)  between ()  use (1.476, 290)
on (9/1, 9/15, 11/1, 12/24..12/26)  between ()  use (1.476, 290)

on (easter)   between ()    use (1.476, 290)
