################################################################
# Ruleset for Freenet Sorglos
# Internet by Call with Application
#
# Created 2002-03-07 by Valentin Funk <valentin.funk@computerfabrik.de>
#
# One of the cheapest call-by-call provider with application in
# Germany.
# Note: Balanced as Freenet StandardTarif until application.
################################################################

################################################################
# Name of the ruleset - absolutly requiered!
################################################################

name=Freenet_Sorglos

################################################################
# Currency settings
################################################################

currency_symbol=EUR
currency_position=right
currency_digits=2

################################################################
# Connection settings
# 1,29 ¢/Min mit minutengenauer Abrechnung
################################################################

per_connection=0.0
minimum_costs=0.0
default=(0.0129, 60)
