################################################################
# Ruleset for Freenet StandardTarif
# Internet by Call _without_ Application
#
# Created 2002-04-29 by Valentin Funk <valentin.funk@computerfabrik.de>
#
# An ordinary german internet call-by-call rate. Also used as
# fallback rate until application for other Freenet rates.
################################################################

################################################################
# Name of the ruleset - absolutly requiered!
################################################################

name=Freenet_StandardTarif

################################################################
# Currency settings
################################################################

currency_symbol=EUR
currency_position=right
currency_digits=2

################################################################
# Connection settings
# 2,505 ¢/Min mit minutengenauer Abrechnung
################################################################

per_connection=0.0
minimum_costs=0.0
default=(0.02505, 60)
