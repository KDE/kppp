################################################################
# 21 Jul 2003 (Please change when TT increases the costs)
#
# I take no responsibility for the validity and accuracy of
# this information, if you are so concerned please contact
# your own telephone provider and configure this yourself
# and do not use this file.
################################################################
# Bu dosya sadece 0822'li hatlar icin gecerlidir.
#
# Lutfen TT zam yaptiginda use(xxxxx,yyy) ibarelerindeki xxxxx'i
# yeni kontur ucreti ile degistirin.
# Bu dosyanin nasil hazirlandigini ogrenmek istiyorsaniz
# /usr/share/apps/kppp/rules dizinindeki TEMPLATE dosyasini
# inceleyin.
#
#
# Mesut Sismanoglu
# mesuts@usa.net
#
# İsmail Dönmez
# ismail.donmez@boun.edu.tr
#
################################################################
################################################################
#
# NAME OF THE RULESET. This is NEEDED for accounting purposes.
#
################################################################
name=Turk_Telekom_Internet
################################################################
# currency settings
################################################################
# defines TL (Turkish Lira) to be used as currency
# symbol (not absolutely needed, default = "$")
currency_symbol=TL

# Define the position of the currency symbol.
# (not absolutely needed, default is "right")
currency_position=right 

# Define the number of significant digits.
# (not absolutely needed, default is "2"
currency_digits=0

# This is the default rule which is used when no other rule
# applies.
default=(72000, 324)
################################################################
# connection settings
################################################################

# Hafta arasi 1. indirim
on(monday..friday) between (7:00..7:59) use(72000,309)

 # Hafta arasi normal tarife
on(monday..friday) between (8:00..19:59) use(72000,216)

 # Hafta arasi 1. indirim
on(monday..friday) between (20:00..22:29) use(72000,309)

 # Hafta arasi 2. indirim
on(monday..friday) between (22:30..6:59) use(72000,360)

 # Cumartesi 1. indirim
on(saturday) between (7:00..22:29) use(72000,309)

 # Cumartesi 2. indirim
on(saturday) between (22:30..6:59) use(72000,360)

 # Pazar ve resmi tatil gunleri 2. indirim
on(1/1, 04/23, 05/19, 08/30, 10/29, sunday) between () use(72000,360)

################################################################
