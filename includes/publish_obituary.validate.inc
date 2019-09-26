<?php

/**
 * @file
 * Handles the form validation of the publish obituary form
 * All hooks are in the .module file.
 */


/**
 * Master validation function for the publish obituary form - uses per-stage 
 * validation and calls functions for each one.
 * 
 * @param type $form
 * @param type $form_state
 * @return type 
 */
function publish_obituary_form_validate($form, &$form_state) {
  if ($form_state['triggering_element']['#value'] == 'Back') {
    return;
  }  
  elseif ($form_state['triggering_element']['#value'] == 'Next Step' || $form_state['triggering_element']['#value'] == 'Complete') {

    switch ($form_state['stage']) { 
        
      case 'kind_of_advertisement':
        return publish_obituary_kind_of_advertisement_validate($form, $form_state);
      break;  

      case 'publication_details': 
        return publish_obituary_publication_details_validate($form, $form_state);  
      break;
  
      case 'advertiser_details': 
        return publish_obituary_advertiser_details_validate($form, $form_state);  
      break;

      case 'deceased_details': 
        return publish_obituary_deceased_details_validate($form, $form_state);  
      break;

      case 'family_friends':
        return publish_obituary_family_friends_validate($form, $form_state);
        break;

      case 'funeral_details':
        return publish_obituary_funeral_details_validate($form, $form_state);
        break;

      case 'certificate_photo':
        return publish_obituary_certificate_photo_validate($form, $form_state);
        break;
    }
  }
}

/**
 * Validation for the kind_of_advertisement step
 * 
 * @param type $form
 * @param type $form_state 
 */
function publish_obituary_kind_of_advertisement_validate($form, &$form_state) {
  if (!$form_state['values']['adv_choosed']) {
    form_set_error('adv_choosed', 'You must choose the kind of an advertisement before continuing!');
  }
}

/**
 * Validation for the publication_details step
 * 
 * @param type $form
 * @param type $form_state 
 */
function publish_obituary_publication_details_validate($form, &$form_state) {
  if ($form_state['values']['pub_choosed']==2) { 
    if (!$form_state['values']['media']) {
      form_set_error('media', 'Media field is required.');
    }
    if (!$form_state['values']['media']) {
      form_set_error('media', 'Media field is required.');
    }
    if (!$form_state['values']['pub_date']) {
      form_set_error('pub_date', 'Publication date field is required.');
    }
    if (!$form_state['values']['style']) {
      form_set_error('style', 'Style field is required.');
    }
    if (!$form_state['values']['common_sizes']) {
      if (!$form_state['values']['height'] || !$form_state['values']['column']) {
        form_set_error('common_sizes', 'Please choose Common sizes field or Complete custom size.');
        if (!$form_state['values']['height']) {
          form_set_error('height', 'Custom width size field is required if common sizes is empty.');
        }
        if (!$form_state['values']['column']) {
          form_set_error('column', 'Custom column size field is required if common sizes is empty.');
        }
      }
    }
  }
}

/**
 * Validation for the advertiser_details step
 * 
 * @param type $form
 * @param type $form_state 
 */
function publish_obituary_advertiser_details_validate($form, &$form_state) {
}

/**
 * Validation for the deceased_details step
 * 
 * @param type $form
 * @param type $form_state 
 */
function publish_obituary_deceased_details_validate($form, &$form_state) {
}

/**
 * Validation for the family_friends step
 * 
 * @param type $form
 * @param type $form_state 
 */
function publish_obituary_family_friends_validate($form, &$form_state) {
}

/**
 * Validation for the funeral_details step
 * 
 * @param type $form
 * @param type $form_state 
 */
function publish_obituary_funeral_details_validate($form, &$form_state) {
  if ($form_state['values']['template_choosed']==1) {
    if ($form_state['values']['def_religion']!='Sikh') {
      if (!$form_state['values']['wake_street_1']) {
        form_set_error('wake_street_1', 'Wake Address field is required.');
      }
    }
    $cortege_leaves_title = cortege_leaves_on_get_title($form_state['values']['def_religion']);
    if (!$form_state['values']['cortege_leaves']) {
      form_set_error('cortege_leaves', $cortege_leaves_title .' field is required.');
    }
    if ($form_state['values']['def_religion']=='Catholic') {
      if (!$form_state['values']['cortege_massat']) {
        form_set_error('cortege_massat', 'For Funeral Mass at field is required.');
      }
      if (!$form_state['values']['cortege_masstime']) {
        form_set_error('cortege_masstime', 'At Time field is required.');
      }
    }
    $for_title = $form_state['values']['def_religion']=='Catholic' ? 'Thereafter, for' : 'For';
    if (!$form_state['values']['cortege_for']) {
      form_set_error('cortege_for', $for_title .' field is required.');
    }
    $at_title = $form_state['values']['def_religion']=='Sikh' ? 'For Cremation at' : 'At';
    if (!$form_state['values']['cortege_at']) {
      form_set_error('cortege_at', $at_title .' field is required.');
    }
    if (!$form_state['values']['cortege_time']) {
      form_set_error('cortege_time', 'At Time field is required.');
    }
  } else {
    if (!$form_state['values']['own_template']['value']) {
      form_set_error('own_template', 'Funeral Details is required.');
    }
  }
}

/**
 * Validation for the certificate_photo step
 * 
 * @param type $form
 * @param type $form_state 
 */
function publish_obituary_certificate_photo_validate($form, $form_state) {
}



/**
 * Function to get cortege leaves title for different religion
 */
function cortege_leaves_on_get_title($dec_religion) {
  switch ($dec_religion) {
    case 'Sikh':
      $leaves_title = 'Buses Leaves on';
      break;

    case 'Soka':
      $leaves_title = 'SSA Memorial Service on';
      break;
    
    default:
      $leaves_title = 'Cortege Leaves on';
      break;
  }

  return $leaves_title;
} 