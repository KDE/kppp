################################################################
#
# R�gles pour Onetel, connexion locale ou nationale
# 
# Fait le 28 octobre 2002 par G�rard Delafond
# d'apr�s David Faure, lui-m�me d'apr�s, etc.
################################################################


name=OneTel
currency_symbol=EUR
currency_position=right
currency_digits=2


################################################################
# Param�tres de connexion
################################################################

# NOTE: les r�gles sont parcourues du d�but vers la fin du fichier
#       donc seulement la derni�re r�gle appropri�e est prise en
#       consid�ration. Placez donc les r�gles les plus g�n�rales
#       avant les plus sp�cifiques.

# Ceci est un co�t suppl�mentaire �ventuel par appel. Si vous
# n'�tes pas concern�, posez-le �gal � z�ro ou commentez-le.
per_connection=0.0

# Frais minimum par appel. Si le co�t d'un appel est inf�rieur
# � cette valeur, alors cette derni�re est le co�t retenu.
minimum_costs=0.0

# C'est ce que vous payez pour la premi�re minute de
# connexion, peu importe si l'appel dure 1 ou 59 secondes.
# tarifs �tablis d'apr�s 
# http://www.9online.fr/espaceabonnes/telephonie/comparatif.asp
# soit 1,7 c d'euro la minute
flat_init_costs=(0.01,60)

# Ceci est la r�gle utilis�e par d�faut lorsqu'aucune autre ne
# s'applique. Le premier nombre est le prix correspondant � la
# dur�e en secondes qui est le second nombre.
default=(0.01, 60)
# (quelle est l'utilit� de ce param�tre ?!?!)
