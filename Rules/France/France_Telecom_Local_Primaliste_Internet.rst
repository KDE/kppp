################################################################
#
# R�gles pour France T�l�com
# Appels locaux et abonnement sp�cial "Primaliste Internet"
#
# Fichier original (je pense...) par
# $Id$
# (C) 1997 Czo <sirol@asim.lip6.fr>
#
# Modifi� par Pascal Benito <pascal.benito@free.fr> 
# le 11 Ao�t 2000.
#
################################################################

# Les nouveaux tarifs de France T�l�com sont en vigueur
# depuis le 1er Septembre 1999
#
# Pour les appels locaux, France T�l�com indique qu'une minute
# de communication devrait co�ter:
# ( ceci ne doit pas tenir compte des arrondis )
#
# 0.14 F TTC en tarif r�duit
# 0.28 F TTC in tarif normal
#

name=France_Telecom_Local_Primaliste_Internet
currency_symbol=F
currency_position=right
currency_digits=2


################################################################
# Param�tres de connection
################################################################

# NOTE: les r�gles sont parcourues du d�but vers la fin du fichier
#       donc seulement la derni�re r�gle appropri�e est prise en
#       consid�ration. Placez donc les r�gles les plus g�n�rales
#       avant les plus sp�cifiques.

# Ceci est un co�t suppl�mentaire �ventuel par appel. Si vous
# n'�tes pas concern�, posez le �gal � z�ro ou commentez-le.
per_connection=0.0

# Frais minimum par appel. Si le co�t d'un appel est inf�rieur
# � cette valeur, alors cette derni�re est le co�t retenu.
minimum_costs=0.0

# C'est ce que vous payez pour les trois premi�res minutes de
# connexion, peu importe si l'appel dure 1 ou 179 secondes.
flat_init_costs=(0.74,180)

# Ceci est la r�gle utilis�e par d�faut lorsqu'aucune autre ne
# s'applique. Le premier nombre est le prix correspondant � la
# dur�e en secondes qui est le second nombre.
default=(0.28, 60)
# (quelle est l'utilit� de ce param�tre ?!?!)

on (monday..friday)	between (00:00..07:59) use (0.07, 60)
on (monday..friday)	between	(08:00..18:59) use (0.28, 60)
on (monday..friday)	between (19:00..21:59) use (0.14, 60)
on (monday..friday)	between (22:00..23:59) use (0.07, 60)

on (saturday..sunday)	between (00:00..07:59) use (0.07, 60)
on (saturday..sunday)	between (08:00..21:59) use (0.14, 60)
on (saturday..sunday)	between (22:00..23:59) use (0.07, 60)

# Jours f�ri�s :-))
on (01/01, easter+1, 05/01, 05/08, easter+38, easter+50, 07/14, 08/15, 11/01, 11/11, 12/25) between (00:00..07:59) use (0.07, 60)
on (01/01, easter+1, 05/01, 05/08, easter+38, easter+50, 07/14, 08/15, 11/01, 11/11, 12/25) between (08:00..21:59) use (0.14, 60)
on (01/01, easter+1, 05/01, 05/08, easter+38, easter+50, 07/14, 08/15, 11/01, 11/11, 12/25) between (22:00..23:59) use (0.07, 60)
