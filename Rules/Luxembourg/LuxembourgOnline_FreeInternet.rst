################################################################
#
# KPPP accounting rules for "Free Internet" by Luxembourg Online
# (www.internet.lu; dialup number 27300030)
#
# For this ISP you'll only have to pay the costs of a local phone
# call.
# The information is taken from the current (as of 11.02.2005)
# official price list of the "Entreprise des Postes et
# Télécommunications Luxembourg". (Taxes are included.)
#
# 11.02.2005
# Gilles Schintgen <gilles@vonet.lu>
#
# ##############################################################

################################################################
# name of the ruleset
################################################################
name=LuxembourgOnline_FreeInternet

################################################################
# currency settings
################################################################
currency_symbol=€
currency_position=right
currency_digits=2

################################################################
# connection settings
################################################################

# NOTE: rules are applied from top to bottom - the
#       LAST matching rule is the one used for the
#       cost computations.

# This is charged whenever you connect. If you don't have to
# pay per-connection, use "0" here or comment it out.
# per_connection=0.0

# Minimum costs per connection. If the costs of a phone
# call are less than this value, this value is used instead
# minimum_costs=0.0

# This is what you pay for the first unit.
# flat_init_costs=(0.0, 0)

# This is the default rule which is used when no other rule
# applies. One unit (60 seconds) accounts for 0.0309 €
# (= 1,25 LUF)
default=(0.0309, 60)

# Normal Costs
on (monday..friday) between (00:00..07:59) use (0.0154, 60)
on (monday..friday) between (08:00..18:59) use (0.0309, 60)
on (monday..friday) between (19:00..23:59) use (0.0154, 60)
on (saturday..sunday) between () use (0.0154, 60)

# Legal Holidays
# these days are billed just as if they were sundays
on (01/01, easter+1, 05/01, easter+39, easter+50) between () use (0.0154, 60)
on (06/23, 08/15, 11/01, 12/25) between () use (0.0154, 60)

# 01/01:     Nouvel An (New Year)
# easter+1:  Lundi de Pâques (Easter Monday)
# 05/01:     Fête du travail (Labor Day)
# easter+39: Ascension
# easter+50: Lundi de Pentecôte (Whit Monday)
# 06/23:     Fête nationale (national holiday)
# 08/15:     Assomption (Assumption)
# 11/01:     Toussaint (All Saint's Day)
# 12/25:     Noël (Christmas)
