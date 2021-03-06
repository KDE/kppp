################################################################
# There is a russian translation of same file named 
# "$prefix/kppp/Rules/TEMPLATE"
# Code page of this file is KOI8-R
################################################################
#
# Disclaimer/Лицензия
# Этот шаблон от (c) Mario Weilguni <mweilguni@kde.org>
# Он лицензируется на тех же условиях, что и пакет kppp,
# частью которого он является.
#
################################################################
#
# ПРИМЕЧАНИЕ ПЕРЕВОДЧИКА:
#            Этот файл является переводом файла "TEMPLATE", расположенного
#            вероятно в директории "/usr/share/apps/kppp/Rules"
#
# Это пример установленных правил для kppp. Вы можете использовать
# его как шаблон, когда Вы будете создавать свой набор правил. Если Вы
# будете это делать, удалите все комментарии и добавьте свои. Это значительно
# упростит другим пользователям проверку этого набора правил.
#
# Пожалуйста, подпишите этот тарифный план своим именем с e-mail адресом
# чтобы я мог связаться с Вами в случае необходимости.
#
# ПРИМЕЧАНИЕ: Правила в этом шаблоне не имеют особого смысла и
#       приведены только в демонстрационных целях
#
# ПРИМЕЧАНИЕ ПО ИМЕНАМ ФАЙЛОВ:
#  Когда вы создаете свой набор правил, используйте "_" в имени файла
#  вместо пробелов и используйте расширение ".rst"
#     т.е. "Austria city calls"
#          --> файл должен быть сохранен как "Austria_city_calls.rst"
#
# Благодарю, Bernd Wuebben
# wuebben@math.cornell.edu / wuebben@kde.org
#
# Перевод: (c) Александр А. Пучков, 03.08.2001
# Translation: (c) Alex A. Puchkov, 03.08.2001
# Текущий e-mail: mailex@nm.ru / mailex@mail.ru
#
# ПРИМЕЧАНИЕ ПЕРЕВОДЧИКА: Если Вы обнаружили ошибки или неточности,
#            или же хотите внести свои пожелания - пишите мне по
#            указанному выше e-mail адресу.
################################################################


################################################################
#
# НАЗВАНИЕ ТАРИФА. Оно нужно для счета (accounting purposes).
#
################################################################
name=default

################################################################
# Установки валюты
################################################################

# Задаем ATS (Австрийский шиллинг) для использования его как символ
# валюты (не обязательно нужен, по умолчанию = "$")
# currency_symbol=ATS
currency_symbol=$

# Задаем позицию символа валюты.
# (не обязательно нужно, по умолчанию это "right" /справа/)
currency_position=right

# Задаем число значимых цифр.
# (не обязательно нужно, по умолчанию это "2")
currency_digits=2



################################################################
# Установки соединения
################################################################

# ПРИМЕЧАНИЕ: правила применяются из начала в конец - это значит,
#       что только ПОСЛЕДНЕЕ соответствующее правило используется
#       для вычисления затрат.

# Это оплачивается каждый раз, когда Вы соединяетесь с провайдером.
# Если Вы не платите при соединении, используйте "0" здесь или
# закомментируйте эту строку.
per_connection=0.0


# Минимальный взнос при соединении. Если взнос за телефонный
# звонок меньше чем это значение, то взамен используется это значение (?)
# Примечание переводчика: см. в оригинале, если непонятно.
minimum_costs=0.0


# Вы платите .74 за первые 180 секунд (3 минуты), при этом безразлично,
# сколько Вы подключены - 1 секунду или 180 секунд.
# Это правило получает приоритет в течении первых 180 секунд
# над любым другим правилом, в особенности над правилом по умолчанию.
# Посмотрите на файл costgraphs.gif в директории docs дистрибутива
# kppp для графической иллюстрации выше написанного.
flat_init_costs=(0.74,180)

# Это правило по умолчанию, которое используется когда не применяются
# другие правила. Первый компонент "0.1" это цена за одну "единицу"
# /"unit"/, где "72" это время в секундах.
# Следовательно, следующее правило означает: "Каждые 72 секунды 0.1
# ATS прибавляется к счету"
default=(0.1, 72)

#
# Более сложные правила:
#

# "с понедельника до воскресенья с 00:00 до 23:59 взнос
#  0.2 каждые 72 секунды"
# Примечание переводчика: в оригинале с 12:00 am до 11:59 pm
on () between () use (0.2, 2)

# Тоже самое, что и выше
on (monday..sunday) between () use (0.2, 2)

# Тоже самое, что и выше. Вы должны использовать 24-х часовое время,
# или вычисления не будут корректными. (Пример: пишите 15:00 для 3 pm)
on (monday..sunday) between (0:00..23:59) use (0.2, 2)

# применительно к пятнице, субботе, воскресенью и понедельнику с 8:00 до 13:00
on (friday..monday) between (8:00..13:00) use(0.3,72)

# ВНИМАНИЕ:
on(monday..friday) between (21:00..5:00) use (0.4,2)
# НЕ включая субботу 0:00-5:00, понедельник..пятница, как указано.

# Применительно к указанной дате (католическому Рождеству)
on (12/25) between () use (0.3,72)

# Интервал дат и один день недели
on (12/25..12/27, 12/31, 07/04, monday) between () use (0.4, 72)

# Используйте это для Пасхи
# Примечание переводчика: Для России, вероятно, лучше не использовать это
#            значение, так как православная Пасха может не совпадать
#            с католической по дате
on (easter) between () use (0.3,72)

# Пасха + 50 дней (Троицин день) [Pfingstmontag/ Pentecost Monday]
# Примечание переводчика: см. выше
on (easter+50) between () use (0.3,72)

on (thursday) between (20:00..21:52) use (8.2, 1)


# Правила "on()" прежде всего связаны только с текущим временем. Вы можете
# также создать правила, зависящие от количества секунд, в течении которых
# Вы были подключены к своему провайдеру, задавая это время как третий аргумент
# к "use()".
# К примеру, пусть будет нормальный тариф вечером - 0.20 за минуту,
# и он уменьшается на 20% после 1-го часа соединения. Это может быть
# смоделировано так:

on () between (19:30..08:00) use (0.20, 60)
on () between (19:30..08:00) use (0.16, 60, 3600)

# Примечание к этим правилам, справедливое и к другим правилам:
#            Правила чувствительны к порядку, в котором они следуют
