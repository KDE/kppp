#####################################################################
#GREECE EPAK CONECTION!
#With this connection you pay less
#People with digital phone have to use it!
#Othrwise you'll get broke!
#note that ISP's EPAK number MUST startx with 0965-xxx-xxx
#otherwise the do not have EPAK charging
#
#Done by Broxopoios 04 November 1999 and updated (see charges!)
#The latest charges are included (29/11/99)!
#For new charges look at http://www.ote.gr
#E-mail me:
#root@sound.gr  
#http://broxopoios.cjb.net
#####################################################################

name=EPAK
currency_symbol=Drx
currency_position=right 
currency_digits=2
per_connection=8.26

# Each Unit costs 7drx+18%vat=7.08drx
# Every 210 sec 1 Unit is added to the bill
# Monday-Saturdays (22:00-08:00) and Sundays the cost drops to 1 Unit per 210sex 

# default : Cost of  seconds in the time periods(Monday..Saturday,8:00..22:00)   
default=(8.26, 210)

# (other time periods / Weekend)
on (monday..saturday)   between (00:00..08:00) use (8.26, 420)
on (monday..saturday) between (22:00..23:59) use (8.26, 420)
on (sunday) between (00:00..23:59) use (8.26, 420)


