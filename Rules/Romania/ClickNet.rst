##################################################################
# RomTelecom ClickNet Start (Tarife cu TVA   20 martie 2005)     #
# acces internet la 0870 222 222                                 #
# actualizat de Sorin Batariuc <sorin@bonbon.net>                #
##################################################################

# Nota:
# 1) Tarif I (normal): 60 secunde = 0.0125 Euro (fara TVA)
# 2) Tarif II (redus): 60 secunde = 0.0070 Euro (fara TVA)
#
# TVA = 19%
#

name=ClickNet Start
currency_symbol=Euro
currency_position=right
currency_digits=6
per_connection=0.00
minimum_costs=0.00833
default=(0.014875, 60)

# TARIFUL I
on (monday..friday) between (08:00..19:59) use (0.014875, 60)

# TARIFUL II
on (1/1) between () use (0.00833, 60)
on (1/2) between () use (0.00833, 60)
on (5/1) between () use (0.00833, 60)
on (12/1) between () use (0.00833, 60)
on (12/31) between () use (0.00833, 60)
on (monday..friday) between (20:00..07:59) use (0.00833, 60)
on (saturday..sunday) between (00:00..23:59) use (0.00833, 60)

