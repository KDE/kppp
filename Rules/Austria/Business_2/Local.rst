################################################################
#
# Austria City rate ruleset
#
# created by Mario Weilguni <mweilguni@sime.com>
# 	   
################################################################

name=Austria_Local_Area
currency_symbol=ATS
currency_position=right
currency_digits=2
default=(0.936, 158.4)

# tagestarif
on (monday..friday) between (08:00..18:00) use (0.936, 72)

# Feiertage
on (5/1,12/25,12/26,11/1,easter+1,easter+39,easter+50,1/1,10/26,1/6,12/8) between (00:00..23:59) use (0.936, 158.4)
