##########################################################
# kppp ruleset for all Macedonian ISPs (except MTnet)
# This ruleset is for interurban connections 
# For the latest prices, call the phone number 971
# 
# Created on 4-Jun-2001
# Last modified on 1-Jul-2004
#
# Darko Spasovski, darkos@mt.net.mk
# Georgi Stanojevski, georgi@unet.com.mk
##########################################################

name=Macedonia_GenericISP_interurban

currency_symbol=DEN
currency_position=right
currency_digits=2

per_connection=0.0
minimum_costs=0.0
default=(1.00, 20)

#############################################################
# 06:00-17:59 3.00 den. za 1 minuta (1 den za 20 sekundi)
# 18:00-05:59 1.50 den. za 1 minuta (.05 den za 20 sekundi)
# Vo cenite ne e presmetan DDV (+18%)
#############################################################

on () between (6:00..17:59) use (1.00,20)
on () between (18:00..5:59) use (0.50,20)
