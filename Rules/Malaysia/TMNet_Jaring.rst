################################################################
# This is the cost rule for local calls in Malaysia.
# This cost rule include Dial-Up charge from Telekom and 
# Access charges from both Jaring or TMnet
#
# Paul Tan
# root@vicert.eu.org | petra@shellyeah.org
################################################################


################################################################
#
# NAME OF THE RULESET. This is NEEDED for accounting purposes.
#
################################################################
name=Malaysia_TelekomISP

################################################################
# currency settings
################################################################

currency_symbol=RM
currency_position=left 
currency_digits=2

################################################################
# connection settings
################################################################

per_connection=0.025
minimum_costs=0.025
flat_init_costs=(0.025,60)
default=(0.025, 60)
