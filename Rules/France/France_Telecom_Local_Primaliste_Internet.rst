################################################################
#
# Règles pour France Télécom
# Appels locaux et abonnement spécial "Primaliste Internet"
# aussi appelé "Malicio Nuit"
#
# Fichier original (je pense...) par
# $Id$
# (C) 1997 Czo <sirol@asim.lip6.fr>
#
# Modifié par Pascal Benito <pascal.benito@free.fr> 
# le 11 Août 2000.
#
# Passé a l'euro par David Faure <david@mandrakesoft.com> avec
# indications de Frederic Delaporte <fredericdelaporte@free.fr>
#
################################################################


name=France_Telecom_Local_Primaliste_Internet
currency_symbol=EUR
currency_position=right
currency_digits=2


################################################################
# Paramètres de connection
################################################################

# NOTE: les règles sont parcourues du dèbut vers la fin du fichier
#       donc seulement la dernière règle appropriée est prise en
#       considération. Placez donc les règles les plus générales
#       avant les plus spécifiques.

# Ceci est un coût supplémentaire éventuel par appel. Si vous
# n'êtes pas concerné, posez le égal à zéro ou commentez-le.
per_connection=0.0

# Frais minimum par appel. Si le coût d'un appel est inférieur
# à cette valeur, alors cette dernière est le coût retenu.
minimum_costs=0.0

# C'est ce que vous payez pour la première minute de
# connexion, peu importe si l'appel dure 1 ou 59 secondes.
flat_init_costs=(0.09,60)

# Ceci est la règle utilisée par défaut lorsqu'aucune autre ne
# s'applique. Le premier nombre est le prix correspondant à la
# durée en secondes qui est le second nombre.
default=(0.03, 60)
# (quelle est l'utilité de ce paramètre ?!?!)

on (monday..friday)	between (00:00..07:59) use (0.01, 60)
on (monday..friday)	between	(08:00..18:59) use (0.03, 60)
on (monday..friday)	between (19:00..21:59) use (0.02, 60)
on (monday..friday)	between (22:00..23:59) use (0.01, 60)

on (saturday..sunday)	between (00:00..07:59) use (0.01, 60)
on (saturday..sunday)	between (08:00..21:59) use (0.02, 60)
on (saturday..sunday)	between (22:00..23:59) use (0.01, 60)

# Jours fériés :-))
on (01/01, easter+1, 05/01, 05/08, easter+38, easter+50, 07/14, 08/15, 11/01, 11/11, 12/25) between (00:00..07:59) use (0.01, 60)
on (01/01, easter+1, 05/01, 05/08, easter+38, easter+50, 07/14, 08/15, 11/01, 11/11, 12/25) between (08:00..21:59) use (0.02, 60)
on (01/01, easter+1, 05/01, 05/08, easter+38, easter+50, 07/14, 08/15, 11/01, 11/11, 12/25) between (22:00..23:59) use (0.01, 60)
