################################################################
# LOKALNI SAOBRACAJ
# Obuhvata razgovore obavljene u okviru mesne mre¾e.
# Ovi razgovori tarifiraju se impulsnim intervalom od 2 minuta
# u jakom saobraæaju i impulsnim intervalom od 4 minuta u period
# u slabog saobraæaja.
#
# JAK SAOBRACAJ
# Podrazumeva razgovore obavljene u vremenskom periodu
# od 07-15 h i od 17-21 h
#
# SLAB SAOBRACAJ
# * Podrazumeva razgovore obavljene u vremenskom periodu od
#  15-17 h i od 21-07 h narednog dana
# * Razgovore obavljene vikendom:
#   od subote u 21 h do ponedeljka do 07 h
# * Razgovore obavljene u dane dr¾avnih praznika
#
# CENA IMPULSA
# za fizièka lica iznosi 0,3644 din
# za pravna lica iznosi 0,6023 din.
#
################################################################
#
# NAME OF THE RULESET. This is NEEDED for accounting purposes.
#
################################################################
name=Lokalni_poziv.rst

################################################################
# currency settings
################################################################

currency_symbol=DIN
currency_position=right 
currency_digits=2

################################################################
# connection settings
################################################################

per_connection=0.0
minimum_costs=0.0

# You pay .74 for the first 180 secons ( 3minutes) no matter
# whether you are connected for 1 second or 180 seconds.
# This rule will take priority during the first 180 seconds
# over any other rule, in particular the 'default' rule.
# have a look at costgraphs.gif in the docs directory
# of the kppp distribution for a graphic illustration.

flat_init_costs=(0.3644,120)

# This is the default rule which is used when no other rule
# applies. The first component "0.1" is the price of one
# "unit", while "72" is the duration in seconds.
# Therefore the following rule means: "Every 72 seconds 0.1 
# ATS are added to the bill"

default=(0.3644, 120)

# more complicated rules:

# "on monday until sunday from 12:00 am until 11:59 pm the costs
# are 0.2 each 72 seconds"
#on () between () use (0.2, 2)

on (monday..friday) between (0:00..7:00) use (0.3644, 240)
on (monday..friday) between (15:00..16:17) use (0.3644, 240)
on (monday..friday) between (21:00..23:59) use (0.3644, 240)
on (saturday) between (21:00..23:59) use (0.3644, 240)
on (sunday) between () use (0.3644, 240)

# Praznici
# 1.1, 2.1	Nova Godina
# 7.1. 		Bozic 
# 27.3.		SRJ
# 28.4.		Srbija
# 1.5, 2.5	Prvi Maj
on (01.01, 02.01, 07.01, 27.03, 28.4, 01.05, 02.05) between () use (0.3644, 240)
