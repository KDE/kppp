#=========================================================================
# Planet Interkom by call
# http://www.planet-interkom.de/planet/access/access_index.asp
#
# Authentifikation: chap
# Username: internet@planetbycall.de
# Passwort: internet
# in /etc/ppp/chap-secrets eintragen: internet@planetbycall.de	*	internet
# 
# 05-Jul-2000 numbermumbler <numbermumbler@i.am> http://i.am/numbermumbler
# 16-Aug-2000 tages-tarif korrigiert; zugangsdaten geaendert
#=========================================================================
name=Planet Interkom by call
currency_position=right
currency_symbol=DM
currency_digits=2
per_connection=0.0
minimum_costs=0.0
default=(0.00041666666667, 1)
on (monday..friday) between (18:00..07:59) use (0.00041666666667, 1)
on (monday..friday) between (08:00..17:59) use (0.00053333333333, 1)
on (easter-2, easter, easter+1, easter+39, easter+49, easter+50) between (0:00..23:59) use (0.00041666666667, 1)
on (01/01, 05/01, 10/03, 12/24, 12/25, 12/26, 12/31) between (0:00..23:59) use (0.00041666666667, 1)
