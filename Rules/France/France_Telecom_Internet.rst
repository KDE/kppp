################################################################
#
# R�gles pour France T�l�com
# Appels "Internet"
# Valable vers tous les num�ros Internet 
#  commen�ant par :  08 36 06 13 1. ,  08 36 01 9. .. ,  
#  08 60 .. .. ..,
#  et aux num�ros :  08 36 01 13 13  et  08 36 01 30 13
#
# Fichier original (je pense...) par
# $Id$
# (C) 1997 Czo <sirol@asim.lip6.fr>
#
# Modifi� par Pascal Benito <pascal.benito@free.fr> 
# le 11 Ao�t 2000.
#
# Pass� a l'euro par David Faure <david@mandrakesoft.com> avec
# indications de Frederic Delaporte <fredericdelaporte@free.fr>
#
# Modifi� par Cyril Bosselut <bosselut@b1project.com> 
# Renom� en France_Telecom_Internet.rst
# le 22 Juillet 2004.
#
################################################################


name=France_Telecom_Internet
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
flat_init_costs=(0.10,60)

# Ceci est la r�gle utilis�e par d�faut lorsqu'aucune autre ne
# s'applique. Le premier nombre est le prix correspondant � la
# dur�e en secondes qui est le second nombre.
default=(0.02, 60)
# (quelle est l'utilit� de ce param�tre ?!?!)

