#####################################################################
#GREECE LOCAL DIGITAL CONECTION!
#With this connection you pay more than EPAK
#Only use it when your ISP does not support EPAK charging or
#when you have your phone blocked not to call zero(0)-beggining nums.
#
#Done by Broxopoios 04 November 1999
#The latest charges are included!
#For new charges look at http://www.ote.gr
#E-mail me
#root@sound.gr  
#http://broxopoios.cjb.net
#####################################################################

name=digital
currency_symbol=Drx
currency_position=right 
currency_digits=2
per_connection=8.26


# Each Unit costs 7drx+18%vat=8.26drx
# Every 1 min 1 Unit is added to the bill
# Monday-Saturdays (22:00-08:00) and Sundays the cost drops to 1 Unit per 2mins

# default : Cost of  seconds in the time periods (Monday..Saturday,8:00..22:00) 
default=(8.26, 60)

# (other time periods / Weekend)
on (monday..saturday) between (00:00..08:00) use (8.26, 120)
on (monday..saturday) between (22:00..23:59) use (8.26, 120)
on (sunday) between (00:00..23:59) use (8.26, 120)


