################################################################
# Czech Vydriduch Telecom Internet 2002 rate ruleset
# Tarif Mini
# created 2002/01/12 by Petr Simandl <simandl@mujmail.cz>
################################################################

name=Tarif_INTERNET_2002_MINI
currency_symbol=Kc
currency_position=right 
currency_digits=2
per_connection=0.0
minimum_costs=0.0

# Svatky se snizenym tarifem:
# 01/01         Novy rok
# easter+1      Velikonocni pondeli
# 05/01         Svatek prace
# 05/08         Den osvobozeni od fasistu
# 07/05         Cyril a Metodej
# 07/06         Jan Hus
# 09/28         sv. Vaclav
# 10/28         Den vzniku sam. Ceskoslovenska
# 11/17         Den boje studentu za svobodu
# 12/24         Stedry den
# 12/25         1. svatek vanocni
# 12/26         2. svatek vanocni

on (01/01, easter+1, 05/01, 05/08, 07/05, 07/06, 09/28, 10/28, 11/17, 12/24, 12/25, 12/26) between () use (01.60,120,   0)
on (01/01, easter+1, 05/01, 05/08, 07/05, 07/06, 09/28, 10/28, 11/17, 12/24, 12/25, 12/26) between () use (00.80, 60, 120)
on (01/01, easter+1, 05/01, 05/08, 07/05, 07/06, 09/28, 10/28, 11/17, 12/24, 12/25, 12/26) between () use (00.35, 60, 600)

on (monday..friday)   between (06:00..17:59) use (04.00,120,   0)
on (monday..friday)   between (06:00..17:59) use (02.00, 60, 120)
on (monday..friday)   between (06:00..17:59) use (01.15, 60, 600)

on (monday..friday)   between (18:00..05:59) use (01.60,120,   0)
on (monday..friday)   between (18:00..05:59) use (00.80, 60, 120)
on (monday..friday)   between (18:00..05:59) use (00.35, 60, 600)

on (saturday..sunday) between ()             use (01.60,120,   0)
on (saturday..sunday) between ()             use (00.80, 60, 120)
on (saturday..sunday) between ()             use (00.35, 60, 600)
