

################################################################
#
# NAME OF THE RULESET. This is NEEDED for accounting purposes.
#
################################################################
name=Telekom_City

################################################################
# currency settings
################################################################

currency_symbol=DM

currency_position=right

currency_digits=2



################################################################
# connection settings
################################################################

# default - sicherheitshalber die kürzeste Zeit, damit keine
#       böse Überraschung passiert ...

default=(0.12,90)


# Arbeitstage

on (monday..friday) between (0:00..5:00) use (0.12,240)
on (monday..friday) between (5:00..9:00) use (0.12,150)
on (monday..friday) between (9:00..18:00) use (0.12,90)
on (monday..friday) between (18:00..21:00) use (0.12,150)
on (monday..friday) between (21:00..0:00) use (0.12,240)

# Wochenende

on (saturday..sunday) between (0:00..5:00) use (0.12,240)
on (saturday..sunday) between (5:00..21:00) use (0.12,150)
on (saturday..sunday) between (21:00..0:00) use (0.12,240)

# Feiertage : Weihnachten, Tag der Deutschen Einheit, Ostern, Karfreitag, Himmelfahrt, Pfingsten, 1. Mai)

on (12/25..12/26, 10/03, easter, easter+1, easter-2, easter+39, easter+50, 05/01) between (0:00..5:00) use (0.12,240)
on (12/25..12/26, 10/03, easter, easter+1, easter-2, easter+39, easter+50, 05/01) between (5:00..21:00) use (0.12,150)
on (12/25..12/26, 10/03, easter, easter+1, easter-2, easter+39, easter+50, 05/01) between (21:00..0:00) use (0.12,240)



###########################################################################
# please send any suggestions or corrections to friedemann.schorer@gmx.de #
#              these rules are valid as of Aug. 15th, 2001                #
###########################################################################
