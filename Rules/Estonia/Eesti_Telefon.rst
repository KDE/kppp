################################################################
# 25. November 2000
# Autor : Rivo Laks <rivolaks@hot.ee>
# See fail on ainult Eesti Telefoni kohaliku kõne hindade arvestamiseks!
################################################################


# Nimi
name=Eesti_Telefon

### Valuuta
#Sümbol
currency_symbol=EEK
# Positsioon
currency_position=right
# Kohti peale koma
currency_digits=2

# Ühenduse loomise tasu (pole)
per_connection=0.0
# Miinimumtasu ühenduse eest (pole)
minimum_costs=0.0

### Kõnealustustasu 48 senti peale 1. sekundit
flat_init_costs=(0.48,1)

### Normaaltariif 24 senti/minutis (0.4 senti/sekundis)
default=(0.004, 1)

### Soodustariif 18 senti/minutis (0.3 senti/sek)
# Tööpäevadel 19 - 01
on (monday..friday) between (19:00..00:59) use (0.003, 1)
# Puhkepäevadel 07 - 01
on (saturday..sunday) between (07:00..00:59) use (0.003, 1)

### Öötariif 8 senti/minutis (0.13333333... senti/sek)
# Iga päev 01 - 07
on (monday..sunday) between (01:00..06:59) use (0.001333, 1)

### Pühad (soodustariif 07 - 01)
# Iseseisvuspäev
on (02/24) between (07:00..00:59) use (0.003, 1)
# Uusaasta
on (01/01) between (07:00..00:59) use (0.003, 1)
# Kevadpüha
on (05/01) between (07:00..00:59) use (0.003, 1)
# Võidupüha
on (06/23) between (07:00..00:59) use (0.003, 1)
# Jaanipäev
on (06/24) between (07:00..00:59) use (0.003, 1)
# Taasiseseisvumispäev
on (08/20) between (07:00..00:59) use (0.003, 1)
# Esimene jõulupüha
on (12/25) between (07:00..00:59) use (0.003, 1)
# Teine jõulupüha
on (12/26) between (07:00..00:59) use (0.003, 1)
# Suur reede
on (easter) between (07:00..00:59) use (0.003, 1)
