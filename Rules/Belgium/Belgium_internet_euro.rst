##########################################################################
# Ruleset file for the Belgium Internet calls from Belgacom
# Thers rules apply on on may 23 2001
#
# Rules:
#  - Peak hours: 08:00 -> 18:00.
#  - Off-peak  : the rest + national holidays.
#  - Entry costs are 		2 BEF 	  = 0.0496 EUR
#  - Off-Peak: 	40BEF/hour = 0.67 BEF/min = 0.0166 EUR
#  - Peak: 	100BEF/hour= 1.67 BEF/min = 0.0414 EUR
#
# This file uses EURO as currency unit. The EURO will replace the
# Belgian Frank as of Januari 1st, 2002
#
# Philippe.Faes@rug.ac.be
# (after the summer of 2003 this addres might have expired, try one of the
# following: Philippe.Faes@faesvideo.be or Philippe.Faes@faes.net)
# 
# Thanks to Guy Zelck <guy.zelck@eds.com> for letting me use his 1998 version
# of this file as a template.
#########################################################################
# NAME OF THE RULESET. This is NEEDED for accounting purposes.
name=Belgium_internet_euro

################################################################
# currency settings
################################################################

# define BEF (Begische Frank) to be used as currency
currency_symbol=EUR

# Position of the currency symbol, default is "right".
currency_position=right 

# Define the number of significant digits.
# (not absolutely needed, default is "2"
currency_digits=4

################################################################
# connection settings
################################################################

# NOTE: rules are applied from top to bottom - the
#       LAST matching rule is the one used for the
#       cost computations.

# I did the rounding as precise as I thought was nescessary, however
# I don't know the official rates in EURO from Belgacom.
# I'm affraid this will be resolved pretty soon, cuz Belgacom always
# raises its rates anyway. 
# (I just like to say HI to all those smart people using Telenet or ADSL)

# This is charged whenever you connect. If you don't have to
# pay per-connection, use "0" here or comment it out.
per_connection=0.0496


# Minimum costs per connection. If the costs of a phone
# call are less than this value, this value is used instead
minimum_costs=0.0

# Let's take the off-peak hour rate as the default.
default=(0.0166,60)


# Peak hours.
on (monday..friday) between (08:00..18:00) use (0.0414,60)

# Holidays (mm/dd) in '98 have the off-peak rate.
on (01/01, easter, easter+1, 05/01, easter+39, easter+49,easter+50,07/21,08/15, 11/01, 11/11, 12/25) between () use (0.0166,60) 
#
# 01/01     Nieuwjaar.
# easter    Pasen
# easter+1  Paasmaandag
# 05/01     Feest van de arbeid
# easter+39 O.H.-Hemelvaart
# easter+49 Pinksteren
# easter+50 Pinkstermaandag
# 07/21     Nationale Feestdag
# 08/15     O.L.V.-Hemelvaart
# 11/01     Allerheiligen
# 11/11     Wapenstilstand 1918
# 12/25     Kerstmis
