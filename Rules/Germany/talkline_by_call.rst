##########################################################################
#                                                                        #
# German  talknet call by call Online Rules    at  12.August 1999        #
#                                                                        #
# created  99/08/17 by olke<olke@tfz.net>   				 #
#                                                                        #
# 									 #
#                                                                        #
##########################################################################

name=talknet_by_call
currency_symbol=DM 
currency_position=right 
currency_digits=3 

default=(0.144, 180) 

# Montag bis Sonntag 
on (monday..sunday) between (0:00..23:59)   use (0.144, 180) 
