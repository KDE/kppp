################################################################
# Ruleset for Romanian RomTelecom ( inclusiv TVA )
# written by Marian Radulescu <marian_r@mail.dnttm.ro>
# 1 Nov 1999 (pentru convorbiri Fix-Mobil)
################################################################
# Luni ... Samabata		Duminica (si sarb. legale)
# 07.00 - 19.00 -- 8imp./min	07.00 - 23.00 -- 8imp./min
# 19.00 - 23.00 -- 8imp./min    23.00 - 07.00 -- 8imp./min
# 23.00 - 07.00 -- 8imp./min
# Pret impuls = 500 Lei
#################################################################
name=RomTelecom_GSM
currency_symbol=Lei
currency_position=right
currency_digits=2
per_connection=0.0
minimum_costs=4000.00
default=(4000.00, 60)
on (monday..saturday) between (07:00..18:59) use (4000.00,60)
on (monday..saturday) between (19:00..22:59) use (4000.00,60)
on (monday..saturday) between (23:00..06:59) use (4000.00,60)
on (sunday) between (07:00..22:59) use (4000.00,60)
on (sunday) between (23:00..06:59) use (4000.00,60)
