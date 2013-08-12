<?php

/**
 * @file
 * Config administration section of Springboard.
 */

/**
 * Page call back to list blocks for a subpage.
 */
function springboard_admin_block_page($path) {
  $blocks = array();
  $sql = "SELECT menu_name, mlid
    FROM {menu_links}
    WHERE link_path = :path";
  if ($admin = db_query($sql, array(':path' => 'springboard/' . $path))->fetchAssoc()) {
    $sql = "SELECT m.*, ml.*
      FROM {menu_links} ml
      INNER JOIN {menu_router} m ON ml.router_path = m.path
      WHERE menu_name = :menu_name
      AND ml.plid = :mlid
      AND hidden = 0";
    $result = db_query($sql, $admin, array('fetch' => PDO::FETCH_ASSOC));
    foreach ($result as $item) {
      _menu_link_translate($item);
      if (!$item['access']) {
        continue;
      }
      // The link description, either derived from 'description' in hook_menu()
      // or customized via menu module is used as title attribute.
      if (!empty($item['localized_options']['attributes']['title'])) {
        $item['description'] = $item['localized_options']['attributes']['title'];
        unset($item['localized_options']['attributes']['title']);
      }
      $block = $item;
      $block['content'] = theme('admin_block_content', array('content' => system_admin_menu_block($item)));
      if (!empty($block['content'])) {
        $block['show'] = TRUE;
      }

      // Prepare for sorting as in function _menu_tree_check_access().
      // The weight is offset so it is always positive, with a uniform 5-digits.
      if (isset($block['show']) && $block['show']) {
        $blocks[(50000 + $item['weight']) . ' ' . $item['title'] . ' ' . $item['mlid']] = $block;
      }
    }
  }
  if ($blocks) {
    ksort($blocks);
    return theme('admin_page', array('blocks' => $blocks));
  }
  else {
    return t('You do not have any administrative items.');
  }
}
