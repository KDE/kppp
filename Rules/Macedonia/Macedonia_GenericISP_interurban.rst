##########################################################
# kppp ruleset for all Macedonian ISPs (except MTnet)
# This ruleset is for interurban connections 
# For the latest prices, call the phone number 9860
# 
# Created on 4-Jun-2001
# Last modified on 6-Jan-2003
#
# Darko Spasovski, darkos@mt.net.mk
##########################################################

name=Macedonia_GenericISP_interurban

currency_symbol=DEN
currency_position=right
currency_digits=2

per_connection=0.0
minimum_costs=0.0
default=(1.20, 30)

#############################################################
# 06:00-17:59 1.20 den. za 30 sekundi
# 18:00-05:59 1.20 den. za 1 minuta
# Vo cenite ne e presmetan DDV (+19%)
#############################################################

on () between (6:00..17:59) use (1.20,30)
on () between (18:00..5:59) use (1.20,60)
