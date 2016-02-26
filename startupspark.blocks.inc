<?php

/**
 * @file
 * Block functions.
 */

/**
 * Implements hook_block_info().
 *
 * @link https://api.drupal.org/api/function/hook_block_info/7 @endlink
 */
function startupspark_block_info() {
  $blocks['startup_filter'] = array(
    'info'   => 'Filter of startups',
    // https://api.drupal.org/api/drupal/includes!common.inc/group/block_caching/7
    'cache'  => DRUPAL_NO_CACHE,
    'status' => 1,
    'region' => 'sidebar_first',
  );

  return $blocks;
}

/**
 * Implements hook_block_view().
 */
function startupspark_block_view($delta = '') {
  switch ($delta) {
    case 'startup_filter':

      $block['title'] = 'Фильтр';

      // Prevent strict warning: "Only variables should be passed by reference".
      // https://drupal.stackexchange.com/q/60411
      $form = drupal_get_form('form_search_projects_block');

      // Using this technique, you can encapsulate the js and css loading in your module_block_view
      // https://api.drupal.org/api/function/hook_block_info/7#comment-50868
      // https://api.drupal.org/api/function/drupal_add_css/7#comment-53463
      $block['content'] = array(
        '#markup' => drupal_render($form),
        '#attached' => array('css' => array(STARTUPSPARK_ROOT . '/css/custom.css')),
      );

      return $block;
  }
}
