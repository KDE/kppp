##################################################################
# RomTelecom A.S.I. - Tarife cu TVA  (8 martie 2004)             #
# Claudiu Costin <claudiuc@kde.org>                              #
# actualizari de la Sorin Batariuc <sorin@bonbon.net>            #
##################################################################

# Nota:
# 1) Tarif I (normal): 60 secunde = 0.0095 Euro (fara TVA)
# 2) Tarif II (redus): 60 secunde = 0.0045 Euro (fara TVA)
# 3) Tariful este valabil de la 1 Iunie 2004
#
# TVA = 19%
#
#
# Referinta: "Anexa II Apeluri de la abonat in centrale automate cu Call Colection si Acces Internet.doc"
#

name=Romtelecom Acces Special Internet
currency_symbol=Euro
currency_position=right
currency_digits=6
per_connection=0.00
minimum_costs=0.005355
default=(0.011305, 60)

# TARIFUL I
on (monday..friday) between (08:00..19:59) use (0.011305, 60)

# TARIFUL II
on (1/1) between () use (0.005355, 60)
on (1/2) between () use (0.005355, 60)
on (5/1) between () use (0.005355, 60)
on (12/1) between () use (0.005355, 60)
on (12/31) between () use (0.005355, 60)
on (monday..friday) between (20:00..07:59) use (0.005355, 60)
on (saturday..sunday) between (00:00..23:59) use (0.005355, 60)

