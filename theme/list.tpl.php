<?php

/**
 * @file
 * Define html code using variables provided by theme.
 */

?>

<div class="container-fluid">

  <?php foreach($content as $row): ?>

    <div class="row">
      <div class="media">
        <div class="media-left">
          <a href="#">
            <?php if (!empty($row['logo'])): ?>
              <img src="<?php print $row['logo']; ?>" style="width:100px;height:100px;" alt="100x100">
            <?php else: ?>
              <img style="height:100px;width:100px;" alt="100x100" src="https://imgholder.ru/100x100">
            <?php endif; ?>
          </a>
        </div>
        <div class="media-body">
          <div class="btn-group btn-group-xs" role="group" aria-label="Extra-small button group">
            <a href="#" class="btn btn-default disabled" role="button">
              <span class="glyphicon glyphicon-eye-open" aria-hidden="true"></span>
              <?php if (!empty($row['view_count'])): print $row['view_count']; else: ?>0<?php endif; ?>
            </a>
            <a href="javascript:void(0)" class="btn btn-default disabled" role="button">
              <span class="glyphicon glyphicon-bookmark" aria-hidden="true"></span>&nbsp;
              <?php if (!empty($row['category'])): print $row['category']; else: ?>
                <em class="text-muted">категория не указана</em></em>
              <?php endif; ?>
            </a>
          </div>
          <h4>
            <a href="<?php
            if (!empty($row['name'])): print '/projects/' . $row['name'];
            else: ?>#<?php endif; ?>">
              <?php if (!empty($row['title'])): print $row['title']; else: ?>
                <em class="text-muted">заголовок отсутствует</em>
              <?php endif; ?>
            </a>
          </h4>

          <?php if (!empty($row['description'])): print $row['description']; else: ?>
            <em class="text-muted">описание отсутствует</em>
          <?php endif; ?>
        </div>
      </div>

      <hr>
    </div>

  <?php endforeach; ?>

</div>
