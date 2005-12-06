################################################################
# Regułki obliczania kosztów połączeń lokalnych w sieci TP S.A.
# Obowiązują od 1 lipca 1999, podwyżka 16 stycznia 2000
# autor: Jacek Stolarczyk <jacek@mer.chemia.polsl.gliwice.pl>
################################################################

name=TP_Lokalne
currency_symbol=PLN
currency_position=right
currency_digits=2
per_connection=0.0
minimum_costs=0.0

# Co 360 sekund dodawane bedzie 33 groszy do rachunku.
# Jest to tak zwana taryfa nocna.
default=(0.33, 360)
# niestety w ciągu dnia czas między impulsami jest 2x krótszy
on (monday..sunday) between (8:00..21:59) use (0.33, 180)

