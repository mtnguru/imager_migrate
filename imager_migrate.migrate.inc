<?php
/**
 * @file
 * Examples and test fodder for migration of Media entities and fields.
 */

/*
 * Implementation of hook_migrate_api().
 */


/*
 * Implementation of hook_migrate_api().
 */
function imager_migrate_migrate_api() {
  $api = array(
    'api' => 2,
  );
  return $api;
}


/**
 * Migration class for media images.
 */
class imgMig extends DynamicMigration {
  public function __construct(array $arguments) {
    $this->arguments = $arguments;
    parent::__construct(MigrateGroup::getInstance('imgMig'));
    $this->description = t('Import images into Drupal');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'SrcFile' => array(
                            'type' => 'varchar',
                            'length' => 255,
                            'not null' => TRUE,
                            'description' => 'Image filename',
                           ),
        'SrcPath' => array(
                            'type' => 'varchar',
                            'length' => 255,
                            'not null' => TRUE,
                            'description' => 'Image filename',
                           ),
      ),
      MigrateDestinationMedia::getKeySchema()
    );


    $this->source = new MigrateSourceCSV($arguments['source_file'], 
                                         array(), 
                                         array('header_rows' => 1), 
                                         $this->fields());

    $this->destination = new MigrateDestinationMedia('image');

    // The source images are in a local directory - specify the parent.
    $this->addFieldMapping('source_dir',              'SrcPath');
    // The 'value' of the media destination is mapped to the source field
    // representing the media itself - in this case, a filename relative to
    // source_dir.
    $this->addFieldMapping('value',                   'SrcFile');
    $this->addFieldMapping('destination_dir',         'DestPath');
    $this->addFieldMapping('destination_file',        'DestFile');
//  $this->addFieldMapping('file_replace',            'DestPath');

    // Fields on the entity can be mapped in the usual way.
//  $this->addFieldMapping('field_media_description', 'Desc');

    $geo_arguments = array(
       'lat' => array('source_field' => 'Latitude'),
       'lon' => array('source_field' => 'Longitude'),
    );
    
//  $this->addFieldMapping('field_latitude',          'Latitude');
//  $this->addFieldMapping('field_longitude',         'Longitude');

    $this->addFieldMapping('field_who',          'Who');

    $this->addFieldMapping('field_file_image_title_text', 'Title');
    $this->addFieldMapping('field_description',           'What');
    $this->addFieldMapping('uid')
         ->defaultValue(1);

    $this->addUnmigratedDestinations(array(
      'field_who:language',
      'field_description:format', 'field_description:language',
      'field_file_image_title_text:language',
      'field_file_image_alt_text','field_file_image_alt_text:language',
      'file_replace', 'preserve_files', 'timestamp',
	 ));
  }

  /**
   * Construct the machine name from the source file name.
   */
  protected function generateMachineName($class_name = NULL) {
    $pieces = explode("/", $this->arguments['source_file']);
    $filename = strtolower(end($pieces));
    $machinename = preg_replace('/_csv/','', preg_replace('/_+/', '_', preg_replace('/[^a-z0-9]/', '_', $filename)));
    $machinename = 'pic_' . $machinename;
    return $machinename;
  }

  function prepareRow($row){
//  $row->geo_type = 'point';
  }

  function fields() {
    return array(
//    'geo_type' => 'point stuff',
    );
  }
}