################################################################
#
#        Portugal Telecom Ruleset for local calls
#        origem: Continente
#        
#        changes 2000-01-02
#        Jos� Carlos Monteiro
#        jcm@oninet.pt
################################################################

name=PT Local (Continente)

currency_symbol=$
currency_position=right 
currency_digits=0

# No Activation
per_connection=0

# Pre�o inicial (60 s)
minimum_costs=18.72
flat_init_costs=(18.72,60)

# Economico: semana das 21:00 �s 09:00; fim-de-semana todo o dia
#
default=(0.03705, 1)

# desconto de 30% ap�s o 10� minuto
on (saturday..sunday) between () use (0.025935, 1, 600)
on () between (21:00..09:00) use (0.025935, 1, 600)

# Daylight rates
on (monday..friday) between (09:00..21:00) use (0.078, 1)
on (monday..friday) between (09:00..21:00) use (0.0546, 1, 600)

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
on (1/1, easter-47, easter-2, easter, 4/25, 5/1, easter+60, 6/10, 8/15, 10/5, 11/1, 12/1,12/8, 12/25) between () use (0.03705, 1)
on (1/1, easter-47, easter-2, easter, 4/25, 5/1, easter+60, 6/10, 8/15, 10/5, 11/1, 12/1,12/8, 12/25) between () use (0.025935, 1, 600)

