################################################################
# Danish Mobilix Wanadoo internet rate ruleset per 2000.1.1
#
# by Jens Svalgaard Frederiksen <svalle@imada.sdu.dk>
################################################################

name=Mobilix_Wanadoo
currency_symbol=DKK
currency_position=right 
currency_digits=2

################################################################
# connection settings
################################################################

per_connection=0.25

# Cheap rate:
default=(0.010, 6)
default=(0.008, 6, 3600)
# Note: For Night rate at 0.07 DKK/minute look at the end

# Weekday rates:
on (monday..friday) between (08:00..16:00) use (0.027, 6)
#on (monday..friday) between (08:00..16:00) use (0.027, 6)
#on (monday..friday) between (16:00..19:00) use (0.018, 6)

# Saturday rates:
on (saturday)       between (08:00..19:00) use (0.018, 6)

# Sunday rates:
# (same as cheap rate)

# National holidays:

# Newyear (nytårsdag)
on (01/01)     between () use (0.010, 6)
on (01/01)     between () use (0.008, 6, 3600)
# Easter (påske)
on (easter -3) between () use (0.010, 6)
on (easter -3) between () use (0.008, 6, 3600)
on (easter -2) between () use (0.010, 6)
on (easter -2) between () use (0.008, 6, 3600)
on (easter +1) between () use (0.010, 6)
on (easter +1) between () use (0.008, 6, 3600)
# Store bededag
on (easter+26) between () use (0.010, 6)
on (easter+26) between () use (0.008, 6, 3600)
# Kristi himmelfart
on (easter+39) between () use (0.010, 6)
on (easter+39) between () use (0.008, 6, 3600)
# Pinse
on (easter+50) between () use (0.010, 6)
on (easter+50) between () use (0.008, 6, 3600)
# Christmas
on (12/25)     between () use (0.010, 6)
on (12/25)     between () use (0.008, 6, 3600)
on (12/26)     between () use (0.010, 6)
on (12/26)     between () use (0.008, 6, 3600)

# Night rate
on ()          between (00:00..08:00) use (0.007, 6)
