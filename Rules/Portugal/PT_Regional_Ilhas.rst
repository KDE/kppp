################################################################
#
#        Portugal Telecom Ruleset for regional calls
#        origem: Ilhas
#        
#        changes 2000-01-02
#        Jos� Carlos Monteiro
#        jcm@oninet.pt
################################################################

name=PT Regional (Ilhas)

currency_symbol=$
currency_position=right 
currency_digits=0

# No Activation
per_connection=0

# Pre�o inicial (20 s)
minimum_costs=17.92
flat_init_costs=(17.92,20)

# Economico: semana das 21:00 �s 09:00; fim-de-semana todo o dia
#
default=(0.448, 3)

# Daylight rates
on (monday..friday) between (9:00..21:00) use (0.868, 3)

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
on (1/1, easter-47, easter-2, easter, 4/25, 5/1, easter+60, 6/10, 8/15, 10/5, 11/1, 12/1,12/8, 12/25) between () use (0.448, 3)
