##################Mobilcom-01019freenet Internet by call##################
#                                                                        #
# German  Mobilcom-Freenet Juni 1999                                     #
#                                                                        #
# created 99/05/25 by Olaf Doering <olaf.doering@muenster.de> and        #
#         99/06/12 by Alexander Schmidt <APSchmidt@gmx.de>               #
#                                                                        #
#    Mobilcom Angebot für Internet by call, stand vom 12.06.99           #
##########################################################################

name=Mobilcom_freenet
currency_symbol=DM 
currency_position=right 
currency_digits=2 
minimum_costs=0.00
per_connection=0.00
default=(0.05, 60) 

# Montag bis Sonntag 
on (monday..sunday) between (0:00..23:59)   use (0.05, 60) 

