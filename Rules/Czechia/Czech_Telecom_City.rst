################################################################
# Czech Telecom rate ruleset
#
# created 98/06/18 by Miroslav Flídr <flidr@kky.zcu.cz>
#
# no extensive testing...
################################################################

name=Czech_Telecom_City
currency_symbol=Kc
currency_position=right 
currency_digits=2
per_connection=0.0
minimum_costs=0.0
default=(2.60, 360)

on (monday..friday)   between (7:00..19:00)   use (2.60, 120)
on (monday..friday)   between (19:00..21:00)  use (2.60, 240)

# Svatky se snizenym tarifem:
# 01/01         Novy rok
# easter+1      Velikonocni pondeli
# 05/01         Svatek prace
# 05/08         Den osvobozeni od fasistu
# 07/05         Cyril a Metodej
# 07/06         Jan Hus
# 10/28         Den vzniku sam. Ceskoslovenska
# 12/24         Stedry den
# 12/25         1. svatek vanocni
# 12/26         2. svatek vanocni

on (01/01, easter+1, 05/01, 05/08, 07/05, 07/06, 10/28, 12/24, 12/25, 12/26) between (0:00..23:59) use (2.60, 360)
