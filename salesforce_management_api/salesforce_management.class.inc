<?php

/**
 * @file
 * Defines the class used for communicating with the Salesforce server.
 */

// Include the Salesforce Enterprise Client included in the PHP Toolkit.
require(SALESFORCE_DIR_SOAPCLIENT .'/SforceEnterpriseClient.php');

/**
 * Defines the DrupalSalesforce class used to interact with the server.
 */
class DrupalSalesforce {
  // Define the object object used to connect to Salesforce.
  public $client;

  // Define variables for the API credentials.
  public $username = '';
  public $password = '';
  public $token = '';

  /**
   * Constructs a DrupalSalesforce object with a set of API credentials.
   *
   * @param $username
   *   The username used to login to Salesforce; should be an e-mail address.
   * @param $password
   *   The password used to login to Salesforce.
   * @param $token
   *   The security token appended to the password for logging in via the web
   *     service. Set by logging into Salesforce and navigating to Setup >
   *     My Personal Information > Reset My Security Token.
   */
  function __construct($username, $password, $token) {
    // Set the appropriate member values.
    $this->username = $username;
    $this->password = $password;
    $this->token = $token;

    // Create the client object.
    $this->client = new SforceEnterpriseClient();
  }

  /**
   * Establishes a connection and logs in to the Salesforce server.
   *
   * @return
   *   TRUE or FALSE indicating the success of the login.
   */
  function login() {
    // Default to the WSDL downloaded from the user account if available.
    $wsdl = SALESFORCE_DIR_WSDL .'/enterprise.wsdl.xml';

    // Otherwise fall back to the one included with the Toolkit.
    if (!file_exists($wsdl)) {
      $wsdl = SALESFORCE_DIR_SOAPCLIENT .'/enterprise.wsdl.xml';
    }

    // Connect to the server and login, logging any failures to the watchdog.
    try {
      // Connect to the server.
      $this->client->createConnection($wsdl);

      // Attempt a login with the credentials entered by the user.
      $login = $this->client->login($this->username, $this->password . $this->token);

      // Log the login occurence.
      $this->watchdog(SALESFORCE_LOG_ALL, t('@user (@email) logged into Salesforce.', array('@user' => $login->userInfo->userFullName, '@email' => $login->userInfo->userEmail)));
    }
    catch (Exception $e) {
      // Log the error message.
      $this->watchdog(SALESFORCE_LOG_SOME, t('Could not login to Salesforce.') . 
        '<br>Request: <pre>'. check_plain($this->client->getLastRequest()) .'</pre><br>' .
        'Error: <pre>'. check_plain($e->faultstring) .'</pre>', WATCHDOG_ERROR);

      // Indicate the failed login.
      return $e->getMessage();
    }

    // Indicate the successful login.
    return TRUE;
  }

  /**
   * Logs a message to the watchdog based on the Salesforce log settings.
   *
   * @param $level
   * @param $message
   * @param $severity
   * @param $link
   */
  function watchdog($level, $message, $severity = WATCHDOG_NOTICE, $link = NULL) {
    // Log nothing for notices if the related log level is not greater than or
    // equal to the level of this message.
    switch ($severity) {
      case WATCHDOG_NOTICE:
        if (variable_get('salesforce_management_api_activity_log', SALESFORCE_LOG_SOME)  < $level) {
          return;
        }
        break;

      case WATCHDOG_WARNING:
      case WATCHDOG_ERROR:
        if (variable_get('salesforce_management_api_error_log', SALESFORCE_LOG_ALL)  < $level) {
          return;
        }
        break;

      default:
        break;
    }

    // Log the message to the watchdog.
    watchdog('salesforce', $message, array(), $severity, $link);
  }

  /**
   * Retrieves an object from Salesforce with standard fields and any data in
   *   fields defined in the fieldmap object.
   *
   * @param $ids
   *   An array of Salesforce IDs for the objects to retrieve.
   * @param $fieldmap
   *   The fieldmap through which to filter the data.
	 * @param $extra_fields
	 *	 An array of extra fields to retreive that are not part of the fieldmap
   * @return
   *   The single matching Salesforce objects or an array of all the objects
   *     if more than one are returned.
   */
  function retrieve($ids, $fieldmap, $extra_fields = NULL) {
    // Load the fieldmap so we can get the object name.
    $map = salesforce_management_api_fieldmap_load($fieldmap);

    $object = salesforce_management_api_fieldmap_objects_load('salesforce', $map['salesforce']);

    $fields = array_keys($object['fields']);
		
		// apend extra fields if available
		if (!empty($extra_fields)) {
			foreach($extra_fields as $field) {
				if (!in_array($field, $fields)) {
					$fields[] = $field;
				}
			}
		}

    return $this->client->retrieve(implode(', ', $fields), $map['salesforce'], $ids);
  }
}
