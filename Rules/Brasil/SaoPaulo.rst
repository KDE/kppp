# Suitable for State of Sao Paulo - Brazil
#
# Cobranca aplicada pela Telefonica em Sao Paulo. Os valores foram
# obtidos na pagina da Telefonica (www.telefonica.net.br) e sao
# de 13 de abril de 2000.
#
# Ricardo Biloti
# mailto:biloti@ime.unicamp.br
#

name=Telefonica
currency_position=left
currency_digits=5

per_connection=0.08571
#flat_init_costs=(0.08571,240)
minimum_costs=0.08571
default=(0.08571, 240)

on (monday..saturday) between (0:00..06:00) use (0.00000, 100)
on (saturday) between (14:00..23:59) use (0.00000, 100)
on (sunday) between () use (0.00000, 100)

# Feriados nacionais:
# Ano novo
# Tiradentes
# Dia do trabalho
# Proclamacao da Independencia
# Padroeira do Brasil
# Proclamacao da Republica
# Natal
on (01/01, 04/21, 05/01, 09/07, 10/12, 11/15, 12/25) between () use (0.00000,100)

