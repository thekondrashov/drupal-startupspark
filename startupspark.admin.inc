<?php
/**
 * @file
 * Administrative page callbacks of the module.
 */

/**
 * Main administrative settings.
 */
function startupspark_admin_settings() {
  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings fieldset'),
  );
  $form['settings']['placeholder']['#markup'] = '<em>Эта страница ещё не создана.</em>';
  return system_settings_form($form);
}
