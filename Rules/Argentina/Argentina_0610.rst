################################################################
#
# This is the cost rule for Internet (0610) calls in Argentina, 
# if your carrier are Telecom or Telefonica, as of Sep.2000. 
# 
# Matías Alejo García
# matiu00@yahoo.com 
################################################################

name=Argentina_0610

################################################################
# currency settings
################################################################
currency_symbol=$
currency_position=left
currency_digits=2

################################################################
# connection settings
################################################################
default=(0.007865, 60)

on (monday..friday)   between (08:00..19:59) use (0.01573, 60)
on (saturday)         between (08:00..12:59) use (0.01573, 60)

