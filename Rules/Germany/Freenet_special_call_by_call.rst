

################################################################
#
# NAME OF THE RULESET. This is NEEDED for accounting purposes.
#
################################################################
name=Freenet_special_call_by_call

################################################################
# currency settings
################################################################

currency_symbol=DM

currency_position=right

currency_digits=2



################################################################
# connection settings
################################################################

# default

default=(0.029,60)


on () between (0:00..9:00) use (0.017,60)
on () between (9:00..18:00) use (0.029,60)
on () between (18:00..0:00) use (0.017,60)

###########################################################################
# please send any suggestions or corrections to friedemann.schorer@gmx.de #
# these rules are valid as of Aug 15th, 2001                              #
###########################################################################
