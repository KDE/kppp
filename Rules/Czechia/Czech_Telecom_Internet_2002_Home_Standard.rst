# by Michal Zejdl <zejdlm@volny.cz>

name=Czech_Telecom_Internet_2002_Home_Standard
currency_symbol=Kc
currency_position=right 
currency_digits=2
per_connection=0.0
minimum_costs=0.0

default=(1.40, 60)

on (monday..friday) between (6:00..17:59) use (2.80, 120)
on (monday..friday) between (6:00..17:59) use (1.40, 60, 120)
on (monday..friday) between (6:00..17:59) use (0.80, 60, 600)
on (monday..friday) between (18:00..5:59) use (1.10, 120)
on (monday..friday) between (18:00..5:59) use (0.55, 60, 120)
on (monday..friday) between (18:00..5:59) use (0.25, 60, 600)
on (saturday..sunday) between () use (1.10, 120)
on (saturday..sunday) between () use (0.55, 60, 120)
on (saturday..sunday) between () use (0.25, 60, 600)
