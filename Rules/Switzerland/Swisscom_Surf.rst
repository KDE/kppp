# Swisscom surfing rates of 0840 Business NumbersISP, Switzerland
# Created by J. Wezel (jwezel@access.ch)
# Sun Sep 14 01:30:33 1997
# Changed by Daniel Brönnimann (dbroenni@g26.ethz.ch)
# Thu Nov 19 1998
# Valid from 1-Aug-97
# Changed by Uli Pfeiffer (uli@bluewin.ch), 29/04/2000

name=Swisscom_Surf

currency_symbol=CHF

currency_position=left
currency_digits=2
per_connection=0.0
minimum_costs=0.00
default=(0.10, 90)

# Business-Surf
on (monday..friday) between (8:00..16:00) use (0.10, 90)

# Evening-Surf
on (monday..friday) between (16:00..22:00) use (0.10, 200)

# Moonlight-Surf / Weekend-night Surf
on (monday..sunday) between (22:00..8:00) use (0.10, 514)

# Weekend-day Surf
on (saturday..sunday) between (8:00..22:00) use (0.10, 200)
on (01/01, 01/02, easter-2, easter+1) between (8:00..22:00) use(0.10, 200)
on (easter+39, easter+50, 08/01, 12/25,12/26) between (8:00..22:00) use (0.10, 200)

