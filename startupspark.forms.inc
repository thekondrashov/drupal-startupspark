<?php

/**
 * @file
 * Custom forms used in the module.
 */

/**
 * Page placeholder.
 */
function startupspark_placeholder_form($form, &$form_state) {
  $form['placeholder']['#markup'] = '<em>Эта страница ещё не создана</em>';
  return $form;
}

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
