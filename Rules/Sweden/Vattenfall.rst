##############################################################
# Swedish rate ruleset for calls with Vattenfall
# Created 2000-Aug-01 by Fredrik Ismyren  <myrn@despammed.com>
# Updated 2000-Aug-30 by Fredrik Ismyren
# May be distributed freely. I take no responsibility for
# the correctness of the information in this file.
##############################################################

name=Vattenfall_Nationellt
currency_symbol=SEK
currency_position=right 
currency_digits=2

# Så här beräknar Telia samtalskostnaden (enligt vad jag förstår...)
#
# * en öppningsavgift debiteras för alla samtal så fort man fått svar
# * därefter beräknas kostnaden per sekund
#
# Kostnaden per sekund beror av tiden:
# * hög taxa vardagar kl 08:00-18:00
# * låg taxa övrig tid
#
# Som vardagar räknas måndag till fredag. Dock ej:
# * röda dagar i almanackan
# * Vissa övriga dagar. Vilka dessa är verkar något oklart, och kan kanske
#   variera från år till år efter Telias tycke och smak. Men en tumregel är
#   att "aftnar" typ julafton och nyårsafton inte räknas som vardagar.


# Öppningsavgift: 45 öre
per_connection=0.45
minimum_costs=0.0

# Hög taxa: 19,5 öre/min måndag-fredag 8-18
on (monday..friday) between (8:00..17:59) use (0.00325, 1)

# Låg taxa: 11 öre/min (räknat per sekund)
default=(0.0018333333333, 1)

# Röda helgdagar (de som infaller på en lördag/söndag utelämnade)
# nyårsdagen
on (01/01) between () use (0.0018333333333, 1)
# trettondag jul
on (01/06) between () use (0.0018333333333, 1)
# långfredagen
on (easter-2) between () use (0.0018333333333, 1)
# annandag påsk
on (easter+1) between () use (0.0018333333333, 1)
# första maj
on (05/01) between () use (0.0018333333333, 1)
# kristi himmelfärds dag
on (easter+39) between () use (0.0018333333333, 1)
# annandag pingst
on (easter+50) between () use (0.0018333333333, 1)
# juldagen
on (12/25) between () use (0.0018333333333, 1)
# annandag jul
on (12/26) between () use (0.0018333333333, 1)

# "aftnar" (osäkert om följande är korrekt & fullständigt)
# trettondagsafton
on (01/05) between () use (0.0018333333333, 1)
# julafton
on (12/24) between () use (0.0018333333333, 1)
# nyårsafton
on (12/31) between () use (0.0018333333333, 1)
