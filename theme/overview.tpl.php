<?php

/**
 * @file
 * Define html code using variables provided by theme.
 * You shouldn't have php logic in template files.
 *
 * @link https://github.com/aramboyajyan/drupal7-boilerplate/blob/master/theme/sample-template.tpl.php @endlink
 * @link https://drupal.stackexchange.com/a/123147 @endlink
 */
?>

<div class="row">
  <div class="col-xs-12 col-md-3 col-md-push-9">
    <?php if (!empty($startup->logo) && FALSE === stristr($startup->logo, 'data:image/')): ?>
      <img src="<?php print image_style_url('thumbnail', file_load($startup->logo)->uri); ?>" alt="..." class="img-responsive img-thumbnail" style="width:100%">
    <?php else: ?>
      <img src="https://imgholder.ru/300x300" alt="..." class="img-responsive img-thumbnail" style="width:100%">
    <?php endif ?>
  </div>
  <div class="col-xs-12 col-md-9 col-md-pull-3">
    <div class="page-header">
      <?php if (!empty($startup->title)): ?>
        <h1><?php print $startup->title; ?></h1>
      <?php else: ?>
        <h1><small><em>Нет названия</em></small></h1>
      <?php endif ?>
    </div>
    <?php if (!empty($startup->description)): ?>
      <p class="lead"><?php print $startup->description; ?></p>
    <?php else: ?>
      <p class="lead text-muted"><em>Описание отсутствует</em></p>
    <?php endif ?>
  </div>
</div>

<hr>
<div class="panel-group" role="tablist">
  <div class="panel panel-default">
    <div class="panel-heading" role="tab">
      <h4 class="panel-title">
        Участники
        <?php if (!empty($startup->staff_count)): ?>
          <span class="badge"><?php print $startup->staff_count; ?></span>
        <?php endif ?>
      </h4>
    </div>
    <div class="panel-collapse collapse in" role="tabpanel">
      <div class="panel-body">
        <?php if (!empty($staff)): print $staff; endif; ?>
      </div>
    </div>
  </div>
  <div class="panel panel-default">
    <div class="panel-heading" role="tab">
      <h4 class="panel-title">
        Инвесторы
      </h4>
    </div>
    <div class="panel-collapse collapse in" role="tabpanel">
      <div class="panel-body">
        <?php if (!empty($startup->funder)): ?>
          <pre><?php print $startup->funder; ?></pre>
        <?php else: ?>
          <p class="lead text-muted"><em>Инвесторы отсутствуют</em></p>
        <?php endif ?>
      </div>
    </div>
  </div>
  <div class="panel panel-default">
    <div class="panel-heading" role="tab" id="headingThree">
      <h4 class="panel-title">
        <a class="collapsed" role="button" data-toggle="collapse" data-parent="#accordion" href="#collapseThree" aria-expanded="false" aria-controls="collapseThree">
          Контактные данные
        </a>
      </h4>
    </div>
    <div id="collapseThree" class="panel-collapse collapse" role="tabpanel" aria-labelledby="headingThree">
      <div class="panel-body">
        <?php if (!empty($startup->contact)): ?>
          <pre><?php print $startup->contact; ?></pre>
        <?php else: ?>
          <p class="lead text-muted"><em>Контактные данные отсутствуют</em></p>
        <?php endif ?>
      </div>
    </div>
  </div>
</div>
