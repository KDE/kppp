################################################################
# Slovak Telecom rate ruleset for 019XY numbers after 1.7.2001
#
# created 01/07/31 by kayle <kayle@szm.sk>
#
# DISCLAIMER: Use at your own risk		;)
################################################################

name=Internetovy_tarif_019XY
currency_symbol=Sk
currency_position=right
currency_digits=2
per_connection=1.50
minimum_costs=0.0
default=(1.50, 240)

# on (monday..friday)   between (0:00..7:00)      use (1.50, 240)
on (monday..friday)   between (7:00..19:00)     use (1.50, 90)
# on (monday..friday)   between (19:00..24:00)    use (1.50, 240)

on (saturday..sunday)    between ()    use (1.50, 290)

on (1/1, 1/6, 5/1, 5/8, 7/5, 8/29)  between ()  use (1.50, 290)
on (9/1, 9/15, 11/1, 12/24..12/26)  between ()  use (1.50, 290)

on (easter)   between ()    use (1.50, 290)


