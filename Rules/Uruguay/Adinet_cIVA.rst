################################################################
#
# Disclaimer/License
# This Template ist (c) by Mario Weilguni <mweilguni@kde.org>
# It ist licenced under the same terms as the kppp package, 
# which it is part of
#
################################################################
#
# Cost rules for Adinet including VAT
#
# Otto Duarte
# duvoel@adinet.com.uy
################################################################

################################################################
name=Adinet_cIVA

################################################################
# currency settings
################################################################

currency_symbol=$
currency_position=right
currency_digits=2

################################################################
default=(0.8612, 60)

# tarifa reducida
on (monday..friday) between (00:00..08:59) use (0.8612, 100)
on (monday..friday) between (21:00..23:59) use (0.8612, 100)
on (saturday..sunday) between (00:00..23:59) use (0.8612, 100)

# feriados
on (1/1) between (00:00..23:59) use (0.8612, 100)
on (1/5) between (00:00..23:59) use (0.8612, 100)
on (7/8) between (00:00..23:59) use (0.8612, 100)
on (8/25) between (00:00..23:59) use (0.8612, 100)
on (12/25) between (00:00..23:59) use (0.8612, 100)
