<?php

/**
 * @file
 * Implements ctools multi step form in a reusable fashion for Springboard.
 */


/**
 * Create callback for standard ctools wizard.
 */
function springboard_admin_ctools_form_wizard($step, $info) {
  // Redirect to the first step if no step is provided.
  $steps = $info['steps'];
  reset($steps);
  $step1 = key($steps);
  if (empty($step)) {
    drupal_goto($info['form_path'] . '/' . $step1);
    return;
  }

  // Include required ctools files.
  ctools_include('wizard');
  ctools_include('object-cache');

  $form_info = array(
    'id' => 'springboard_admin_' . $info['form_name'] . '_form_wizard',
    'path' => $info['form_path'] . '/%step',
    'show trail' => TRUE,
    'show back' => TRUE,
    'show return' => FALSE,
    'free trail' => FALSE,
    'show cancel' => TRUE,
    'next text' => 'Save & Continue',
    'back text' => 'Previous',
    'return text' => 'Save',
    'finish text' => 'Complete',
    'cancel text' => 'Quit',
    'next callback' => 'springboard_admin_ctools_form_wizard_subtask_next',
    'finish callback' => 'springboard_admin_ctools_form_wizard_subtask_finish',
    'return callback' => 'springboard_admin_ctools_form_wizard_subtask_return',
    'cancel callback' => 'springboard_admin_ctools_form_wizard_subtask_cancel',
  );

  // Define the callbacks for the forms of each step.
  if (isset($info['forms'])) {
    $form_info['forms'] = $info['forms'];
  }
  else {
    foreach ($info['steps'] as $step_name => $title) {
      $form_info['forms'][$step_name] = array(
        'title' => $title,
        'form id' => $form_info['id'] . '_' . $step_name,
        'include' => $info['file'],
      );
    }
  }

  // Make cached data available within each step's $form_state array.
  $form_state = array(
    '#return_path' => $info['return_path'],
  );
  global $user;
  $form_state['#form_object_id'] = $form_info['id'] . '_' . $user->uid;
  $form_state['#form_object'] = springboard_admin_get_ctools_page_cache($form_state['#form_object_id']);

  // Retreive the form as a Ctools multi-step form.
  $form = ctools_wizard_multistep_form($form_info, $step, $form_state);

  // Manually add the cTools trail.
  // If free trail, then next button doesn't work, but without free trail then
  // the links to the forms won't show up.  Put the forms here manually to
  // compensate.  This is fortunate, because now we can call our own custom
  // theme function.
  $links= array();
  $previous = TRUE;
  foreach ($form_state['form_info']['order'] as $id => $title) {
    if ($id == $form_state['step']) {
      $previous = FALSE;
      $class = 'wizard-trail-current';
    }
    elseif ($previous) {
      $not_first = TRUE;
      $class = 'wizard-trail-previous';
    }
    else {
      $class = 'wizard-trail-next';
    }
    $path = ctools_wizard_get_path($form_info, $id);
    $links[] = array(
      'path' => $form_info['path'],
      'href' => $path[0],
      'title' => $title,
      'options' => array(
        'class' => $class,
      ),
      'localized_options' => array(),
      'active' => $class == 'wizard-trail-current' ? TRUE : FALSE,
    );
  }
  $form['ctools_trail'] = array(
    '#markup' => theme(array('springboard_admin_ctools_wizard_tabs__' . $form_info['id'], 'springboard_admin_ctools_wizard_tabs'), array('links' => $links)),
    '#weight' => -1000,
  );

  // Move the cancel button to be positioned first.
  $form['buttons']['cancel']['#weight'] = -3000;

  return $form;
}

/**
 * Theme call back to render multi form trail as tabs.
 */
function theme_springboard_admin_ctools_wizard_tabs(&$variables) {
  $links = array();
  foreach ($variables['links'] as $link) {
    $links[] = array(
      '#theme' => 'menu_local_task',
      '#link' => $link,
      '#active' => !empty($link['active']) ? TRUE : FALSE,
    );
  }
  return '<div class="tabs">' . theme('menu_local_tasks', array('primary' => $links)) . '</div>';
}

/**
 * Provides default behavior to occur on any form submitted within a wizard.
 */
function springboard_admin_save_values_to_cache($form, &$form_state) {
  if (isset($form_state['input']) && is_array($form_state['input'])) {
    ctools_include('wizard');
    ctools_include('object-cache');
    foreach ($form_state['input'] as $key => $value) {
      $form_state['#form_object']->$key = $value;
    }
    ctools_object_cache_set('springboard_admin', $form_state['#form_object_id'], $form_state['#form_object']);
  }
}

function springboard_admin_ajax_save($form, &$form_state) {
  springboard_admin_save_values_to_cache($form, $form_state);
}


/**
 * @name springboard_admin_ctools_cache Ctools cache callback for form wizard
 * @{
 */

/**
 * Retreives an object from the cache.
 *
 * @param string $name
 *  The name of the cached object to retreive.
 */
function springboard_admin_get_ctools_page_cache($name) {
  ctools_include('object-cache');
  $cache = ctools_object_cache_get('springboard_admin', $name);

  // If the cached object doesn't exist yet, create an empty object.
  if (!$cache) {
    $cache = new stdClass();
  }

  return $cache;
}

/**
 * Creates or updates an object in the cache.
 *
 * @param string $name
 *  The name of the object to cache.
 *
 * @param object $data
 *  The object to be cached.
 */
function springboard_admin_set_ctools_page_cache($name, $data) {
  ctools_include('object-cache');
  ctools_object_cache_set('springboard_admin', $name, $data);
}

/**
 * Removes an item from the object cache.
 *
 * @param string $name
 *  The name of the object to destroy.
 */
function springboard_admin_clear_ctools_page_cache($name) {
  ctools_include('object-cache');
  ctools_object_cache_clear('springboard_admin', $name);
}

/**
 * @} springboard_admin_ctools_cache
 */


/**
 * @name springboard_admin_ctools_subtasks Subtask callbacks for form wizard
 * @{
 */

/**
 * Callback executed when the 'next' button is clicked.
 */
function springboard_admin_ctools_form_wizard_subtask_next(&$form_state) {
  // No default behavior
}

/**
 * Callback executed when the 'cancel' button is clicked.
 */
function springboard_admin_ctools_form_wizard_subtask_cancel(&$form_state) {
  springboard_admin_clear_ctools_page_cache($form_state['#form_object_id']);

  // Redirect the user to the provided return path.
  drupal_goto($form_state['#return_path']);
}

/**
 * Callback to use when entire form is completed.
 */
function springboard_admin_ctools_form_wizard_subtask_finish(&$form_state) {
  springboard_admin_clear_ctools_page_cache($form_state['#form_object_id']);

  // Redirect the user to the provided return path.
  drupal_goto($form_state['#return_path']);
}

/**
 * Callback to use when user clicks final submit button.
 */
function springboard_admin_ctools_form_wizard_subtask_return(&$form_state) {
  springboard_admin_clear_ctools_page_cache($form_state['#form_object_id']);

  // Redirect the user to the provided return path.
  drupal_goto($form_state['#return_path']);
}

/**
 * @} springboard_admin_ctools_subtasks
 */
