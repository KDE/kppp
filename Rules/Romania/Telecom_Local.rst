################################################################
# Ruleset for Romanian RomTelecom (inclusiv TVA)
# written by Codre Adrian <codreadrian@softhome.net>
# ultima modificare a fost facuta la 26 August 2001
################################################################
name=Telecom_Local
currency_symbol=Lei
currency_position=right
currency_digits=2
per_connection=0.00
minimum_costs=708.05
default=(708.05, 80)
on (monday..friday) between (07:00..15:59) use (708.05,80)
on (monday..friday) between (16:00..21:59) use (708.05,120)
on (monday..sunday) between (22:00..06:59) use (708.05,600)
on (saturday..sunday) between (07:00..21:59) use (708.05,240)
