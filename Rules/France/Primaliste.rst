################################################################
#
# Règles pour France Télécom
# Appels locaux avec options Primaliste Internet
#
# Fichier original par
# $Id$
# (C) 1997 Czo <sirol@asim.lip6.fr>
#
# fichier adapté par Laurent Montel <montell@club-internet.fr>
# le 8 mai 1999
# pour prendre en compte ces options
################################################################

# Les nouveaux tarifs de France Télécom sont en vigueur
# depuis le 1er mars 1999.
#
# Pour les appels locaux, France Télécom indique qu'une minute
# de communication devrait coûter:
# ( A mon avis, ceci ne tient pas compte des arrondis )
#
# 0.14 F TTC en tarif réduit
# 0.28 F TTC in tarif normal
#

name=France_Primaliste
currency_symbol=F
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

# C'est ce que vous payez pour les trois premières minutes de
# connexion, peu importe si l'appel dure 1 ou 179 secondes.
flat_init_costs=(0.74,180)

# Ceci est la règle utilisée par défaut lorsqu'aucune autre ne
# s'applique. Le premier nombre est le prix correspondant à la
# durée en secondes qui est le second nombre.
default=(0.28, 60)
# Ceci n'engage que moi, mais si toutes les heures de la semaine
# sont incluses dans les règles ci-dessous; quelle est l'utilité
# de ce paramètre ?!?!

on (monday..sunday)   between (22:00..23:59) use (0.07, 60)
on (monday..sunday)   between (00:00..07:59) use (0.07, 60)
on (monday..friday)   between (08:00..18:59) use (0.28, 60)
on (monday..friday)   between (19:00..21:59) use (0.14, 60)
# changement le 1er mars 1999 le samedi de 8 Heures à 12 Heures ca coute 0.14 francs par minute
on (saturday..sunday) between (08:00..21:59) use (0.14, 60)


# Jours fériés :-))
on (01/01, easter+1, 05/01, 05/08, easter+38, easter+50, 07/14, 08/15, 11/01, 11/11, 12/25) between (00:00..23:59) use (0.14, 60)

