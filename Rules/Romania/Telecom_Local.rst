################################################################
# Ruleset for Romanian RomTelecom (inclusiv TVA)
# written by Codre Adrian <codreadrian@softhome.net>
# 24 Mar 2001 (pentru convorbiri locale)
################################################################
name=RomTelecom_Local
currency_symbol=Lei
currency_position=right
currency_digits=2
per_connection=0.00
minimum_costs=595.00
default=(595.00, 80)
on (monday..friday) between (07:00..15:59) use (595.00,80)
on (monday..friday) between (16:00..21:59) use (595.00,120)
on (monday..sunday) between (22:00..06:59) use ( 595.00,600)
on (saturday..sunday) between (07:00..21:59) use (595.00,120)
