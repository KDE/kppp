################################################################
#
# kppp ruleset for Easynet easy-call accounts
# created 02/02/01 by Jochen Fecht
#
################################################################


################################################################
#
# NAME OF THE RULESET. This is NEEDED for accounting purposes.
#
################################################################
name=Easynet easy-call-Tarif


################################################################
# currency settings
################################################################

currency_symbol=EUR
currency_position=right 
currency_digits=3


################################################################
# connection settings
################################################################

per_connection=0.0
minimum_costs=0.0
default=(0.0148, 60)
on () between (18:00..8:59) use (0.0097, 60)



