# Suitable for State of Rio de Janeiro - Brazil
#
# Cobranca aplicada pela Telemar no Rio de Janeiro. Os valores foram obtidos
# na pagina da Telemar (www.telemar.com.br) e sao de 27 de março de 2002.
#
# Renato G. dos Santos
# mailto:linuxiado@rba.omnqf.nom.br
#

name=Telemar
currency_symbol=R$
currency_position=left
currency_digits=5

per_connection=0.09258
#flat_init_costs=(0.09258,240)
minimum_costs=0.09258
default=(0.09258, 240)

on (monday..saturday) between (0:00..06:00) use (0.00000, 100)
on (saturday) between (14:00..23:59) use (0.00000, 100)
on (sunday) between () use (0.00000, 100)

# Feriados nacionais:
#			 Ano novo
#			 Tiradentes
#			 Dia do trabalho
#			 Proclamacao da Independencia
#			 Padroeira do Brasil
#			 Proclamacao da Republica
#			 Natal

on (01/01, 04/21, 05/01, 09/07, 10/12, 11/15, 12/25) between () use (0.00000,100)
