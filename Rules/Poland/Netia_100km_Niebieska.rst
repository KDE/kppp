################################################################
# Regułki obliczania kosztów połączeń międzymiastowych w sieci Netia,
# na odległość do 100 km między centralami wojewódzkimi.
# Taryfa niebieska (o tańszym abonamencie)
# autor: Jacek Stolarczyk <jacek@mer.chemia.polsl.gliwice.pl>
################################################################

name=Netia_100km_Niebieska
currency_symbol=PLN
currency_position=right
currency_digits=2
per_connection=0.0
minimum_costs=0.0
default=(0.59, 60)

# taryfa do 100km
on (monday..friday)  between (8:00..18:00)  use (0.59, 60)
on (monday..friday)  between (18:00..22:00)  use (0.44, 60)
on (monday..friday)  between (22:00..8:00)  use (0.29, 60)
on (saturday..sunday)  between (8:00..22:00)  use (0.44, 60)
on (saturday..sunday)  between (22:00..8:00)  use (0.29, 60)

# Święta (taryfa sobotnio-niedzielna)
on (01/01, easter, easter+1, 05/01, 05/03, 08/15, 11/01, 11/11, 12/25, 12/26) between (8:00..22:00) use (0.44, 60)
on (01/01, easter, easter+1, 05/01, 05/03, 08/15, 11/01, 11/11, 12/25, 12/26) between (22:00..8:00) use (0.29, 60)

#objaśnienia dni świątecznych
# 01/01		Nowy Rok
# easter		Niedziela Wielkanocna
# easter+1	Poniedziałek Wielkanocny
# 05/01		Święto Pracy
# 05/03		Dzień Konstytucji 3 Maja
# 08/15		Wniebowzięcie
# 11/01		Wszystkich Świętych
# 11/11		Święto Niepodległości
# 12/25		Boże Narodzenie
# 12/26		Boże Narodzenie
# niestety na liście brakuje Bożego Ciała - nie wiem jaka reguła rządzi jego
# wyborem tego czwartku

