################################################################
# Regu³ki obliczania kosztów po³±czeñ lokalnych w sieci TP S.A.
# Obowi±zuj± od 1 lipca 2000
# autor: Jacek Stolarczyk <jacek@mer.chemia.polsl.gliwice.pl>
# poprawki: Dariusz Dobosz <D.Dobosz@pik-net.pl>
# aktualizacje: http://www.dobex.w.pl
################################################################

name=TPSA
currency_symbol=PLN
currency_position=right
currency_digits=2
per_connection=0.0
minimum_costs=0.0

# Co 360 sekund dodawane bêdzie 35 groszy do rachunku.
# Jest to tak zwana taryfa nocna.
default=(0.35, 360)
# niestety w ci±gu dnia czas miêdzy impulsami jest 2x krótszy
on (monday..friday) between (8:00..21:59) use (0.35, 180)
# ¦wiêta
on (1/01, 1/05, 3/05, 15/08, 1/11, 11/11, 25/12, 26/12) between () use (0.35, 360)
