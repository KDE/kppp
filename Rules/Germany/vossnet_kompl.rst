##########################################################################
#                                                                        #
# German  vossnet-Komplett Online Rules    at  11.October 1999           #
#                                                                        #
# created  99/10/11 by olke<olke@tfz.net>   				 #
#                                                                        #
# der Tarif, welchen man mit Strom- und/oder Telefon-Vertrag bekommt	 #
#                                                                        #
##########################################################################

name=vossnet-komplett
currency_symbol=DM 
currency_position=right 
currency_digits=2 
default=(0.12, 90) 

# Montag bis Freitag 
on (monday..friday) between (9:00..18:00)   use (0.12, 90) 
on (monday..friday) between (18:00..21:00)   use (0.12, 150)
on (monday..friday) between (21:00..5:00)   use (0.12, 240)

# Wochenende
on (saturday..sunday) between (21:00..5:00)   use (0.12, 240) 
on (saturday..sunday) between (5:00..21:00)   use (0.12, 150) 

# Feiertage  ein Dankeschön an Joachim Wesner,Stefan Markgraf,Stefan Troeger für die Feiertage
on (01/01, easter-2, easter, easter+1, 05/01, easter+39, easter+49, easter+50, 10/03, 12/24, 12/25, 12/26, 12/31) between (21:00..5:00) use (0.12, 240)
on (01/01, easter-2, easter, easter+1, 05/01, easter+39, easter+49, easter+50, 10/03, 12/24, 12/25, 12/26, 12/31) between (5:00..21:00) use (0.12, 150)

