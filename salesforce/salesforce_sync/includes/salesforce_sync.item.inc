<?php

/**
 * @file
 * Helper for constructing common objects. Although these classes could be used
 * to save a bit of time, this file is primarily intended for documentation.
 */

/**
 * A general controller that can be used when storing general sync items.
 */
class SalesforceSyncItemController extends DrupalDefaultEntityController implements DrupalEntityControllerInterface {

  /**
   * Constructor: sets basic variables.
   *
   * @param string $entity_type
   *   The specific type of sync item being loaded.
   */
  public function __construct($entity_type) {
    $this->entityType = $entity_type;
    $this->entityInfo = entity_get_info($entity_type);
    $this->entityCache = array();
    $this->hookLoadArguments = array();
    $this->idKey = $this->entityInfo['entity keys']['id'];
    $this->revisionKey = FALSE;
    $this->cache = !empty($this->entityInfo['static cache']);
  }

  /**
   * Implements DrupalEntityControllerInterface::resetCache().
   */
  public function resetCache(array $ids = NULL) {
    parent::resetCache($ids);
  }

  /**
   * Implements DrupalEntityControllerInterface::load().
   */
  public function load($ids = array(), $conditions = array()) {
    parent::load($ids, $conditions);
  }
}

/**
 * Helper class to build the base structure of a sync item.
 */
class SalesforceSyncItem extends Entity {



  /**
   * @name entity_class_overrides Override Entity methods
   * @{
   */

  /**
   * Override Entity::__construct to set up our preferences.
   */
  public function __construct(array $values = array(), $entity_type = NULL) {
    $this->object_type = '';
    $this->dedupe_field = NULL;
    $this->operation = NULL;
    $this->sync_rules = array();
    parent::__construct($values, $entity_type);
    if (!isset($this->sobject)) {
      $this->sobject = new SalesforceObject($this->object_type);
    }
  }

  /**
   * Override Entity::setUp() for when there is no declared entity.
   */
  protected function setUp() {
    $entity_info = entity_get_info($this->entityType);
    if ($entity_info) {
      parent::setUp();
    }
  }

  /**
   * Override Entity::save() for when there is no declared entity.
   */
  protected function save() {
    if ($this->entityInfo) {
      parent::save();
    }
    return FALSE;
  }

  /**
   * Override Entity::delete() for when there is no declared entity.
   */
  protected function delete() {
    if ($this->entityInfo) {
      parent::delete();
    }
    return FALSE;
  }

  /**
   * Override Entity::export() for when there is no declared entity.
   */
  protected function export() {
    if ($this->entityInfo) {
      parent::export();
    }
    return FALSE;
  }

  /**
   * Override Entity::view() for when there is no declared entity.
   */
  protected function view() {
    if ($this->entityInfo) {
      parent::view();
    }
    return FALSE;
  }

  /**
   * Override Entity::buildContent() for when there is no declared entity.
   */
  protected function buildContent() {
    if ($this->entityInfo) {
      parent::buildContent();
    }
    return FALSE;
  }

  /**
   * Override Entity::getTranslation() for when there is no declared entity.
   */
  protected function getTranslation() {
    if ($this->entityInfo) {
      parent::getTranslation();
    }
    return FALSE;
  }

  /**
   * @}
   */
}

/**
 * Helper class to build the base structure of a sObject.
 */
class SalesforceObject {

  /**
   * Unique identifier provided by Salesforce.
   *
   * @var string
   */
  public $Id;

  /**
   * Type of object in Salesforce, such as 'Contact' or 'Opportunity'.
   *
   * @var string
   */
  public $type;

  /**
   * Subtype of the object type in Salesforce.  Not all have a record.
   *
   * @var type
   */
  public $record;

  /**
   * Values of the fields for this record.
   *
   * The array is formatted as follows:
   *   <code>
   *     $fields = array(
   *       'FirstName' => 'John',
   *       'LastName' => 'Doe',
   *       'Email' => 'john.doe@example.com',
   *     );
   *   </code>
   *
   * @var array
   */
  public $fields = array();

  /**
   * Instructions to Salesforce for which fields to null.
   *
   * The array is an unassociated array of field names, such as:
   *   <code>
   *     $fieldsToNull = array(
   *       'FirstName',
   *       'LastName',
   *       'Email',
   *     );
   *   </code>
   *
   * @var type
   */
  public $fieldsToNull = array();

  /**
   * Constructs a sObject.
   *
   * @param array $fields
   *   An array of field names and values of the names for this record.  It is
   *   formatted as follows:
   *   <code>
   *     $fields = array(
   *       'FirstName' => 'John',
   *       'LastName' => 'Doe',
   *       'Email' => 'john.doe@example.com',
   *     );
   *   </code>
   * @param string $object_type
   *   The type of object the item is as it lives in Salesforce, such as
   *   'Contact' or 'Opportunity'.
   * @param string $record
   *   Subtype of the object type.
   */
  public function __construct($fields = array(), $object_type = '', $record = NULL) {
    $this->type = $object_type;
    $this->record = $record;
    $this->fields = $fields;
  }

}
