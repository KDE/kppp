##########################################################
# kppp ruleset for all Macedonian ISPs (except MTnet)
# This ruleset is for local connections
# 
# Created on 4-Jun-2001
# Darko Spasovski, darkos@mt.net.mk
##########################################################

name=Macedonia_GenericISP_local

currency_symbol=DEN
currency_position=right
currency_digits=2

per_connection=0.0
minimum_costs=0.0
default=(0.80, 180)

#############################################################
# 06:00-17:59 0.80 den. za 3 minuti
# 18:00-05:59 0.80 den. za 6 minuti
# Vo cenite ne e presmetan DDV (+19%)
#############################################################

on () between (6:00..17:59) use (0.80,180)
on () between (18:00..5:59) use (0.80,360)
