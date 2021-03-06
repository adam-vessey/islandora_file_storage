<?php
/**
 * @file
 * Handles the display of file storage objects.
 */

/**
 * Implements hook_preprocess_theme().
 */
function islandora_file_storage_preprocess_islandora_file_storage(array &$variables) {
  module_load_include('inc', 'islandora', 'includes/metadata');
  module_load_include('inc', 'islandora', 'includes/utilities');
  drupal_add_js('misc/form.js');
  drupal_add_js('misc/collapse.js');

  $islandora_object = $variables['islandora_object'];

  if (isset($islandora_object['OBJ']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $islandora_object['OBJ'])) {
    $mime_detect = new MimeDetect();
    $extension = $mime_detect->getExtension($islandora_object['OBJ']->mimetype);

    $variables['islandora_file_storage_info'] = t('This is a downloadable object of filetype @extension and size @size.', array(
      '@extension' => $extension,
      '@size' => islandora_convert_bytes_to_human_readable($islandora_object['OBJ']->size),
    ));

    $display['download'] = array(
      '#type' => 'item',
      '#title' => t('Download'),
      '#markup' => l(t('@label', array('@label' => $islandora_object['OBJ']->label)), "islandora/object/$islandora_object->id/datastream/OBJ/download"),
    );
    $variables['islandora_file_storage_download'] = drupal_render($display['download']);
  }

  // Thumbnail.
  if (isset($islandora_object['TN']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $islandora_object['TN'])) {
    $thumbnail_size_url = url("islandora/object/{$islandora_object->id}/datastream/TN/view");
    $params = array(
      'title' => $islandora_object->label,
      'path' => $thumbnail_size_url,
    );
    $variables['islandora_thumbnail_img'] = theme('image', $params);
  }
  $variables['parent_collections'] = islandora_get_parents_from_rels_ext($islandora_object);
  $variables['metadata'] = islandora_retrieve_metadata_markup($islandora_object);
  $variables['description'] = islandora_retrieve_description_markup($islandora_object);
}
