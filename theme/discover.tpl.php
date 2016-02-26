<?php

/**
 * @file
 * Define html code using variables provided by theme.
 *
 * @link https://getbootstrap.com/components/#media @endlink
 * @link http://bootply.com/1Tq35rlkbx Masonry CSS with Bootstrap @endlink
 */

?>

<div class="row" style="-moz-column-width:24em;-webkit-column-width:24em;-moz-column-gap:1em;-webkit-column-gap:1em;">

  <?php foreach($content as $row): ?>

    <div style="display:inline-block;padding:.25rem;width:100%;">
      <div class="thumbnail">
        <?php if (!empty($row['logo'])): ?>
          <img src="<?php print image_style_url('medium', file_load($row['logo'])->uri); ?>" alt="100%x200">
        <?php else: ?>
          <img style="height:200px;width:100%" alt="100%x200" src="https://imgholder.ru/200x200">
        <?php endif; ?>

        <div class="caption">
          <ol class="breadcrumb small">
            <li class="active">
              <span class="glyphicon glyphicon-eye-open" aria-hidden="true"></span>
              <?php if (!empty($row['view_count'])): print $row['view_count']; else: ?>0<?php endif; ?>
            </li>
            <li>
              <a href="javascript:void(0)">
                <span class="glyphicon glyphicon-bookmark" aria-hidden="true"></span>
                <?php if (!empty($row['category'])): print $row['category']; else: ?>
                  <em class="text-muted">категория не указана</em></em>
                <?php endif; ?>
              </a>
            </li>
          </ol>
          <div class="media">
            <div class="media-left">
              <img class="media-object img-thumbnail" alt="64x64" src="https://logo.clearbit.com/google.com?size=64" style="width:64px;height:64px;">
            </div>
            <div class="media-body">
              <h4 class="media-heading">
                <?php if (!empty($row['title'])): print $row['title']; else: ?>
                  <em class="text-muted">заголовок отсутствует</em>
                <?php endif; ?>
                <?php if (!empty($row['funder'])): ?>
                  <br>
                  <small><?php print $row['funder']; ?></small>
                <?php endif; ?>
              </h4>
            </div>
          </div>
          <p>
            <?php if (!empty($row['description'])): print $row['description']; else: ?>
              <em class="text-muted">описание отсутствует</em>
            <?php endif; ?>
          </p>
          <p>
            <a href="<?php
            if (!empty($row['name'])): print '/projects/' . $row['name'];
            else: ?>#<?php endif; ?>" class="btn btn-primary" role="button">
              <?php print t('Overview'); ?>
            </a>
          </p>
        </div>
      </div>
    </div>

  <?php endforeach; ?>

</div>
