################################################################
# Regu³ki obliczania kosztów po³±czeñ lokalnych w sieci Netia
# Taryfa zielona (abonament 28.06PLN)
# Napisane na podstawie http://www.netia.pl/dom/taryfa.html
# autor: Jacek Stolarczyk <jacek@mer.chemia.polsl.gliwice.pl>
################################################################

name=Netia_Lokalne_Zielona
currency_symbol=PLN
currency_position=right
currency_digits=2
per_connection=0.0
minimum_costs=0.0

# Co 180 sekund dodawane bêd± 32 grosze do rachunku.
# Jest to taryfa nocna
default=(0.32, 360)
# niestety w ci±gu dnia czas miêdzy impulsami jest 2x krótszy
on (monday..sunday) between (8:00..21:59) use (0.32, 180)


