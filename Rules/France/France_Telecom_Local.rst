################################################################
#
# R�gles pour France T�l�com
# Appels locaux
#
# Fichier original (je pense...) par
# $Id$
# (C) 1997 Czo <sirol@asim.lip6.fr>
#
# Modifi� par Pascal Benito <pascal.benito@free.fr> 
# le 11 Ao�t 2000.
#
# Pass� a l'euro par David Faure <david@mandrakesoft.com>
# le 8 Avril 2002.
#
################################################################

# Pour les appels locaux, France T�l�com indique qu'une minute
# de communication devrait co�ter:
# ( ceci ne doit pas tenir compte des arrondis )
#
# 0.02 EUR TTC en tarif r�duit
# 0.03 EUR TTC in tarif normal
#

name=France_Telecom_Local
currency_symbol=EUR
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

# C'est ce que vous payez pour la premi�re minute de
# connexion, peu importe si l'appel dure 1 ou 59 secondes.
flat_init_costs=(0.09,60)

# Ceci est la r�gle utilis�e par d�faut lorsqu'aucune autre ne
# s'applique. Le premier nombre est le prix correspondant � la
# dur�e en secondes qui est le second nombre.
default=(0.03, 60)
# (quelle est l'utilit� de ce param�tre ?!?!)

on (monday..friday)	between (00:00..07:59) use (0.02, 60)
on (monday..friday)	between	(08:00..18:59) use (0.03, 60)
on (monday..friday)	between (19:00..23:59) use (0.02, 60)

on (saturday..sunday)	between (00:00..23:59) use (0.02, 60)

# Jours f�ri�s :-))
on (01/01, easter+1, 05/01, 05/08, easter+38, easter+50, 07/14, 08/15, 11/01, 11/11, 12/25) between (00:00..23:59) use (0.02, 60)

