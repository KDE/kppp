################################################################
# Regu³ki obliczania kosztów po³±czeñ lokalnych w sieci Netia
# Taryfa niebieska (abonament 24.40PLN)
# Napisane na podstawie http://www.netia.pl/dom/taryfa.html
# autor: Jacek Stolarczyk <jacek@mer.chemia.polsl.gliwice.pl>
################################################################

name=Netia_Lokalne_Niebieska
currency_symbol=PLN
currency_position=right
currency_digits=2
per_connection=0.0
minimum_costs=0.0

# Co 360 sekund dodawane bedzie 33 groszy do rachunku.
# Jest to taryfa nocna
default=(0.33, 360)
# niestety w ci±gu dnia czas miêdzy impulsami jest 2x krótszy
on (monday..sunday) between (8:00..21:59) use (0.33, 180)

