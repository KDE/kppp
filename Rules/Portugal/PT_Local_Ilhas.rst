################################################################
#
#        Portugal Telecom Ruleset for local calls
#        origem: Ilhas
#        
#        changes 2000-01-02
#        José Carlos Monteiro
#        jcm@oninet.pt
################################################################

name=PT Local (Ilhas)

currency_symbol=$
currency_position=right 
currency_digits=0

# No Activation
per_connection=0

# Preço inicial (60 s)
minimum_costs=17.92
flat_init_costs=(17.92, 60)

# Economico: semana das 21:00 às 09:00; fim-de-semana todo o dia
#
default=(0.1064, 3)

# desconto de 30% após o 10º minuto
on (saturday..sunday) between () use (0.07448, 3, 600)
on () between (21:00..09:00) use (0.07448, 3, 600)

# Daylight rates
on (monday..friday) between (09:00..21:00) use (0.224, 3)
on (monday..friday) between (09:00..21:00) use (0.1568, 3, 600)

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
on (1/1, easter-47, easter-2, easter, 4/25, 5/1, easter+60, 6/10, 8/15, 10/5, 11/1, 12/1,12/8, 12/25) between () use (0.1064, 3)
on (1/1, easter-47, easter-2, easter, 4/25, 5/1, easter+60, 6/10, 8/15, 10/5, 11/1, 12/1,12/8, 12/25) between () use (0.07448, 3, 600)

