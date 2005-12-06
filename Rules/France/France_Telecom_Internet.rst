################################################################
#
# Règles pour France Télécom
# Appels "Internet"
# Valable vers tous les numéros Internet 
#  commençant par :  08 36 06 13 1. ,  08 36 01 9. .. ,  
#  08 60 .. .. ..,
#  et aux numéros :  08 36 01 13 13  et  08 36 01 30 13
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
# Modifié par Cyril Bosselut <bosselut@b1project.com> 
# Renomé en France_Telecom_Internet.rst
# le 22 Juillet 2004.
#
################################################################


name=France_Telecom_Internet
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
flat_init_costs=(0.10,60)

# Ceci est la règle utilisée par défaut lorsqu'aucune autre ne
# s'applique. Le premier nombre est le prix correspondant à la
# durée en secondes qui est le second nombre.
default=(0.02, 60)
# (quelle est l'utilité de ce paramètre ?!?!)

