<?php

/**
 * @file
 * Implements a donation form wizard.
 */

module_load_include('inc', 'springboard_admin', 'includes/wizard/wizard.api');

/**
 * Generate a multistep form based on springboard_admin wizard.api
 */
function springboard_admin_donation_form_wizard($step) {
  $info = array(
    'form_name' => 'donation',
    'form_path' => 'admin/springboard/donation-forms/wizard',
    'return_path' => 'admin/springboard/donation-forms',
    'steps' => array(
      'setup' => t('Step 1: Set up'),
      'confirmations' => t('Step 2: Confirmations'),
      'customize' => t('Step 3: Customize'),
      'check' => t('Step 4: Check'),
      'advanced' => t('Advanced (optional)'),
    ),
    'file' => drupal_get_path('module', 'springboard_admin') . '/includes/donation_wizard/donation_form.wizard.inc',
  );
  return springboard_admin_ctools_form_wizard($step, $info);
}


/**
 * @name springboard_admin_donation_forms Forms in the donation form wizard
 * @{
 */

/**
 * Form for 'Set Up'
 */
function springboard_admin_donation_form_wizard_setup($form, &$form_state) {
  $form['setup'] = array(
    '#type' => 'textfield',
    '#title' => t('Set up'),
    '#default_value' => 'Step 1',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => FALSE,
//    '#ajax' => array(
//      'callback' => 'springboard_admin_ajax_save',
//    ),
  );
  return $form;
}

/**
 * Form for 'Confirmations'
 */
function springboard_admin_donation_form_wizard_confirmations($form, &$form_state) {
  $form['confirmations'] = array(
    '#type' => 'textfield',
    '#title' => t('Confirmations'),
    '#default_value' => 'Step 2',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => FALSE,
  );
  return $form;
}

/**
 * Form for 'Customize'
 */
function springboard_admin_donation_form_wizard_customize($form, &$form_state) {
  $form['customize'] = array(
    '#type' => 'textfield',
    '#title' => t('Customize'),
    '#default_value' => 'Step 3',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => FALSE,
  );
  return $form;
}

/**
 * Form for 'Check'
 */
function springboard_admin_donation_form_wizard_check($form, &$form_state) {
  $form['check'] = array(
    '#type' => 'textfield',
    '#title' => t('Check'),
    '#default_value' => 'Step 4',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => FALSE,
  );
  return $form;
}

/**
 * Form for 'Advanced'
 */
function springboard_admin_donation_form_wizard_advanced($form, &$form_state) {
  $form['advanced'] = array(
    '#type' => 'textfield',
    '#title' => t('Advanced'),
    '#default_value' => 'Step 5',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => FALSE,
  );
  return $form;
}

/**
 * @} springboard_admin_donation_forms
 */


/**
 * @name springboard_admin_donation_form_submit Submit callback for Donation forms
 * @{
 */

/**
 * Form for 'Set Up'
 */
function springboard_admin_donation_form_wizard_setup_submit($form, &$form_state) {
  dsm('submit');
  springboard_admin_save_values_to_cache($form, $form_state);
}

/**
 * Form for 'Confirmations'
 */
function springboard_admin_donation_form_wizard_confirmations_submit($form, &$form_state) {
  springboard_admin_save_values_to_cache($form, $form_state);
}

/**
 * Form for 'Customize'
 */
function springboard_admin_donation_form_wizard_customize_submit($form, &$form_state) {
  springboard_admin_save_values_to_cache($form, $form_state);
}

/**
 * Form for 'Check'
 */
function springboard_admin_donation_form_wizard_check_submit($form, &$form_state) {
  springboard_admin_save_values_to_cache($form, $form_state);
}

/**
 * Form for 'Advanced'
 */
function springboard_admin_donation_form_wizard_advanced_submit($form, &$form_state) {
  springboard_admin_save_values_to_cache($form, $form_state);
}

/**
 * @} springboard_admin_donation_form_submit
 */
