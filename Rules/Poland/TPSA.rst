################################################################
# Regu³ki obliczania kosztów po³±czeñ lokalnych w sieci TP S.A.
# Aktualizacja: 4.03.2002
# autor: Jacek Stolarczyk <jacek@mer.chemia.polsl.gliwice.pl>
# poprawki: Dariusz Dobosz <D.Dobosz@pik-net.pl>
# aktualizacje: http://www.dobex.prv.pl
################################################################

name=TPSA
currency_symbol=PLN
currency_position=right
currency_digits=2
per_connection=0.0
minimum_costs=0.0

# Co 360 sekund dodawane bêdzie 31 groszy do rachunku.
# Jest to tak zwana taryfa nocna.
default=(0.31, 360)
# niestety w ci±gu dnia czas miêdzy impulsami jest 2x krótszy
on (monday..friday) between (8:00..21:59) use (0.31, 180)
# ¦wiêta
on (1/01, 1/05, 3/05, 1/11, 11/11, 25/12, 26/12) between () use (0.31, 360)
