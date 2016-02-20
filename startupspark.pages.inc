<?php

/**
 * @file
 * Collection of module pages.
 */

/**
 * Page callback.
 */
function startupspark_project_page($startup, $action = NULL) {

  drupal_set_title('');

  if (!empty($action)) {
    switch ($action) {

      // redirect to `projects/%name`.
      case 'overview':
        return drupal_goto('projects/' . $startup->name);

      // TODO: Call edit form here.
      case 'edit':
        return 'Редактирование';

      // TODO: Call delete form here.
      case 'delete':
        return 'Удаление';

      // only listed above allowed. if no match, set 404 code.
      default:
        // Use exit() if you're seeing two error pages:
        // https://api.drupal.org/api/function/drupal_not_found/7#comment-23683
        drupal_not_found();
        exit;
    }
  }

  // Инкремент значения поля с количеством просмотров.
  db_update(STARTUPSPARK_SCHEMA_PREFIX . 'startup')
    ->expression('view_count', 'view_count + 1')
    ->condition('id', $startup->id)
    ->execute();


  $uids = db_select(STARTUPSPARK_SCHEMA_PREFIX . 'staff', 'sf')
    ->fields('sf', array('id_user'))
    ->condition('sf.id_startup', $startup->id)
    ->range(0, 10)
    ->execute()
    ->fetchCol();

  $rows = array();
  if(!empty($uids)) {
    $users = db_select('users', 'u')
      ->fields('u', array('name'))
      ->condition('u.uid', $uids, 'IN')
      ->execute()
      ->fetchCol();

    foreach ($users as $user) {
      $rows[] = array($user);
    }
  }

  $header = array(
    'Имя пользователя',
  );
  $output = theme(
    'table', array('header' => $header, 'rows' => $rows)
  );

  $variables = array(
    'startup' => $startup,
    'staff'   => $output,
  );
  $output = theme('startupspark-overview', $variables);
  return $output;
}

/**
 * Page callback.
 *
 * @link http://alvinalexander.com/drupal/drupal-7-form-theme-table-module-example A Drupal 7 form table example @endlink
 * @link http://drupalguide.ru/articles/database-api-drupal-7-vyborka-dannyh-i-ih-udalenie-dbselect-dbquery-i-dbdelete-chast-3 @endlink
 * @link http://knackforge.com/node/120 Creating pager in Drupal (without db_select() and PagerDefault) @endlink
 */
function startupspark_top_alltime_page($arg = NULL) {

  $per_page = 15;

  // execute the query
  $results = db_select(STARTUPSPARK_SCHEMA_PREFIX . 'startup', 's')
    ->extend('PagerDefault')
    ->fields('s')
    ->orderBy('s.view_count', 'DESC')
    ->limit($per_page)
    ->execute()
    // If set to PDO::FETCH_ASSOC, PDO::FETCH_NUM, or PDO::FETCH_BOTH
    // the returned value with be an array of arrays.
    // For any other value it will be an array of objects.
    // https://api.drupal.org/api/function/DatabaseStatementInterface::fetchAllAssoc/7
    ->fetchAllAssoc('name', PDO::FETCH_ASSOC);

  $output = theme('startupspark-discover', array('content' => $results));

  if (NULL !== $arg) {
    $page_level = pager_find_page() * $per_page;
    $range = ($page_level + 1) . '-' . ($page_level + $per_page);
    drupal_set_title(drupal_get_title() . ' #' . $range);

    $output .= theme('pager');
  }
  return $output;
}

/**
 * Autocomplete callback.
 * Function to get the autocomplete result.
 * $string = string for search
 *
 * @link http://dgo.to/854216 @endlink
 */
function json_list_projects($type, $string) {

  // http://timonweb.com/posts/how-to-create-an-ajax-autocomplete-textfield-in-drupal-7/#c318
  // Return the result to the form in json
  drupal_json_output(
    drupal_map_assoc(
      db_select(STARTUPSPARK_SCHEMA_PREFIX . 'startup', 's')
        ->fields('s', array($type))
        ->condition($type, '%' . db_like($string) . '%', 'LIKE')
        ->range(0, 5) // only 5 results will show
        ->orderby($type)
        ->execute()
        ->fetchCol()
    )
  );
}
