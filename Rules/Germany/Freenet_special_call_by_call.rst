###############################################################
#
# German Freenet Special Call by Call accounting rules
# created 2001-10-13 Herbert Nachbagauer (herby74@freemail.de)
#
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

