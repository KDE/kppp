################################################################
#
# This is the cost rule for Local calls in Argentina, 
# if your carrier are Telecom or Telefonica, as of Sep.2000. 
# 
# Matías Alejo García
# matiu00@yahoo.com 
################################################################

name=Argentina_local

################################################################
# currency settings
################################################################
currency_symbol=$
currency_position=left
currency_digits=2

################################################################
# connection settings
################################################################
default=(0.024, 120)

on (monday..friday)   between (08:00..19:59) use (0.012, 120)
on (saturday)         between (08:00..12:59) use (0.012, 120)

