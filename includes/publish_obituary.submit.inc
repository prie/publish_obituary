<?php

/**
 * @file 
 * Handles the form submission of the publish obituary form
 */


/**
 * Handles what to do with the submitted form depending on what stage has been
 * completed.
 * 
 * @see publish_obituary_form()
 * @see publish_obituary_form_validate()
 * 
 * @param type $form
 * @param type $form_state 
 */
function publish_obituary_form_submit($form, &$form_state) {
  
  switch ($form_state['stage']) {
    
    case 'adv_review':
      $form_state['multistep_values'][$form_state['stage']] = $form_state['values'];
      if ($form_state['triggering_element']['#value'] != 'Back') {
        $oid = publish_obituary_review_submit($form, $form_state);
        $form_state['obituary_oid']=$oid;
        $form_state['complete'] = TRUE;
      }
     break;
 
    default:
      $form_state['multistep_values'][$form_state['stage']] = $form_state['values'];
      $form_state['new_stage'] = publish_obituary_move_to_next_stage($form, $form_state);
     break;
 
  } 
  if (isset($form_state['complete']) && $form_state['multistep_values']['kind_of_advertisement']['adv_choosed']==1) drupal_goto('online-obituaries/create-complete');   
  
  if ($form_state['triggering_element']['#value'] == 'Back') {
    $form_state['new_stage'] = publish_obituary_move_to_previous_stage($form, $form_state);
  } 
  //$form_state['cache'] = FALSE;
  if (isset($form_state['multistep_values']['form_build_id'])) {
    $form_state['values']['form_build_id'] = $form_state['multistep_values']['form_build_id'];
  }
  $form_state['multistep_values']['form_build_id'] = $form_state['values']['form_build_id'];
  $form_state['stage'] = $form_state['new_stage'];
  $form_state['rebuild'] = TRUE;
    
}

 /**
  * Handles the submission of the final stage
  * 
  * Save a datas to database
  * 
  * @param type $form
  * @param type $form_state 
  */
function publish_obituary_review_submit($form, &$form_state) {
  
  global $user;
  $multistep = $form_state['multistep_values'];
  
  switch ($form_state['kof']) {
    case 'death-announcement':
      $f_type = 1;
      $f_family = array_key_exists('family_friends', $multistep) ? serialize($multistep['family_friends']) : '';
      $f_specific = array_key_exists('funeral_details', $multistep) ? serialize($multistep['funeral_details']) : '';
      break;
    case 'acknowledgement':
      $f_type = 2;
      $f_family = '';
      $f_specific = array_key_exists('acknowledgement_details', $multistep) ? serialize($multistep['acknowledgement_details']) : '';
      break;
    case 'condolence':
      $f_type = 3;
      $f_family = '';
      $f_specific = array_key_exists('condolence_details', $multistep) ? serialize($multistep['condolence_details']) : '';
      break;
    case 'in-memoriam':
      $f_type = 4;
      $f_family = '';
      $f_specific = array_key_exists('memoriam_details', $multistep) ? serialize($multistep['memoriam_details']) : '';
      break;
  }
  /**/
  $f_publication = array_key_exists('publication_details', $multistep) ? serialize($multistep['publication_details']) : '';
  $f_deceased = array_key_exists('deceased_details', $multistep) ? serialize($multistep['deceased_details']) : '';
  $f_certificate = array_key_exists('certificate_photo', $multistep) ? serialize($multistep['certificate_photo']) : '';

  $oid = db_insert('online_obituaries_orders')
    ->fields(array(
      'uid' => $user->uid,
      'status' => 0,
      'form_type' => $f_type,
      'kind' => $multistep['kind_of_advertisement']['adv_choosed'],
      'publication' => $f_publication,
      'advertiser' => serialize($multistep['advertiser_details']),
      'deceased' => $f_deceased,
      'family' => $f_family,
      'type_details' => $f_specific,
      'upload' => $f_certificate,
      'created' => REQUEST_TIME,
    ))
    ->execute();
  
  if ($oid) {
    // Manage file uploaded
    if (!empty($f_certificate)) {
      publish_obituary_file_upload($multistep['certificate_photo']['deceased_photo'], 'publish_obituary', 'deceased_photo');
      publish_obituary_file_upload($multistep['certificate_photo']['death_certificate'], 'publish_obituary', 'death_certificate');
      if ($form_state['kof']=='condolence' || $form_state['kof']=='in-memoriam') {
        publish_obituary_file_upload($multistep['certificate_photo']['company_logo'], 'publish_obituary', 'company_logo');
      }
      publish_obituary_file_upload($multistep['certificate_photo']['indemnity_upload'], 'publish_obituary', 'indemnity_upload');
    }
    
    // Send email
    if ($multistep['kind_of_advertisement']['adv_choosed'] == 1) {
      publish_obituary_email_order($oid, variable_get('settings_from_email',''), $oid);
      publish_obituary_email_order($oid, $multistep['advertiser_details']['email'], $oid);
    }
    
    /*
    global $base_url;
    $kind = $multistep['kind_of_advertisement']['adv_choosed'] == 1 ? 'Print' : 'Online';
    if ($kind=='Print') $style = $multistep['publication_details']['style'] == 1 ? 'Color' : 'Black and White';

    $from = variable_get('settings_from_email','');
    $to = $multistep['advertiser_details']['email'];
    $subject = $kind . ' Obituary Form Received (Order #'. $oid .')';
    switch ($kind) {
      case 'Online':
        $body='Dear '. $multistep['advertiser_details']['name'] .',
        <br/><br/>
        YOUR FORM HAS BEEN SUCCESSFULLY SUBMITTED
        <br/><br/>
        We are currently vetting the contents and the obituary will be published as soon as it is approved. 
        <br/><br/>
        Let us know if you have any other questions and if you have any feedback, please do not hesitate to contact us.
        <br/><br/>
        Best Regards,<br/>
        Customer Care<br/>
        ____________<br/>
        <a href="'.$base_url.'">www.obituaries.sg</a>';
        break;
      
      case 'Print':
        $body='Dear '. $multistep['advertiser_details']['name'] .',
        <br/><br/>
        YOUR FORM HAS BEEN SUCCESSFULLY SUBMITTED
        <br/><br/>
        The details are:<br/>
        1. '. publish_obituary_get_media_name($multistep['publication_details']['media']) .'<br/>
        2. '. format_date(strtotime($multistep['publication_details']['pub_date']), 'custom', 'd F Y') .'<br/>
        3. '. $style .'<br/>
        4. '. publish_obituary_public_size($multistep['publication_details']['height'], $multistep['publication_details']['column'], $multistep['publication_details']['common_sizes']) .'<br/>
        5. $'. $multistep['publication_details']['price'] .'
        <br/><br/>
        We will get in touch with you very soon.
        <br/><br/>
        Let us know if you have any other questions and if you have any feedback, please do not hesitate to contact us.
        <br/><br/>
        Best Regards,<br/>
        Customer Care<br/>
        ____________<br/>
        <a href="'.$base_url.'">www.obituaries.sg</a>';
        break;
    }
    $result = simple_mail_send($from, $to, $subject, $body);
    */
  }
  
  return $oid;
  //cache_clear_all();
}

/**
 * Returns what to show on the completion page.
 * 
 * @return type 
 */
function publish_obituary_complete() {
  $complete_thank = '<h3>Thank you for your submission</h3>';
  $complete_thank .= '<p>'. variable_get('thank_message') .'</p>';
  $complete_thank .= '<div class="complete-page-links">' . l('Ongoing Funerals', 'ongoing-funerals', array('attributes' => array('class' => 'back-to-form-btn')));
  //if ($koadv == 'online') $complete_thank .= l('Checkout', '#', array('attributes' => array('class' => 'checkout-btn'))) . '</div>';
  
  return $complete_thank;
}

/**
 * function to send email on when submit obituaries order
 */
function publish_obituary_email_order($oid, $to, $order_number) {
  // Send email
  global $base_url;
  $oborder = obituaries_order_load($oid);

  if ($oborder->kind == 1) {
    $kind = 'Print';
    //$publication = unserialize($oborder->publication);
    //$style = $publication['style'] == 1 ? 'Color' : 'Black and White';
  } else {
    $kind = 'Online';
  }
  $advertiser = unserialize($oborder->advertiser);

  $from = variable_get('settings_from_email','');
  $to = $to;
  $subject = $kind . ' Obituary Form Received (Order #'. $order_number .')';
  /*
  switch ($oborder->kind) {
    case 2:
      $body='Dear '. $advertiser['name'] .',
      <br/><br/>
      YOUR FORM HAS BEEN SUCCESSFULLY SUBMITTED
      <br/><br/>
      We are currently vetting the contents and the obituary will be published as soon as it is approved. 
      <br/><br/>
      Let us know if you have any other questions and if you have any feedback, please do not hesitate to contact us.
      <br/><br/>
      Best Regards,<br/>
      Customer Care<br/>
      ____________<br/>
      <a href="'.$base_url.'">www.obituaries.sg</a>';
      break;
    
    case 1:
      $body='Dear '. $advertiser['name'] .',
      <br/><br/>
      YOUR FORM HAS BEEN SUCCESSFULLY SUBMITTED
      <br/><br/>
      The details are:<br/>
      1. '. publish_obituary_get_media_name($publication['media']) .'<br/>
      2. '. format_date(strtotime($publication['pub_date']), 'custom', 'd F Y') .'<br/>
      3. '. $style .'<br/>
      4. '. publish_obituary_public_size($publication['height'], $publication['column'], $publication['common_sizes']) .'<br/>
      5. $'. $publication['price'] .'
      <br/><br/>
      We will get in touch with you very soon.
      <br/><br/>
      Let us know if you have any other questions and if you have any feedback, please do not hesitate to contact us.
      <br/><br/>
      Best Regards,<br/>
      Customer Care<br/>
      ____________<br/>
      <a href="'.$base_url.'">www.obituaries.sg</a>';
      break;
  }
  */
  $email_details = publish_obituary_checkout_email($oborder);
  
  $body = 'Dear '. $advertiser['name'] .',
  <br/><br/>
  YOUR OBITUARY ORDER HAS BEEN SUCCESSFULLY COMPLETED
  <br/><br/>
  The details are:<br/>
  '. $email_details .'
  <br/><br/>
  Best Regards,<br/>
  Customer Care<br/>
  ____________<br/>
  <a href="'.$base_url.'">www.obituaries.sg</a>';
  
  $result = simple_mail_send($from, $to, $subject, $body);
}

/**
 * Form to submit edited Obituary.
 * 
 * @param type $form
 * @param type $form_state
 * @return type 
 */
function publish_obituary_edit_form_submit($form, &$form_state) {
  $new_values = $form_state['values'];

  if ($form_state['triggering_element']['#value'] == 'Save') {
  $node = node_load($new_values['nid']);
  $node_wrapper = entity_metadata_wrapper('node', $node);
  $node_wrapper->field_obituary_deceased_photo->set(array('fid' => $new_values['deceased_photo']));
  $node_wrapper->title->set($new_values['deceased_name']);
  $node_wrapper->field_obituary_age->set($new_values['deceased_age']);
  $node_wrapper->field_obituary_departed->set(strtotime($new_values['departed_date']));
  $node_wrapper->field_obituary_religion->set(strtolower($new_values['deceased_religion']));
  $node_wrapper->body->set(array('value'=>$new_values['deceased_personality']));
  if ($new_values['template']==1) {
    if ($new_values['deceased_religion']!='sikh') {
      $node_wrapper->field_wake_street_address_1->set($new_values['street_one']);
      $node_wrapper->field_wake_street_address_2->set($new_values['street_two']);

      if ($new_values['street_one'] != $new_values['wake_address_one']) {
        $node_wrapper->field_updated_wake_address->set(1);
      }
    }
    $node_wrapper->field_cortage_leaves_on->set(strtotime($new_values['cortege_leaves_on']));
    $node_wrapper->field_cortage_leaves_for->set(strtolower($new_values['cortege_for']));
    $node_wrapper->field_cortage_leaves_at->set($new_values['cortege_at']);
    $node_wrapper->field_cortage_leaves_at_time->set(strtotime($new_values['cortege_time']));
    $node_wrapper->field_obituary_remarks->set(array('value'=>$new_values['other_remarks']));
    // Manage night services / prayers collection field.  
    if ($new_values['deceased_religion']=='christian' || $new_values['deceased_religion']=='catholic') {
      // update the existing values
      $night_services = $node_wrapper->field_night_services_prayers->value();
      foreach ($night_services as $nk => $night_service) {
        $night_wrapper = entity_metadata_wrapper('field_collection_item', $night_service);
        $night_wrapper->field_night_serv_pray_date->set(strtotime($new_values['more_nightserv'][$nk]['night_services']));
        $night_wrapper->save();
      }
      // Adding new values if exist
      if (count($night_services) < count($new_values['more_nightserv'])-1) {
        for ($i=count($night_services); $i < count($new_values['more_nightserv'])-1; $i++) { 
          // Create the collection entity and set it's "host".
          $collection = entity_create('field_collection_item', array('field_name' => 'field_night_services_prayers'));
          $collection->setHostEntity('node', $node);  

          // Now define the collection parameters.
          $night_wrapper = entity_metadata_wrapper('field_collection_item', $collection);
          $night_wrapper->field_night_serv_pray_date->set(strtotime($new_values['more_nightserv'][$i]['night_services']));
          $night_wrapper->save();
        }
      }
    }
  } else {
    $node_wrapper->field_custom_funeral->set(array('value'=>$new_values['own_template']['value']));
  }
  // update the existing family spouse values
  $fam_spouse = $node_wrapper->field_obituary_spouse->value();
  $spouse_wrapper = entity_metadata_wrapper('field_collection_item', $fam_spouse);
  $spouse_wrapper->field_husband_wife->set($new_values['spouse_as']);
  $spouse_wrapper->field_spouse_name->set($new_values['spouse_name']);
  $spouse_wrapper->save();
/*
  // update the existed family brothers values
  $fam_brothers = $node_wrapper->field_obituary_brothers->value();
  foreach ($fam_brothers as $fbk => $fam_brother) {
    $brother_wrapper = entity_metadata_wrapper('field_collection_item', $fam_brother);
    $brother_wrapper->field_brothers_name->set($new_values['more_brothers'][$fbk]['brother_name']);
    $brother_wrapper->field_sister_in_law_name->set($new_values['more_brothers'][$fbk]['sister_law_name']);
    $brother_wrapper->save();
  }
  // Adding new values family brothers if exist
  if (count($fam_brothers) < count($new_values['more_brothers'])-1) {
    for ($i=count($fam_brothers); $i < count($new_values['more_brothers'])-1; $i++) { 
      // Create the collection entity and set it's "host".
      $collection = entity_create('field_collection_item', array('field_name' => 'field_obituary_brothers'));
      $collection->setHostEntity('node', $node);  

      // Now define the collection parameters.
      $brother_wrapper = entity_metadata_wrapper('field_collection_item', $collection);
      $brother_wrapper->field_brothers_name->set($new_values['more_brothers'][$i]['brother_name']);
      $brother_wrapper->field_sister_in_law_name->set($new_values['more_brothers'][$i]['sister_law_name']);
      $brother_wrapper->save();
    }
  }
*/
  obituary_family_update($node, $node_wrapper, $new_values, 'field_obituary_father', 'field_father_name', 'field_mother_name', 'more_fathers', 'father_name', 'mother_name');
  obituary_family_update($node, $node_wrapper, $new_values, 'field_obituary_sons', 'field_sons_name', 'field_daughters_in_law_name', 'more_sons', 'son_name', 'daughter_law_name');
  obituary_family_update($node, $node_wrapper, $new_values, 'field_obituary_daughters', 'field_daughters_name', 'field_sons_in_law_name', 'more_daughters', 'daughter_name', 'sons_law_name');
  obituary_family_update($node, $node_wrapper, $new_values, 'field_obituary_grandsons', 'field_grandsons_name', 'field_granddaughters_in_law_name', 'more_grandsons', 'grandson_name', 'granddaughter_law_name');
  obituary_family_update($node, $node_wrapper, $new_values, 'field_obituary_granddaughters', 'field_granddaughters_name', 'field_grandsons_in_law_name', 'more_granddaughters', 'granddaughter_name', 'grandson_law_name');
  obituary_family_update($node, $node_wrapper, $new_values, 'field_obituary_great_grandsons', 'field_great_grandsons_name', 'field_great_granddaughters_name', 'more_greatgrandsons', 'greatgrandson_name', 'greatgranddaughter');
  obituary_family_update($node, $node_wrapper, $new_values, 'field_obituary_brothers', 'field_brothers_name', 'field_sister_in_law_name', 'more_brothers', 'brother_name', 'sister_law_name');
  obituary_family_update($node, $node_wrapper, $new_values, 'field_obituary_sisters', 'field_sisters_name', 'field_brothers_in_law_name', 'more_sisters', 'sister_name', 'brother_law_name');
  obituary_family_update($node, $node_wrapper, $new_values, 'field_obituary_father_in_law', 'field_father_in_law_name', 'field_mother_in_law_name', 'more_fatherlaws', 'fatherlaw_name', 'motherlaw_name');
  obituary_family_update($node, $node_wrapper, $new_values, 'field_obituary_brothers_in_law', 'field_brothers_law_main_name', 'field_sisters_law_pair_name', 'more_brotherslaw', 'brotherlawmain_name', 'sisterlawpair_name');
  obituary_family_update($node, $node_wrapper, $new_values, 'field_obituary_sisters_in_law', 'field_sisters_law_main_name', 'field_brothers_law_pair_name', 'more_sisterslaw', 'sisterlawmain_name', 'brotherlawpair_name');
  obituary_family_update($node, $node_wrapper, $new_values, 'field_obituary_nephews', 'field_nephews_name', 'field_nieces_in_law_name', 'more_nephews', 'nephew_name', 'niece_law_name');
  obituary_family_update($node, $node_wrapper, $new_values, 'field_obituary_nieces', 'field_nieces_name', 'field_nephews_in_law_name', 'more_nieces', 'niece_name', 'nephew_law_name');
  obituary_family_update($node, $node_wrapper, $new_values, 'field_obituary_grandnephews', 'field_grandnewphews_name', 'field_grandnieces_name', 'more_grandnephews', 'grandnephew_name', 'grandniece_name');
  obituary_family_update($node, $node_wrapper, $new_values, 'field_obituary_grandnieces', 'field_grandnieces_real_name', 'field_grandnephews_in_law_name', 'more_grandnieces', 'grandniece_real_name', 'grandnephew_law_name');
  obituary_family_update($node, $node_wrapper, $new_values, 'field_obituary_great_grandnephew', 'field_great_grandnephews_name', 'field_great_grandnieces_name', 'more_greatgrandnephews', 'greatgrandnephew_name', 'greatgrandniece_name');
  obituary_family_update($node, $node_wrapper, $new_values, 'field_obituary_uncles', 'field_uncle_name', 'field_uncles_spouses', 'more_uncles', 'uncle_name', 'uncle_spouse_name');
  obituary_family_update($node, $node_wrapper, $new_values, 'field_obituary_aunties', 'field_aunties_name', 'field_aunties_spouses_name', 'more_aunties', 'auntie_name', 'auntie_spouse_name');
  obituary_family_update($node, $node_wrapper, $new_values, 'field_obituary_cousins', 'field_cousins_name', 'field_cousins_in_law_name', 'more_cousins', 'cousin_name', 'cousin_law_name');
  obituary_family_update($node, $node_wrapper, $new_values, 'field_obituary_god_sons', 'field_god_sons_name', 'field_god_daughters_in_law_name', 'more_godsons', 'godson_name', 'goddaughter_law_name');
  obituary_family_update($node, $node_wrapper, $new_values, 'field_obituary_god_daughters', 'field_god_daughters_name', 'field_god_sons_in_law_name', 'more_goddaughters', 'goddaughter_name', 'godsons_law_name');
  obituary_family_update($node, $node_wrapper, $new_values, 'field_obituary_god_grandsons', 'field_god_grandsons_name', 'field_god_granddaughters_in_law_', 'more_godgrandsons', 'godgrandson_name', 'godgranddaughter_law_name');
  obituary_family_update($node, $node_wrapper, $new_values, 'field_obituary_god_granddaugh', 'field_god_granddaughters_name', 'field_god_grandsons_in_law_name', 'more_godgranddaughters', 'godgranddaughter_name', 'godgrandson_law_name');
  obituary_family_update($node, $node_wrapper, $new_values, 'field_obituary_god_great_grandso', 'field_god_great_grandsons_name', 'field_god_great_granddaughters_n', 'more_godgreatgrandsons', 'godgreatgrandson_name', 'godgreatgranddaughter_name');
  obituary_family_update($node, $node_wrapper, $new_values, 'field_obituary_caregivers', 'field_caregivers_name', '', 'more_caregivers', 'caregiver_name', '');
  obituary_family_update($node, $node_wrapper, $new_values, 'field_obituary_other', 'field_other_title', 'field_other_name', 'more_others', 'other_title', 'other_name');
  //publish_obituary_family_update($node_wrapper, $parent_field, $son_field_one, );

  //dpm($fam_spouses);
  $node_wrapper->save();

  //publish_obituary_file_upload($multistep['certificate_photo']['indemnity_upload'], 'publish_obituary');
  
  drupal_set_message('Your obituary has been updated', 'status');
  }

  if ($form_state['triggering_element']['#value'] == 'Cancel') {
    return;
  }
}

/**
 * Updated function for obituary family-friend field collection edit
 */
function obituary_family_update($node, $node_wrapper, $new_values, $parent_field, $main_field, $pair_field, $more_wrapper, $main_inp, $pair_inp) {
  // update the existed family items values
  $family_items = $node_wrapper->$parent_field->value();
  foreach ($family_items as $fbk => $fam_item) {
    $family_wrapper = entity_metadata_wrapper('field_collection_item', $fam_item);
    $family_wrapper->$main_field->set($new_values[$more_wrapper][$fbk][$main_inp]);
    if (!empty($pair_field) && !empty($pair_inp)) {
      $family_wrapper->$pair_field->set($new_values[$more_wrapper][$fbk][$pair_inp]);
    }
    $family_wrapper->save();
  }
  // Adding new values family brothers if exist
  if (count($family_items) < count($new_values[$more_wrapper])-1) {
    for ($i=count($family_items); $i < count($new_values[$more_wrapper])-1; $i++) { 
      // Create the collection entity and set it's "host".
      $collection = entity_create('field_collection_item', array('field_name' => $parent_field));
      $collection->setHostEntity('node', $node);  

      // Now define the collection parameters.
      $family_wrapper = entity_metadata_wrapper('field_collection_item', $collection);
      $family_wrapper->$main_field->set($new_values[$more_wrapper][$i][$main_inp]);
      if (!empty($pair_field) && !empty($pair_inp)) {
        $family_wrapper->$pair_field->set($new_values[$more_wrapper][$i][$pair_inp]);
      }
      $family_wrapper->save();
    }
  }
}

/**
 * Submit function for setting of obituary form
 */
function publish_obituary_setting_form_submit($form, &$form_state) {
  variable_set('public_media', $form_state['values']['public_media']);
  variable_set('public_gst', $form_state['values']['public_gst']);
  variable_set('public_common_sizes', $form_state['values']['public_common_sizes']);
  variable_set('public_width', $form_state['values']['public_width']);
  variable_set('public_columns', $form_state['values']['public_columns']);
  variable_set('funeral_for_at', $form_state['values']['funeral_for_at']);
  variable_set('funeral_bible', $form_state['values']['funeral_bible']);
  variable_set('thank_message', $form_state['values']['obituaries_thanks']);

  drupal_set_message(t('Your configuration has been saved.'));
}

/**
 * Submit function for setting of memoriam form
 */
function publish_memoriam_setting_form_submit($form, &$form_state) {
  variable_set('memoriam_messages', $form_state['values']['memoriam_messages']);

  drupal_set_message(t('Your configuration has been saved.'));
}

/**
 * Manage file upload
 */
function publish_obituary_file_upload($field, $module, $var_name) {
  //uploading photo to system
  //if (isset($multistep['certificate_photo']['deceased_photo'])) {
  if (isset($field)) {  
    //remove existing photo while clicking remove button
    if ($field == 0){
      $file_exist_id = variable_get($var_name);
      $file_exist_file = file_load($file_exist_id);
      if (!empty($file_exist_file)) {
        file_usage_delete($file_exist_file, $module, 'file', $file_exist_id);
        file_delete($file_exist_file, TRUE);
      }
    } else {
      //adding photo
      $current_file = file_load($field);
      if (!empty($current_file->fid)) {
        $file_id = $current_file->fid;
        variable_set($var_name, $file_id);
        file_usage_add($current_file, $module, 'file', $file_id);
        $current_file->status = FILE_STATUS_PERMANENT;
        file_save($current_file);
      }
    }
  }
}

/**
 * Detail email send on obituary online checkout or print submit
 */
function publish_obituary_checkout_email($oborder) {
  
  // Advertisement Type
  $html = '<h3 style="margin:20px 0 10px 0;background:#f4f4f4;padding:10px 0;font-size:15px;text-transform:uppercase;font-weight:bold;">Advertisement Type</h3>';
  $html .= $oborder->kind == 1 ? 'Print' : 'Online';

  // Publication details
  if ($oborder->kind == 1) {
    $publication = unserialize($oborder->publication);
    $html .= '<h3 style="margin:20px 0 10px 0;background:#f4f4f4;padding:10px 0;font-size:15px;text-transform:uppercase;font-weight:bold;">Publication Details</h3>';
    
    if ($publication['pub_choosed']==1) {
      $html .= '<div style="margin-top:10px;font-weight:bold;">Publication Choosed</div>';
      $html .= $publication['pub_choosed'] == 1 ? 'Schedule an onsite visit' : 'Create your own';
    } else {
      $html .= '<div style="margin-top:10px;font-weight:bold;">Media</div>';
      $html .= publish_obituary_get_media_val($publication['media'])['media_name'];

      $html .= '<div style="margin-top:10px;font-weight:bold;">Publication Date</div>';
      $html .= format_date(strtotime($publication['pub_date']), 'custom', 'd F Y');

      $html .= '<div style="margin-top:10px;font-weight:bold;">Style</div>';
      $html .= $publication['style'] == 1 ? 'Full Color' : 'Black and White';

      $html .= '<div style="margin-top:10px;font-weight:bold;">Size</div>';
      $html .= publish_obituary_public_size($publication['height'], $publication['column'], $publication['common_sizes']);
      $html .= '$ ' . $publication['price'];

      //$html .= '<div style="margin-top:10px;font-weight:bold;">Request for Quote</div>';
      //$html .= $publication['quote_me'] == 0 ? 'No' : 'Yes';
    }
  }
  
  // Advertiser details
  $advertiser = unserialize($oborder->advertiser);
  //$countries = country_get_list();
  $country_name = variable_get('adv_country'); //$countries[$advertiser['country']];
  $html .= '<h3 style="margin:20px 0 10px 0;background:#f4f4f4;padding:10px 0;font-size:15px;text-transform:uppercase;font-weight:bold;">Advertiser Details</h3>';
  
  $html .= '<div style="margin-top:10px;font-weight:bold;">Advertiser Name</div>';
  $html .= $advertiser['name'];

  $html .= '<div style="margin-top:10px;font-weight:bold;">Contact Number</div>';
  $html .= $advertiser['contact_number'];

  $html .= '<div style="margin-top:10px;font-weight:bold;">Email Address</div>';
  $html .= $advertiser['email'];

  $html .= '<div style="margin-top:10px;font-weight:bold;">Billing Address</div>';
  $html .= $advertiser['company_name'] . ' ' . $advertiser['street_address_1'] . ' ' . $country_name . ' ' . $advertiser['zip'];

  $html .= '<div style="margin-top:10px;font-weight:bold;">Relationship to Deceased</div>';
  $html .= $advertiser['relationship'];
  
  $publication_onsite = isset($publication) && $publication['pub_choosed']==1 ? TRUE : FALSE;

  // Deceased details
  $html .= '<h3 style="margin:20px 0 10px 0;background:#f4f4f4;padding:10px 0;font-size:15px;text-transform:uppercase;font-weight:bold;">Deceased Details</h3>';

  if (!$publication_onsite) {
    $deceased = unserialize($oborder->deceased);

    if ($oborder->form_type==2 && $oborder->kind==2) {
      $obi_node = node_load(trim($deceased['deceased_name']));
      $deceased_name = $obi_node->title;
    } else {
      $deceased_name = $deceased['deceased_name'];
    }
  
    $html .= '<div style="margin-top:10px;font-weight:bold;">Deceased Name</div>';
    $html .= $deceased_name;
  
    $html .= '<div style="margin-top:10px;font-weight:bold;">Departed Date</div>';
    $html .= format_date(strtotime($deceased['departed_date']), 'custom', 'd F Y');
    
    if ($oborder->form_type != 2) {
      $html .= '<div style="margin-top:10px;font-weight:bold;">Age</div>';
      $html .= $deceased['age'];
    }
  
    $html .= '<div style="margin-top:10px;font-weight:bold;">Religion</div>';
    $html .= $deceased['religion'];
  }
  
  // Family & friends
  if ($oborder->form_type == 1) {
    $html .= '<h3 style="margin:20px 0 10px 0;background:#f4f4f4;padding:10px 0;font-size:15px;text-transform:uppercase;font-weight:bold;">Family and Friends</h3>';

    if (!$publication_onsite) {
      $family_friends = unserialize($oborder->family);
      if (!empty($family_friends['spouse_name'])) {
        $html .= '<div><strong>' . $family_friends['spouse_as'] . '<strong></div>';
        $html .= $family_friends['spouse_name'] .'<br><br>';
      }

      $fathers = publish_obituary_get_descent($family_friends, 'more_fathers', 'father_name', 'mother_name');
      if (!empty($fathers['main'])) {
        $html .= '<div><strong>Father</strong></div>';
        $html .= $fathers['main'] .'<br><br>';
      }
      if (!empty($fathers['pair'])) {
        $html .= '<div><strong>Mother</strong></div>';
        $html .= $fathers['pair'] .'<br><br>';
      }

      $sons = publish_obituary_get_descent($family_friends, 'more_sons', 'son_name', 'daughter_law_name');
      if (!empty($sons['main'])) {
        $html .= '<div><strong>Sons</strong></div>';
        $html .= $sons['main'] .'<br><br>';
      }
      if (!empty($sons['pair'])) {
        $html .= '<div><strong>Daughters-in-law</strong></div>';
        $html .= $sons['pair'] .'<br><br>';
      }

      $daughters = publish_obituary_get_descent($family_friends, 'more_daughters', 'daughter_name', 'sons_law_name');
      if (!empty($daughters['main'])) {
        $html .= '<div><strong>Daughters</strong></div>';
        $html .= $daughters['main'] .'<br><br>';
      }
      if (!empty($daughters['pair'])) {
        $html .= '<div><strong>Sons-in-law</strong></div>';
        $html .= $daughters['pair'] .'<br><br>';
      }

      $grandsons = publish_obituary_get_descent($family_friends, 'more_grandsons', 'grandson_name', 'granddaughter_law_name');
      if (!empty($grandsons['main'])) {
        $html .= '<div><strong>Grandsons</strong></div>';
        $html .= $grandsons['main'] .'<br><br>';
      }
      if (!empty($grandsons['pair'])) {
        $html .= '<div><strong>Granddaughters-in-law</strong></div>';
        $html .= $grandsons['pair'] .'<br><br>';
      }

      $granddaughters = publish_obituary_get_descent($family_friends, 'more_granddaughters', 'granddaughter_name', 'grandson_law_name');
      if (!empty($granddaughters['main'])) {
        $html .= '<div><strong>Granddaughters</strong></div>';
        $html .= $granddaughters['main'] .'<br><br>';
      }
      if (!empty($granddaughters['pair'])) {
        $html .= '<div><strong>Grandsons-in-law</strong></div>';
        $html .= $granddaughters['pair'] .'<br><br>';
      }

      $greatgrandsons = publish_obituary_get_descent($family_friends, 'more_greatgrandsons', 'greatgrandson_name', 'greatgranddaughter'); 
      if (!empty($greatgrandsons['main'])) {
        $html .= '<div><strong>Great Grandsons</strong></div>';
        $html .= $greatgrandsons['main'] .'<br><br>';
      }
      if (!empty($greatgrandsons['pair'])) {
        $html .= '<div><strong>Great Granddaughters</strong></div>';
        $html .= $greatgrandsons['pair'] .'<br><br>';
      }

      $brothers = publish_obituary_get_descent($family_friends, 'more_brothers', 'brother_name', 'sister_law_name');
      if (!empty($brothers['main'])) {
        $html .= '<div><strong>Brothers</strong></div>';
        $html .= $brothers['main'] .'<br><br>';
      }
      if (!empty($brothers['pair'])) {
        $html .= '<div><strong>Sisters-in-law</strong></div>';
        $html .= $brothers['pair'] .'<br><br>';
      }

      $sisters = publish_obituary_get_descent($family_friends, 'more_sisters', 'sister_name', 'brother_law_name');
      if (!empty($sisters['main'])) {
        $html .= '<div><strong>Sisters</strong></div>';
        $html .= $sisters['main'] .'<br><br>';
      }
      if (!empty($sisters['pair'])) {
        $html .= '<div><strong>Brothers-in-law</strong></div>';
        $html .= $sisters['pair'] .'<br><br>';
      }

      $fatherslaw = publish_obituary_get_descent($family_friends, 'more_fatherlaws', 'fatherlaw_name', 'motherlaw_name');
      if (!empty($fatherslaw['main'])) {
        $html .= '<div><strong>Father-in-law</strong></div>';
        $html .= $fatherslaw['main'] .'<br><br>';
      }
      if (!empty($fatherslaw['pair'])) {
        $html .= '<div><strong>Mother-in-law</strong></div>';
        $html .= $fatherslaw['pair'] .'<br><br>';
      }

      $brotherslaw = publish_obituary_get_descent($family_friends, 'more_brotherslaw', 'brotherlawmain_name', 'sisterlawpair_name');
      if (!empty($brotherslaw['main'])) {
        $html .= '<div><strong>Brothers-in-law</strong></div>';
        $html .= $brotherslaw['main'] .'<br><br>';
      }
      if (!empty($brotherslaw['pair'])) {
        $html .= '<div><strong>Sisters-in-law</strong></div>';
        $html .= $brotherslaw['pair'] .'<br><br>';
      }

      $sisterslaw = publish_obituary_get_descent($family_friends, 'more_sisterslaw', 'sisterlawmain_name', 'brotherlawpair_name');
      if (!empty($sisterslaw['main'])) {
        $html .= '<div><strong>Sisters-in-law</strong></div>';
        $html .= $sisterslaw['main'] .'<br><br>';
      }
      if (!empty($sisterslaw['pair'])) {
        $html .= '<div><strong>Brothers-in-law</strong></div>';
        $html .= $sisterslaw['pair'] .'<br><br>';
      }

      $nephews = publish_obituary_get_descent($family_friends, 'more_nephews', 'nephew_name', 'niece_law_name');
      if (!empty($nephews['main'])) {
        $html .= '<div><strong>Nephews</strong></div>';
        $html .= $nephews['main'] .'<br><br>';
      }
      if (!empty($nephews['pair'])) {
        $html .= '<div><strong>Nieces-in-law</strong></div>';
        $html .= $nephews['pair'] .'<br><br>';
      }

      $nieces = publish_obituary_get_descent($family_friends, 'more_nieces', 'niece_name', 'nephew_law_name');
      if (!empty($nieces['main'])) {
        $html .= '<div><strong>Nieces</strong></div>';
        $html .= $nieces['main'] .'<br><br>';
      }
      if (!empty($nieces['pair'])) {
        $html .= '<div><strong>Nephews-in-law</strong></div>';
        $html .= $nieces['pair'] .'<br><br>';
      }

      $grandnephews = publish_obituary_get_descent($family_friends, 'more_grandnephews', 'grandnephew_name', 'grandniece_name');
      if (!empty($grandnephews['main'])) {
        $html .= '<div><strong>Grandnephews</strong></div>';
        $html .= $grandnephews['main'] .'<br><br>';
      }
      if (!empty($grandnephews['pair'])) {
        $html .= '<div><strong>Grandnieces-in-law</strong></div>';
        $html .= $grandnephews['pair'] .'<br><br>';
      }

      $grandnieces = publish_obituary_get_descent($family_friends, 'more_grandnieces', 'grandniece_real_name', 'grandnephew_law_name');
      if (!empty($grandnieces['main'])) {
        $html .= '<div><strong>Grandnieces</strong></div>';
        $html .= $grandnieces['main'] .'<br><br>';
      }
      if (!empty($grandnieces['pair'])) {
        $html .= '<div><strong>Grandnephews-in-law</strong></div>';
        $html .= $grandnieces['pair'] .'<br><br>';
      }
      $greatgrandnephews = publish_obituary_get_descent($family_friends, 'more_greatgrandnephews', 'greatgrandnephew_name', 'greatgrandniece_name');
      if (!empty($greatgrandnephews['main'])) {
        $html .= '<div><strong>Great Grandnephews</strong></div>';
        $html .= $greatgrandnephews['main'] .'<br><br>';
      }
      if (!empty($greatgrandnephews['pair'])) {
        $html .= '<div><strong>Great Grandnieces</strong></div>';
        $html .= $greatgrandnephews['pair'] .'<br><br>';
      }
      $uncles = publish_obituary_get_descent($family_friends, 'more_uncles', 'uncle_name', 'uncle_spouse_name');
      if (!empty($uncles['main'])) {
        $html .= '<div><strong>Uncles</strong></div>';
        $html .= $uncles['main'] .'<br><br>';
      }
      if (!empty($uncles['pair'])) {
        $html .= '<div><strong>Spouses</strong></div>';
        $html .= $uncles['pair'] .'<br><br>';
      }
      $aunties = publish_obituary_get_descent($family_friends, 'more_aunties', 'auntie_name', 'auntie_spouse_name');
      if (!empty($aunties['main'])) {
        $html .= '<div><strong>Aunties</strong></div>';
        $html .= $aunties['main'] .'<br><br>';
      }
      if (!empty($aunties['pair'])) {
        $html .= '<div><strong>Spouses</strong></div>';
        $html .= $aunties['pair'] .'<br><br>';
      }
      $cousins = publish_obituary_get_descent($family_friends, 'more_cousins', 'cousin_name', 'cousin_law_name');
      if (!empty($cousins['main'])) {
        $html .= '<div><strong>Cousins</strong></div>';
        $html .= $cousins['main'] .'<br><br>';
      }
      if (!empty($cousins['pair'])) {
        $html .= '<div><strong>Cousins-in-law</strong></div>';
        $html .= $cousins['pair'] .'<br><br>';
      }

      $godsons = publish_obituary_get_descent($family_friends, 'more_godsons', 'godson_name', 'goddaughter_law_name');
      if (!empty($godsons['main'])) {
        $html .= '<div><strong>God Sons</strong></div>';
        $html .= $godsons['main'] .'<br><br>';
      }
      if (!empty($godsons['pair'])) {
        $html .= '<div><strong>God Daughters-in-law</strong></div>';
        $html .= $godsons['pair'] .'<br><br>';
      }

      $goddaughters = publish_obituary_get_descent($family_friends, 'more_goddaughters', 'goddaughter_name', 'godsons_law_name');
      if (!empty($goddaughters['main'])) {
        $html .= '<div><strong>God Daughters</strong></div>';
        $html .= $goddaughters['main'] .'<br><br>';
      }
      if (!empty($goddaughters['pair'])) {
        $html .= '<divG<strong>od Sons-in-law</strong></div>';
        $html .= $goddaughters['pair'] .'<br><br>';
      }

      $godgrandsons = publish_obituary_get_descent($family_friends, 'more_godgrandsons', 'godgrandson_name', 'godgranddaughter_law_name');
      if (!empty($godgrandsons['main'])) {
        $html .= '<div><strong>God Grandsons</strong></div>';
        $html .= $godgrandsons['main'] .'<br><br>';
      }
      if (!empty($godgrandsons['pair'])) {
        $html .= '<div><strong>God Granddaughters-in-law</strong></div>';
        $html .= $godgrandsons['pair'] .'<br><br>';
      }

      $godgranddaughters = publish_obituary_get_descent($family_friends, 'more_godgranddaughters', 'godgranddaughter_name', 'godgrandson_law_name');
      if (!empty($godgranddaughters['main'])) {
        $html .= '<div><strong>God Granddaughters</strong></div>';
        $html .= $godgranddaughters['main'] .'<br><br>';
      }
      if (!empty($godgranddaughters['pair'])) {
        $html .= '<div><strong>God Grandsons-in-law</strong></div>';
        $html .= $godgranddaughters['pair'] .'<br><br>';
      }

      $godgreatgrandsons = publish_obituary_get_descent($family_friends, 'more_godgreatgrandsons', 'godgreatgrandson_name', 'godgreatgranddaughter_name');
      if (!empty($godgreatgrandsons['main'])) {
        $html .= '<div><strong>God Great Grandsons</strong></div>';
        $html .= $godgreatgrandsons['main'] .'<br><br>';
      }
      if (!empty($godgreatgrandsons['pair'])) {
        $html .= '<div><strong>God Great Granddaughters</strong></div>';
        $html .= $godgreatgrandsons['pair'] .'<br><br>';
      }

      $caregivers = publish_obituary_get_descent($family_friends, 'more_caregivers', 'caregiver_name', '');
      if (!empty($caregivers['main'])) {
        $html .= '<div><strong>Caregivers</strong></div>';
        $html .= $caregivers['main'] .'<br><br>';
      }

      $others = publish_obituary_get_descent($family_friends, 'more_others', 'other_name', '');
      if (!empty($others['main'])) {
        $html .= '<div><strong>Others</strong></div>';
        $html .= $others['main'];
      }
    }
  }
  
  // Service (form type)
  $serv_type = unserialize($oborder->type_details);
  // Funeral details
  if ($oborder->form_type == 1) {
    $html .= '<h3 style="margin:20px 0 10px 0;background:#f4f4f4;padding:10px 0;font-size:15px;text-transform:uppercase;font-weight:bold;">Funeral Details</h3>';
    
    if (!$publication_onsite) {
      if ($serv_type['template_choosed']==1) {
        if ($deceased['religion']!='Sikh') {
          $w_street_2 = !empty($serv_type['wake_street_2']) ? '<div>'. $serv_type['wake_street_2'] .'</div>' : '';
          $html .= '<div style="margin-top:10px;font-weight:bold;">Wake Location</div>';
          $html .= '<div>'. $serv_type['wake_street_1'] . '</div>' . $w_street_2;// '<div>' . $advertiser['zip'] . '</div><div>' . $country_name .'</div>';
        }

        if ($deceased['religion']=='Christian' || $deceased['religion']=='Catholic') {
          $night_title = $deceased['religion']=='Christian' ? 'Night Services' : 'Night Prayers';
          $html .= '<div style="margin-top:10px;font-weight:bold;">'. $night_title .'</div>';

          $night_serv = '';
          foreach ($serv_type['more_nightserv'] as $key => $value) {
            if (is_array($value)) {
              if (!empty($value['night_services'])) {
                $night_serv .= $key==0 ? format_date(strtotime($value['night_services']), 'custom', 'd F Y \A\t h:i A') : ','. format_date(strtotime($value['night_services']), 'custom', 'd F Y \A\t h:i A');
              }
            }
          }

          $html .= $night_serv;
        }

        if ($deceased['religion']=='Soka') {
          $html .= '<div style="margin-top:10px;font-weight:bold;">SSA Memorial Service on</div>';
          $html .= format_date(strtotime($serv_type['ssa_memorial']), 'custom', 'd F Y \A\t h:i A');

          $html .= '<div style="margin-top:10px;font-weight:bold;">Funeral Service on</div>';
          $html .= format_date(strtotime($serv_type['funeral_service']), 'custom', 'd F Y \A\t h:i A');
          /*
          $html .= '<div style="margin-top:10px;font-weight:bold;">Cortege Leaves at</div>';
          $html .= format_date(strtotime($serv_type['cortege_leaves_at']), 'custom', 'h:i A');
          */
        }

        switch ($deceased['religion']) {
          case 'Soka':
            $leaves_title = 'Cortege Leaves at';
            break;
          
          default:
            $leaves_title = 'Cortege Leaves on';
            break;
        }
        $html .= '<div style="margin-top:10px;font-weight:bold;">'. $leaves_title .'</div>';
        $html .= format_date(strtotime($serv_type['cortege_leaves']), 'custom', 'd F Y \A\t h:i A');

        if ($deceased['religion']=='Sikh') {
          /*
          $html .= '<div style="margin-top:10px;font-weight:bold;">Buses Leaves on</div>';
          $html .= format_date(strtotime($serv_type['buses_leaves']), 'custom', 'd F Y \A\t h:i A');
          */
          $from_street_2 = !empty($serv_type['from_address_2']) ? '<div>'. $serv_type['from_address_2'] .'</div>' : '';
          //$from_country = $countries[$serv_type['from_country']];
          $html .= '<div style="margin-top:10px;font-weight:bold;">From</div>';
          $html .= '<div>'. $serv_type['from_address_1'] .'</div>'. $from_street_2; //'<div>'. $serv_type['from_zip'] .'</div><div>'. $from_country .'</div>';

          $and_street_2 = !empty($serv_type['and_address_2']) ? '<div>'. $serv_type['and_address_2'] .'</div>' : '';
          //$and_country = $countries[$serv_type['and_country']];
          $html .= '<div style="margin-top:10px;font-weight:bold;">And</div>';
          $html .= '<div>'. $serv_type['and_address_1'] .'</div>'. $and_street_2; //'<div>'. $serv_type['and_zip'] .'</div><div>'. $and_country .'</div>';
        }

        $html .= '<div style="margin-top:10px;font-weight:bold;">For</div>';
        $html .= $serv_type['cortege_for'];

        $html .= '<div style="margin-top:10px;font-weight:bold;">At</div>';
        $html .= $serv_type['cortege_at'];

        $html .= '<div style="margin-top:10px;font-weight:bold;">At Time</div>';
        $html .= format_date(strtotime($serv_type['cortege_time']), 'custom', 'h:i A');

        if ($deceased['religion']=='Christian' || $deceased['religion']=='Catholic') {
          $bible_verses = !empty($serv_type['bible_verse']) ? '<p>'. publish_obituary_get_bible_word($serv_type['bible_verse']) .'</p>' : '';
          $bible_verses .= !empty($serv_type['custom_verse']) ? '<p>'. $serv_type['custom_verse'] .'</p>' : '';
          $html .= '<div style="margin-top:10px;font-weight:bold;">Bible Verses</div>';
          $html .= $bible_verses;
        }
      } else {
        //$html .= '<div style="margin-top:10px;font-weight:bold;">Bible Verses</div>';
        $html .= $serv_type['own_template']['value'];
      }
    }
  }
  
  // Acknowledgement details
  if ($oborder->form_type == 2) {
    $html .= '<h3 style="margin:20px 0 10px 0;background:#f4f4f4;padding:10px 0;font-size:15px;text-transform:uppercase;font-weight:bold;">Acknowledgement Details</h3>';

    if (!$publication_onsite) {
      $html .= '<div style="margin-top:10px;font-weight:bold;">Acknowledgement</div>';
      $html .= $serv_type['message'];

      $html .= '<div style="margin-top:10px;font-weight:bold;">Special Thanks to</div>';
      $html .= $serv_type['special_thanks'];

      $html .= '<div style="margin-top:10px;font-weight:bold;">Other remarks</div>';
      $html .= $serv_type['other_remarks'];
    }
  }

  // Condolence details
  if ($oborder->form_type == 3) {
    $html .= '<h3 style="margin:20px 0 10px 0;background:#f4f4f4;padding:10px 0;font-size:15px;text-transform:uppercase;font-weight:bold;">Condolence Details</h3>';

    if (!$publication_onsite) {
      $html .= '<div style="margin-top:10px;font-weight:bold;">Condolence Message</div>';
      $html .= $serv_type['message']['value'];

      $html .= '<div style="margin-top:10px;font-weight:bold;">From</div>';
      $html .= $serv_type['condo_from']['value'];

      $html .= '<div style="margin-top:10px;font-weight:bold;">Other remarks</div>';
      $html .= $serv_type['other_remarks'];
    }
  }

  // Memoriam details
  if ($oborder->form_type == 4) {
    $html .= '<h3 style="margin:20px 0 10px 0;background:#f4f4f4;padding:10px 0;font-size:15px;text-transform:uppercase;font-weight:bold;">In Memoriam Details</h3>';

    if (!$publication_onsite) {
      $memo_message = !empty($serv_type['custom_message']) ? $serv_type['custom_message'] : publish_obituary_get_memo_message_word($serv_type['select_message']);
    
      $rememberer='';
      foreach ($serv_type['more_rememberer'] as $key => $value) {
        if (is_array($value)) {
          if (!empty($value['rememberer_name'])) $rememberer .= '<div>' . $value['rememberer_name'] . '</div>';
        }
      }
      /*
      $html .= '<div style="margin-top:10px;font-weight:bold;">Year of Anniversary</div>';
      $html .= $serv_type['anniversary'] .'<br>'. $serv_type['anniversary_info'];
      */
      $html .= '<div style="margin-top:10px;font-weight:bold;">Message</div>';
      $html .= $memo_message;

      $html .= '<div style="margin-top:10px;font-weight:bold;">Dearly missed and fondly remembered by</div>';
      $html .= $rememberer;
    }
  }
  
  // Upload form & photo
  if ($oborder->form_type != 2) { //if not acknowledgement
    $html .= '<h3 style="margin:20px 0 10px 0;background:#f4f4f4;padding:10px 0;font-size:15px;text-transform:uppercase;font-weight:bold;">Upload Forms and Photo</h3>';

    if (!$publication_onsite) {
      $doc_upload = unserialize($oborder->upload);
      $deceased_photo = file_load($doc_upload['deceased_photo']);
      $death_certific = file_load($doc_upload['death_certificate']);
      $indemnity_form = file_load($doc_upload['indemnity_upload']);

      $html .= '<div style="margin-top:10px;font-weight:bold;">Deceased Photo</div>';
      //$html .= '<img src="' . image_style_url('medium', $deceased_photo->uri) . '">';
      $html .= '<a href="' . file_create_url($deceased_photo->uri) . '">'. $deceased_photo->filename . '</a>';

      if ($oborder->form_type==3 || $oborder->form_type==4) {
        $deceased_logo = file_load($doc_upload['company_logo']);
        $html .= '<div style="margin-top:10px;font-weight:bold;">Company Logo</div>';
        if (!empty($deceased_logo)) {
          $html .= '<img src="'. image_style_url('thumbnail', $deceased_logo->uri) .'">';
        }
      }

      $html .= '<div style="margin-top:10px;font-weight:bold;">Death Certificate</div>';
      $html .= '<a href="' . file_create_url($death_certific->uri) . '">'. $death_certific->filename . '</a>';

      $html .= '<div style="margin-top:10px;font-weight:bold;">Indemnity Form</div>';
      if (!empty($indemnity_form)) {
        $html .= '<a href="' . file_create_url($indemnity_form->uri) . '">'. $indemnity_form->filename . '</a>';
      }
    }
  }

  return $html;
}