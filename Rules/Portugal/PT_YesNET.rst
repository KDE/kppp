################################################################
#
#        Portugal Telecom Ruleset for local calls for
#        Internet numbers 67
#        origem: Continente
#        
#        changes 2000-01-02
#        José Carlos Monteiro
#        jcm@oninet.pt
################################################################

name=PT YesNET 67

currency_symbol=$
currency_position=right 
currency_digits=0

# No Activation
per_connection=0

# Preço inicial (180 s)
minimum_costs=10.53
flat_init_costs=(10.53,180)

# Economico: semana das 18:00 às 09:00; fim-de-semana todo o dia
#
default=(0.027495, 1)

# Daylight rates
on (monday..friday) between (9:00..18:00) use (0.05655, 1)

# Feriados Nacionais (tarifa económica):
# 1/1        Ano Novo
# easter-47  Carnaval
# easter-2   Sexta-feira Santa
# easter     Páscoa
# 4/25       25 de Abril
# 5/1        Dia do Trabalhador
# easter+60  Corpo de Deus
# 6/10       Dia de Portugal
# 8/15       Nossa Senhora da Assunção
# 10/5       Dia da República
# 11/1       Dia de Todos os Santos
# 12/1       Dia da Independência
# 12/8       Imaculada Conceição
# 12/25      Natal
#
on (1/1, easter-47, easter-2, easter, 4/25, 5/1, easter+60, 6/10, 8/15, 10/5, 11/1, 12/1,12/8, 12/25) between () use (0.027495, 1)

