######################################################################
#                                                                    #
# German Compuserve Pro		Online Rules	at  05.December 2001 #
#                                                                    #
# created 01/12/05 by Timo <timo@userland.de>                        #
######################################################################

name=Compuserve Pro
currency_symbol=EUR
currency_position=right 
currency_digits=2
default=(0.009, 60)

# Täglich von 18:00 bis 08:59
on (monday..sunday) between (18:00..08:59) use (0.014, 60)

