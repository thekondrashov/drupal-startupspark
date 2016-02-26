<?php

/**
 * @file
 * Theme functions.
 */

/**
 * Implements startupspark_thumb_upload theme callback.
 * Returns HTML for a managed file element with thumbnail.
 *
 * @link https://stackoverflow.com/a/31849073 @endlink
 * @link https://stackoverflow.com/a/32993171 @endlink
 */
function theme_startupspark_thumb_upload($variables) {

  // Hide/remove a upload/remove button from managed_file upload field:
  // http://writeyourcode.com?p=330
  //unset($variables['element']['upload_button'], $variables['element']['remove_button']);

  // To hide the upload button: https://drupal.stackexchange.com/a/32359
  //$element['upload_button']['#access'] = FALSE;

  $element = $variables['element'];
  $attributes = array();
  if (isset($element['#id'])) {
    $attributes['id'] = $element['#id'];
  }
  if (!empty($element['#attributes']['class'])) {
    $attributes['class'] = (array) $element['#attributes']['class'];
  }
  $attributes['class'][] = 'form-managed-file';
  $attributes['class'][] = 'media';
  $output = '';
  $output .= '<div' . drupal_attributes($attributes) . '>';
  if (isset($element['#file']->uri)) {
    $output .= '<div class="media-left">';
    $output .= '<img class="media-object img-thumbnail" src="' . image_style_url('thumbnail', $element['#file']->uri) . '" />';
    $output .= '</div>';
    $element['filename']['#markup'] .= ' <span class="file-size">(' . format_size($element['#file']->filesize) . ')</span> ';
    $element['filename']['#markup'] = '<div class="media-heading">' . $element['filename']['#markup'] . '</div>';
  }
  $output .= '<div class="image-widget-data media-body">';
  $output .= drupal_render_children($element);
  $output .= '</div>';
  $output .= '</div>';
  return $output;
}
