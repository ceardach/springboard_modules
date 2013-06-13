<?php

/**
 * @file
 * Implements the Springboard toolbar.
 */


/**
 * Implements hook_preprocess_page().
 */
function springboard_admin_preprocess_page(&$variables) {
  $arg_0 = arg(0);
  $arg_1 = arg(1);
  if ($arg_0 == 'admin' && empty($arg_1)) {
    $variables['theme_hook_suggestions'][] = 'springboard_toolbar';
    $variables['url'] = url('admin/springboard', array('absolute' => TRUE));
  }
}

/**
 * Page callback for the toolbar.
 */
function springboard_admin_toolbar() {
  drupal_add_css(drupal_get_path('module', 'springboard_admin') . '/includes/toolbar/springboard_toolbar.css');
  drupal_add_js(drupal_get_path('module', 'springboard_admin') . '/includes/toolbar/springboard_toolbar.js');
  return theme('springboard_toolbar_content');
}

/**
 * Theme callback for springboard_toolbar_content.
 */
function theme_springboard_toolbar_content() {
  $internal_options = $external_options = array(
    'absolute' => TRUE,
    'attributes' => array(
      'target' => 'springboard_admin-iframe',
    ),
  );
  $external_options['external'] = TRUE;
  $major_links = array(
    l('Springboard', 'admin/springboard', $internal_options),
    l('Drupal', '', $internal_options),
    l('Salesforce.com', 'https://login.salesforce.com', $external_options),
    l('MyEmail Tool', 'https://mail.google.com', $external_options),
    l('System Status', 'admin/reports/status', $internal_options),
    l('Billing', 'https://jacksonriver.harvestapp.com', $external_options),
    l('Help', 'admin/springboard/help', $internal_options),
  );
  $minor_links = array(
    l('My Profile', 'user', $internal_options),
    l('Administrators', 'admin/people', $internal_options),
    l('Settings', 'admin/springboard/config', $internal_options),
  );

  $output = '<div class="minor-links">';
  $output .= theme('item_list', array('items' => $minor_links));
  $output .= '</div><div class="major-links">';
  $output .= theme('item_list', array('items' => $major_links));
  $output .= '</div>';

  return $output;
}
