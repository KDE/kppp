################################################################
# Ruleset for Romanian RomTelecom (inclusiv TVA )
# written by Marian Radulescu <marian_r@mail.dnttm.ro>
# 1 Nov 1999 (pentru convorbiri Locale)
################################################################
# Luni ... Sambata	         Duminica (si sarb. legale)
# 07.00 - 19.00  -- 1imp./ 1min  07.00 - 23.00  -- 1imp./ 2min
# 19.00 - 23.00  -- 1imp./ 2min  23.00 - 07.00  -- 1imp./10min
# 23.00 - 07.00  -- 1imp./10min
# Pret impuls = 500 lei
################################################################
name=RomTelecom_Local
currency_symbol=Lei
currency_position=right
currency_digits=2
per_connection=0.00
minimum_costs=500.00
default=(500.00, 60)
on (monday..saturday) between (07:00..18:59) use (500.00,60)
on (monday..saturday) between (19:00..22:59) use (250.00,60)
on (monday..saturday) between (23:00..06:59) use ( 50.00,60)
on (sunday) between (07:00..22:59) use (250.00,60)
on (sunday) between (23:00..06:59) use ( 50.00,60)
