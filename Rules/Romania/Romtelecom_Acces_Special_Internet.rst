##################################################################
# RomTelecom A.S.I. - Tarife fara TVA (18 ianuarie 2004)         #
# Claudiu Costin <claudiuc@kde.org>                              #
##################################################################

# Nota:
# 1) 1 Impuls = 0.0167 Euro
#    (pentru un consum intre 0 si 10000 impulsuri lunar)
# 2) 1 Impuls = 0.0155 Euro
#    (pentru un consum mai mare de 10000 impulsuri lunar)
#
#
# Referinta: "TARIFE ROMTELECOM - ROM compiled on 2004.01.04-[1073472174].pdf"
#


name=Romtelecom Acces Special Internet
currency_symbol=Euro
currency_position=right
currency_digits=4
per_connection=0.00
minimum_costs=0.008
default=(0.008,60)

# TARIFUL I
on () between (08:00..21:59) use (0.008,60)

# TARIFUL II
on () between (22:00..23:59) use (0.003,60)
on () between (00:00..07:59) use (0.003,60)

