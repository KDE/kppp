################################################################
# Slovak Telecom rate ruleset
#
# created 98/01/29 by Juraj Bednár <juraj@bednar.sk>
#
# no extensive testing...
# updated 00/03/04 by carcass <carcass@gmx.net>
# updates 01/07/03 by Juraj Bednar <juraj@bednar.sk>
################################################################

name=ST_medzimesto
currency_symbol=Sk
currency_position=right 
currency_digits=2
per_connection=1.476
minimum_costs=0.0
default=(1.476, 20)

on (monday..friday)   between (19:00..7:00)  use (1.476, 40)
on (saturday..sunday)   between ()   use (1.476, 60)

on (1/1, 1/6, 5/1, 5/8, 7/5, 8/29)  between ()  use (1.476, 60)
on (9/1, 9/15, 11/1, 12/24..12/26)  between ()  use (1.476, 60)

on (easter)   between ()    use (1.476, 60)
