################################################################
# Regu³y obliczania kosztów po³±czeñ modemowych w Netii dla u¿ytkowników TP S.A.
# Obowi±zuj± od 1 lipca 2000
# autor: Dariusz Dobosz <D.Dobosz@pik-net.pl>
# aktualizacje: http:/www.dobex.w.pl/
################################################################

name=Internetia
currency_symbol=PLN
currency_position=right
currency_digits=2
per_connection=0.0
minimum_costs=0.0

# Co 450 sekund bêdzie naliczany impuls o warto¶ci 33 groszy
# To znaczy miêdzy 22,00 a 8,00
default=(0.33, 450)
# poza tym to tak jak w TP SA, ale trochê taniej.
between (8:00..21:59) use (0.33, 180)
