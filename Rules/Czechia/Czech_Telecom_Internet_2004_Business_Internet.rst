#***********************************************************#
# Czech Telecom rate ruleset			                  #
# Internet Business 2004                                    #
# Created 2004/11/01 Jan Klepek <j.klepek@centrum.cz>       #
#***********************************************************#

name=Czech_Telecom_Internet_2004_Business_Ineternet
currency_symbol=Kc
currency_position=right
currency_digits=2
per_connection=0.0
minimum_costs=0.0

default=(1.40, 60)

on (monday..friday) between (6:00..17:59) use (1.51, 60)
on (monday..friday) between (6:00..17:59) use (0.84, 60, 600)
on (monday..friday) between (18:00..5:59) use (0.70, 60)
on (monday..friday) between (18:00..5:59) use (0.33, 60, 600)
on (saturday..sunday) between () use (0.70, 60)
on (saturday..sunday) between () use (0.33, 60, 600)
on (01/01, 04/20, 04/21, 05/01, 05/08, 07/05, 07/06, 09/28, 10/28, 11/17, 12/24..12/26) between () use (0.70, 60)
on (01/01, 04/20, 04/21, 05/01, 05/08, 07/05, 07/06, 09/28, 10/28, 11/17, 12/24..12/26) between () use (0.33, 60, 600)
