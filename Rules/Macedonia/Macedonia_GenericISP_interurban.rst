##########################################################
# kppp ruleset for all Macedonian ISPs (except MTnet)
# This ruleset is for interurban connections 
# 
# Created on 4-Jun-2001
# Darko Spasovski, darkos@mt.net.mk
##########################################################

name=Macedonia_GenericISP_interurban

currency_symbol=DEN
currency_position=right
currency_digits=2

per_connection=0.0
minimum_costs=0.0
default=(0.80, 30)

#############################################################
# 06:00-17:59 0.80 den. za 30 sekundi
# 18:00-05:59 0.80 den. za 1 minuta
# Vo cenite ne e presmetan DDV (+19%)
#############################################################

on () between (6:00..17:59) use (0.80,30)
on () between (18:00..5:59) use (0.80,60)
