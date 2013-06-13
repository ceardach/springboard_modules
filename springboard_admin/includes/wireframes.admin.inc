<?php

/**
 * @file
 * Stub pages for the administration pages that link to wireframes.
 */


/**
 * Page callback for the Marketing & Analytics page.
 */
function springboard_admin_analytics_page() {
  return '<img src="https://jacksonriver.mybalsamiq.com/mockups/904424.png?key=4ebfa64aedfa1d6558bfbc0db1aa168b79d6b6b2" />';
}

/**
 * Page callback for the config page.
 */
function springboard_admin_config_page() {
  return '<img src="https://jacksonriver.mybalsamiq.com/mockups/904429.png?key=4ebfa64aedfa1d6558bfbc0db1aa168b79d6b6b2" />';
}

/**
 * Page callback for the dashboard page.
 */
function springboard_admin_dashboard_page() {
  return '<img src="https://jacksonriver.mybalsamiq.com/mockups/913960.png?key=4ebfa64aedfa1d6558bfbc0db1aa168b79d6b6b2" />';
}

/**
 * Page callback for the donation forms overview page.
 */
function springboard_admin_donation_forms_page() {
  $output = '<p>Same behavior as "Forms" page.</p>';
  $output .= '<img src="https://jacksonriver.mybalsamiq.com/mockups/896615.png?key=4ebfa64aedfa1d6558bfbc0db1aa168b79d6b6b2" />';
  return $output;
}

/**
 * Page callback for the forms overview page.
 */
function springboard_admin_forms_page() {
  $output = '<p>Same behavior as "Donation Forms" page.</p>';
  $output .= '<img src="https://jacksonriver.mybalsamiq.com/mockups/896615.png?key=4ebfa64aedfa1d6558bfbc0db1aa168b79d6b6b2" />';
  return $output;
}

/**
 * Page callback for the results overview page.
 */
function springboard_admin_reports_page() {
  return '<img src="https://jacksonriver.mybalsamiq.com/mockups/904425.png?key=4ebfa64aedfa1d6558bfbc0db1aa168b79d6b6b2" />';
}

/**
 * Page callback for the templates page.
 */
function springboard_admin_templates_page() {
  return '<img src="https://jacksonriver.mybalsamiq.com/mockups/904423.png?key=4ebfa64aedfa1d6558bfbc0db1aa168b79d6b6b2" />';
}

/**
 * Helper to attach a wireframe to an existing page.
 *
 * When a page is partially formatted, this will attach the wireframe to the end
 * so we can see how the page should be fully formatted.
 */
function _springboard_admin_add_wireframes_to_page(&$page) {
  $section = arg(2);
  $subsection = arg(3);
  if (!empty($section) && empty($subsection)) {
    $pages_with_attachments = array(
      'config',
      'donation-forms',
      'forms',
      'reports',
    );
    if (in_array($section, $pages_with_attachments)) {
      $section = str_replace('-', '_', $section);
      $wireframe_function = 'springboard_admin_' . $section .'_page';
      $page['content']['system_main']['main']['#markup'] .= $wireframe_function();
    }
  }
}
