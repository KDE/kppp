################################################################
# Ruleset for Freenet Sorglos Tarif (Internet by Call with
# Application)
# based on the KDE Template
#   created  2002/03/07 by
#	Valentin Funk <valentin.funk@computerfabrik.de>
#
# Again the cheapest call-by-call provider in Germany
# though it requires application
################################################################

name=Freenet_Sorglos

################################################################
# currency settings
################################################################

currency_symbol=EUR
currency_position=right
currency_digits=2

################################################################
# connection settings
# 1,29 ¢ / min mit minutengenauer Abrechnung
################################################################

per_connection=0.0
minimum_costs=0.0
default=(0.0129, 60)
