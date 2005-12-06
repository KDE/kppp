################################################################
#
# Règles pour Le 9, connexion locale
# 
# Fait le 27 octobre 2002 par Gérard Delafond
# d'après David Faure, lui-même d'après, etc.
################################################################


name=le_9_local
currency_symbol=EUR
currency_position=right
currency_digits=2


################################################################
# Paramètres de connexion
################################################################

# NOTE: les règles sont parcourues du dèbut vers la fin du fichier
#       donc seulement la dernière règle appropriée est prise en
#       considération. Placez donc les règles les plus générales
#       avant les plus spécifiques.

# Ceci est un coût supplémentaire éventuel par appel. Si vous
# n'êtes pas concerné, posez-le égal à zéro ou commentez-le.
per_connection=0.0

# Frais minimum par appel. Si le coût d'un appel est inférieur
# à cette valeur, alors cette dernière est le coût retenu.
minimum_costs=0.0

# C'est ce que vous payez pour la première minute de
# connexion, peu importe si l'appel dure 1 ou 59 secondes.
# tarifs établis d'après 
# http://www.9online.fr/espaceabonnes/telephonie/comparatif.asp
# soit 1,2 c d'euro 
flat_init_costs=(0.12,60)

# Ceci est la règle utilisée par défaut lorsqu'aucune autre ne
# s'applique. Le premier nombre est le prix correspondant à la
# durée en secondes qui est le second nombre.
default=(0.012, 60)
# (quelle est l'utilité de ce paramètre ?!?!)
