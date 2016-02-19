<?php

/**
 * @file
 * Helper functions used in the module.
 */

/**
 * Helper function - check if theme is enabled.
 *
 * @link https://drupal.stackexchange.com/a/59037 @endlink
 *
 * @param string $theme_name
 *   The name of theme.
 *
 * @return bool
 */
function theme_exists($theme_name) {
  $themes = list_themes();
  return isset($themes[$theme_name]) && $themes[$theme_name]->status == 1;
}

/**
 * Helper function - sanitize.
 * Returns a sanitized string, typically for URLs.
 *
 * @link https://stackoverflow.com/a/2668953 @endlink
 *
 * @param string $string
 *   The string to sanitize.
 * @param bool $force_lowercase
 *   Force the string to lowercase?
 * @param bool $anal
 *   If set to *true*, will remove all non-alphanumeric characters.
 *
 * @return string
 */
function sanitize($string, $force_lowercase = TRUE, $anal = FALSE) {
  $strip = array("~", "`", "!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "_", "=", "+", "[", "{", "]",
    "}", "\\", "|", ";", ":", "\"", "'", "&#8216;", "&#8217;", "&#8220;", "&#8221;", "&#8211;", "&#8212;",
    "â€”", "â€“", ",", "<", ".", ">", "/", "?");
  $clean = trim(str_replace($strip, "", strip_tags($string)));
  $clean = preg_replace('/\s+/', "-", $clean);
  $clean = ($anal) ? preg_replace("/[^a-zA-Z0-9]/", "", $clean) : $clean ;
  return ($force_lowercase) ?
    (function_exists('mb_strtolower')) ?
      mb_strtolower($clean, 'UTF-8') :
      strtolower($clean) :
    $clean;
}
