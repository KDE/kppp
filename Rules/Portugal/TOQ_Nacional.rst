################################################################
#
#        Chamadas Nacionais via TOQ (1091)
#        origem: Continente
#        
#        changes 2000-01-02
#        Jos� Carlos Monteiro
#        jcm@oninet.pt
################################################################

name=TOQ Nacional (Continente)

currency_symbol=$
currency_position=right 
currency_digits=0

# No Activation
per_connection=0

# Pre�o m�nimo
minimum_costs=0

# Econ�mico: semana das 19:00 �s 08:00; fim-de-semana todo o dia
#
default=(0.2535, 1)

# Hor�rio Normal (dias �teis das 08:00 �s 19:00)
on (monday..friday) between (8:00..19:00) use (0.4875, 1)

# Feriados Nacionais (tarifa econ�mica):
# 1/1        Ano Novo
# easter-47  Carnaval
# easter-2   Sexta-feira Santa
# easter     P�scoa
# 4/25       25 de Abril
# 5/1        Dia do Trabalhador
# easter+60  Corpo de Deus
# 6/10       Dia de Portugal
# 8/15       Nossa Senhora da Assun��o
# 10/5       Dia da Rep�blica
# 11/1       Dia de Todos os Santos
# 12/1       Dia da Independ�ncia
# 12/8       Imaculada Concei��o
# 12/25      Natal
#
on (1/1, easter-47, easter-2, easter, 4/25, 5/1, easter+60, 6/10, 8/15, 10/5, 11/1, 12/1,12/8, 12/25) between () use (0.2535, 1)
