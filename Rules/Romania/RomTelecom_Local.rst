##################################################################
# Ruleset for Romanian RomTelecom (inclusiv TVA)                 #
# written by Codre Adrian <codreadrian@softhome.net>             #
# << actualizat la 1 Noiembrie 2002 (pentru convorbiri locale) >>#
##################################################################

name=Telecom_Local
currency_symbol=$
currency_position=left
currency_digits=5

on (monday..sunday) between (07:00..21:59) use (0.01071,60)
on (monday..sunday) between (22:00..06:59) use (0.00357,60)
