############################################################
# RomTelecom - Tarife care includ TVA (azi, 9 august 2002) #
# Scrise de RumburaK - nu te mint da nu te lua dupa mine   #
# Injura-ma la: rumburak@rol.ro                            #
# Doamnelor si domnisoarelor, va astept cu bezele!         #
############################################################
name=RomTelecom
currency_symbol=Lei
currency_position=right
currency_digits=2
per_connection=0.00
minimum_costs=702.10
default=(702.10,160)
on (monday..friday) between (07:00..15:59) use (702.10,160)
on (monday..friday) between (16:00..21:59) use (702.10,240)
on (monday..sunday) between (22:00..06:59) use (702.10,600)
on (saturday..sunday) between (07:00..21:59) use (702.10,480)
