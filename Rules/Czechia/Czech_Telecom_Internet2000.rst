# submitted 2000/06/20 by Vaclav Slavik <v.slavik@volny.cz>

name=Czech_Telecom_Internet2000
currency_symbol=Kc
currency_position=right 
currency_digits=2
per_connection=0.00
minimum_costs=0.00


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

on (01/01, easter+1, 05/01, 05/08, 07/05, 07/06, 10/28, 12/24, 12/25, 12/26) between (0:00..23:59) use (2.60, 290)
on (01/01, easter+1, 05/01, 05/08, 07/05, 07/06, 10/28, 12/24, 12/25, 12/26) between (0:00..23:59) use (2.60, 750, 290)

# Nejlevnejsi tarif:

on (monday..friday)   between (21:00..7:00) use (2.60, 290)
on (monday..friday)   between (21:00..7:00) use (2.60, 750, 290)
on (saturday..sunday) between ()            use (2.60, 290)
on (saturday..sunday) between ()            use (2.60, 750, 290)

# Spicka:

on (monday..friday)   between (7:00..17:00) use (2.60, 120)
on (monday..friday)   between (7:00..17:00) use (2.60, 180, 120)

# Slabe I:

on (monday..friday)   between (17:00..19:00) use (2.60, 120)
on (monday..friday)   between (17:00..19:00) use (2.60, 390, 120)

# Slabe II:

on (monday..friday)   between (19:00..21:00) use (2.60, 290)
on (monday..friday)   between (19:00..21:00) use (2.60, 390, 290)
