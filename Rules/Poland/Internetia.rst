################################################################
# Reguły obliczania kosztów połączeń modemowych w Netii dla użytkowników TP S.A.
# Aktualizacja: 4.03.2002
# autor: Dariusz Dobosz <D.Dobosz@pik-net.pl>
# aktualizacje: http:/www.dobex.prv.pl/
################################################################

name=Internetia
currency_symbol=PLN
currency_position=right
currency_digits=2
per_connection=0.0
minimum_costs=0.0

# Co 360 sekund będzie naliczany impuls o wartości 25 grosze.
# To znaczy między 22,00 a 8,00
default=(0.25, 360)
# poza tym co 180 sekund.
on (monday..friday) between (8:00..21:59) use (0.25, 180)
