################################################################
# Ruleset for Freenet Super CbC
# Internet by Call _without_ Application
#
# Created 2002-04-29 by Valentin Funk <valentin.funk@computerfabrik.de>
#
# One of the cheapest internet call-by-call rates.
# Notes:
# Once, you have to use a special dial in number: 019231760!
# Twice, you could be stressed by additional HTML code for
# advertisement.
################################################################

################################################################
# Name of the ruleset - absolutly requiered!
################################################################

name=Freenet_Super_CbC

################################################################
# Currency settings
################################################################

currency_symbol=EUR
currency_position=right
currency_digits=2

################################################################
# Connection settings
# 1,45 ¢/Min 8-18 Uhr
# 0,89 ¢/Min 18-8 Uhr
################################################################

per_connection=0.0
minimum_costs=0.0

default=(0.0145, 60)

on () between (08:00..18:00) use (0.0145, 60)
on () between (18:00..08:00) use (0.0089, 60)
