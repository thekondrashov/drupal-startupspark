/**
 * @file
 * Instantiate a slider (with JQuery).
 * https://github.com/seiyria/bootstrap-slider
 */

;(function ($) {
  $('input[name="rangefield"]')
    .slider()
    .on('slideStart', function(e) {
      // store start value
      $(this).data('oldRange', e.target.value);
    })
    .on('slideStop', function(e) {
      // compare new and previously stored value to prevent extra requests
      if ($(this).data('oldRange') !== e.target.value) {
        // custom jQuery event: https://drupal.stackexchange.com/a/123479
        $(this).trigger('finishedinput');
      }
    });
})(window.jQuery);
