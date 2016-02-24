<?php

/**
 * @file
 * Custom forms used in the module.
 */

/**
 * Form builder.
 *
 * @link http://dgo.to/854216 Textfield that uses autocomplete @endlink
 * @link https://drupal.stackexchange.com/q/81810 @endlink
 */
function form_search_projects($form, &$form_state) {

  // https://api.drupal.org/api/drupal/developer!topics!forms_api_reference.html/7#default_tab
  $form['vertical_tabs'] = array(
    '#type' => 'vertical_tabs',
  );
  $form['tab1'] = array(
    '#type' => 'fieldset',
    '#title' => 'По роду деятельности',
    '#collapsible' => TRUE,
    '#group' => 'vertical_tabs',
    '#description' => 'Введите ключевое слово для поиска в поле ввода ниже и нажмите Enter или Tab.',
  );
  $default_value = '';
  if($cache = cache_get('search-keyword')) {
    $default_value = $cache->data;
    cache_clear_all('search-keyword', 'cache', TRUE);
  }
  // Autocomplete field
  $form['tab1']['keyword'] = array(
    '#type'              => 'textfield',
    '#size'              => 30,
    '#title'             => 'Род деятельности (например, спорт).',
    '#attributes'        => array('placeholder' => array('Ключевое слово')),
    '#description'       => 'Введите ключевое слово (например: спорт)',
    '#default_value'     => $default_value,
    // Define an autocomplete callback for textfield,
    '#autocomplete_path' => 'search/projects/autocomplete/category',

    // sets up a callback that will fire every time the value of the field is changed.
    '#ajax' => array(
      'callback' => 'custom_forms_category_update',
      'keypress' => TRUE,
      'progress' => array(
        'message' => 'Поиск...',
        'type' => 'bar', // throbber (default)
      ),
    ),
  );

  $form['tab2'] = array(
    '#type' => 'fieldset',
    '#title' => 'По количеству участников',
    '#collapsible' => TRUE,
    '#group' => 'vertical_tabs',
    '#description' => 'Для того, чтобы начать поиск, передвиньте ползунок в желаемую позицию.',
  );

  $query = db_select(STARTUPSPARK_SCHEMA_PREFIX . 'startup');
  // Найти максимальное значение: http://xandeadx.ru/blog/drupal/88
  // Selecting the Max and Min records in one MySQL command: https://stackoverflow.com/a/4254586
  // Not chainable! https://api.drupal.org/api/function/SelectQuery::addExpression/7#comment-43713
  $query->addExpression('MIN(staff_count)');
  $query->addExpression('MAX(staff_count)');

  // list() will give an error if the input array is too short.
  // use array_pad on the array to ensure its length: https://php.net/list#113189
  // can list() work with associative arrays somehow? https://stackoverflow.com/a/21136003
  list($min, $max) = array_pad(array_values($query->execute()->fetchAssoc()), 2, 10);

  $range = '[' . $min . ',' . $max . ']';

  // http://complexdan.com?p=43
  $form['tab2']['rangefield'] = array(
    //'#id' => 'rangefield',
    '#type' => 'textfield',
    '#prefix' => '<p class="control-label" for="rangefield">Искать в указанном диапазоне:</p><br>',
    '#default_value' => 1,
    '#ajax' => array(
      'callback' => 'custom_forms_count_update',
      'event' => 'finishedinput', // custom jQuery event: https://drupal.stackexchange.com/a/123479
      'progress' => array(
        'message' => 'Поиск...',
        'type' => 'bar',
      ),
    ),
    // https://github.com/seiyria/bootstrap-slider
    '#attributes'    => array(
      'data-slider-min'  => $min,
      'data-slider-max'  => $max,
      'data-slider-value'  => $range,
      'data-slider-range'  => TRUE,
      'data-slider-ticks'  => $range,
      'data-slider-ticks-labels'  => '["Минимум (' . $min . ')", "Максимум (' . $max . ')"]',
      'data-slider-tooltip-split'  => TRUE,
    ),
    '#attached' => array(
      'css' => array(
        array(
          'data'  => 'https://cdnjs.cloudflare.com/ajax/libs/bootstrap-slider/6.0.16/css/bootstrap-slider.min.css',
          'type'  => 'external',
          'group' => CSS_THEME,
        ),
        array(
          'data'  => '.slider-selection{background:#d099d9}',
          'type'  => 'inline',
          'group' => CSS_THEME,
        ),
      ),
      'js' => array(
        array(
          'data'  => 'https://cdnjs.cloudflare.com/ajax/libs/bootstrap-slider/6.0.16/bootstrap-slider.min.js',
          'type'  => 'external',
          'scope' => 'header',
        ),
        array(
          'data'  => STARTUPSPARK_ROOT . '/js/slider.js',
          'scope' => 'footer',
        ),
      ),
    ),
  );

  // Здесь будут результаты.
  // http://complexdan.com?p=43
  $form['results'] = array(
    '#markup' => '',
    '#prefix' => '<div id="container-results">',
    '#suffix' => '</div>',
  );

  return $form;
}

function startupspark_add_startup_form($form, &$form_state) {

  $form['title']['#markup'] =
    '<h3>Участие в стартапе</h3><p><em>Можно одновременно участвовать только в одном стартапе.</em></p>';

  // https://api.drupal.org/api/drupal/developer!topics!forms_api_reference.html/7#fieldset
  $form['connect'] = array(
    '#type' => 'fieldset',
    '#title' => 'Присоединиться к существующему стартапу',
    '#collapsed' => FALSE,
  );
  // Autocomplete field
  $form['connect']['exist'] = array(
    '#type'              => 'textfield',
    '#size'              => 30,
    '#suffix' => '
      <span id="edit-exist" class="help-block">'
      . 'Если не помните название, вы можете воспользоваться '
      . l('поиском', '/search/projects', array('attributes' => array('target' => '_blank')))
      . '.</span>',
    '#attributes'        => array('placeholder' => array('Начните вводить название...')),
    '#description'       => 'Введите любое слово из названия',
    '#autocomplete_path' => 'search/projects/autocomplete/title',

    // sets up a callback that will fire every time the value of the field is changed.
    '#ajax' => array(
      'callback'         => 'custom_forms_startup_exist',
      'keypress'         => TRUE,
      'progress'         => array(
        'message'        => 'Поиск...',
        'type'           => 'bar',
      ),
    ),
  );

  // saving two lines of code: https://stackoverflow.com/q/8405386#comment35040665_8405715
  $form['connect']['results']['#markup'] = '<div id="container-results"></div>';

  $form['connect']['submit'] = array(
    '#type'       => 'submit',
    '#value'      => 'Присоединиться',
    '#attributes' => array('class' => array('btn-primary')),
    '#states'     => array(
      'enabled' => array(
        ':input[name="exist"]' => array('empty' => FALSE),
      ),
    ),
    '#validate'   => array('startupspark_connect_to_exist_form_validate'),
    '#submit'     => array('startupspark_connect_to_exist_form_submit'),
  );

  $form['new'] = array(
    '#type'   => 'fieldset',
    '#title'  => 'Создать новый стартап',
  );
  $form['new']['title'] = array(
    '#type'        => 'textfield',
    '#title'       => 'Название',
    '#description' => 'Короткое название, характеризующее ваш проект.',
    '#attributes'  => array('placeholder' => array('Например, Работающий вечный двигатель')),
    '#maxlength'   => 100,
  );
  $form['new']['description'] = array(
    '#type'        => 'textarea',
    '#title'       => 'Описание',
    '#description' => 'Подробно опишите ваш проект, чтобы о нём могло узнать как можно больше людей.',
    '#attributes'  => array('placeholder' => array('Миссия проекта, цели и т.д.')),
  );
  // Autocomplete field
  $form['new']['category'] = array(
    '#type'              => 'textfield',
    '#size'              => 80,
    '#title'             => 'Род деятельности',
    '#attributes'        => array('placeholder' => array('Например, спорт')),
    '#description'       => 'Род деятельности / категория. Или выберете из списка уже имеющихся.',
    '#autocomplete_path' => 'search/projects/autocomplete/category',
  );

  $form['new']['logo'] = array(
    '#type'               => 'managed_file',
    '#title'              => t('Logo'),
    '#theme'              => 'startupspark_thumb_upload',
    //'#required'           => TRUE,
    '#description'        => 'Допустимые форматы изображения: gif bmp png jpg jpeg',
    '#upload_location'    => 'public://startupspark/',
    '#upload_validators'  => array(
      'file_validate_size'             => array(variable_get('user_picture_file_size', 0.5) * 1024 * 1024),
      'file_validate_is_image'         => array(),
      'file_validate_extensions'       => array('gif bmp png jpg jpeg'),
      'file_validate_image_resolution' => array(variable_get('user_picture_dimensions', '85x85')),
    ),
  );
  //$form['#attributes'] = array('enctype' => "multipart/form-data");

  $form['new']['funder'] = array(
    '#type'        => 'textarea',
    '#title'       => 'Инвесторы',
    '#description' => 'Список инвесторов проекта.',
  );
  $form['new']['contact'] = array(
    '#type'        => 'textarea',
    '#title'       => 'Контактные данные',
    '#attributes'  => array(
      'placeholder' => array("Twitter, Inc.\n1355 Market Street, Suite 900\nSan Francisco, CA 94103\nP: (123) 456-7890")
    ),
    '#description' => 'Укажите контактные данные в свободной форме. В качестве подсказки вы можете придерживаться приведённого образца.',
  );

  $form['new']['submit'] = array(
    '#type'        => 'submit',
    '#value'       => 'Создать',
    '#suffix'      => '<p><em>Вы автоматически будете добавлены в список ключевых участников нового стартапа.</em></p>',
    '#validate'    => array('startupspark_new_startup_form_validate'),
    //'#submit'      => array('startupspark_add_startup_form_submit'),
    '#attributes'  => array('class' => array('btn-primary')),
  );

  return $form;
}

// Проверка полей формы на ошибки ввода
function startupspark_connect_to_exist_form_validate($form, &$form_state) {

  if (mb_strlen($form_state['values']['exist']) < 3) {
    form_set_error('exist', 'Слишком короткое название.');
  } else {

    // TODO: replace two db requests with single complex db request

    $startup = db_select(STARTUPSPARK_SCHEMA_PREFIX . 'startup', 's')
      ->fields('s', array('id'))
      ->condition('s.title', $form_state['values']['exist'])
      ->execute()
      ->fetchField();

    if(empty($startup)) {
      form_set_error('exist', 'Стартап с таким названием не найден! Проверьте правильность введённого названия.');
    } else {
      global $user;

      $num = db_select(STARTUPSPARK_SCHEMA_PREFIX . 'staff', 'sf')
        ->fields('sf')
        ->condition('sf.id_startup', $startup)
        ->condition('sf.id_user', $user->uid)
        ->execute()
        ->rowCount();

      if ($num > 0) {
        form_set_error('exist', 'Вы уже состоите в числе участников данного стартапа.');
      }
    }
  }
}

// Проверка полей формы на ошибки ввода
function startupspark_new_startup_form_validate($form, &$form_state) {

  if (mb_strlen($form_state['values']['title']) < 3) {
    form_set_error('title', 'Слишком короткое название.');
  } else {
    $startup = db_select(STARTUPSPARK_SCHEMA_PREFIX . 'startup', 's')
      ->fields('s', array('id'))
      ->condition('s.title', $form_state['values']['title'])
      ->execute()
      ->fetchField();

    if (!empty($startup)) {
      form_set_error('title', 'Стартап с таким названием уже существует.');
    }
  }
}

function startupspark_connect_to_exist_form_submit($form, &$form_state) {

  $startup = db_select(STARTUPSPARK_SCHEMA_PREFIX . 'startup', 's')
    ->fields('s', array('id'))
    ->condition('s.title', $form_state['values']['exist'])
    ->execute()
    ->fetchField();

  if(empty($startup)) {
    drupal_set_message('Стартап с таким названием не найден! Проверьте правильность введённого названия.', 'error', FALSE);
  } else {
    global $user;

    // db_merge - Insert a row in the db, or update the existing if it's already there.
    // http://dgo.to/310085, https://drupal.stackexchange.com/a/1350
    db_merge(STARTUPSPARK_SCHEMA_PREFIX . 'staff')
      ->insertFields(array(
        'id_user' => $user->uid,
        'id_startup' => $startup,
      ))
      ->updateFields(array(
        'id_startup' => $startup,
      ))
      ->key(array('id_user' => $user->uid))
      ->execute();

    $num_updated = db_update(STARTUPSPARK_SCHEMA_PREFIX . 'startup')
      ->expression('staff_count', 'staff_count + 1')
      ->condition('id', $startup)
      ->execute();

    if(empty($num_updated)) {
      drupal_set_message('Ошибка добавления в список участников стартапа.', 'error');
    }

    drupal_set_message('Настройки успешно обновлены.');
    drupal_goto('projects/' . $startup);
  }
}

/**
 * Submit the startup's addition form in the own project user page.
 *
 * @param $form
 *   General variable used in drupal, defining the structure & the fields of a
 *   form.
 * @param &$form_state
 *   General reference, used to control the processing of the form.
 */
function startupspark_add_startup_form_submit($form, &$form_state) {

  $values = $form_state['values'];

  // The managed file element handles moving the uploaded file for you,
  // so there's no need to call `file_save_upload()` manually.
  // https://stackoverflow.com/a/18807226

  // Load the file via file.fid.
  $file = file_load($values['logo']);
  // Важно помнить, что ф-я file_save_upload() загружает файл и помечает его как "Временный",
  // поэтому в submit-е формы нужно всегда изменять статус файла на "Постоянный"
  // (FILE_STATUS_PERMANENT) и вызывать file_save().
  // http://xandeadx.ru/blog/drupal/353
  // Change status to permanent.
  $file->status = FILE_STATUS_PERMANENT;
  $file = file_save($file); // Save.
  // Record that the module is using the file.
  // (Without the call to file_usage_add, file_managed_file_validate produces an error
  // upon saving the form, saying that the uploaded file may not be referenced.)
  file_usage_add($file, 'startupspark', 'user', 1); // attach the file to user 1: https://drupal.stackexchange.com/a/64492

  $startup = db_insert(STARTUPSPARK_SCHEMA_PREFIX . 'startup')
    ->fields(array(
      'name'        => sanitize($values['title']),
      'logo'        => $file->fid, //file_create_url($file->uri), // TODO: need to check when empty
      'title'       => $values['title'],
      'funder'      => $values['funder'],
      'contact'     => $values['contact'],
      'category'    => $values['category'],
      'description' => $values['description'],
      'staff_count' => 1,
    ))
    ->execute();

  if(empty($startup)) {
    drupal_set_message('Произошла ошибка при добавлении.', 'error');
  } else {
    global $user;

    db_merge(STARTUPSPARK_SCHEMA_PREFIX . 'staff')
      ->insertFields(array(
        'id_user' => $user->uid,
        'id_startup' => $startup,
      ))
      ->updateFields(array(
        'id_startup' => $startup,
      ))
      ->key(array('id_user' => $user->uid))
      ->execute();

    drupal_set_message('Стартап успешно добавлен.');
    drupal_goto('projects/' . $startup);
  }
}

/**
 * AJAX callback.
 */
function custom_forms_startup_exist($form, &$form_state) {

  if (empty($form_state['values']['exist'])) {
    // TODO: need to test
    return FALSE;
  }

  // execute the query
  $results = db_select(STARTUPSPARK_SCHEMA_PREFIX . 'startup', 's')
    ->fields('s')
    ->condition('s.title', $form_state['values']['exist'])
    ->range(0, 1)
    ->execute()
    ->fetchAllAssoc('title', PDO::FETCH_ASSOC);

  if (empty($results)) {
    // TODO: need to test
    return FALSE;
  }
  $output = '<h4>Краткая информация:</h4>'
    . theme('startupspark_list', array('content' => $results));

  $form['results'] = array(
    '#markup' => $output,
    '#prefix' => '<div id="container-results">',
    '#suffix' => '</div>',
  );

  $commands[] = ajax_command_replace('#container-results', drupal_render($form['results']));
  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * AJAX callback.
 *
 * http://complexdan.com?p=43
 */
function custom_forms_category_update($form, &$form_state) {

  // TODO: need to implement pager

  // execute the query
  $results = db_select(STARTUPSPARK_SCHEMA_PREFIX . 'startup', 's')
    ->fields('s', array('name', 'logo', 'title', 'description', 'category', 'view_count'))
    ->condition('s.category', '%' . db_like($form_state['values']['keyword']) . '%', 'LIKE')
    ->orderBy('s.view_count', 'DESC')
    ->execute()
    ->fetchAllAssoc('name', PDO::FETCH_ASSOC);

  $output = '<h3>Найдено стартапов: ' . sizeof($results) . '</h3>'
    . theme('startupspark_list', array('content' => $results));

  $form['results'] = array(
    '#markup' => $output,
    '#prefix' => '<div id="container-results">',
    '#suffix' => '</div>',
  );

  $commands[] = ajax_command_replace('#container-results', drupal_render($form['results']));
  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * AJAX callback.
 */
function custom_forms_count_update($form, &$form_state) {

  // TODO: need to implement pager

  // execute the query
  $results = db_select(STARTUPSPARK_SCHEMA_PREFIX . 'startup', 's')
    ->fields('s')
    // Использование оператора BETWEEN в условии: http://xandeadx.ru/blog/drupal/88
    ->condition('s.staff_count', explode(',', $form_state['values']['rangefield']), 'BETWEEN')
    ->orderBy('s.staff_count', 'DESC')
    ->execute()
    ->fetchAllAssoc('name', PDO::FETCH_ASSOC);

  // TODO: use plural function
  $output = '<h3>Найдено стартапов: ' . sizeof($results) . '</h3>'
    . theme('startupspark_list', array('content' => $results));

  $form['results'] = array(
    '#markup' => $output,
    '#prefix' => '<div id="container-results">',
    '#suffix' => '</div>',
  );

  $commands[] = ajax_command_replace('#container-results', drupal_render($form['results']));
  return array('#type' => 'ajax', '#commands' => $commands);
}

function startupspark_edit_startup_form($form, &$form_state, $startup = NULL) {

  drupal_set_title('');

  $form['header']['#markup'] = '<h3>Редактирование информации о стартапе</h3>';

  $form['startup_id'] = array(
    '#type'   => 'hidden',
    '#value'  => $startup->id,
  );
  $form['original_title'] = array(
    '#type'   => 'hidden',
    '#value'  => $startup->title,
  );

  $form['edit'] = array(
    '#type'   => 'fieldset',
    '#title'  => t('Edit'),
  );

  $form['edit']['title'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Name'),
    '#required'      => TRUE,
    '#attributes'    => array('placeholder' => array('Например, Работающий вечный двигатель')),
    '#maxlength'     => 80,
    '#default_value' => $startup->title,
    '#suffix' =>
      '<span id="edit-title" class="help-block"><span class="text-warning">' . 'Не изменяйте без необходимости! Изменение названия повлечёт изменение текущей ссылки на домашнюю страницу стартапа, а прежняя ссылка станет нерабочей (за исключением короткой ссылки вида "/projects/{id}", которая не зависит от названия).' . '</span></span>',
  );
  $form['edit']['description'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Description'),
    '#required'      => TRUE,
    '#maxlength'     => 512,
    '#attributes'    => array('placeholder' => array('Миссия проекта, цели и т.д.')),
    '#default_value' => $startup->description,
  );
  // Autocomplete field
  $form['edit']['category'] = array(
    '#type'              => 'textfield',
    '#size'              => 30,
    '#title'             => 'Род деятельности',
    '#required'          => TRUE,
    '#maxlength'         => 80,
    '#attributes'        => array('placeholder' => array('Например, спорт')),
    '#description'       => 'Род деятельности / категория. Или выберете из списка уже имеющихся.',
    '#default_value'     => $startup->category,
    '#autocomplete_path' => 'search/projects/autocomplete/category',
  );

  $form['edit']['logo'] = array(
    // `managed_file` element type handles most of image upload logic for you:
    // https://drupal.stackexchange.com/a/25518
    '#type'               => 'managed_file',
    '#title'              => t('Logo'),
    // assign #theme directly to the managed_file in other case it won't be
    // rebuilt after file upload: http://dgo.to/1673514/6696338
    '#theme'              => 'startupspark_thumb_upload',
    '#required'           => TRUE,
    '#description'        => 'Допустимые форматы изображения: gif bmp png jpg jpeg', // TODO: need also `svg`
    '#default_value'      => $startup->logo,
    //'#default_value'      => 2,
    '#upload_location'    => 'public://startupspark/',
    // Validate the uploaded picture.
    '#upload_validators'  => array(
      // Maximum file size in bytes.
      'file_validate_size'             => array(variable_get('user_picture_file_size', 0.5) * 1024 * 1024),
      // Validates file is really an image.
      'file_validate_is_image'         => array(),
      // Allowed extensions.
      'file_validate_extensions'       => array('gif bmp png jpg jpeg'),
      // https://drupal.stackexchange.com/a/35001
      'file_validate_image_resolution' => array(variable_get('user_picture_dimensions', '85x85')),
    ),
    // https://toster.ru/q/285260
    //'#attributes' => array(
    //  'multiple' => 'multiple',
    //),
  );

  $form['edit']['funder'] = array(
    '#type'          => 'textarea',
    '#title'         => 'Инвесторы',
    '#prefix'        => '<hr>',
    '#required'      => TRUE,
    '#maxlength'     => 512,
    '#description'   => 'Список инвесторов проекта.',
    '#default_value' => $startup->funder,
  );
  $form['edit']['contact'] = array(
    '#type'          => 'textarea',
    '#title'         => 'Контактные данные',
    '#maxlength'     => 512,
    '#required'      => TRUE,
    '#default_value' => $startup->contact,
  );

  $form['edit']['submit'] = array(
    '#type'   => 'submit',
    '#value'  => t('Save'),
    '#states' => array(
      'enabled' => array(
        ':input[name="title"]'       => array('empty' => FALSE),
        ':input[name="funder"]'      => array('empty' => FALSE),
        ':input[name="contact"]'     => array('empty' => FALSE),
        ':input[name="category"]'    => array('empty' => FALSE),
        ':input[name="description"]' => array('empty' => FALSE),
      ),
    ),
  );

  return $form;
}

// Проверка полей формы на ошибки ввода
function startupspark_edit_startup_form_validate($form, &$form_state) {

  if(mb_strlen($form_state['values']['title']) < 3) {
    form_set_error('title', 'Слишком короткое название.');
  } else {

    if($form_state['values']['title'] !== $form_state['values']['original_title']) {
      $startup = db_select(STARTUPSPARK_SCHEMA_PREFIX . 'startup', 's')
        ->fields('s', array('id'))
        ->condition('s.title', $form_state['values']['title'])
        ->execute()
        ->fetchField();

      if(!empty($startup)) {
        form_set_error('title', 'Стартап с таким названием уже существует.');
      }
    }
  }

  //form_error($form, t(''));
  //drupal_set_message('The file you have uploaded is: <pre>'.print_r($file, TRUE).'</pre>');

}

function startupspark_edit_startup_form_submit($form, &$form_state) {

  $values = $form_state['values'];

  if(!empty($values['logo'])) {
    // Загружаем файл (его объект)
    $file = file_load($values['logo']);
    // Сохраняем файл, если ему присвоен статус "временно".
    if (1 != $file->status) {
      // Статус (0 - временный, 1 - постоянный)
      $file->status = FILE_STATUS_PERMANENT;
      $file = file_save($file);
    }
    // Ошибка: 'Используемый в поле attach image файл не может быть использован': http://tsjee.ru/blog/68
    // Прибавляем единичку использования
    /**
     * $file - Объект файла
     * второй аргумент - название модуля
     * третий - тип сущности, node и т.д.
     * четвертый - идентификатор
     */
    file_usage_add($file, 'startupspark', 'startup', $values['startup_id']);
    //file_usage_add($file, 'startupspark', 'user', $file->uid);
    //file_usage_add($file, 'core', 'theme', $fid);
  }

  $num_updated = db_update(STARTUPSPARK_SCHEMA_PREFIX . 'startup')
    ->fields(array(
      'name'        => sanitize($form_state['values']['title']),
      'logo'        => $file->fid, //file_create_url($file->uri), // TODO: need to check when empty
      'title'       => $values['title'],
      'funder'      => $values['funder'],
      'contact'     => $values['contact'],
      'category'    => $values['category'],
      'description' => $values['description'],
      //'staff_count' => 1,
    ))
    ->condition('id', $values['startup_id'])
    ->execute();

  if(empty($num_updated)) {
    drupal_set_message('Информация о стартапе осталась без изменений.', 'warning');
  } else {
    drupal_set_message('Стартап успешно отредактирован.');
    drupal_goto('projects/' . $values['startup_id']);
  }
}
