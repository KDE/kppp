################################################################
# Viimati muudetud : 31/10/2002
# Autor : Rivo Laks <rivolaks@hot.ee>
# Tariifid Eesti Telefonile (kohalik k�ne)
################################################################


# Nimi
name=Eesti_Telefon

### Valuuta
#S�mbol
currency_symbol=kr
# Positsioon
currency_position=right
# Kohti peale koma
currency_digits=2

# �henduse loomise tasu (pole)
per_connection=0.0
# Miinimumtasu �henduse eest (pole)
minimum_costs=0.0

### K�nealustustasu 48 senti peale 1. sekundit
flat_init_costs=(0.48,1)

### Normaaltariif 34 senti/minutis (0.56666666... senti/sekundis)
default=(0.005666, 1)

### Soodustariif 28 senti/minutis (0.4666... senti/sek)
# T��p�evadel 19 - 01
on (monday..friday) between (19:00..00:59) use (0.004666, 1)
# Puhkep�evadel 07 - 01
on (saturday..sunday) between (07:00..00:59) use (0.004666, 1)

### ��tariif 14 senti/minutis (0.23333333... senti/sek)
# Iga p�ev 01 - 07
on (monday..sunday) between (01:00..06:59) use (0.002333, 1)

### P�had (soodustariif 07 - 01)
# Iseseisvusp�ev
on (02/24) between (07:00..00:59) use (0.004666, 1)
# Uusaasta
on (01/01) between (07:00..00:59) use (0.004666, 1)
# Kevadp�ha
on (05/01) between (07:00..00:59) use (0.004666, 1)
# V�idup�ha
on (06/23) between (07:00..00:59) use (0.004666, 1)
# Jaanip�ev
on (06/24) between (07:00..00:59) use (0.004666, 1)
# Taasiseseisvumisp�ev
on (08/20) between (07:00..00:59) use (0.004666, 1)
# Esimene j�ulup�ha
on (12/25) between (07:00..00:59) use (0.004666, 1)
# Teine j�ulup�ha
on (12/26) between (07:00..00:59) use (0.004666, 1)
# Suur reede
on (easter-2) between (07:00..00:59) use (0.004666, 1)
