##################################################################
# RomTelecom Interjudetean - Tarife cu TVA (8 martie 2004)       #
# Claudiu Costin <claudiuc@kde.org>                              #
# sugestii de Sorin Batariuc <sorin@bonbon.net>                  #
##################################################################

# Nota:
# 1) 1 Impuls = 0.0167 Euro (fara TVA)
#    (pentru un consum intre 0 si 10000 impulsuri lunar)
# 2) 1 Impuls = 0.0155 Euro (fara TVA)
#    (pentru un consum mai mare de 10000 impulsuri lunar)
# TVA = 19%
#
#
# Referinta: "TARIFE ROMTELECOM - ROM compiled on 2004.01.04-[1073472174].pdf"
#

name=Romtelecom Interjudetean
currency_symbol=Euro
currency_position=right
currency_digits=4
per_connection=0.00
minimum_costs=0.019873
default=(0.019873,40)

# TARIFUL I
on () between (08:00..21:59) use (0.019873,15)

# TARIFUL II
on () between (22:00..23:59) use (0.019873,20)
on () between (00:00..07:59) use (0.019873,20)

