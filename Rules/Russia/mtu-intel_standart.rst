################################################################
# "MTU-Intel" (Russia, Moscow) rate ruleset
# Created by "Alex A. Puchkov" <mailex@nm.ru>
#
# Code page of this file is KOI8-R
# Date: 03.08.2001, Version 0.1b
################################################################
#
# Disclaimer/Лицензия
# Этот тариф основан на шаблоне от (c) Mario Weilguni <mweilguni@kde.org>,
# и своем собственном переводе этого шаблона, который можно найти в директории
# "/usr/share/apps/kppp/Rules"
# Он лицензируется на тех же условиях, что и пакет kppp.
# Корректность расчетов не гарантирована.
#
# ПРИМЕЧАНИЕ: Если Вы обнаружили ошибки или неточности,
#             или же хотите внести свои пожелания - пишите мне по
#             указанному выше e-mail адресу.
#
################################################################

################################################################
# НАЗВАНИЕ ТАРИФА. Оно нужно для счета (accounting purposes).
################################################################
name=MTU_INTEL_standart

################################################################
# Установки соединения
################################################################

# Текущий тариф на коммутируемый доступ в Интернет от компании МТУ-Интел
# Повременной доступ г.Москва, модемные пулы 995-55-55/56, 721-33-11 и другие
#         Дневной тариф, действует  с  9:30 до 20:00 часов.   0,75 у.е./Час
#         Вечерний тариф, действует с 20:00 до 02:00 часов.   0,90 у.е./Час
#         Ночной тариф, действует   с 02:00 до 09:30 часов.   0,35 у.е./Час
	 
# Если Вы работаете первые 30 секунд, то Вы не платите ничего :-)
flat_init_costs=(0.0, 30)

# Дневной тариф, действует  с  9:30 до 20:00 часов. - 0,75 $/Час
# Это правило по умолчанию, и в данном случае оно означает:
# "Каждые 30 секунд 0.00625 прибавляется к счету (дневной тариф - 0,75 $ в час)"
default=(0.00625, 30)

# Вечерний тариф, действует с 20:00 до 02:00 часов. - 0,90 $/Час
# "Каждый день недели с 20:00 до 01:59 взнос 0.0075 каждые 30 секунд"
on () between (20:00..01:59) use (0.0075, 30)

# Ночной тариф, действует   с 02:00 до 09:30 часов. - 0,35 $/Час
# "Каждый день недели с 02:00 до 09:29 взнос 0.0029166 каждые 30 секунд"
on () between (02:00..09:29) use (0.0029166, 30)
