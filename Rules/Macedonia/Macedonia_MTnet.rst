##########################################################
# kppp ruleset for MTnet, local Macedonian ISP
# See www.mt.net.mk for price updates
# 
# Last updated on 4-Jun-2001
#
# Darko Spasovski, darkos@mt.net.mk
##########################################################

name=Macedonia_MTnet

currency_symbol=DEN
currency_position=right
currency_digits=2

per_connection=0.0
minimum_costs=0.0
default=(1.60, 60)

#############################################################
# 06:00-17:59 od ponedelnik do petok, 1.60 den. za minuta
# 18:00-23:59 od ponedelnik do petok, 0.80 den. za minuta
# 06:00-23:59 vikend i praznici, 0.80 den. za minuta
#             (za praznici se smetaat samo 1.01 i 2.05)
# 00:00-05:59 sekoj den vo nedelata, 0.35 den. za minuta
# Vo cenite ne e presmetan DDV (+18%)
#############################################################

on (monday..friday) between (6:00..17:59) use (1.60,60)
on (monday..friday) between (18:00..23:59) use (0.80,60)
on (saturday..sunday) between (6:00..23:59) use (0.80,60)
on (01/01, 05/02) between (6:00..23:59) use (0.80,60)
on () between (0:00..5:59) use (0.35,60)
