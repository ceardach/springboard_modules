<?php

/**
 *  @file Form arrays, validate and submit handlers
 */

function quick_donate_register($edit, $nid, $sid) {
  
  $fundraiser_data = _quick_donate_fundraiser_order_data(array('sid' => $sid));
 
  $order_id = $fundraiser_data['order_id'];
  $order = uc_order_load($order_id);
  $data = serialize($order->data);

  $uid = $order->uid;
  $ref_token = _quick_donate_get_token_from_order($order);

  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $nid,
  );
  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $uid,
  );
  $form['sid'] = array(
    '#type' => 'value',
    '#value' => $sid,
  );
  $form['order_id'] = array(
    '#type' => 'value',
    '#value' => $fundraiser_data['order_id'],
  );
  $form['gateway_id'] = array(
    '#type' => 'value',
    '#value' => $order->data['gateway'],
  );
  $form['ref_token'] = array(
    '#type' => 'value',
    '#value' => $ref_token,
  );
  $form['data'] = array(
    '#type' => 'value',
    '#value' => $data,
  );
  $form['quick_donate'] = array(
    '#type' => 'fieldset',
    '#title' => t('Register for quick donation.'),
  );
  $form['quick_donate']['quick_donate_message'] = array(
    '#type' => 'markup',
    '#value' => variable_get('quick_donate_registration_message', t('The quick donate service allows you to streamline the donation process by replacing tedious forms with a single one-click donation experience. Quick Donate also allows you to bypass the website entirely, permitting you to conveniently donate from your phone via SMS.')),
    '#prefix' => '<p>',
    '#suffix' => '</p>',
  );
  $form['quick_donate']['register'] = array(
    '#type' => 'checkbox',
    '#title' => t('Register me for quick donation'),
  );
  $form['quick_donate']['phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Phone'),
    '#required' => TRUE,
  );
  $form['quick_donate']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Register now'),
  );
  $form['#validate'][] = 'quick_donate_validate_registration';
  $form['#submit'][] = 'quick_donate_submit_registration';
  return $form;
}

/**
 * Validation callback for confirmation page registration form.
 */
function quick_donate_validate_registration(&$form, $form_state) {

}

/**
 * Submit callback for confirmation page registration form.
 */
function quick_donate_submit_registration($form, $form_state) {
  $uid = $form_state['values']['uid'];
  $gateway_id = $form_state['values']['gateway_id'];
  $ref_token = $form_state['values']['ref_token'];
  $order_id = $form_state['values']['order_id'];
  $sid = $form_state['values']['sid'];
  $data = $form_state['values']['data'];
  quick_donate_register_user($uid, 1);
  quick_donate_save_payment_method($uid, $gateway_id, $ref_token, $order_id, $sid, $data);

}

function quick_donate_payment_methods($account) {

  $payment_methods = quick_donate_load_payment_methods($account->uid);
  foreach ($payment_methods as $payment_method) {
    $content .= theme('quick_donate_payment_method', $payment_method);
  }
  return theme_box('Payment Methods', $content);
}

/**
 * Payment method edit form.
 */
function quick_donate_edit_payment_method($edit, $user, $method) {

  $month_options = array(
    '1' => t('January'),
    '2' => t('February'),
    '3' => t('March'),
    '4' => t('April'),
    '5' => t('May'),
    '6' => t('June'),
    '7' => t('July'),
    '8' => t('August'),
    '9' => t('September'),
    '10' => t('October'),
    '11' => t('November'),
    '12' => t('December'),
  );
  $year = date('Y');
  for ($i = 0; $i < 10; ++$i) {
    $year_options[$year + $i] = $year + $i;
  }
  
  $countries = uc_country_select(uc_get_field_name('country'), 840, NULL, 'name', uc_address_field_required('country'));
  $zones = uc_zone_select(uc_get_field_name('zone'), $arg1->billing_zone, NULL, 840, 'name', uc_address_field_required('zone'));
  $form['gateway_type'] = array(
    '#type' => 'value',
    '#value' => $method['gateway_type'],
  );
  $form['ref_id'] = array(
    '#type' => 'value',
    '#value' => $method['ref_token'],
  );
  $form['billing_first_name'] = array(
    '#type' => 'textfield',
    '#title' => t('First name'),
    '#description' => t(''),
    '#size' => 20,
    '#required' => TRUE,
    '#default_value' => $method['billing_first_name'],
  );
  $form['billing_last_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Last Name'),
    '#description' => t(''),
    '#size' => 20,
    '#required' => TRUE,
    '#default_value' => $method['billing_last_name'],
  );
  $form['billing_street1'] = array(
    '#type' => 'textfield',
    '#title' => t('Address 1'),
    '#description' => t(''),
    '#required' => TRUE,
    '#default_value' => $method['billing_street1'],
  );
  $form['billing_street2'] = array(
    '#type' => 'textfield',
    '#title' => t('Address 2'),
    '#description' => t('Apt or suite #'),
    '#required' => TRUE,
    '#default_value' => $method['billing_street2'],
  );
  $form['billing_city'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#description' => t(''),
    '#size' => 30,
    '#required' => TRUE,
    '#default_value' => $method['billing_city'],
  );
  $form['billing_zone'] = array(
    '#type' => 'select',
  	'#title' => 'State',
    '#options' => $zones['#options'],
    '#required' => TRUE,
  	'#default_value' => $method['billing_zone'],
  );
  $form['billing_country'] = array(
    '#type' => 'select',
  	'#title' => 'Country',
    '#options' => $countries['#options'],
    '#required' => TRUE,
  	'#default_value' => $method['billing_country'],
  );
  $form['billing_postal_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Zipcode'),
    '#description' => t(''),
    '#size' => 10,
    '#required' => TRUE,
    '#default_value' => $method['billing_postal_code'],
  );
  $form['cc_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Credit card number'),
    '#description' => t(''),
    '#required' => TRUE,
  );
  $form['cc_exp'] = array(
    '#type' => 'fieldset',
    '#title' => t('Expiration date'),
  );
  $form['cc_exp']['exp_month'] = array(
    '#type' => 'select',
    '#title' => t(''),
    '#options' => $month_options,
    '#required' => TRUE,
  );
  $form['cc_exp']['exp_year'] = array(
    '#type' => 'select',
    '#title' => t(''),
    '#options' => $year_options,
    '#required' => TRUE,
  );
  $form['cc_cvv'] = array(
    '#type' => 'textfield',
    '#title' => t('CVV'),
    '#description' => t('This is the three digit code on the back of your card'),
    '#size' => 3,
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['#validate'][] = 'quick_donate_validate_edit_payment_method';
  $form['#submit'][] = 'quick_donate_submit_edit_payment_method';
  return $form;
}


// TODO: tweak validation paths to error correct form elements, lookup definitions for helper functions and module_load_include()
// parent file.
function quick_donate_validate_edit_payment_method(&$form, &$form_state) {

  $values = $form_state['values'];
  // validate credit card information
  $cc_number = $values['cc_number'];
  $cc_expiration_month =  $values['exp_month'];
  $cc_expiration_year = $values['exp_year'];
  $cc_cvv = $values['cc_cvv'];
  
  //validate cc number
  if ((variable_get('uc_credit_validate_numbers', TRUE) && !_valid_card_number($cc_number)) || !ctype_digit($cc_number)) {
    form_set_error('cc_number',"You have entered an invalid credit card number.");
  }

  // Validate the card expiration date.
  if (!_valid_card_expiration($cc_expiration_month, $cc_expiration_year)) {
    form_set_error('cc_exp][card_exp_date',t('The credit card you entered has expired.'));
  }
  else {
    $form_state['values']['cc_exp_date'] = array(
      'day' => 1,
      'month' => $values['exp_month'],
      'year' => $values['exp_year'],
    );
  }

  // Validate the CVV Number
  if (variable_get('uc_credit_cvv_enabled', TRUE) && !_valid_cvv($cc_cvv)) {
    form_set_error('cc_cvv',t('You have entered an invalid CVV number.'));
  }
}

function quick_donate_submit_edit_payment_method($form, $form_state) {
  module_load_include('inc', 'fundraiser', 'fundraiser_billing');
  $values = $form_state['values'];
  
  $billing_info = array(
    'master_order_id' =>  $values['order_id'],
    'first_name' => $values['billing_first_name'],
    'last_name' => $values['billing_last_name'],
    'email' => $values['email'],
    'address_1' => $values['billing_address1'],
    'address-2' => $values['billing_address2'],
    'city' => $values['billing_city'],
    'country' => $values['billing_country'],
    'state' => $values['billing_zone'],
    'zipcode' => $values['billing_postal_code'],
    'card_num' => $values['cc_number'],
    'card_exp_date' => $values['cc_exp_date'],
    'card_cvv' => $values['cc_cvv'],
  );
  $ret = fundraiser_update_billing_info($values['gateway_type'], $values['ref_id'], $billing_info);
  drupal_set_message('gateway update function return values:' . print_r($ret, TRUE));
}