################################################################
# Ruleset for Romanian RomTelecom (inclusiv TVA)
# written by Codre Adrian <codreadrian@softhome.net>
# << actualizat la 13 Noiembrie 2001 (pentru convorbiri locale) >>
################################################################
name=Telecom_Local
currency_symbol=Lei
currency_position=right
currency_digits=2
per_connection=0.00
minimum_costs=761.6
default=(761.6, 160)
on (monday..friday) between (07:00..15:59) use (761.6,160)
on (monday..friday) between (16:00..21:59) use (761.6,240)
on (monday..sunday) between (22:00..06:59) use (761.6,600)
on (saturday..sunday) between (07:00..21:59) use (761.6,480)
