################################################################
# Regu�y obliczania koszt�w po��cze� modemowych w Netii dla u�ytkownik�w TP S.A.
# Obowi�zuj� od 1 lipca 2000
# autor: Dariusz Dobosz <D.Dobosz@pik-net.pl>
# aktualizacje: http:/www.dobex.w.pl/
################################################################

name=Internetia
currency_symbol=PLN
currency_position=right
currency_digits=2
per_connection=0.0
minimum_costs=0.0

# Co 450 sekund b�dzie naliczany impuls o warto�ci 33 groszy
# To znaczy mi�dzy 22,00 a 8,00
default=(0.33, 450)
# poza tym to tak jak w TP SA, ale troch� taniej.
between (8:00..21:59) use (0.33, 180)
