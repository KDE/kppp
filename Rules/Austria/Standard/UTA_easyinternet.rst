############################################################################
#									   #
#	Ruleset for UTA Easyinternet 					   #
#	(also applyable for the UTA Internet Unlimited, but I'm an	   #
#	Easyinternet customer = no basic fee ;-)) 			   #
#	Created on August 26, 2000 according to the latest UTA tarifs      #
#	by Patrick Eixelsberger (pat.eix@utanet.at)			   #
#									   #
#	(All following comments are German, because IMHO only Austrians    #
#	will be interested in this file and I'm too lazy for English 	   #
#	now ;-) )							   #
#									   #
############################################################################


name=UTA Easyinternet

# Währungssymbol= ATS für österreichische Schillinge
# Bitte ändern, wenn der EURO da ist ;-)
currency_symbol=ATS

currency_position=right 

currency_digits=2

per_connection=0.0

minimum_costs=0.0


# = Geschäftstarif: ATS 0.33 pro Minute, aber mit sekundengenauer
# Abrechnung, daher wird jede Sekunde um ATS 0.0055 weitergezählt
default=(0.0055, 1)


# = Freizeittarif: ATS 0.16 pro Minute, aber mit sekundengenauer Abrechnung,
# daher wird jede Sekunde um ATS 0.002667 weitergezählt
# Freizeit = Montag-Freitag 18:00-8:00, 
#            Samstag 0:00-23:59
#	     Sonntag 0:00-23:59
on (monday..friday) between (18:00..8:00) use (0.002667, 1)
on (saturday..sunday) between (0:00..23:59) use (0.002667, 1)

# Feiertage, übernommen aus 
# $KDEDIR/share/apps/kppp/Rules/Austria/Standard/Online.rst
# (von Mario Weilguni <mweilguni@sime.com>)
on (5/1,12/25,12/26,11/1,easter+1,easter+39,easter+50,1/1,10/26,1/6,12/8) between (00:00..23:59) use (0.002667, 1)

# = wiederum Freizeittarif, siehe oben

