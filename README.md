[english](https://github.com/thekondrashov/drupal-startupspark/blob/master/README.en.md) | **русский**
- - -

[![Deploy to Azure][Azure]](https://deploy.azure.com/?repository=https://github.com/thekondrashov/drupal-startupspark) ![Drupal core][Drupal] [![Software License][MIT]](https://github.com/thekondrashov/drupal-startupspark/blob/master/LICENSE) ![build with ❤][Love]

# [`Drupal 7`](https://drupal.org) Startups park module

Тестовое задание "Агрегатор стартапов" от компании [СимбирСофт](http://www.simbirsoft.com/ruru/) на [вакансию Drupal-разработчика](http://dimitrovgrad.hh.ru/vacancy/15710221).
> :construction: В настоящее время проект находится в очень ранней стадии разработки. Помните об этом! Этот документ следует рассматривать как черновик. Вы были уведомлены. ;)

## Содержание

> - [Демо](#Демо)
> - [Задание](#Задание)
    - [Цель](#Цель)
    - [Базовые возможности](#Базовые-возможности)
    - [Требования к задачам](#Требования-к-задачам)
    - [Требования к дизайну](#Требования-к-дизайну)
    - [Требования к реализации](#Требования-к-реализации)
    - [Требования к поставке (список файлов)](#Требования-к-поставке-список-файлов)
    - [Пункты, которые добавят вам кармы](#Пункты-которые-добавят-вам-кармы)
> - [Лицензия](#Лицензия-и-копирайты)

## Демо
Демо будет находиться здесь: https://zu.azurewebsites.net (Microsoft Azure с DreamSpark-подпиской - Netherlands-F1)
> По умолчанию веб-приложения в [Azure](https://portal.azure.com) с [DreamSpark-подпиской](http://www.dreamspark.ru) выгружаются после определенного времени простоя. Поэтому первый запуск может длиться дольше обычного.

Пользовательские переменные окружения (environment variables) в Azure:

|Ключ|Значение|Предназначение|
|---|---|---|
|`DEPLOYMENT_TARGET`|`%HOME%\site\wwwroot\sites\all\modules\startupspark`|Место развёртывания приложения. Обычно это папка `wwwroot `. [Источник](https://github.com/projectkudu/kudu/wiki/Deployment-hooks#environment-variables)|

Развертывание приложения происходит автоматически с помощью git после побликации изменений в `master`-ветке данного репозитория.
Для интеграции и [непрерывного развертывания из сетевых git-репозиториев](https://azure.microsoft.com/ru-ru/documentation/articles/web-sites-publish-source-control/#Step7) используется [Kudu](https://github.com/projectkudu/kudu/wiki).

Версия Drupal: `7.41`

Версия PHP: `5.6`

База данных: ClearDB MySQL 5.5

Данный демо-сайт закрыт от индексации поисковыми роботами с помощью инструкций в файле `robots.txt`:
```
User-agent: *
Disallow: /
```

**[⬆ вернуться к содержанию](#Содержание)**

## Задание

### Цель
Создание сайта - агрегатора стартапов. Любой человек может зарегистрироваться и добавить информацию о своём стартапе.

### Базовые возможности
- [x] регистрация пользователей;
- [x] отображение на главной списка самых популярных стартапов;
- [x] поиск с помощью фильтра;
- [x] просмотр информации о стартапе.
- [x] добавление/редактирование информации о стартапе.

### Требования к задачам
- [x] 1. Информация о стартапе должна содержать следующие поля:
  - [x] название;
  - [x] логотип;
  - [x] описание;
  - [x] род деятельности;
  - [x] количество участников/сотрудников;
  - [x] список ключевых участников;
  - [x] список инвесторов;
  - [x] контактные данные.
- [x] 2. Создать модуль реализующий аутентификацию и регистрацию пользователей в модальных окнах.
- [x] 3. После успешной регистрации/аутентификации пользователя он должен быть перенаправлен:
  - [x] на страницу добавления информации о стартапе, если у данного пользователя нет добавленных;
  - [x] на страницу информации о стартапе, если он уже был добавлен.
- [x] 4. Каждый пользователь может добавить только 1 стартап.
- [x] 5. Реализовать страницу для поиска стартапов с помощью фильтра. Фильтр должен быть организован в виде выпадающих списков. При выборе значения фильтр должен автоматически применяться.
- [x] 6. Фильтр должен содержать следующие критерии:
  - [x] род деятельности;
  - [x] количество участников/сотрудников.
- [x] 7. Фильтр также должен находиться на главной и перенаправлять на соответствующую страницу только при нажатии на кнопку поиска.
- [ ] 8. «Упаковать» весь созданный функционал и материалы с помощью Features, Node Export и т.д. для возможности развёртывания сайта «с нуля».

### Требования к дизайну
- [x] Информация о стартапе должна выводится в следующем виде:

<table width="600">
  <tr> 
    <th colspan="3" align="left">Название</th>
    <td rowspan="2" align="center">Логотип</td>
  </tr>
  <tr>
    <td colspan="3" height="100">Описание</td>
  </tr>
  <tr>
    <td colspan="4">Участники</td>
  </tr>
  <tr>
    <td width="150" height="100">&nbsp;</td>
    <td width="150">&nbsp;</td>
    <td width="150">&nbsp;</td>
    <td width="150">&nbsp;</td>
  </tr>
  <tr>
    <td colspan="4">Инвесторы</td>
  </tr>
  <tr>
    <td height="100">&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
  </tr>
  <tr>
    <td colspan="4">Контактные данные</td>
  </tr>
  <tr>
    <td>&nbsp;</td>
    <td></td>
    <td></td>
    <td></td>
  </tr>
  <tr>
    <td>&nbsp;</td>
    <td></td>
    <td></td>
    <td></td>
  </tr>
</table>

Дополнительные требования к дизайну отсутствуют.

### Требования к реализации
- [x] БД MySQL.
- [x] Модальные окна должны быть реализованы с помощью CTools modal API.
- [x] Вывод информации о стартапе должен быть реализован с помощью темплейтов (.tpl).

### Требования к поставке (список файлов)
- [x] Все исходные файлы.
- [ ] Дамп БД.
- [ ] Дамп файлов.
- [ ] Файлы для импорта материалов.
- [ ] Пошаговая инструкция для развёртывания сайта «с нуля».

### Пункты, которые добавят вам кармы
- [ ] Возможность развёртывания сайта «с нуля» с помощью Drush cook или Drupal install profile
- [ ] При выборе значения фильтра на главной необходимо подгружать количество

**[⬆ вернуться к содержанию](#Содержание)**

## Лицензия и копирайты

Данное программное обеспечение выпускается под [лицензией MIT](https://github.com/thekondrashov/drupal-startupspark/blob/master/LICENSE).

<!-- https://github.com/projectkudu/slingshot -->

[Azure]: https://img.shields.io/badge/Deploy-Azure-00abec.svg?style=flat&logoWidth=18&logo=data:image/svg%2Bxml;charset=utf8,%3Csvg%20xmlns=%27http://www.w3.org/2000/svg%27%20viewBox=%270%200%2041%2032%27%3E%3Cpath%20fill=%27%23fff%27%20d=%27M24.5%2016.134c-1.076%200-1.99.646-2.42%201.56H6.69c-2.582%200-4.734-2.098-4.734-4.734S4.054%208.226%206.69%208.226h2.152c.108-.538.27-1.076.484-1.56.108-.16.16-.377.27-.538%201.075-1.883%203.12-3.12%205.433-3.12%202.1%200%203.926%201.022%205.056%202.53.646-.324%201.345-.54%202.044-.7C20.627%202.47%2018.043.91%2015.03.91c-3.497%200-6.51%202.153-7.747%205.22H6.69c-3.764.054-6.83%203.12-6.83%206.886s3.065%206.834%206.83%206.834h15.442c.43.914%201.345%201.56%202.42%201.56%201.454%200%202.637-1.185%202.637-2.637-.057-1.453-1.24-2.636-2.69-2.636zm8.178%2011.73c4.627%200%208.608-3.928%208.608-8.555s-3.712-8.34-8.34-8.396c-1.56-3.12-4.733-5.218-8.446-5.218-4.52%200-8.285%203.174-9.2%207.37-2.206.216-4.09%201.4-5.272%203.068h11.244c.753-.968%201.937-1.56%203.282-1.56%202.313%200%204.196%201.883%204.196%204.196s-1.883%204.197-4.196%204.197c-1.345%200-2.475-.592-3.282-1.56H8.737c.484%203.55%203.336%206.456%207.048%206.456h16.893z%27/%3E%3C/svg%3E "Deploy to Azure"
[Drupal]: https://img.shields.io/badge/Drupal%20core-v7.x-0077c0.svg?style=flat&logo=data:image/svg%2Bxml;charset=utf8,%3Csvg%20xmlns=%27http://www.w3.org/2000/svg%27%20width=%271792%27%20height=%271792%27%3E%3Cpath%20fill=%27%23fff%27%20d=%27M1295%201586q-5-19-24-5-30%2022-87%2039t-131%2017q-129%200-193-49-5-4-13-4-11%200-26%2012-7%206-7.5%2016t7.5%2020q34%2032%2087.5%2046t102.5%2012.5%2099-4.5q41-4%2084.5-20.5t65-30%2028.5-20.5q12-12%207-29zm-39-115q-19-47-39-61-23-15-76-15-47%200-71%2010-29%2012-78%2056-26%2024-12%2044%209%208%2017.5%204.5t31.5-23.5q3-2%2010.5-8.5t10.5-8.5%2010-7%2011.5-7%2012.5-5%2015-4.5%2016.5-2.5%2020.5-1q27%200%2044.5%207.5t23%2014.5%2013.5%2022q10%2017%2012.5%2020t12.5-1q23-12%2014-34zm355-281q0-22-5-44.5t-16.5-45-34-36.5-52.5-14q-33%200-97%2041.5t-129%2083.5-101%2042q-27%201-63.5-19t-76-49-83.5-58-100-49-111-19q-115%201-197%2078.5T461%201280q-2%20112%2074%20164%2029%2020%2062.5%2028.5T701%201481q57%200%20132-32.5t134-71%20120-70.5%2093-31q26%201%2065%2031.5t71.5%2067%2068%2067.5%2055.5%2032q35%203%2058.5-14t55.5-63q28-41%2042.5-101t14.5-106zm53-160q0%20164-62%20304.5t-166%20236-242.5%20149.5-290.5%2054-293-57.5-247.5-157T192%201318t-64-302q0-89%2019.5-172.5t49-145.5T267%20579.5t78.5-94T424%20416t64.5-46.5T531%20345q14-8%2051-26.5t54.5-28.5%2048-30%2060.5-44q36-28%2058-72.5T833%2018q129%20155%20186%20193%2044%2029%20130%2068t129%2066q21%2013%2039%2025t60.5%2046.5%2076%2070.5%2075%2095%2069%20122%2047%20148.5T1664%201030z%27/%3E%3C/svg%3E "Drupal core"
[Love]: https://img.shields.io/badge/build%20with-%E2%9D%A4-d35b09.svg?style=flat "build with ❤"
[MIT]: https://img.shields.io/badge/license-MIT-brightgreen.svg?style=flat "The MIT License (MIT)"
