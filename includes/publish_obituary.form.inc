<?php

/**
 * @file
 * Handles the form elements of the publish obituary form
 * All hooks are in the .module file.
 */

/**
 * Master form which calls an individual form for each step.
 * 
 * @see publish_obituary_form_validate().
 * @see publish_obituary_form_submit().
 * 
 * @param type $form
 * @param string $form_state
 * @return type 
 */
function publish_obituary_form($form, &$form_state, $kof) {

  require_once DRUPAL_ROOT . '/includes/locale.inc';
     
  if (!isset($form_state['stage'])) $form_state['stage'] = 'kind_of_advertisement';
  $form_state['kof'] = $kof;
 
  $form = array();
  $form = publish_obituary_get_header($form, $form_state);

  switch ($form_state['stage']) {
    
    case 'kind_of_advertisement':
      return publish_obituary_kind_of_advertisement_form($form, $form_state);
      break;  
    
    case 'publication_details': 
      return publish_obituary_publication_details_form($form, $form_state);  
      break;
    
    case 'advertiser_details': 
      return publish_obituary_advertiser_details_form($form, $form_state);  
      break;
 
    case 'deceased_details': 
      return publish_obituary_deceased_details_form($form, $form_state);  
      break;

    case 'family_friends': 
      return publish_obituary_family_friends_form($form, $form_state);  
      break;

    case 'funeral_details':
      return publish_obituary_funeral_details_form($form, $form_state);
      break;

    case 'acknowledgement_details':
      return publish_obituary_acknowledgement_details_form($form, $form_state);
      break;

    case 'condolence_details':
      return publish_obituary_condolence_details_form($form, $form_state);
      break;

    case 'memoriam_details':
      return publish_obituary_memoriam_details_form($form, $form_state);
      break;

    case 'certificate_photo':
      return publish_obituary_certificate_photo_form($form, $form_state);
      break;

    case 'adv_review':
      return publish_obituary_adv_review_form($form, $form_state);
      break;
 
    default:
      return publish_obituary_kind_of_advertisement_form($form, $form_state);
     break; 
  }
  
  return $form;
    
}


/**
 * Form for the kind_of_advertisement step.
 * 
 * @see publish_obituary_form().
 *  
 * @param type $form
 * @param type $form_state
 * @return type 
 */
function publish_obituary_kind_of_advertisement_form($form, &$form_state) {
  global $user;
  global $base_url;
  if ($user->uid==0) {
    drupal_set_message('To publish an obituary, please <a href="'. $base_url .'/user/login?destination='. current_path() .'">login</a> or <a href="'. $base_url .'/user/register?destination='. current_path() .'">register</a> an account.', 'error');
  }
  $values = isset($form_state['multistep_values']['kind_of_advertisement']) ? $form_state['multistep_values']['kind_of_advertisement'] : array();

  $form['form_title'] = array(
    '#markup' => '<h2>Advertisement Type</h2>',
    '#suffix' => '<div class="step">'.$form_state['active_stage'].'/'.$form_state['count_stage'].'</div>',
  );

  $form['kind_of_advertisement']['adv_choosed'] = array(
    '#type' => 'radios',
    '#options' => array(1 => 'Print', 2 => 'Online'),
    //'#required' => TRUE,
    '#disabled' => $user->uid==0 ? TRUE : FALSE,
    '#default_value' => isset($values['adv_choosed']) ? $values['adv_choosed'] : NULL,
  );
  
  $form['next'] = array(
    '#type' => 'submit',
    '#value' => t('Next Step')
  );
    
  return $form;
}

/**
 * Form for the publication_details step.
 * 
 * @see publish_obituary_form().
 */  
function publish_obituary_publication_details_form($form, &$form_state) {
  $values = isset($form_state['multistep_values']['publication_details']) ? $form_state['multistep_values']['publication_details'] : array();

  $form['form_title'] = array(
    '#markup' => '<h2>Publication Details</h2>',
    '#suffix' => '<div class="step">'.$form_state['active_stage'].'/'.$form_state['count_stage'].'</div>',
  );

  $form['publication_details']['pub_choosed'] = array(
    '#type' => 'radios',
    '#options' => array(2 => 'Create online', 1 => 'Schedule an onsite visit'),
    '#required' => TRUE,
    '#default_value' => isset($values['pub_choosed']) ? $values['pub_choosed'] : NULL,
  );

  $form['publication_details']['media'] = array(
    '#type' => 'select',
    '#title' => 'Media',
    '#options' => publish_obituary_get_media_list(),
    '#ajax' => array(
      'event' => 'change',
      'wrapper' => 'price_wrapper',
      'callback' => 'price_ajax_callback',
      'method' => 'replace',
    ),
    //'#required' => TRUE,
    //'#disabled' => TRUE,
    '#default_value' => isset($values['media']) ? $values['media'] : NULL,
  );

  $form['publication_details']['pub_date'] = array(
    '#title' => 'Publication Date',
    '#type' => 'date_text',
    '#date_format' => 'd F Y',
    //'#required' => TRUE,
    //'#disabled' => TRUE,
    '#default_value' => isset($values['pub_date']) ? $values['pub_date'] : NULL,
  );

  $form['publication_details']['style'] = array(
    '#type' => 'radios',
    '#title' => 'Full Color or Black and White',
    '#options' => array(1 => 'Full Color', 2 => 'Black and White'),
    '#ajax' => array(
      'event' => 'change',
      'wrapper' => 'price_wrapper',
      'callback' => 'price_ajax_callback',
      'method' => 'replace',
    ),
    //'#required' => TRUE,
    //'#disabled' => TRUE,
    '#default_value' => isset($values['style']) ? $values['style'] : NULL,
  );

  $form['publication_details']['common_sizes'] = array(
    '#type' => 'select',
    '#title' => 'Common Sizes',
    '#options' => publish_obituary_common_sizes_list(),
    '#ajax' => array(
      'event' => 'change',
      'wrapper' => 'price_wrapper',
      'callback' => 'price_ajax_callback',
      'method' => 'replace',
    ),
    //'#required' => TRUE,
    //'#disabled' => TRUE,
    '#default_value' => isset($values['common_sizes']) ? $values['common_sizes'] : NULL,
  );

  $form['publication_details']['custom_size'] = array(
    'height' => array(
      '#type' => 'select',
      '#title' => 'Custom Size',
      '#options' => publish_obituary_custom_height_list(),
      '#prefix' => '<div class="form-item-wrapper form-item-custom-size-price">',
      //'#required' => TRUE,
      //'#disabled' => TRUE,
      '#ajax' => array(
				'event' => 'change',
				'wrapper' => 'price_wrapper',
				'callback' => 'price_ajax_callback',
				'method' => 'replace',
			),
      '#default_value' => isset($values['height']) ? $values['height'] : NULL,
    ),
    'column' => array(
      '#type' => 'select',
      '#options' => publish_obituary_custom_column_list(),
      //'#required' => TRUE,
      //'#disabled' => TRUE,
      '#ajax' => array(
				'event' => 'change',
				'wrapper' => 'price_wrapper',
				'callback' => 'price_ajax_callback',
				'method' => 'replace',
			),
      '#default_value' => isset($values['column']) ? $values['column'] : NULL,
    ),
  );

  /**
   * Preparing price for size ajax
   */
  if (!empty($form_state['values']['media']) || !empty($form_state['values']['style']) || !empty($form_state['values']['height']) || !empty($form_state['values']['column']) || !empty($form_state['values']['common_sizes'])) {
    if (!empty($form_state['values']['media'])) {
      $media = publish_obituary_get_media_val($form_state['values']['media']);
      $media_price = $media['size_rates'];
    } else {
      $media_price = 0;
    }    
    if (!empty($form_state['values']['common_sizes'])) {
      $size_arr = explode('-', $form_state['values']['common_sizes']);
      $size_price = $size_arr[0] * $size_arr[1] * $media_price;
    } else {
      if (!empty($form_state['values']['height']) || !empty($form_state['values']['column'])) {
        if (!empty($form_state['values']['height'])) {
          $width_s = $form_state['values']['height'];
        } else {
          $width_s = 1;
        }
        if (!empty($form_state['values']['column'])) {
          $column_s = $form_state['values']['column'];
        } else {
          $column_s = 1;
        }
        $size_price = $width_s * $column_s * $media_price;
      } else {
        $size_price = 0;
      }
    }
    if (!empty($form_state['values']['style']) && $form_state['values']['style'] == 1) {
      $style_charge = ($media['color_rates'] * $size_price) / 100;
    } else {
      $style_price = 0;
    }
    $sub_price = $size_price + $style_charge;
    $gst_price = (variable_get('public_gst') * $sub_price) / 100;
    $price = $sub_price + $gst_price;
  } elseif (isset($values['price'])) {
    $price = str_replace(',', '', $values['price']);
  } else {
    $price = 0;
  }

  $form['publication_details']['price'] = array(
    '#type' => 'textfield',
    '#value' => number_format($price, 2),
    '#prefix' => '<div id="price_wrapper" class="publication-price"><div class="currency-symbol">$</div>',
    '#suffix' => '</div></div>',
    '#disabled' => TRUE,
    '#default_value' => isset($values['price']) ? $values['price'] : NULL,
  );

  $form['publication_details']['quote_me'] = array(
    '#type' => 'checkbox',
    '#title' => 'Please quote me.',
    //'#disabled' => TRUE,
    '#default_value' => isset($values['quote_me']) ? $values['quote_me'] : NULL,
  );

  $form['back'] = array(
    '#type' => 'submit',
    '#value' => t('Back')
  );
  
  $form['next'] = array(
    '#type' => 'submit',
    '#value' => t('Next Step')
  );
  return $form;
}

	/**
	 * Ajax callback function
	 */
	function price_ajax_callback($form, &$form_state) {
		return $form['publication_details']['price'];
	}

/**
 * Form for the advertiser_details step.
 * 
 * @see publish_obituary_form().
 *  
 * @param type $form
 * @param type $form_state
 * @return type 
 */
function publish_obituary_advertiser_details_form($form, &$form_state) {
  global $user;
  $user = user_load($user->uid);
  
  $values = isset($form_state['multistep_values']['advertiser_details']) ? $form_state['multistep_values']['advertiser_details'] : array();
  
  $form['form_title'] = array(
    '#markup' => '<h2>Advertiser Details</h2>',
    '#suffix' => '<div class="step">'.$form_state['active_stage'].'/'.$form_state['count_stage'].'</div>',
  );

  $form['advertiser_details']['name'] = array(
    '#type' => 'textfield',
    '#title' => 'Advertiser Name',
    '#attributes' => array('placeholder' => 'Name'),
    '#required' => TRUE,
    '#default_value' => isset($values['name']) ? $values['name'] : $user->realname,
  );

  $form['advertiser_details']['company_name'] = array(
    '#type' => 'textfield',
    '#title' => 'Company Name (if applicable)',
    '#attributes' => array('placeholder' => 'Company Name'),
    '#default_value' => isset($values['company_name']) ? $values['company_name'] : field_get_items('user', $user, 'field_account_address')[0]['organisation_name'],
  );

  $form['advertiser_details']['contact_number'] = array(
    '#type' => 'textfield',
    '#title' => 'Contact Number',
    '#attributes' => array('placeholder' => 'Contact Number'),
    '#required' => TRUE,
    '#default_value' => isset($values['contact_number']) ? $values['contact_number'] : field_get_items('user', $user, 'field_account_phone')[0]['number'],
  );

  $form['advertiser_details']['email'] = array(
    '#type' => 'textfield',
    '#title' => 'Email Address',
    '#required' => TRUE,
    '#attributes' => array('placeholder' => 'Email Address'),
    '#default_value' => isset($values['email']) ? $values['email'] : $user->mail,
  );

  $form['advertiser_details']['billing_address'] = array(
    'street_address_1' => array(
      '#type' => 'textfield',
      '#title' => 'Billing Address',
      '#attributes' => array('placeholder' => 'Street Address 1'),
      '#required' => TRUE,
      '#default_value' => isset($values['street_address_1']) ? $values['street_address_1'] : field_get_items('user', $user, 'field_account_address')[0]['thoroughfare'],
    ),
    'street_address_2' => array(
      '#type' => 'textfield',
      '#attributes' => array('placeholder' => 'Street Address 2'),
      '#default_value' => isset($values['street_address_2']) ? $values['street_address_2'] : field_get_items('user', $user, 'field_account_address')[0]['premise'],
    ),
    'zip' => array(
      '#type' => 'textfield',
      '#attributes' => array('placeholder' => 'Zip Code'),
      '#required' => TRUE,
      '#default_value' => isset($values['zip']) ? $values['zip'] : field_get_items('user', $user, 'field_account_address')[0]['postal_code'],
    ),
    'country' => array(
      '#type' => 'select',
      '#options' => array('' => 'Country') + country_get_list(),
      '#default_value' => isset($values['country']) ? $values['country'] : field_get_items('user', $user, 'field_account_address')[0]['country'],
    ),
  );

  $form['advertiser_details']['relationship'] = array(
    '#type' => 'textfield',
    '#title' => 'Relationship to Deceased',
    '#attributes' => array('placeholder' => 'Your Relantionship'),
    '#required' => TRUE,
    '#default_value' => isset($values['relationship']) ? $values['relationship'] : NULL,
  );
  
  $form['back'] = array(
    '#type' => 'submit',
    '#value' => t('Back')
  );
  
  $form['next'] = array(
    '#type' => 'submit',
    '#value' => t('Next Step')
  );
 
  return $form;
}


/**
 * Form for the deceased_details step.
 * 
 * @see publish_obituary_form().
 *  
 * @param type $form
 * @param type $form_state
 * @return type 
 */
function publish_obituary_deceased_details_form($form, &$form_state) {
  
  $values = isset($form_state['multistep_values']['deceased_details']) ? $form_state['multistep_values']['deceased_details'] : array();
  
  $form['form_title'] = array(
    '#markup' => '<h2>Deceased Details</h2>',
    '#suffix' => '<div class="step">'.$form_state['active_stage'].'/'.$form_state['count_stage'].'</div>',
  );

  if ($form_state['kof'] == 'acknowledgement' && $form_state['multistep_values']['kind_of_advertisement']['adv_choosed'] == 2) {
    /*
    $o_obi = views_get_view_result('online_obituaries','default');
    $dec_opt=array();
    foreach ($o_obi as $o_key => $o_val) {
      $dec_opt[$o_val->nid] = $o_val->node_title;
    }
    $form['deceased_details']['deceased_name'] = array(
      '#type' => 'select',
      '#title' => 'Deceased Name',
      '#options' => $dec_opt,
      '#ajax' => array(
        'event' => 'change',
        //'wrapper' => 'depart_religion_wrapper',
        'callback' => 'obi_decease_ajax_callback',
        //'method' => 'replace',
      ),
      //'#attributes' => array('placeholder' => 'Deceased Name'),
      '#required' => TRUE,
      '#default_value' => isset($values['deceased_name']) ? $values['deceased_name'] : NULL,
    );
    */
    $form['deceased_details']['deceased_name'] = array(
      '#type' => 'textfield',
      '#title' => 'Deceased Name',
      '#maxlength' => 128,
      '#autocomplete_path' => 'obituary/autocomplete',
      '#attributes' => array('placeholder' => 'Deceased Name'),
      '#required' => TRUE,
      '#default_value' => isset($values['deceased_name']) ? $values['deceased_name'] : NULL,
    );
    /*
    if (!empty($form_state['values']['deceased_name'])) {
      $obi_node = node_load($form_state['values']['deceased_name']);
      $depart_date = date('Y-m-d H:i:s', field_get_items('node', $obi_node, 'field_obituary_departed')[0]['value']);
      $obi_religion = ucfirst(field_get_items('node', $obi_node, 'field_obituary_religion')[0]['value']);
    } else {
      if (isset($values['departed_date'])) {
        $depart_date = $values['departed_date'] .' 00:00:00';
        $obi_religion = $values['religion'];
        unset($values['departed_date']);
        unset($values['religion']);
      } else {
        $depart_date = NULL;
        $obi_religion = NULL;
      }
    }
    */
    $form['deceased_details']['departed_date'] = array(
      '#title' => 'Departed Date',
      '#type' => 'date_text',
      '#date_format' => 'd F Y',
      //'#required' => TRUE,
      '#disabled' => TRUE,
      '#prefix' => '<div id="depart_date_wrapper" style="display:none;">',
      '#suffix' => '</div>',
      //'#default_value' => $depart_date,
      '#default_value' => isset($values['departed_date']) ? $values['departed_date'] : NULL,
    );
  } else {
    $form['deceased_details']['deceased_name'] = array(
      '#type' => 'textfield',
      '#title' => 'Deceased Name',
      '#attributes' => array('placeholder' => 'Deceased Name'),
      '#required' => TRUE,
      '#default_value' => isset($values['deceased_name']) ? $values['deceased_name'] : NULL,
    );

    $form['deceased_details']['departed_date'] = array(
      '#title' => 'Departed Date',
      '#type' => 'date_text',
      '#date_format' => 'd F Y',
      '#required' => TRUE,
      '#default_value' => isset($values['departed_date']) ? $values['departed_date'] : NULL,
    );
  }
  
  if ($form_state['kof'] == 'death-announcement') {
    $form['deceased_details']['age'] = array(
      '#type' => 'textfield',
      '#title' => 'Age',
      '#attributes' => array('placeholder' => 'Deceased Age'),
      '#required' => TRUE,
      '#default_value' => isset($values['age']) ? $values['age'] : NULL,
    );
  }
  
  if ($form_state['kof'] == 'acknowledgement' && $form_state['multistep_values']['kind_of_advertisement']['adv_choosed'] == 2) {
    $form['deceased_details']['religion'] = array(
      '#type' => 'select',
      '#title' => 'Religion',
      '#options' => array(
        '' => '- Select -',
        'Christian' => 'Christian',
        'Catholic' => 'Catholic',
        'Buddhist' => 'Buddhist',
        'Taoist' => 'Taoist',
        'Hindu' => 'Hindu',
        'Sikh' => 'Sikh',
        'Soka' => 'Soka',
        'Free thinker' => 'Free thinker',
        'Others' => 'Others',
      ),
      //'#value' => $obi_religion,
      //'#required' => TRUE,
      '#disabled' => TRUE,
      '#prefix' => '<div id="depart_religion_wrapper" style="display:none;">',
      '#suffix' => '</div>',
      '#default_value' => isset($values['religion']) ? $values['religion'] : NULL,
    );
  } else {
    if ($form_state['kof'] == 'death-announcement' || ($form_state['kof'] == 'acknowledgement' && $form_state['multistep_values']['kind_of_advertisement']['adv_choosed'] == 1)) {
      $form['deceased_details']['religion'] = array(
        '#type' => 'select',
        '#title' => 'Religion',
        '#options' => array(
          'Christian' => 'Christian',
          'Catholic' => 'Catholic',
          'Buddhist' => 'Buddhist',
          'Taoist' => 'Taoist',
          'Hindu' => 'Hindu',
          'Sikh' => 'Sikh',
          'Soka' => 'Soka',
          'Free thinker' => 'Free thinker',
          'Others' => 'Others',
        ),
        '#required' => TRUE,
        '#default_value' => isset($values['religion']) ? $values['religion'] : NULL,
      );
    }
  }

  if ($form_state['kof'] == 'death-announcement') {
    $form['deceased_details']['personality'] = array(
      '#title' => 'Deceased Personality',
      '#type' => 'textarea',
      '#rows' => 7,
      '#attributes' => array('placeholder' => 'Describe deceased personality'),
      '#default_value' => isset($values['personality']) ? $values['personality'] : NULL,
    );
  }
  
  $form['back'] = array(
    '#type' => 'submit',
    '#value' => t('Back')
  );
  
  $form['next'] = array(
    '#type' => 'submit',
    '#value' => t('Next Step')
  );
  
  return $form;
}

/**
 * Ajax callback function
 */
function obi_decease_ajax_callback(&$form, &$form_state) {
  $commands = array();
  $commands[] = ajax_command_replace("#depart_date_wrapper", render($form['deceased_details']['departed_date']));
  $commands[] = ajax_command_replace("#depart_religion_wrapper", render($form['deceased_details']['religion']));
  $page = array('#type' => 'ajax', '#commands' => $commands);
  //ajax_deliver($page);
  return $page;
}

/**
 * Form for the family_friends step.
 * 
 * @see publish_obituary_form().
 *  
 * @param type $form
 * @param type $form_state
 * @return type 
 */
function publish_obituary_family_friends_form($form, &$form_state, $no_js_use = FALSE) {

  $values = isset($form_state['multistep_values']['family_friends']) ? $form_state['multistep_values']['family_friends'] : array();
  
  $form['form_title'] = array(
    '#markup' => '<h2>Family and Friends</h2>',
    '#suffix' => '<div class="step">'.$form_state['active_stage'].'/'.$form_state['count_stage'].'</div>',
  );

  $form['family_friends']['spouse'][0]['spouse_as'] = array(
    '#type' => 'select',
    '#tree' => TRUE,
    '#title' => 'Spouse',
    '#options' => array(
      '' => '- Select -',
      'husband' => 'Husband',
      'wife' => 'Wife',
    ),
    '#default_value' => isset($values['spouse_as']) ? $values['spouse_as'] : NULL,
  );

  $form['family_friends']['spouse'][0]['spouse_name'] = array(
    '#type' => 'textfield',
    '#tree' => TRUE,
    //'#attributes' => array('placeholder' => 'Name'),
    '#default_value' => isset($values['spouse_name']) ? $values['spouse_name'] : NULL,
  );

  /**
   * Form element as ajax callback return for more fathers
   */
  $form['family_friends']['more_fathers'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-fathers">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['fnum_names'])) {
    $form_state['fnum_names'] = 1;
  }

  for ($i = 0; $i < $form_state['fnum_names']; $i++) {
    $form['family_friends']['more_fathers'][$i]['father_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Father' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Son Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => isset($values['more_fathers'][$i]['father_name']) ? $values['more_fathers'][$i]['father_name'] : NULL,
    );

    $form['family_friends']['more_fathers'][$i]['mother_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Mother' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Daughter-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => isset($values['more_fathers'][$i]['mother_name']) ? $values['more_fathers'][$i]['mother_name'] : NULL,
    );
  }

  $form['family_friends']['more_fathers']['add_father'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_father',
    '#submit' => array('fathers_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'fathers_add_more_callback',
      'wrapper' => 'more-fathers',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_fathers']['remove_father']['#ajax'])) {
       unset($form['more_fathers']['remove_father']['#ajax']);
    }
    unset($form['more_fathers']['add_father']['#ajax']);
  }
  /** End add more ajax for father */

  /**
   * Form element as ajax callback return for more sons
   */
  $form['family_friends']['more_sons'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-sons">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['pnum_names'])) {
    $form_state['pnum_names'] = 1;
  }

  for ($i = 0; $i < $form_state['pnum_names']; $i++) {
    $form['family_friends']['more_sons'][$i]['son_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Sons' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Son Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => isset($values['more_sons'][$i]['son_name']) ? $values['more_sons'][$i]['son_name'] : NULL,
    );

    $form['family_friends']['more_sons'][$i]['daughter_law_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Daughters-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Daughter-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => isset($values['more_sons'][$i]['daughter_law_name']) ? $values['more_sons'][$i]['daughter_law_name'] : NULL,
    );
  }

  $form['family_friends']['more_sons']['add_son'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_son',
    '#submit' => array('sons_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'sons_add_more_callback',
      'wrapper' => 'more-sons',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_sons']['remove_son']['#ajax'])) {
       unset($form['more_sons']['remove_son']['#ajax']);
    }
    unset($form['more_sons']['add_son']['#ajax']);
  }
  /** End add more ajax for sons */

  /**
   * Form element as ajax callback return for more daughters
   */
  $form['family_friends']['more_daughters'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-daughters">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['dnum_names'])) {
    $form_state['dnum_names'] = 1;
  }
	
  for ($i = 0; $i < $form_state['dnum_names']; $i++) {
    $form['family_friends']['more_daughters'][$i]['daughter_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Daughters' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Daughter Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => isset($values['more_daughters'][$i]['daughter_name']) ? $values['more_daughters'][$i]['daughter_name'] : NULL,
    );
    $form['family_friends']['more_daughters'][$i]['sons_law_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Sons-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Son-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => isset($values['more_daughters'][$i]['sons_law_name']) ? $values['more_daughters'][$i]['sons_law_name'] : NULL,
    );
  }
  
  $form['family_friends']['more_daughters']['add_daughter'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_daughter',
    '#submit' => array('daughters_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'daughters_add_more_callback',
      'wrapper' => 'more-daughters',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_daughters']['remove_daughter']['#ajax'])) {
       unset($form['more_daughters']['remove_daughter']['#ajax']);
    }
    unset($form['more_daughters']['add_daughter']['#ajax']);
  }
  /** End add more ajax for daughter */

  /**
   * Form element as ajax callback return for more grandson
   */
  $form['family_friends']['more_grandsons'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-grandsons">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['gsnum_names'])) {
    $form_state['gsnum_names'] = 1;
  }
	
  for ($i = 0; $i < $form_state['gsnum_names']; $i++) {
    $form['family_friends']['more_grandsons'][$i]['grandson_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Grandsons' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Grandson Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => isset($values['more_grandsons'][$i]['grandson_name']) ? $values['more_grandsons'][$i]['grandson_name'] : NULL,
    );
    $form['family_friends']['more_grandsons'][$i]['granddaughter_law_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Granddaughters-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Granddaughter-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => isset($values['more_grandsons'][$i]['granddaughter_law_name']) ? $values['more_grandsons'][$i]['granddaughter_law_name'] : NULL,
    );
  }
  
  $form['family_friends']['more_grandsons']['add_grandson'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_grandson',
    '#submit' => array('grandsons_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'grandsons_add_more_callback',
      'wrapper' => 'more-grandsons',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_grandsons']['remove_grandson']['#ajax'])) {
       unset($form['more_grandsons']['remove_grandson']['#ajax']);
    }
    unset($form['more_grandsons']['add_grandson']['#ajax']);
  }
  /** End add more ajax for grandson */

  /**
   * Form element as ajax callback return for more granddaughter
   */
  $form['family_friends']['more_granddaughters'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-granddaughters">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['gdnum_names'])) {
    $form_state['gdnum_names'] = 1;
  }
	
  for ($i = 0; $i < $form_state['gdnum_names']; $i++) {
    $form['family_friends']['more_granddaughters'][$i]['granddaughter_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Granddaughters' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Granddaughter Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => isset($values['more_granddaughters'][$i]['granddaughter_name']) ? $values['more_granddaughters'][$i]['granddaughter_name'] : NULL,
    );
    $form['family_friends']['more_granddaughters'][$i]['grandson_law_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Grandsons-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Grandson-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => isset($values['more_granddaughters'][$i]['grandson_law_name']) ? $values['more_granddaughters'][$i]['grandson_law_name'] : NULL,
    );
  }
  
  $form['family_friends']['more_granddaughters']['add_granddaughter'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_granddaughter',
    '#submit' => array('granddaughters_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'granddaughters_add_more_callback',
      'wrapper' => 'more-granddaughters',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_granddaughters']['remove_granddaughter']['#ajax'])) {
       unset($form['more_granddaughters']['remove_granddaughter']['#ajax']);
    }
    unset($form['more_granddaughters']['add_granddaughter']['#ajax']);
  }
  /** End add more ajax for granddaughter */

    /**
   * Form element as ajax callback return for more great grandson
   */
  $form['family_friends']['more_greatgrandsons'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-greatgrandsons">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['ggsnum_names'])) {
    $form_state['ggsnum_names'] = 1;
  }
	
  for ($i = 0; $i < $form_state['ggsnum_names']; $i++) {
    $form['family_friends']['more_greatgrandsons'][$i]['greatgrandson_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Great Grandsons' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Great Grandson Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => isset($values['more_greatgrandsons'][$i]['greatgrandson_name']) ? $values['more_greatgrandsons'][$i]['greatgrandson_name'] : NULL,
    );
    $form['family_friends']['more_greatgrandsons'][$i]['greatgranddaughter'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Great Granddaughters' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Great Granddaughter Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => isset($values['more_greatgrandsons'][$i]['greatgranddaughter']) ? $values['more_greatgrandsons'][$i]['greatgranddaughter'] : NULL,
    );
  }
  
  $form['family_friends']['more_greatgrandsons']['add_greatgrandson'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_greatgrandson',
    '#submit' => array('greatgrandsons_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'greatgrandsons_add_more_callback',
      'wrapper' => 'more-greatgrandsons',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_greatgrandsons']['remove_greatgrandson']['#ajax'])) {
       unset($form['more_greatgrandsons']['remove_greatgrandson']['#ajax']);
    }
    unset($form['more_greatgrandsons']['add_greatgrandson']['#ajax']);
  }
  /** End add more ajax for great grandson */

  /**
   * Form element as ajax callback return for more brother
   */
  $form['family_friends']['more_brothers'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-brothers">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['btnum_names'])) {
    $form_state['btnum_names'] = 1;
  }
	
  for ($i = 0; $i < $form_state['btnum_names']; $i++) {
    $form['family_friends']['more_brothers'][$i]['brother_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Brothers' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Brother Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => isset($values['more_brothers'][$i]['brother_name']) ? $values['more_brothers'][$i]['brother_name'] : NULL,
    );
    $form['family_friends']['more_brothers'][$i]['sister_law_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Sisters-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Sister-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => isset($values['more_brothers'][$i]['sister_law_name']) ? $values['more_brothers'][$i]['sister_law_name'] : NULL,
    );
  }
  
  $form['family_friends']['more_brothers']['add_brother'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_brother',
    '#submit' => array('brothers_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'brothers_add_more_callback',
      'wrapper' => 'more-brothers',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_brothers']['remove_brother']['#ajax'])) {
       unset($form['more_brothers']['remove_brother']['#ajax']);
    }
    unset($form['more_brothers']['add_brother']['#ajax']);
  }
  /** End add more ajax for brother */

  /**
   * Form element as ajax callback return for more sister
   */
  $form['family_friends']['more_sisters'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-sisters">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['stnum_names'])) {
    $form_state['stnum_names'] = 1;
  }
	
  for ($i = 0; $i < $form_state['stnum_names']; $i++) {
    $form['family_friends']['more_sisters'][$i]['sister_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Sisters' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Sister Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => isset($values['more_sisters'][$i]['sister_name']) ? $values['more_sisters'][$i]['sister_name'] : NULL,
    );
    $form['family_friends']['more_sisters'][$i]['brother_law_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Brothers-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Brother-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => isset($values['more_sisters'][$i]['brother_law_name']) ? $values['more_sisters'][$i]['brother_law_name'] : NULL,
    );
  }
  
  $form['family_friends']['more_sisters']['add_sister'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_sister',
    '#submit' => array('sisters_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'sisters_add_more_callback',
      'wrapper' => 'more-sisters',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_sisters']['remove_sister']['#ajax'])) {
       unset($form['more_sisters']['remove_sister']['#ajax']);
    }
    unset($form['more_sisters']['add_sister']['#ajax']);
  }
  /** End add more ajax for sister */  

  /**
   * Form element as ajax callback return for more father in law
   */
  $form['family_friends']['more_fatherlaws'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-fatherlaws">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['flnum_names'])) {
    $form_state['flnum_names'] = 1;
  }

  for ($i = 0; $i < $form_state['flnum_names']; $i++) {
    $form['family_friends']['more_fatherlaws'][$i]['fatherlaw_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Father-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Son Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => isset($values['more_fatherlaws'][$i]['fatherlaw_name']) ? $values['more_fatherlaws'][$i]['fatherlaw_name'] : NULL,
    );

    $form['family_friends']['more_fatherlaws'][$i]['motherlaw_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Mother-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Daughter-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => isset($values['more_fatherlaws'][$i]['motherlaw_name']) ? $values['more_fatherlaws'][$i]['motherlaw_name'] : NULL,
    );
  }

  $form['family_friends']['more_fatherlaws']['add_fatherlaw'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_fatherlaw',
    '#submit' => array('fatherlaws_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'fatherlaws_add_more_callback',
      'wrapper' => 'more-fatherlaws',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_fatherlaws']['remove_fatherlaw']['#ajax'])) {
       unset($form['more_fatherlaws']['remove_fatherlaw']['#ajax']);
    }
    unset($form['more_fatherlaws']['add_fatherlaw']['#ajax']);
  }
  /** End add more ajax for father in law */

  /**
   * Form element as ajax callback return for more brothers in law
   */
  $form['family_friends']['more_brotherslaw'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-brotherslaw">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['btlnum_names'])) {
    $form_state['btlnum_names'] = 1;
  }
	
  for ($i = 0; $i < $form_state['btlnum_names']; $i++) {
    $form['family_friends']['more_brotherslaw'][$i]['brotherlawmain_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Brothers-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Brother Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => isset($values['more_brotherslaw'][$i]['brotherlawmain_name']) ? $values['more_brotherslaw'][$i]['brotherlawmain_name'] : NULL,
    );
    $form['family_friends']['more_brotherslaw'][$i]['sisterlawpair_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Sisters-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Sister-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => isset($values['more_brotherslaw'][$i]['sisterlawpair_name']) ? $values['more_brotherslaw'][$i]['sisterlawpair_name'] : NULL,
    );
  }
  
  $form['family_friends']['more_brotherslaw']['add_brotherlaw'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_brotherlaw',
    '#submit' => array('brotherslaw_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'brotherslaw_add_more_callback',
      'wrapper' => 'more-brotherslaw',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_brotherslaw']['remove_brotherlaw']['#ajax'])) {
       unset($form['more_brotherslaw']['remove_brotherlaw']['#ajax']);
    }
    unset($form['more_brotherslaw']['add_brotherlaw']['#ajax']);
  }
  /** End add more ajax for brothers in law */

  /**
   * Form element as ajax callback return for more sisters in law
   */
  $form['family_friends']['more_sisterslaw'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-sisterslaw">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['stlnum_names'])) {
    $form_state['stlnum_names'] = 1;
  }
	
  for ($i = 0; $i < $form_state['stlnum_names']; $i++) {
    $form['family_friends']['more_sisterslaw'][$i]['sisterlawmain_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Sisters-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Sister Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => isset($values['more_sisterslaw'][$i]['sisterlawmain_name']) ? $values['more_sisterslaw'][$i]['sisterlawmain_name'] : NULL,
    );
    $form['family_friends']['more_sisterslaw'][$i]['brotherlawpair_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Brothers-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Brother-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => isset($values['more_sisterslaw'][$i]['brotherlawpair_name']) ? $values['more_sisterslaw'][$i]['brotherlawpair_name'] : NULL,
    );
  }
  
  $form['family_friends']['more_sisterslaw']['add_sisterlaw'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_sisterlaw',
    '#submit' => array('sisterslaw_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'sisterslaw_add_more_callback',
      'wrapper' => 'more-sisterslaw',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_sisterslaw']['remove_sisterlaw']['#ajax'])) {
       unset($form['more_sisterslaw']['remove_sisterlaw']['#ajax']);
    }
    unset($form['more_sisterslaw']['add_sisterlaw']['#ajax']);
  }
  /** End add more ajax for sisters in law */

  /**
   * Form element as ajax callback return for more nephew
   */
  $form['family_friends']['more_nephews'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-nephews">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['npnum_names'])) {
    $form_state['npnum_names'] = 1;
  }
	
  for ($i = 0; $i < $form_state['npnum_names']; $i++) {
    $form['family_friends']['more_nephews'][$i]['nephew_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Nephews' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Nephew Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => isset($values['more_nephews'][$i]['nephew_name']) ? $values['more_nephews'][$i]['nephew_name'] : NULL,
    );
    $form['family_friends']['more_nephews'][$i]['niece_law_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Nieces-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Niece-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => isset($values['more_nephews'][$i]['niece_law_name']) ? $values['more_nephews'][$i]['niece_law_name'] : NULL,
    );
  }
  
  $form['family_friends']['more_nephews']['add_nephew'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_nephew',
    '#submit' => array('nephews_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'nephews_add_more_callback',
      'wrapper' => 'more-nephews',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_nephews']['remove_nephew']['#ajax'])) {
       unset($form['more_nephews']['remove_nephew']['#ajax']);
    }
    unset($form['more_nephews']['add_nephew']['#ajax']);
  }
  /** End add more ajax for nephew */

  /**
   * Form element as ajax callback return for more niece
   */
  $form['family_friends']['more_nieces'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-nieces">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['ncnum_names'])) {
    $form_state['ncnum_names'] = 1;
  }
	
  for ($i = 0; $i < $form_state['ncnum_names']; $i++) {
    $form['family_friends']['more_nieces'][$i]['niece_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Nieces' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Niece Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => isset($values['more_nieces'][$i]['niece_name']) ? $values['more_nieces'][$i]['niece_name'] : NULL,
    );
    $form['family_friends']['more_nieces'][$i]['nephew_law_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Nephews-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Nephew-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => isset($values['more_nieces'][$i]['nephew_law_name']) ? $values['more_nieces'][$i]['nephew_law_name'] : NULL,
    );
  }
  
  $form['family_friends']['more_nieces']['add_niece'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_niece',
    '#submit' => array('nieces_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'nieces_add_more_callback',
      'wrapper' => 'more-nieces',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_nieces']['remove_niece']['#ajax'])) {
       unset($form['more_nieces']['remove_niece']['#ajax']);
    }
    unset($form['more_nieces']['add_niece']['#ajax']);
  }
  /** End add more ajax for niece */

  /**
   * Form element as ajax callback return for more grandnephew
   */
  $form['family_friends']['more_grandnephews'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-grandnephews">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['gnpnum_names'])) {
    $form_state['gnpnum_names'] = 1;
  }
	
  for ($i = 0; $i < $form_state['gnpnum_names']; $i++) {
    $form['family_friends']['more_grandnephews'][$i]['grandnephew_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Grandnephews' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Grandnephew Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => isset($values['more_grandnephews'][$i]['grandnephew_name']) ? $values['more_grandnephews'][$i]['grandnephew_name'] : NULL,
    );
    $form['family_friends']['more_grandnephews'][$i]['grandniece_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Grandnieces-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Grandniece Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => isset($values['more_grandnephews'][$i]['grandniece_name']) ? $values['more_grandnephews'][$i]['grandniece_name'] : NULL,
    );
  }
  
  $form['family_friends']['more_grandnephews']['add_grandnephew'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_grandnephew',
    '#submit' => array('grandnephews_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'grandnephews_add_more_callback',
      'wrapper' => 'more-grandnephews',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_grandnephews']['remove_grandnephew']['#ajax'])) {
       unset($form['more_grandnephews']['remove_grandnephew']['#ajax']);
    }
    unset($form['more_grandnephews']['add_grandnephew']['#ajax']);
  }
  /** End add more ajax for grandnephew */

  /**
   * Form element as ajax callback return for more grandniece
   */
  $form['family_friends']['more_grandnieces'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-grandnieces">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['gcpnum_names'])) {
    $form_state['gcpnum_names'] = 1;
  }
	
  for ($i = 0; $i < $form_state['gcpnum_names']; $i++) {
    $form['family_friends']['more_grandnieces'][$i]['grandniece_real_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Grandnieces' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Grandnephew Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => isset($values['more_grandnieces'][$i]['grandniece_real_name']) ? $values['more_grandnieces'][$i]['grandniece_real_name'] : NULL,
    );
    $form['family_friends']['more_grandnieces'][$i]['grandnephew_law_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Grandnephews-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Grandniece Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => isset($values['more_grandnieces'][$i]['grandnephew_law_name']) ? $values['more_grandnieces'][$i]['grandnephew_law_name'] : NULL,
    );
  }
  
  $form['family_friends']['more_grandnieces']['add_grandniece'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_grandniece',
    '#submit' => array('grandnieces_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'grandnieces_add_more_callback',
      'wrapper' => 'more-grandnieces',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_grandnieces']['remove_grandniece']['#ajax'])) {
       unset($form['more_grandnieces']['remove_grandniece']['#ajax']);
    }
    unset($form['more_grandnieces']['add_grandniece']['#ajax']);
  }
  /** End add more ajax for grandniece */

  /**
   * Form element as ajax callback return for more great grandnephew
   */
  $form['family_friends']['more_greatgrandnephews'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-greatgrandnephews">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['ggnpnum_names'])) {
    $form_state['ggnpnum_names'] = 1;
  }
	
  for ($i = 0; $i < $form_state['ggnpnum_names']; $i++) {
    $form['family_friends']['more_greatgrandnephews'][$i]['greatgrandnephew_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Great Grandnephews' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Grandnephew Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => isset($values['more_greatgrandnephews'][$i]['greatgrandnephew_name']) ? $values['more_greatgrandnephews'][$i]['greatgrandnephew_name'] : NULL,
    );
    $form['family_friends']['more_greatgrandnephews'][$i]['greatgrandniece_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Great Grandnieces' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Grandniece Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => isset($values['more_greatgrandnephews'][$i]['greatgrandniece_name']) ? $values['more_greatgrandnephews'][$i]['greatgrandniece_name'] : NULL,
    );
  }
  
  $form['family_friends']['more_greatgrandnephews']['add_greatgrandnephew'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_greatgrandnephew',
    '#submit' => array('greatgrandnephews_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'greatgrandnephews_add_more_callback',
      'wrapper' => 'more-greatgrandnephews',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_greatgrandnephews']['remove_greatgrandnephew']['#ajax'])) {
       unset($form['more_greatgrandnephews']['remove_greatgrandnephew']['#ajax']);
    }
    unset($form['more_greatgrandnephews']['add_greatgrandnephew']['#ajax']);
  }
  /** End add more ajax for great grandnephew */

  /**
   * Form element as ajax callback return for more uncle
   */
  $form['family_friends']['more_uncles'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-uncles">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['unnum_names'])) {
    $form_state['unnum_names'] = 1;
  }

  for ($i = 0; $i < $form_state['unnum_names']; $i++) {
    $form['family_friends']['more_uncles'][$i]['uncle_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Uncles' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'God Son Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => isset($values['more_uncles'][$i]['uncle_name']) ? $values['more_uncles'][$i]['uncle_name'] : NULL,
    );

    $form['family_friends']['more_uncles'][$i]['uncle_spouse_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Spouses' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'God Daughter-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => isset($values['more_uncles'][$i]['uncle_spouse_name']) ? $values['more_uncles'][$i]['uncle_spouse_name'] : NULL,
    );
  }

  $form['family_friends']['more_uncles']['add_uncle'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_uncle',
    '#submit' => array('uncles_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'uncles_add_more_callback',
      'wrapper' => 'more-uncles',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_uncles']['remove_uncle']['#ajax'])) {
       unset($form['more_uncles']['remove_uncle']['#ajax']);
    }
    unset($form['more_uncles']['add_uncle']['#ajax']);
  }
  /** End add more ajax for uncle */

  /**
   * Form element as ajax callback return for more auntie
   */
  $form['family_friends']['more_aunties'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-aunties">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['aunum_names'])) {
    $form_state['aunum_names'] = 1;
  }

  for ($i = 0; $i < $form_state['aunum_names']; $i++) {
    $form['family_friends']['more_aunties'][$i]['auntie_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Aunties' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'God Son Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => isset($values['more_aunties'][$i]['auntie_name']) ? $values['more_aunties'][$i]['auntie_name'] : NULL,
    );

    $form['family_friends']['more_aunties'][$i]['auntie_spouse_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Spouses' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'God Daughter-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => isset($values['more_aunties'][$i]['auntie_spouse_name']) ? $values['more_aunties'][$i]['auntie_spouse_name'] : NULL,
    );
  }

  $form['family_friends']['more_aunties']['add_auntie'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_auntie',
    '#submit' => array('aunties_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'aunties_add_more_callback',
      'wrapper' => 'more-aunties',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_aunties']['remove_auntie']['#ajax'])) {
       unset($form['more_aunties']['remove_auntie']['#ajax']);
    }
    unset($form['more_aunties']['add_auntie']['#ajax']);
  }
  /** End add more ajax for auntie */

  /**
   * Form element as ajax callback return for more cousin
   */
  $form['family_friends']['more_cousins'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-cousins">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['csnum_names'])) {
    $form_state['csnum_names'] = 1;
  }

  for ($i = 0; $i < $form_state['csnum_names']; $i++) {
    $form['family_friends']['more_cousins'][$i]['cousin_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Cousins' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'God Son Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => isset($values['more_cousins'][$i]['cousin_name']) ? $values['more_cousins'][$i]['cousin_name'] : NULL,
    );

    $form['family_friends']['more_cousins'][$i]['cousin_law_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Cousins-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'God Daughter-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => isset($values['more_cousins'][$i]['cousin_law_name']) ? $values['more_cousins'][$i]['cousin_law_name'] : NULL,
    );
  }

  $form['family_friends']['more_cousins']['add_cousin'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_cousin',
    '#submit' => array('cousins_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'cousins_add_more_callback',
      'wrapper' => 'more-cousins',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_cousins']['remove_cousin']['#ajax'])) {
       unset($form['more_cousins']['remove_cousin']['#ajax']);
    }
    unset($form['more_cousins']['add_cousin']['#ajax']);
  }
  /** End add more ajax for cousin */

  /**
   * Form element as ajax callback return for more god sons
   */
  $form['family_friends']['more_godsons'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-godsons">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['gosnum_names'])) {
    $form_state['gosnum_names'] = 1;
  }

  for ($i = 0; $i < $form_state['gosnum_names']; $i++) {
    $form['family_friends']['more_godsons'][$i]['godson_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'God Sons' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'God Son Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => isset($values['more_godsons'][$i]['godson_name']) ? $values['more_godsons'][$i]['godson_name'] : NULL,
    );

    $form['family_friends']['more_godsons'][$i]['goddaughter_law_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'God Daughters-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'God Daughter-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => isset($values['more_godsons'][$i]['goddaughter_law_name']) ? $values['more_godsons'][$i]['goddaughter_law_name'] : NULL,
    );
  }

  $form['family_friends']['more_godsons']['add_godson'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_godson',
    '#submit' => array('godsons_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'godsons_add_more_callback',
      'wrapper' => 'more-godsons',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_godsons']['remove_godson']['#ajax'])) {
       unset($form['more_godsons']['remove_godson']['#ajax']);
    }
    unset($form['more_godsons']['add_godson']['#ajax']);
  }
  /** End add more ajax for god sons */

  /**
   * Form element as ajax callback return for more god daughters
   */
  $form['family_friends']['more_goddaughters'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-goddaughters">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['godnum_names'])) {
    $form_state['godnum_names'] = 1;
  }
	
  for ($i = 0; $i < $form_state['godnum_names']; $i++) {
    $form['family_friends']['more_goddaughters'][$i]['goddaughter_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'God Daughters' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'God Daughter Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => isset($values['more_goddaughters'][$i]['goddaughter_name']) ? $values['more_goddaughters'][$i]['goddaughter_name'] : NULL,
    );
    $form['family_friends']['more_goddaughters'][$i]['godsons_law_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'God Sons-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'God Son-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => isset($values['more_goddaughters'][$i]['godsons_law_name']) ? $values['more_goddaughters'][$i]['godsons_law_name'] : NULL,
    );
  }
  
  $form['family_friends']['more_goddaughters']['add_goddaughter'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_goddaughter',
    '#submit' => array('goddaughters_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'goddaughters_add_more_callback',
      'wrapper' => 'more-goddaughters',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_goddaughters']['remove_goddaughter']['#ajax'])) {
       unset($form['more_goddaughters']['remove_goddaughter']['#ajax']);
    }
    unset($form['more_goddaughters']['add_goddaughter']['#ajax']);
  }
  /** End add more ajax for god daughter */

  /**
   * Form element as ajax callback return for more god grandson
   */
  $form['family_friends']['more_godgrandsons'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-godgrandsons">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['gogsnum_names'])) {
    $form_state['gogsnum_names'] = 1;
  }
	
  for ($i = 0; $i < $form_state['gogsnum_names']; $i++) {
    $form['family_friends']['more_godgrandsons'][$i]['godgrandson_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'God Grandsons' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'God Grandson Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => isset($values['more_godgrandsons'][$i]['godgrandson_name']) ? $values['more_godgrandsons'][$i]['godgrandson_name'] : NULL,
    );
    $form['family_friends']['more_godgrandsons'][$i]['godgranddaughter_law_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'God Granddaughters-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'God Granddaughter-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => isset($values['more_godgrandsons'][$i]['godgranddaughter_law_name']) ? $values['more_godgrandsons'][$i]['godgranddaughter_law_name'] : NULL,
    );
  }
  
  $form['family_friends']['more_godgrandsons']['add_godgrandson'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_godgrandson',
    '#submit' => array('godgrandsons_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'godgrandsons_add_more_callback',
      'wrapper' => 'more-godgrandsons',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_godgrandsons']['remove_godgrandson']['#ajax'])) {
       unset($form['more_godgrandsons']['remove_godgrandson']['#ajax']);
    }
    unset($form['more_godgrandsons']['add_godgrandson']['#ajax']);
  }
  /** End add more ajax for god grandson */

  /**
   * Form element as ajax callback return for more god granddaughter
   */
  $form['family_friends']['more_godgranddaughters'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-godgranddaughters">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['ggdnum_names'])) {
    $form_state['ggdnum_names'] = 1;
  }
	
  for ($i = 0; $i < $form_state['ggdnum_names']; $i++) {
    $form['family_friends']['more_godgranddaughters'][$i]['godgranddaughter_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'God Granddaughters' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'God Granddaughter Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => isset($values['more_godgranddaughters'][$i]['godgranddaughter_name']) ? $values['more_godgranddaughters'][$i]['godgranddaughter_name'] : NULL,
    );
    $form['family_friends']['more_godgranddaughters'][$i]['godgrandson_law_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'God Grandsons-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'God Grandson-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => isset($values['more_godgranddaughters'][$i]['godgrandson_law_name']) ? $values['more_godgranddaughters'][$i]['godgrandson_law_name'] : NULL,
    );
  }
  
  $form['family_friends']['more_godgranddaughters']['add_godgranddaughter'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_godgranddaughter',
    '#submit' => array('godgranddaughters_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'godgranddaughters_add_more_callback',
      'wrapper' => 'more-godgranddaughters',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_godgranddaughters']['remove_godgranddaughter']['#ajax'])) {
       unset($form['more_godgranddaughters']['remove_godgranddaughter']['#ajax']);
    }
    unset($form['more_godgranddaughters']['add_godgranddaughter']['#ajax']);
  }
  /** End add more ajax for god granddaughter */

  /**
   * Form element as ajax callback return for more god great grandson
   */
  $form['family_friends']['more_godgreatgrandsons'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-godgreatgrandsons">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['gggsnum_names'])) {
    $form_state['gggsnum_names'] = 1;
  }
	
  for ($i = 0; $i < $form_state['gggsnum_names']; $i++) {
    $form['family_friends']['more_godgreatgrandsons'][$i]['godgreatgrandson_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'God Great Grandsons' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'God Grandson Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => isset($values['more_godgreatgrandsons'][$i]['godgreatgrandson_name']) ? $values['more_godgreatgrandsons'][$i]['godgreatgrandson_name'] : NULL,
    );
    $form['family_friends']['more_godgreatgrandsons'][$i]['godgreatgranddaughter_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'God Great Granddaughters' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'God Granddaughter-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => isset($values['more_godgreatgrandsons'][$i]['godgreatgranddaughter_name']) ? $values['more_godgreatgrandsons'][$i]['godgreatgranddaughter_name'] : NULL,
    );
  }
  
  $form['family_friends']['more_godgreatgrandsons']['add_godgreatgrandson'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_godgreatgrandson',
    '#submit' => array('godgreatgrandsons_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'godgreatgrandsons_add_more_callback',
      'wrapper' => 'more-godgreatgrandsons',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_godgreatgrandsons']['remove_godgreatgrandson']['#ajax'])) {
       unset($form['more_godgreatgrandsons']['remove_godgreatgrandson']['#ajax']);
    }
    unset($form['more_godgreatgrandsons']['add_godgreatgrandson']['#ajax']);
  }
  /** End add more ajax for god great grandson */

  /**
   * Form element as ajax callback return for more caregiver
   */
  $form['family_friends']['more_caregivers'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-caregivers">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['cgvnum_names'])) {
    $form_state['cgvnum_names'] = 1;
  }

  for ($i = 0; $i < $form_state['cgvnum_names']; $i++) {
    $form['family_friends']['more_caregivers'][$i]['caregiver_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Caregivers' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Caregiver Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#suffix' => '</div>',
      '#default_value' => isset($values['more_caregivers'][$i]['caregiver_name']) ? $values['more_caregivers'][$i]['caregiver_name'] : NULL,
    );
  }

  $form['family_friends']['more_caregivers']['add_caregiver'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_caregiver',
    '#submit' => array('caregivers_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'caregivers_add_more_callback',
      'wrapper' => 'more-caregivers',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_caregivers']['remove_caregiver']['#ajax'])) {
       unset($form['more_caregivers']['remove_caregiver']['#ajax']);
    }
    unset($form['more_caregivers']['add_caregiver']['#ajax']);
  }
  /** End add more ajax for caregiver */

  /**
   * Form element as ajax callback return for others
   */
  $form['family_friends']['more_others'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-others">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['othnum_names'])) {
    $form_state['othnum_names'] = 1;
  }
	
  for ($i = 0; $i < $form_state['othnum_names']; $i++) {
    $form['family_friends']['more_others'][$i]['other_title'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Others' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Title (e.g. Dog)',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => isset($values['more_others'][$i]['other_title']) ? $values['more_others'][$i]['other_title'] : NULL,
    );
    $form['family_friends']['more_others'][$i]['other_name'] = array(
      '#type' => 'textfield',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Name (e.g. Buddy)',
      ),
      '#suffix' => '</div>',
      '#default_value' => isset($values['more_others'][$i]['other_name']) ? $values['more_others'][$i]['other_name'] : NULL,
    );
  }
  
  $form['family_friends']['more_others']['add_other'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_other',
    '#submit' => array('others_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'others_add_more_callback',
      'wrapper' => 'more-others',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_others']['remove_other']['#ajax'])) {
       unset($form['more_others']['remove_other']['#ajax']);
    }
    unset($form['more_others']['add_other']['#ajax']);
  }
  /** End add more ajax for others*/

  $form['back'] = array(
    '#type' => 'submit',
    '#value' => t('Back'),
  );
  
  $form['next'] = array(
    '#type' => 'submit',
    '#value' => t('Next Step'),
  );
  
  return $form;
}

/**
 * Add more button submit and ajax callback for father
 */
function fathers_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['fnum_names'])) {
      $form_state['fnum_names'] = 0;
      $form_state['fnum_names']++;
  }

  $form_state['fnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function fathers_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_fathers'];
}
/**
 * Add more button submit and ajax callback for son
 */
function sons_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['pnum_names'])) {
      $form_state['pnum_names'] = 0;
      $form_state['pnum_names']++;
  }

  $form_state['pnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function sons_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_sons'];
}
/**
 * Add more button submit and ajax callback for daughter
 */
function daughters_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['dnum_names'])) {
      $form_state['dnum_names'] = 0;
      $form_state['dnum_names']++;
  }

  $form_state['dnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function daughters_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_daughters'];
}
/**
 * Add more button submit and ajax callback for grandson
 */
function grandsons_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['gsnum_names'])) {
      $form_state['gsnum_names'] = 0;
      $form_state['gsnum_names']++;
  }

  $form_state['gsnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function grandsons_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_grandsons'];
}
/**
 * Add more button submit and ajax callback for granddaughter
 */
function granddaughters_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['gdnum_names'])) {
      $form_state['gdnum_names'] = 0;
      $form_state['gdnum_names']++;
  }

  $form_state['gdnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function granddaughters_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_granddaughters'];
}
/**
 * Add more button submit and ajax callback for great grandson
 */
function greatgrandsons_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['ggsnum_names'])) {
      $form_state['ggsnum_names'] = 0;
      $form_state['ggsnum_names']++;
  }

  $form_state['ggsnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function greatgrandsons_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_greatgrandsons'];
}
/**
 * Add more button submit and ajax callback for brother
 */
function brothers_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['btnum_names'])) {
      $form_state['btnum_names'] = 0;
      $form_state['btnum_names']++;
  }

  $form_state['btnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function brothers_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_brothers'];
}
/**
 * Add more button submit and ajax callback for sister
 */
function sisters_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['stnum_names'])) {
      $form_state['stnum_names'] = 0;
      $form_state['stnum_names']++;
  }

  $form_state['stnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function sisters_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_sisters'];
}
/**
 * Add more button submit and ajax callback for father-in-law
 */
function fatherlaws_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['flnum_names'])) {
      $form_state['flnum_names'] = 0;
      $form_state['flnum_names']++;
  }

  $form_state['flnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function fatherlaws_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_fatherlaws'];
}
/**
 * Add more button submit and ajax callback for brothers in law
 */
function brotherslaw_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['btlnum_names'])) {
      $form_state['btlnum_names'] = 0;
      $form_state['btlnum_names']++;
  }

  $form_state['btlnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function brotherslaw_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_brotherslaw'];
}
/**
 * Add more button submit and ajax callback for sisters in law
 */
function sisterslaw_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['stlnum_names'])) {
      $form_state['stlnum_names'] = 0;
      $form_state['stlnum_names']++;
  }

  $form_state['stlnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function sisterslaw_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_sisterslaw'];
}
/**
 * Add more button submit and ajax callback for nephew
 */
function nephews_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['npnum_names'])) {
      $form_state['npnum_names'] = 0;
      $form_state['npnum_names']++;
  }

  $form_state['npnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function nephews_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_nephews'];
}
/**
 * Add more button submit and ajax callback for niece
 */
function nieces_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['ncnum_names'])) {
      $form_state['ncnum_names'] = 0;
      $form_state['ncnum_names']++;
  }

  $form_state['ncnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function nieces_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_nieces'];
}
/**
 * Add more button submit and ajax callback for grandnephew
 */
function grandnephews_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['gnpnum_names'])) {
      $form_state['gnpnum_names'] = 0;
      $form_state['gnpnum_names']++;
  }

  $form_state['gnpnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function grandnephews_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_grandnephews'];
}
/**
 * Add more button submit and ajax callback for grandniece
 */
function grandnieces_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['gcpnum_names'])) {
      $form_state['gcpnum_names'] = 0;
      $form_state['gcpnum_names']++;
  }

  $form_state['gcpnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function grandnieces_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_grandnieces'];
}
/**
 * Add more button submit and ajax callback for great grandnephew
 */
function greatgrandnephews_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['ggnpnum_names'])) {
      $form_state['ggnpnum_names'] = 0;
      $form_state['ggnpnum_names']++;
  }

  $form_state['ggnpnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function greatgrandnephews_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_greatgrandnephews'];
}
/**
 * Add more button submit and ajax callback for uncle
 */
function uncles_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['unnum_names'])) {
      $form_state['unnum_names'] = 0;
      $form_state['unnum_names']++;
  }

  $form_state['unnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function uncles_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_uncles'];
}
/**
 * Add more button submit and ajax callback for auntie
 */
function aunties_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['aunum_names'])) {
      $form_state['aunum_names'] = 0;
      $form_state['aunum_names']++;
  }

  $form_state['aunum_names']++;
  $form_state['rebuild'] = TRUE;
}

function aunties_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_aunties'];
}
/**
 * Add more button submit and ajax callback for cousin
 */
function cousins_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['csnum_names'])) {
      $form_state['csnum_names'] = 0;
      $form_state['csnum_names']++;
  }

  $form_state['csnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function cousins_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_cousins'];
}
/**
 * Add more button submit and ajax callback for god son
 */
function godsons_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['gosnum_names'])) {
      $form_state['gosnum_names'] = 0;
      $form_state['gosnum_names']++;
  }

  $form_state['gosnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function godsons_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_godsons'];
}
/**
 * Add more button submit and ajax callback for god daughter
 */
function goddaughters_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['godnum_names'])) {
      $form_state['godnum_names'] = 0;
      $form_state['godnum_names']++;
  }

  $form_state['godnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function goddaughters_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_goddaughters'];
}
/**
 * Add more button submit and ajax callback for god grandson
 */
function godgrandsons_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['gogsnum_names'])) {
      $form_state['gogsnum_names'] = 0;
      $form_state['gogsnum_names']++;
  }

  $form_state['gogsnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function godgrandsons_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_godgrandsons'];
}
/**
 * Add more button submit and ajax callback for god granddaughter
 */
function godgranddaughters_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['ggdnum_names'])) {
      $form_state['ggdnum_names'] = 0;
      $form_state['ggdnum_names']++;
  }

  $form_state['ggdnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function godgranddaughters_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_godgranddaughters'];
}
/**
 * Add more button submit and ajax callback for god great grandson
 */
function godgreatgrandsons_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['gggsnum_names'])) {
      $form_state['gggsnum_names'] = 0;
      $form_state['gggsnum_names']++;
  }

  $form_state['gggsnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function godgreatgrandsons_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_godgreatgrandsons'];
}
/**
 * Add more button submit and ajax callback for caregiver
 */
function caregivers_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['cgvnum_names'])) {
      $form_state['cgvnum_names'] = 0;
      $form_state['cgvnum_names']++;
  }

  $form_state['cgvnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function caregivers_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_caregivers'];
}
/**
 * Add more button submit and ajax callback for other
 */
function others_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['othnum_names'])) {
      $form_state['othnum_names'] = 0;
      $form_state['othnum_names']++;
  }

  $form_state['othnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function others_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_others'];
}

/**
 * Form for the funeral_details step of death announcement.
 * 
 * @see publish_obituary_form().
 *  
 * @param type $form
 * @param type $form_state
 * @return type 
 */
function publish_obituary_funeral_details_form($form, &$form_state, $no_js_use = FALSE) {

  $values = isset($form_state['multistep_values']['funeral_details']) ? $form_state['multistep_values']['funeral_details'] : array();
  
  $form['form_title'] = array(
    '#markup' => '<h2>Funeral Details</h2>',
    '#suffix' => '<div class="step">'.$form_state['active_stage'].'/'.$form_state['count_stage'].'</div>',
  );

  $dec_religion = $form_state['multistep_values']['deceased_details']['religion'];

  $form['funeral_details']['def_religion'] = array(
    '#type' => 'hidden',
    '#value' => $dec_religion,
  );

  $form['funeral_details']['template_choosed'] = array(
    '#type' => 'radios',
    '#options' => array(1 => 'Use our template', 2 => 'Write your own'),
    '#required' => TRUE,
    '#default_value' => isset($values['template_choosed']) ? $values['template_choosed'] : 1,
  );

  if ($dec_religion != 'Sikh') {
    $form['funeral_details']['wake_address'] = array(
      'wake_street_1' => array(
        '#type' => 'textfield',
        '#title' => 'Wake Address',
        '#attributes' => array('placeholder' => 'Street Address 1'),
        //'#required' => TRUE,  //edited for own textarea
        '#default_value' => isset($values['wake_street_1']) ? $values['wake_street_1'] : NULL,
      ),
      'wake_street_2' => array(
        '#type' => 'textfield',
        '#attributes' => array('placeholder' => 'Street Address 2'),
        '#default_value' => isset($values['wake_street_2']) ? $values['wake_street_2'] : NULL,
      ),
    );
  }

  if ($dec_religion=='Christian' || $dec_religion=='Catholic') {
    $night_title = $dec_religion=='Christian' ? 'Night Services' : 'Night Prayers';
    /** Start add more ajax for night service */
    $form['funeral_details']['more_nightserv'] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#prefix' => '<div id="more-nightserv">',
      '#suffix' => '</div>',
    );
  
    if (empty($form_state['nsnum_names'])) {
      $form_state['nsnum_names'] = 1;
    }
  
    for ($i = 0; $i < $form_state['nsnum_names']; $i++) {
      $form['funeral_details']['more_nightserv'][$i]['night_services'] = array(
        '#type' => 'date_text',
        '#title' => $i == 0 ? $night_title : '',
        '#date_format' => 'd F Y H:i',
        '#attributes' => array(
          'multiple' => 'multiple',
        ),
        //'#required' => $i == 0 ? TRUE : FALSE,
        '#required' => FALSE,
        '#default_value' => isset($values['more_nightserv'][$i]['night_services']) ? $values['more_nightserv'][$i]['night_services'] : NULL,
      );
    }
  
    $form['funeral_details']['more_nightserv']['add_nightserv'] = array(
      '#type' => 'submit',
      '#value' => '+',
      '#name' => 'add_more_nightserv',
      '#submit' => array('nightserv_add_more_add_one'),
      '#ajax' => array(
        'callback' => 'nightserv_add_more_callback',
        'wrapper' => 'more-nightserv',
      ),
    );
  
    if ($no_js_use) {
      if (!empty($form['more_nightserv']['remove_nightserv']['#ajax'])) {
         unset($form['more_nightserv']['remove_nightserv']['#ajax']);
      }
      unset($form['more_nightserv']['add_nightserv']['#ajax']);
    }
    /** End add more ajax for night service */
  }

  if ($dec_religion == 'Soka') {
    $form['funeral_details']['ssa_memorial'] = array(
      '#title' => 'SSA Memorial Service on',
      '#type' => 'date_text',
      '#date_format' => 'd F Y H:i',
      //'#required' => TRUE,
      '#default_value' => isset($values['ssa_memorial']) ? $values['ssa_memorial'] : NULL,
    );

    $form['funeral_details']['funeral_service'] = array(
      '#title' => 'Funeral Service on',
      '#type' => 'date_text',
      '#date_format' => 'd F Y H:i',
      //'#required' => TRUE, //edited for own textarea
      '#default_value' => isset($values['funeral_service']) ? $values['funeral_service'] : NULL,
    );
    /*
    $form['funeral_details']['cortege_leaves_at'] = array(
      '#title' => 'Cortege Leaves at',
      '#type' => 'date_text',
      '#date_format' => 'H:i',
      '#required' => TRUE,
      '#default_value' => isset($values['cortege_leaves_at']) ? $values['cortege_leaves_at'] : NULL,
    );
    */
  }

  switch ($dec_religion) {
    case 'Soka':
      $leaves_title = 'Cortege Leaves at';
      break;
    
    default:
      $leaves_title = 'Cortege Leaves on';
      break;
  }

  $form['funeral_details']['cortege_leaves'] = array(
    '#title' => $leaves_title,
    '#type' => 'date_text',
    '#date_format' => 'd F Y H:i',
    //'#required' => TRUE,
    '#default_value' => isset($values['cortege_leaves']) ? $values['cortege_leaves'] : NULL,
  );

  if ($dec_religion == 'Sikh') {
    /*
    $form['funeral_details']['buses_leaves'] = array(
      '#title' => 'Buses Leaves on',
      '#type' => 'date_text',
      '#date_format' => 'd F Y H:i',
      //'#required' => TRUE,
      '#default_value' => isset($values['buses_leaves']) ? $values['buses_leaves'] : NULL,
    );
    */

    $form['funeral_details']['bus_from'] = array(
      'from_address_1' => array(
        '#type' => 'textfield',
        '#title' => 'From',
        '#attributes' => array('placeholder' => 'Street Address 1'),
        //'#required' => TRUE, //edited for own textarea
        '#default_value' => isset($values['from_address_1']) ? $values['from_address_1'] : NULL,
      ),
      'from_address_2' => array(
        '#type' => 'textfield',
        '#attributes' => array('placeholder' => 'Street Address 2'),
        '#default_value' => isset($values['from_address_2']) ? $values['from_address_2'] : NULL,
      ),
      /*
      'from_zip' => array(
        '#type' => 'textfield',
        '#attributes' => array('placeholder' => 'Zip Code'),
        //'#required' => TRUE,
        '#default_value' => isset($values['from_zip']) ? $values['from_zip'] : NULL,
      ),
      'from_country' => array(
        '#type' => 'select',
        '#options' => country_get_list(),
        //'#attributes' => array('placeholder' => 'Country'),
        '#required' => TRUE,
        '#default_value' => isset($values['from_country']) ? $values['from_country'] : NULL,
      ),
      */
    );

    $form['funeral_details']['bus_and'] = array(
      'and_address_1' => array(
        '#type' => 'textfield',
        '#title' => 'And',
        '#attributes' => array('placeholder' => 'Street Address 1'),
        //'#required' => TRUE,
        '#default_value' => isset($values['and_address_1']) ? $values['and_address_1'] : NULL,
      ),
      'and_address_2' => array(
        '#type' => 'textfield',
        '#attributes' => array('placeholder' => 'Street Address 2'),
        '#default_value' => isset($values['and_address_2']) ? $values['and_address_2'] : NULL,
      ),
      /*
      'and_zip' => array(
        '#type' => 'textfield',
        '#attributes' => array('placeholder' => 'Zip Code'),
        //'#required' => TRUE,
        '#default_value' => isset($values['and_zip']) ? $values['and_zip'] : NULL,
      ),
      'and_country' => array(
        '#type' => 'select',
        '#options' => country_get_list(),
        //'#attributes' => array('placeholder' => 'Country'),
        '#required' => TRUE,
        '#default_value' => isset($values['and_country']) ? $values['and_country'] : NULL,
      ),
      */
    );
  }

  if ($dec_religion=='Catholic') {
    $form['funeral_details']['cortege_massat'] = array(
      '#title' => 'For Funeral Mass at',
      '#type' => 'textfield',
      '#attributes' => array('placeholder' => 'Church Name'),
      //'#required' => TRUE,
      '#default_value' => isset($values['cortege_massat']) ? $values['cortege_massat'] : NULL,
    );
  
    $form['funeral_details']['cortege_masstime'] = array(
      '#title' => 'At Time',
      '#type' => 'date_text',
      '#date_format' => 'H:i',
      //'#required' => TRUE,
      '#default_value' => isset($values['cortege_masstime']) ? '0000-00-00 '. $values['cortege_masstime'] : NULL,
    );
  }

  $for_title = $dec_religion=='Catholic' ? 'Thereafter, for' : 'For';
  if ($dec_religion=='Sikh') {
    $form_state['values']['cortege_for'] = 'Cremation';
    $for_default = 'Cremation';
    $for_type = 'hidden';
    $at_title = 'For Cremation at';
  } else {
    $for_default = NULL;
    $for_type = 'select';
    $at_title = 'At';
  }
  
  $form['funeral_details']['cortege_for'] = array(
    '#type' => $for_type, 
    '#title' => $for_title,
    '#options' => array(''=>'- Select -') + publish_obituary_funeral_for($dec_religion),
    '#ajax' => array(
      'event' => 'change',
      'wrapper' => 'for_at_wrapper',
      'callback' => 'for_at_ajax_callback',
      'method' => 'replace',
    ),
    //'#required' => TRUE,
    '#default_value' => isset($values['cortege_for']) ? $values['cortege_for'] : $for_default,
  );

  // Preparing variable for At options
  if (!empty($form_state['values']['cortege_for'])) {
    $at_basic = publish_obituary_funeral_at($dec_religion, $form_state['values']['cortege_for']);
    $at_options = array(''=>'- Select -') + $at_basic;
  } else {
    if (isset($form_state['values']['cortege_for'])) {
      $at_options = array(''=>'- Select -');
    } else {
      if (isset($values['cortege_for'])) {
        $at_basic = publish_obituary_funeral_at($dec_religion, $values['cortege_for']);
        $at_options = array(''=>'- Select -') + $at_basic;
      } else {
        $at_options = array(''=>'- Select -');
      }
    }
  }

  $form['funeral_details']['cortege_at'] = array(
    '#title' => $at_title,
    '#type' => 'select',
    '#options' => $at_options,
    '#prefix' => '<div id="for_at_wrapper">',
    '#suffix' => '</div>',
    //'#required' => TRUE,
    '#default_value' => isset($values['cortege_at']) ? $values['cortege_at'] : NULL,
  );

  $form['funeral_details']['cortege_time'] = array(
    '#title' => 'At Time',
    '#type' => 'date_text',
    '#date_format' => 'H:i',
    //'#required' => TRUE,
    '#default_value' => isset($values['cortege_time']) ? '0000-00-00 '. $values['cortege_time'] : NULL,
  );

  if ($dec_religion=='Christian' || $dec_religion=='Catholic') {
    $form['funeral_details']['bible_verse'] = array(
      '#title' => 'Bible Verse',
      '#type' => 'select',
      '#options' => array(''=>'- Select -') + publish_obituary_funeral_bible(),
      /*
      '#options' => array(
        '' => '- Select -',
        'Proverbs 18:10' => 'The name of the LORD is a fortified tower; the righteous run to it and are safe.',
      ),
      */
      '#default_value' => isset($values['bible_verse']) ? $values['bible_verse'] : NULL,
    );

    $form['funeral_details']['custom_verse'] = array(
      '#title' => 'Or Custom Bible Verse',
      '#type' => 'textarea',
      '#rows' => 7,
      '#default_value' => isset($values['custom_verse']) ? $values['custom_verse'] : NULL,
    );
  }

  if ($dec_religion=='Sikh') {
    $form['funeral_details']['bhog'] = array(
      '#title' => 'Bhog Details',
      '#type' => 'textarea',
      '#rows' => 7,
      //'#required' => TRUE, //edited for own textarea
      '#default_value' => isset($values['bhog']) ? $values['bhog'] : NULL,
    );
  }

  $form['funeral_details']['other_remarks'] = array(
    '#title' => 'Other Remarks',
    '#type' => 'textarea',
    '#rows' => 7,
    '#default_value' => isset($values['other_remarks']) ? $values['other_remarks'] : NULL,
  );

  $def_desc = '<span>Please include the wake location and funeral details here.<br>Important information you may wish to include are dates and times of services or prayers and cremation or burial details.<br>Optional information pertaining to wreaths or donations may be included as well.</span>';
  $form['funeral_details']['own_template'] = array(
    //'#title' => 'My own template',
    '#type' => 'text_format',
    '#format' => 'custom_html',
    '#rows' => 10,
    //'#required' => TRUE,
    '#prefix' => '<div class="form-item-own-template" id="funeral-own-template">'. $def_desc,
    '#suffix' => '</div>',
    '#default_value' => isset($values['own_template']['value']) ? $values['own_template']['value'] : NULL,
  );

  $form['back'] = array(
    '#type' => 'submit',
    '#value' => t('Back'),
  );
  
  $form['next'] = array(
    '#type' => 'submit',
    '#value' => t('Next Step')
  );
    
  return $form;
}

/**
 * Add more button submit and ajax callback for night service
 */
function nightserv_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['nsnum_names'])) {
    $form_state['nsnum_names'] = 0;
    $form_state['nsnum_names']++;
  }

  $form_state['nsnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function nightserv_add_more_callback($form, $form_state) {
  return $form['funeral_details']['more_nightserv'];
}

/**
	 * Ajax callback function
	 */
	function for_at_ajax_callback($form, &$form_state) {
		return $form['funeral_details']['cortege_at'];
	}

/**
 * Form for the acknowledgement_details step of acknowledgement.
 * 
 * @see publish_obituary_form().
 *  
 * @param type $form
 * @param type $form_state
 * @return type 
 */
function publish_obituary_acknowledgement_details_form($form, &$form_state) {

  $values = isset($form_state['multistep_values']['acknowledgement_details']) ? $form_state['multistep_values']['acknowledgement_details'] : array();
  
  $form['form_title'] = array(
    '#markup' => '<h2>Acknowledgement Details</h2>',
    '#suffix' => '<div class="step">'.$form_state['active_stage'].'/'.$form_state['count_stage'].'</div>',
  );
  $adv_c = $form_state['multistep_values']['kind_of_advertisement']['adv_choosed'];
  if ($adv_c == 2) {
    $name_arr = explode('(', $form_state['multistep_values']['deceased_details']['deceased_name']);
    $dec_nid = substr(end($name_arr),0,-1);
    $dec_node = node_load($dec_nid);
    $form_state['multistep_values']['deceased_details']['departed_date'] = date('Y-m-d H:i:s', field_get_items('node', $dec_node, 'field_obituary_departed')[0]['value']);
    $form_state['multistep_values']['deceased_details']['religion'] = ucfirst(field_get_items('node', $dec_node, 'field_obituary_religion')[0]['value']);
  }
  $form['acknowledgement_details']['late_family'] = array(
    '#type' => 'textfield',
    '#title' => 'The Family of the Late',
    '#attributes' => array('placeholder' => 'Deceased Name'),
    '#required' => TRUE,
    '#disabled' => $adv_c == 2 ? TRUE : FALSE,
    '#default_value' => $form_state['multistep_values']['deceased_details']['deceased_name'],
    //'#default_value' => isset($values['late_family']) ? $values['late_family'] : NULL,
  );
  
  $form['acknowledgement_details']['departed_on'] = array(
    '#title' => 'Departed on',
    '#type' => 'date_text',
    '#date_format' => 'd F Y',
    '#required' => $adv_c == 2 ? FALSE : TRUE,
    '#disabled' => $adv_c == 2 ? TRUE : FALSE,
    '#default_value' => $form_state['multistep_values']['deceased_details']['departed_date'],
    //'#default_value' => isset($values['departed_on']) ? $values['departed_on'] : NULL,
  );

  $dec_religion = $form_state['multistep_values']['deceased_details']['religion'];
  $message_thanks = acknowledgement_message_thank_default($dec_religion);

  $form['acknowledgement_details']['message'] = array(
    '#title' => 'Message',
    '#type' => 'textarea',
    '#rows' => 7,
    '#required' => TRUE,
    '#attributes' => array('placeholder' => 'Wish to express...'),
    '#default_value' => isset($values['message']) ? $values['message'] : $message_thanks['message'],
  );

  $form['acknowledgement_details']['special_thanks'] = array(
    '#title' => 'Special Thanks to',
    '#type' => 'textarea',
    '#rows' => 7,
    '#attributes' => array('placeholder' => 'Doctors and Nurses...'),
    '#default_value' => isset($values['special_thanks']) ? $values['special_thanks'] : $message_thanks['thanks'],
  );

  if ($dec_religion=='Hindu') {
    // Karumathi
    $form['acknowledgement_details']['karumathi'] = array(
      '#type' => 'fieldset',
      //'#title' => 'Karumathi',
    );
    $form['acknowledgement_details']['karumathi']['karumathi_date'] = array(
      '#title' => 'Karumathi prayers will be held on the night of',
      '#type' => 'date_text',
      '#date_format' => 'd F Y',
      '#default_value' => isset($values['karumathi_date']) ? $values['karumathi_date'] : NULL,
    );
    $form['acknowledgement_details']['karumathi']['karumathi_address'] = array(
      '#type' => 'textfield',
      '#title' => 'Address',
      '#attributes' => array('placeholder' => 'Address'),
      '#default_value' => isset($values['karumathi_address']) ? $values['karumathi_address'] : NULL,
    );

    // Athma Shanthi
    $form['acknowledgement_details']['athma_shanthi'] = array(
      '#type' => 'fieldset',
      //'#title' => 'Athma Shanthi',
    );
    $form['acknowledgement_details']['athma_shanthi']['athma_shanthi_date'] = array(
      '#title' => 'Athma Shanthi prayers will be held on',
      '#type' => 'date_text',
      '#date_format' => 'd F Y',
      '#default_value' => isset($values['athma_shanthi_date']) ? $values['athma_shanthi_date'] : NULL,
    );
    $form['acknowledgement_details']['athma_shanthi']['athma_shanthi_temple'] = array(
      '#type' => 'textfield',
      '#title' => 'At Temple',
      '#attributes' => array('placeholder' => 'Temple Name'),
      '#default_value' => isset($values['athma_shanthi_temple']) ? $values['athma_shanthi_temple'] : NULL,
    );
    $form['acknowledgement_details']['athma_shanthi']['athma_shanthi_time'] = array(
      '#title' => 'At Time',
      '#type' => 'date_text',
      '#date_format' => 'H:i',
      '#default_value' => isset($values['athma_shanthi_time']) ? '0000-00-00 '. $values['athma_shanthi_time'] : NULL,
    );
  }

  $remarks_title = $dec_religion=='Hindu' ? 'Please treat this as a personal invitation' : 'Other Remarks';
  $form['acknowledgement_details']['other_remarks'] = array(
    '#title' => $remarks_title,
    '#type' => 'textarea',
    '#rows' => 7,
    '#default_value' => isset($values['other_remarks']) ? $values['other_remarks'] : NULL,
  );

  $form['back'] = array(
    '#type' => 'submit',
    '#value' => t('Back'),
  );
  
  $form['next'] = array(
    '#type' => 'submit',
    '#value' => t('Next Step')
  );
    
  return $form;
}

/**
 * Function to get default acknowledgement message and special thanks to
 */
function acknowledgement_message_thank_default($religion) {
  switch ($religion) {
    case 'Christian':
      $message = 'Wish to express their heartfelt thanks and appreciation to pastors, relatives, friends, colleagues, church members and business associates for their kind assistance, attendance, condolences, prayers, contributions and wreaths during their recent bereavement.';
      $thanks = 'Doctors and nurses from (Wards of Hospitals, Hospices etc) for their dedicated care, concern and professionalism.';
      break;
    case 'Catholic':
      $message = 'Wish to express their heartfelt thanks and appreciation to priests, relatives, friends, colleagues, church members, prayer groups and business associates for their kind assistance, attendance, condolences, prayers, contributions and wreaths during their recent bereavement.';
      $thanks = 'Doctors and nurses from (Wards of Hospitals, Hospices, etc) for their dedicated care, concern and professionalism.';
      break;
    case 'Taoist':
      $message = 'Wish to express their heartfelt thanks and appreciation to relatives, friends, colleagues, clan associations and business associates for their kind assistance, attendance, condolences, prayers, contributions and wreaths during their recent bereavement.';
      $thanks = 'Doctors and nurses from (Wards of Hospitals, Hospices etc) for their dedicated care, concern and professionalism.';
      break;
    case 'Hindu':
      $message = 'Wish to express their heartfelt thanks and appreciation to relatives, friends, colleagues and business associates for their kind assistance, attendance, condolences, prayers, contributions and wreaths during their recent bereavement.';
      $thanks = 'Doctors and nurses from (Wards of Hospitals, Hospices etc) for their dedicated care, concern and professionalism.';
      break;
    case 'Sikh':
      $message = 'Wish to express their heartfelt thanks and appreciation to relatives, friends, colleagues and business associates for their kind assistance, attendance, condolences, prayers, contributions and wreaths during their recent bereavement.';
      $thanks = 'Doctors and nurses from (Wards of Hospitals, Hospices etc) for their dedicated care, concern and professionalism.';
      break;
    case 'Soka':
      $message = 'Wish to express their heartfelt thanks and appreciation to relatives, friends, colleagues, clan associations and business associates for their kind assistance, attendance, condolences, prayers, contributions and wreaths during their recent bereavement.';
      $thanks = 'Doctors and nurses from (Wards of Hospitals, Hospices etc) for their dedicated care, concern and professionalism.';
      break;
    default:
      $message = 'Wish to express their heartfelt thanks and appreciation to relatives, friends, colleagues, clan associations and business associates for their kind assistance, attendance, condolences, prayers, contributions and wreaths during their recent bereavement.';
      $thanks = 'Doctors and nurses from (Wards of Hospitals, Hospices etc) for their dedicated care, concern and professionalism.';
      break;
  }

  return array('message'=>$message, 'thanks'=>$thanks);
}

/**
 * Form for the condolence_details step of condolence.
 * 
 * @see publish_obituary_form().
 *  
 * @param type $form
 * @param type $form_state
 * @return type 
 */
function publish_obituary_condolence_details_form($form, &$form_state) {

  $values = isset($form_state['multistep_values']['condolence_details']) ? $form_state['multistep_values']['condolence_details'] : array();
  
  $form['form_title'] = array(
    '#markup' => '<h2>Condolence Details</h2>',
    '#suffix' => '<div class="step">'.$form_state['active_stage'].'/'.$form_state['count_stage'].'</div>',
  );

  $def_message = 'With Deepest Sympathy and Heartfelt Condolence<br>To our (Title), (Name) and family<br>On the demise of his/her Beloved (relationship)<br><br>- or -<br><br>To the Family of the Late<br>(Deceased Name)';
  $form['condolence_details']['message'] = array(
    '#title' => 'Condolence Message',
    '#type' => 'text_format',
    '#format' => 'custom_html',
    '#rows' => 10,
    '#required' => TRUE,
    '#attributes' => array('placeholder' => 'With Deepest Sympathies...'),
    '#default_value' => isset($values['message']['value']) ? $values['message']['value'] : $def_message,
  );

  $form['condolence_details']['dec_con_name'] = array(
    '#type' => 'textfield',
    '#title' => 'Deceased Name',
    '#attributes' => array('placeholder' => 'Deceased Name'),
    '#required' => TRUE,
    '#default_value' => $form_state['multistep_values']['deceased_details']['deceased_name'],
    //'#default_value' => isset($values['dec_con_name']) ? $values['dec_con_name'] : NULL,
  );
  
  $form['condolence_details']['departed_on'] = array(
    '#title' => 'Departed on',
    '#type' => 'date_text',
    '#date_format' => 'd F Y',
    '#required' => TRUE,
    '#default_value' => $form_state['multistep_values']['deceased_details']['departed_date'],
    //'#default_value' => isset($values['departed_on']) ? $values['departed_on'] : NULL,
  );

  $def_from = 'Management and Staff of<br>(Company Name)';
  $form['condolence_details']['condo_from'] = array(
    '#title' => 'From',
    '#type' => 'text_format',
    '#format' => 'custom_html',
    '#rows' => 7,
    '#required' => TRUE,
    '#attributes' => array('placeholder' => 'Management and Staff of...'),
    '#default_value' => isset($values['condo_from']['value']) ? $values['condo_from']['value'] : $def_from,
  );

  $form['condolence_details']['other_remarks'] = array(
    '#title' => 'Other Remarks',
    '#type' => 'textarea',
    '#rows' => 7,
    '#default_value' => isset($values['other_remarks']) ? $values['other_remarks'] : NULL,
  );

  $form['back'] = array(
    '#type' => 'submit',
    '#value' => t('Back'),
  );
  
  $form['next'] = array(
    '#type' => 'submit',
    '#value' => t('Next Step')
  );
    
  return $form;
}

/**
 * Form for the memoriam_details step of in-memoriam.
 * 
 * @see publish_obituary_form().
 *  
 * @param type $form
 * @param type $form_state
 * @return type 
 */
function publish_obituary_memoriam_details_form($form, &$form_state, $no_js_use = FALSE) {

  $values = isset($form_state['multistep_values']['memoriam_details']) ? $form_state['multistep_values']['memoriam_details'] : array();

  $form['form_title'] = array(
    '#markup' => '<h2>In Memoriam Details</h2>',
    '#suffix' => '<div class="step">'.$form_state['active_stage'].'/'.$form_state['count_stage'].'</div>',
  );
  /*
  $def_anniversary = '1st Anniversary';
  $form['memoriam_details']['anniversary'] = array(
    '#title' => 'Year of Anniversary',
    '#type' => 'textfield',
    '#required' => TRUE,
    //'#attributes' => array('placeholder' => '<Year> Anniversary In Loving Memory of'),
    '#default_value' => isset($values['anniversary']) ? $values['anniversary'] : $def_anniversary,
  );
  $def_anniversary_info = 'In Loving Memory';
  $form['memoriam_details']['anniversary_info'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => isset($values['anniversary_info']) ? $values['anniversary_info'] : $def_anniversary_info,
  );
  */
  $form['memoriam_details']['select_message'] = array(
    '#type' => 'select',
    '#title' => 'Select Message',
    '#options' => array(''=>'- Select -') + publish_obituary_memo_message_list(),
    //'#required' => TRUE,
    '#default_value' => isset($values['select_message']) ? $values['select_message'] : NULL,
  );

  $form['memoriam_details']['custom_message'] = array(
    '#title' => 'Custom Message',
    '#type' => 'textarea',
    '#rows' => 7,
    //'#required' => TRUE,
    '#attributes' => array('placeholder' => 'Custom message'),
    '#default_value' => isset($values['custom_message']) ? $values['custom_message'] : NULL,
  );

  /** Start add more ajax for rememberer */
  $def_dearly = 'Dearly missed and fondly remembered by family and loved ones';
  $form['memoriam_details']['more_rememberer'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-rememberer">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['drnum_names'])) {
    $form_state['drnum_names'] = 1;
  }

  for ($i = 0; $i < $form_state['drnum_names']; $i++) {
    $form['memoriam_details']['more_rememberer'][$i]['rememberer_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Ending Message' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Name',
      ),
      '#required' => $i == 0 ? TRUE : FALSE,
      '#default_value' => isset($values['more_rememberer'][$i]['rememberer_name']) ? $values['more_rememberer'][$i]['rememberer_name'] : $def_dearly,
    );
  }

  $form['memoriam_details']['more_rememberer']['add_rememberer'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_rememberer',
    '#submit' => array('rememberer_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'rememberer_add_more_callback',
      'wrapper' => 'more-rememberer',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_rememberer']['remove_rememberer']['#ajax'])) {
       unset($form['more_rememberer']['remove_rememberer']['#ajax']);
    }
    unset($form['more_rememberer']['add_rememberer']['#ajax']);
  }
  /** End add more ajax for dearly missed */

  $form['memoriam_details']['other_remarks'] = array(
    '#title' => 'Other Remarks',
    '#type' => 'textarea',
    '#rows' => 7,
    '#default_value' => isset($values['other_remarks']) ? $values['other_remarks'] : NULL,
  );

  $form['back'] = array(
    '#type' => 'submit',
    '#value' => t('Back'),
  );
  
  $form['next'] = array(
    '#type' => 'submit',
    '#value' => t('Next Step')
  );
    
  return $form;
}

/**
 * Add more button submit and ajax callback for dearly missed
 */
function rememberer_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['drnum_names'])) {
      $form_state['drnum_names'] = 0;
      $form_state['drnum_names']++;
  }

  $form_state['drnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function rememberer_add_more_callback($form, $form_state) {
  return $form['memoriam_details']['more_rememberer'];
}

/**
 * Form for the certificate_photo step.
 * 
 * @see publish_obituary_form().
 *  
 * @param type $form
 * @param type $form_state
 * @return type 
 */
function publish_obituary_certificate_photo_form($form, &$form_state) {

  $form['#attributes']['enctype'] = 'multipart/form-data';

  //$photo_id = variable_get('photo_variable_name');
  //$certf_id = variable_get('certf_variable_name');

  $values = isset($form_state['multistep_values']['certificate_photo']) ? $form_state['multistep_values']['certificate_photo'] : array();
  
  $form['form_title'] = array(
    '#markup' => '<h2>Upload Forms and Photo</h2>',
    '#suffix' => '<div class="step">'.$form_state['active_stage'].'/'.$form_state['count_stage'].'</div>',
  );

  $form['certificate_photo']['deceased_photo'] = array(
      '#type' => 'managed_file',
      '#title' => 'Deceased Photo',
      '#upload_location' => 'public://',
      '#required' => TRUE,
      "#upload_validators"  => array("file_validate_extensions" => array("png gif jpg jpeg")),
      '#default_value' => isset($values['deceased_photo']) ? $values['deceased_photo'] : NULL,
  );

  if ($form_state['kof']=='condolence') {
    $form['certificate_photo']['company_logo'] = array(
      '#type' => 'managed_file',
      '#title' => 'Company Logo',
      '#upload_location' => 'public://',
      //'#required' => TRUE,
      "#upload_validators"  => array("file_validate_extensions" => array("png gif jpg jpeg")),
      '#default_value' => isset($values['company_logo']) ? $values['company_logo'] : NULL,
    );
  }

  $form['certificate_photo']['death_certificate'] = array(
    '#type' => 'managed_file',
    '#title' => 'Death Certificate',
    '#upload_location' => 'public://',
    '#required' => TRUE,
    "#upload_validators"  => array("file_validate_extensions" => array("pdf png jpg jpeg")),
    '#default_value' => isset($values['death_certificate']) ? $values['death_certificate'] : NULL,
  );

  //$indemnity = drupal_get_path('module', 'publish_obituary') . '/files/indemnity_form.pdf';
  $adv_choosed = $form_state['multistep_values']['kind_of_advertisement']['adv_choosed'];
  $indemnity = $adv_choosed==2 ? variable_get('file_public_path', conf_path() . '/files') .'/indemnity_files/online_indemnity.pdf' : variable_get('file_public_path', conf_path() . '/files') .'/indemnity_files/print_indemnity.pdf'; 
  $form['certificate_photo']['indemnity_form'] = array(
    '#prefix' => '<label>Indemnity Form (Please download and sign)<span> *</span></label>',
    '#markup' => l('Download', $indemnity, array('attributes' => array('class' => 'indemnity-download', 'target' => '_blank'))),
    //'#markup' => '<div><object data="'. $indemnity .'" type="application/pdf" width="746" height="1100">alt : <a href="'. $indemnity .'">'. $indemnity .'</a></object></div>',
    //'#markup' => '<div><iframe src="http://docs.google.com/gview?url=http://infolab.stanford.edu/pub/papers/google.pdf&embedded=true" style="width:600px; height:500px;" frameborder="0"></iframe></div>',
  );

  $form['certificate_photo']['indemnity_upload'] = array(
    '#type' => 'managed_file',
    //'#title' => 'Upload Indemnity Form',
    '#upload_location' => 'public://indemnity_files',
    //'#required' => TRUE,
    "#upload_validators"  => array("file_validate_extensions" => array("pdf png jpg jpeg")),
    '#default_value' => isset($values['death_certificate']) ? $values['indemnity_upload'] : NULL,
  );

  $form['back'] = array(
    '#type' => 'submit',
    '#value' => t('Back'),
  );
  
  $form['next'] = array(
    '#type' => 'submit',
    '#value' => t('Next Step')
  );
    
  return $form;
}

/**
 * Form for the adv_review step.
 * 
 * @see publish_obituary_form().
 *  
 * @param type $form
 * @param type $form_state
 * @return type 
 */
function publish_obituary_adv_review_form($form, $form_state) {
  if (array_key_exists('publication_details', $form_state['multistep_values']) && $form_state['multistep_values']['publication_details']['pub_choosed'] == 1) {
    $p_onsite = TRUE;
  } else {
    $p_onsite = FALSE;
  }
  
  $form['form_title'] = array(
    '#markup' => '<h2>Review</h2>',
    '#suffix' => '<div class="step">'.$form_state['active_stage'].'/'.$form_state['count_stage'].'</div>',
  );

  $form['adv_review']['step_1'] = array(
    'step_1_title' => array(
      '#markup' => '<h3>Advertisement Type</h3>',
      '#prefix' => '<div class="step-item step-1-kind-of-adv">',
    ),
    'step_1_value' => array(
      '#markup' => $form_state['multistep_values']['kind_of_advertisement']['adv_choosed'] == 1 ? '<div class="adv-choosed"><div class="adv-choosed-print"><span>Print</span></div></div>' : '<div class="adv-choosed"><div class="adv-choosed-online"><span>Online</span></div></div>',
      '#suffix' => '</div>',
    ),
  );

  if ($form_state['multistep_values']['kind_of_advertisement']['adv_choosed'] == 1) {

    $form['adv_review']['step_2p']['step_2p_title'] = array(
        '#markup' => '<h3>Publication Details</h3>',
        '#prefix' => '<div class="step-item step-2p-publication-details">',
        '#suffix' => '<div class="publication-value">',
    );
    if (!$p_onsite) {
      $form['adv_review']['step_2p']['step_2p_pub_media'] = array(
          '#markup' => publish_obituary_get_media_val($form_state['multistep_values']['publication_details']['media'])['media_name'],
          '#prefix' => '<div class="pub-media"><div class="label-item">Media</div>',
          '#suffix' => '</div>',
      );
      $form['adv_review']['step_2p']['step_2p_pub_date'] = array(
          '#markup' => date_format(date_create($form_state['multistep_values']['publication_details']['pub_date']), 'd F Y'),
          '#prefix' => '<div class="pub-date"><div class="label-item">Publication Date</div>',
          '#suffix' => '</div>',
      );
      $form['adv_review']['step_2p']['step_2p_pub_style'] = array(
          '#markup' => $form_state['multistep_values']['publication_details']['style'] == 1 ? 'Full Color' : 'Black and White',
          '#prefix' => '<div class="pub-style"><div class="label-item">Style</div>',
          '#suffix' => '</div>',
      );
      $form['adv_review']['step_2p']['step_2p_pub_size'] = array(
          '#markup' => publish_obituary_public_size($form_state['multistep_values']['publication_details']['height'], $form_state['multistep_values']['publication_details']['column'], $form_state['multistep_values']['publication_details']['common_sizes']),
          '#prefix' => '<div class="size-price-wrapper"><div class="pub-size"><div class="label-item">Size</div>',
          '#suffix' => '</div>',
      );
      $form['adv_review']['step_2p']['step_2p_pub_price'] = array(
          '#markup' => '$ ' . $form_state['multistep_values']['publication_details']['price'],
          '#prefix' => '<div class="pub-price">',
          '#suffix' => '</div></div>',
      );
    }
    $form['adv_review']['step_2p']['step_2p_close_tag'] = array(
      '#markup' => '',
      '#suffix' => '</div></div>',
    );
  }

  $adv_det = $form_state['multistep_values']['advertiser_details'];
  $countries = country_get_list();
  $country_name = $countries[$adv_det['country']];
  variable_set('adv_country', $country_name);

  $form['adv_review']['step_2'] = array(
    'step_2_title' => array(
      '#markup' => '<h3>Advertiser Details</h3>',
      '#prefix' => '<div class="step-item step-2-advertiser-details">',
    ),
    'step_2_adv_name' => array(
      '#markup' => $adv_det['name'],
      '#prefix' => '<div class="advertiser-value"><div class="adv-name"><div class="label-item">Advertiser Name</div>',
      '#suffix' => '</div>',
    ),
    'step_2_adv_contact_number' => array(
      '#markup' => $adv_det['contact_number'],
      '#prefix' => '<div class="adv-contact-number"><div class="label-item">Contact Number</div>',
      '#suffix' => '</div>',
    ),
    'step_2_adv_email' => array(
      '#markup' => $adv_det['email'],
      '#prefix' => '<div class="adv-email"><div class="label-item">Email Address</div>',
      '#suffix' => '</div>',
    ),
    'step_2_adv_bill_address' => array(
      '#markup' => $adv_det['company_name'] . ' ' . $adv_det['street_address_1'] . ' ' . $country_name . ' ' . $adv_det['zip'],
      '#prefix' => '<div class="adv-bill-address"><div class="label-item">Billing Address</div>',
      '#suffix' => '</div>',
    ),
    'step_2_adv_relation' => array(
      '#markup' => $adv_det['relationship'],
      '#prefix' => '<div class="adv-relation"><div class="label-item">Relationship to Deceased</div>',
      '#suffix' => '</div></div></div>',
    ),
  );

  $form['adv_review']['step_3']['step_3_title'] = array(
      '#markup' => '<h3>Deceased Details</h3>',
      '#prefix' => '<div class="step-item step-3-deceased-details">',
      '#suffix' => '<div class="deceased-value">',
  );
  if (!$p_onsite) {
    /*
    if ($form_state['kof']=='acknowledgement' && $form_state['multistep_values']['kind_of_advertisement']['adv_choosed']==2) {
      $name_arr = explode('(', $form_state['multistep_values']['deceased_details']['deceased_name']);
      $dec_nid = substr(end($name_arr),0,-1);
      $n_deceased = node_load($dec_nid);
    }
    */
    $form['adv_review']['step_3']['step_3_decease_name'] = array(
        //'#markup' => isset($n_deceased) ? $n_deceased->title : $form_state['multistep_values']['deceased_details']['deceased_name'],
        '#markup' => $form_state['multistep_values']['deceased_details']['deceased_name'],
        '#prefix' => '<div class="decease-name"><div class="label-item">Deceased Name</div>',
        '#suffix' => '</div>',
    );
    $form['adv_review']['step_3']['step_3_decease_date'] = array(
        '#markup' => date_format(date_create($form_state['multistep_values']['deceased_details']['departed_date']), 'd F Y'),
        '#prefix' => '<div class="decease_depart_date"><div class="label-item">Departed Date</div>',
        '#suffix' => '</div>',
    );
    if ($form_state['kof'] == 'death-announcement') {
      $form['adv_review']['step_3']['step_3_decease_age'] = array(
        '#markup' => $form_state['multistep_values']['deceased_details']['age'],
        '#prefix' => '<div class="decease_age"><div class="label-item">Age</div>',
        '#suffix' => '</div>',
      );
    }
    if ($form_state['kof'] == 'death-announcement' || $form_state['kof']=='acknowledgement') {
      $form['adv_review']['step_3']['step_3_decease_religion'] = array(
        '#markup' => $form_state['multistep_values']['deceased_details']['religion'],
        '#prefix' => '<div class="decease_religion"><div class="label-item">Religion</div>',
        '#suffix' => '</div>',
      );
    }
  }
  $form['adv_review']['step_3']['step_3_close_tag'] = array(
    '#markup' => '',
    '#suffix' => '</div></div>',
  );

  if ($form_state['kof'] == 'death-announcement') {    
    $form['adv_review']['step_4'] = array(
      'step_4_title' => array(
        '#markup' => '<h3>Family and Friends</h3>',
        '#prefix' => '<div class="step-item step-4-family-friend">',
        '#suffix' => '<div class="family-friend-value">',
      ),
      'step_4_empty' => array(
        '#markup' => '',
      ),
    );

    if (!$p_onsite) {
      if (!empty($form_state['multistep_values']['family_friends']['spouse_name'])) {
        $form['adv_review']['step_4']['step_4_spouse_name'] = array(
          '#markup' => '<div>'. $form_state['multistep_values']['family_friends']['spouse_name'] .'</div>',
          '#prefix' => '<div class="item-name spouse-name"><div class="label-item">Spouse</div>',
          '#suffix' => '</div>',
        );
      }

      $family_friends = $form_state['multistep_values']['family_friends'];
      $fathers = publish_obituary_get_descent($family_friends, 'more_fathers', 'father_name', 'mother_name');
      if (!empty($fathers['main'])) {
        $form['adv_review']['step_4']['step_4_father_name'] = array(
          '#markup' => $fathers['main'],
          '#prefix' => '<div class="item-name fathers-name"><div class="label-item">Father</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($fathers['pair'])) {
        $form['adv_review']['step_4']['step_4_mother_name'] = array(
          '#markup' => $fathers['pair'],
          '#prefix' => '<div class="item-name mother-name"><div class="label-item">Mother</div>',
          '#suffix' => '</div>',
        );
      }
      $sons = publish_obituary_get_descent($family_friends, 'more_sons', 'son_name', 'daughter_law_name');
      if (!empty($sons['main'])) {
        $form['adv_review']['step_4']['step_4_son_name'] = array(
          '#markup' => $sons['main'],
          '#prefix' => '<div class="item-name sons-name"><div class="label-item">Sons</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($sons['pair'])) {
        $form['adv_review']['step_4']['step_4_daughterinlaw_name'] = array(
          '#markup' => $sons['pair'],
          '#prefix' => '<div class="item-name daughterinlaw-name"><div class="label-item">Daughters-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $daughters = publish_obituary_get_descent($family_friends, 'more_daughters', 'daughter_name', 'sons_law_name');
      if (!empty($daughters['main'])) { 
        $form['adv_review']['step_4']['step_4_daughter_name'] = array(
          '#markup' => $daughters['main'],
          '#prefix' => '<div class="item-name daughters-name"><div class="label-item">Daughters</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($daughters['pair'])) { 
        $form['adv_review']['step_4']['step_4_soninlaw_name'] = array(
          '#markup' => $daughters['pair'],
          '#prefix' => '<div class="item-name soninlaw-name"><div class="label-item">Sons-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $grandsons = publish_obituary_get_descent($family_friends, 'more_grandsons', 'grandson_name', 'granddaughter_law_name');
      if (!empty($grandsons['main'])) { 
        $form['adv_review']['step_4']['step_4_grandson_name'] = array(
          '#markup' => $grandsons['main'],
          '#prefix' => '<div class="item-name grandsons-name"><div class="label-item">Grandsons</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($grandsons['pair'])) { 
        $form['adv_review']['step_4']['step_4_granddaughterinlaw_name'] = array(
          '#markup' => $grandsons['pair'],
          '#prefix' => '<div class="item-name granddaughterinlaw-name"><div class="label-item">Granddaughters-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $granddaughters = publish_obituary_get_descent($family_friends, 'more_granddaughters', 'granddaughter_name', 'grandson_law_name');
      if (!empty($granddaughters['main'])) { 
        $form['adv_review']['step_4']['step_4_granddaughter_name'] = array(
          '#markup' => $granddaughters['main'],
          '#prefix' => '<div class="item-name granddaughters-name"><div class="label-item">Granddaughters</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($granddaughters['pair'])) { 
        $form['adv_review']['step_4']['step_4_grandsoninlaw_name'] = array(
          '#markup' => $granddaughters['pair'],
          '#prefix' => '<div class="item-name grandsoninlaw-name"><div class="label-item">Grandsons-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $greatgrandsons = publish_obituary_get_descent($family_friends, 'more_greatgrandsons', 'greatgrandson_name', 'greatgranddaughter'); 
      if (!empty($greatgrandsons['main'])) { 
        $form['adv_review']['step_4']['step_4_greatgrandson_name'] = array(
          '#markup' => $greatgrandsons['main'],
          '#prefix' => '<div class="item-name greatgrandsons-name"><div class="label-item">Great Grandsons</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($greatgrandsons['pair'])) { 
        $form['adv_review']['step_4']['step_4_greatgranddaughter_name'] = array(
          '#markup' => $greatgrandsons['pair'],
          '#prefix' => '<div class="item-name greatgranddaughter-name"><div class="label-item">Great Granddaughters</div>',
          '#suffix' => '</div>',
        );
      }
      $brothers = publish_obituary_get_descent($family_friends, 'more_brothers', 'brother_name', 'sister_law_name');
      if (!empty($brothers['main'])) {
        $form['adv_review']['step_4']['step_4_brother_name'] = array(
          '#markup' => $brothers['main'],
          '#prefix' => '<div class="item-name brothers-name"><div class="label-item">Brothers</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($brothers['pair'])) {
        $form['adv_review']['step_4']['step_4_sisterinlaw_name'] = array(
          '#markup' => $brothers['pair'],
          '#prefix' => '<div class="item-name sisterinlaws-name"><div class="label-item">Sisters-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $sisters = publish_obituary_get_descent($family_friends, 'more_sisters', 'sister_name', 'brother_law_name');
      if (!empty($sisters['main'])) {
        $form['adv_review']['step_4']['step_4_sister_name'] = array(
          '#markup' => $sisters['main'],
          '#prefix' => '<div class="item-name sisters-name"><div class="label-item">Sisters</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($sisters['pair'])) {
        $form['adv_review']['step_4']['step_4_brotherinlaw_name'] = array(
          '#markup' => $sisters['pair'],
          '#prefix' => '<div class="item-name brotherinlaws-name"><div class="label-item">Brothers-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $fatherslaw = publish_obituary_get_descent($family_friends, 'more_fatherlaws', 'fatherlaw_name', 'motherlaw_name');
      if (!empty($fatherslaw['main'])) {
        $form['adv_review']['step_4']['step_4_fatherlaw_name'] = array(
          '#markup' => $fatherslaw['main'],
          '#prefix' => '<div class="item-name fatherslaw-name"><div class="label-item">Father-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($fatherslaw['pair'])) {
        $form['adv_review']['step_4']['step_4_motherlaw_name'] = array(
          '#markup' => $fatherslaw['pair'],
          '#prefix' => '<div class="item-name motherlaw-name"><div class="label-item">Mother-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $brotherslaw = publish_obituary_get_descent($family_friends, 'more_brotherslaw', 'brotherlawmain_name', 'sisterlawpair_name');
      if (!empty($brotherslaw['main'])) {
        $form['adv_review']['step_4']['step_4_brotherlawmain_name'] = array(
          '#markup' => $brotherslaw['main'],
          '#prefix' => '<div class="item-name brotherslawmain-name"><div class="label-item">Brothers-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($brotherslaw['pair'])) {
        $form['adv_review']['step_4']['step_4_sisterlawpair_name'] = array(
          '#markup' => $brotherslaw['pair'],
          '#prefix' => '<div class="item-name sisterslawpair-name"><div class="label-item">Sisters-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $sisterslaw = publish_obituary_get_descent($family_friends, 'more_sisterslaw', 'sisterlawmain_name', 'brotherlawpair_name');
      if (!empty($sisterslaw['main'])) {
        $form['adv_review']['step_4']['step_4_sisterlawmain_name'] = array(
          '#markup' => $sisterslaw['main'],
          '#prefix' => '<div class="item-name sisterslawmain-name"><div class="label-item">Sisters-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($sisterslaw['pair'])) {
        $form['adv_review']['step_4']['step_4_brotherlawpair_name'] = array(
          '#markup' => $sisterslaw['pair'],
          '#prefix' => '<div class="item-name brotherslawpair-name"><div class="label-item">Brothers-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $nephews = publish_obituary_get_descent($family_friends, 'more_nephews', 'nephew_name', 'niece_law_name');
      if (!empty($nephews['main'])) {
        $form['adv_review']['step_4']['step_4_nephew_name'] = array(
          '#markup' => $nephews['main'],
          '#prefix' => '<div class="item-name nephews-name"><div class="label-item">Nephews</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($nephews['pair'])) {
        $form['adv_review']['step_4']['step_4_nieceinlaw_name'] = array(
          '#markup' => $nephews['pair'],
          '#prefix' => '<div class="item-name nieceinlaws-name"><div class="label-item">Nieces-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $nieces = publish_obituary_get_descent($family_friends, 'more_nieces', 'niece_name', 'nephew_law_name');
      if (!empty($nieces['main'])) {
        $form['adv_review']['step_4']['step_4_niece_name'] = array(
          '#markup' => $nieces['main'],
          '#prefix' => '<div class="item-name niece-name"><div class="label-item">Nieces</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($nieces['pair'])) {
        $form['adv_review']['step_4']['step_4_nephewinlaw_name'] = array(
          '#markup' => $nieces['pair'],
          '#prefix' => '<div class="item-name nephewinlaws-name"><div class="label-item">Nephews-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $grandnephews = publish_obituary_get_descent($family_friends, 'more_grandnephews', 'grandnephew_name', 'grandniece_name');
      if (!empty($grandnephews['main'])) {
        $form['adv_review']['step_4']['step_4_grandnephew_name'] = array(
          '#markup' => $grandnephews['main'],
          '#prefix' => '<div class="item-name grandnephews-name"><div class="label-item">Grandnephews</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($grandnephews['pair'])) {
        $form['adv_review']['step_4']['step_4_grandniecelaw_name'] = array(
          '#markup' => $grandnephews['pair'],
          '#prefix' => '<div class="item-name grandnieceslaw-name"><div class="label-item">Grandnieces-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $grandnieces = publish_obituary_get_descent($family_friends, 'more_grandnieces', 'grandniece_real_name', 'grandnephew_law_name');
      if (!empty($grandnieces['main'])) {
        $form['adv_review']['step_4']['step_4_grandniece_name'] = array(
          '#markup' => $grandnieces['main'],
          '#prefix' => '<div class="item-name grandniece-name"><div class="label-item">Grandnieces</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($grandnieces['pair'])) {
        $form['adv_review']['step_4']['step_4_grandnephewinlaw_name'] = array(
          '#markup' => $grandnieces['pair'],
          '#prefix' => '<div class="item-name grandnephewinlaws-name"><div class="label-item">Grandnephews-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $greatgrandnephews = publish_obituary_get_descent($family_friends, 'more_greatgrandnephews', 'greatgrandnephew_name', 'greatgrandniece_name');
      if (!empty($greatgrandnephews['main'])) {
        $form['adv_review']['step_4']['step_4_greatgrandnephew_name'] = array(
          '#markup' => $greatgrandnephews['main'],
          '#prefix' => '<div class="item-name greatgrandnephews-name"><div class="label-item">Great Grandnephews</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($greatgrandnephews['pair'])) {
        $form['adv_review']['step_4']['step_4_greatgrandniece_name'] = array(
          '#markup' => $greatgrandnephews['pair'],
          '#prefix' => '<div class="item-name greatgrandnieces-name"><div class="label-item">Great Grandnieces</div>',
          '#suffix' => '</div>',
        );
      }
      $uncles = publish_obituary_get_descent($family_friends, 'more_uncles', 'uncle_name', 'uncle_spouse_name');
      if (!empty($uncles['main'])) {
        $form['adv_review']['step_4']['step_4_uncle_name'] = array(
          '#markup' => $uncles['main'],
          '#prefix' => '<div class="item-name uncles-name"><div class="label-item">Uncles</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($uncles['pair'])) {
        $form['adv_review']['step_4']['step_4_uncle_spouse_name'] = array(
          '#markup' => $uncles['pair'],
          '#prefix' => '<div class="item-name uncle-spouse-name"><div class="label-item">Spouses</div>',
          '#suffix' => '</div>',
        );
      }
      $aunties = publish_obituary_get_descent($family_friends, 'more_aunties', 'auntie_name', 'auntie_spouse_name');
      if (!empty($aunties['main'])) {
        $form['adv_review']['step_4']['step_4_auntie_name'] = array(
          '#markup' => $aunties['main'],
          '#prefix' => '<div class="item-name aunties-name"><div class="label-item">Aunties</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($aunties['pair'])) {
        $form['adv_review']['step_4']['step_4_auntie_spouse_name'] = array(
          '#markup' => $aunties['pair'],
          '#prefix' => '<div class="item-name auntie-spouse-name"><div class="label-item">Spouses</div>',
          '#suffix' => '</div>',
        );
      }
      $cousins = publish_obituary_get_descent($family_friends, 'more_cousins', 'cousin_name', 'cousin_law_name');
      if (!empty($cousins['main'])) {
        $form['adv_review']['step_4']['step_4_cousin_name'] = array(
          '#markup' => $cousins['main'],
          '#prefix' => '<div class="item-name cousins-name"><div class="label-item">Cousins</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($cousins['pair'])) {
        $form['adv_review']['step_4']['step_4_cousinlaw_name'] = array(
          '#markup' => $cousins['pair'],
          '#prefix' => '<div class="item-name cousinlaw-name"><div class="label-item">Cousins-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $godsons = publish_obituary_get_descent($family_friends, 'more_godsons', 'godson_name', 'goddaughter_law_name');
      if (!empty($godsons['main'])) {
        $form['adv_review']['step_4']['step_4_godson_name'] = array(
          '#markup' => $godsons['main'],
          '#prefix' => '<div class="item-name godsons-name"><div class="label-item">God Sons</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($godsons['pair'])) {
        $form['adv_review']['step_4']['step_4_goddaughterinlaw_name'] = array(
          '#markup' => $godsons['pair'],
          '#prefix' => '<div class="item-name goddaughterinlaw-name"><div class="label-item">God Daughters-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $goddaughters = publish_obituary_get_descent($family_friends, 'more_goddaughters', 'goddaughter_name', 'godsons_law_name');
      if (!empty($goddaughters['main'])) {
        $form['adv_review']['step_4']['step_4_goddaughter_name'] = array(
          '#markup' => $goddaughters['main'],
          '#prefix' => '<div class="item-name goddaughters-name"><div class="label-item">God Daughters</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($goddaughters['pair'])) {
        $form['adv_review']['step_4']['step_4_godsoninlaw_name'] = array(
          '#markup' => $goddaughters['pair'],
          '#prefix' => '<div class="item-name godsoninlaw-name"><div class="label-item">God Sons-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $godgrandsons = publish_obituary_get_descent($family_friends, 'more_godgrandsons', 'godgrandson_name', 'godgranddaughter_law_name');
      if (!empty($godgrandsons['main'])) {
        $form['adv_review']['step_4']['step_4_godgrandson_name'] = array(
          '#markup' => $godgrandsons['main'],
          '#prefix' => '<div class="item-name godgrandsons-name"><div class="label-item">God Grandsons</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($godgrandsons['pair'])) {
        $form['adv_review']['step_4']['step_4_godgranddaughterinlaw_name'] = array(
          '#markup' => $godgrandsons['pair'],
          '#prefix' => '<div class="item-name godgranddaughterinlaw-name"><div class="label-item">God Granddaughters-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $godgranddaughters = publish_obituary_get_descent($family_friends, 'more_godgranddaughters', 'godgranddaughter_name', 'godgrandson_law_name');
      if (!empty($godgranddaughters['main'])) {
        $form['adv_review']['step_4']['step_4_godgranddaughter_name'] = array(
          '#markup' => $godgranddaughters['main'],
          '#prefix' => '<div class="item-name godgranddaughters-name"><div class="label-item">God Granddaughters</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($godgranddaughters['pair'])) {
        $form['adv_review']['step_4']['step_4_godgrandsoninlaw_name'] = array(
          '#markup' => $godgranddaughters['pair'],
          '#prefix' => '<div class="item-name godgrandsoninlaw-name"><div class="label-item">God Grandsons-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $godgreatgrandsons = publish_obituary_get_descent($family_friends, 'more_godgreatgrandsons', 'godgreatgrandson_name', 'godgreatgranddaughter_name');
      if (!empty($godgreatgrandsons['main'])) {
        $form['adv_review']['step_4']['step_4_godgreatgrandson_name'] = array(
          '#markup' => $godgreatgrandsons['main'],
          '#prefix' => '<div class="item-name godgreatgrandsons-name"><div class="label-item">God Great Grandsons</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($godgreatgrandsons['pair'])) {
        $form['adv_review']['step_4']['step_4_godgreatgranddaughter_name'] = array(
          '#markup' => $godgreatgrandsons['pair'],
          '#prefix' => '<div class="item-name godgreatgranddaughter-name"><div class="label-item">God Great Granddaughters</div>',
          '#suffix' => '</div>',
        );
      }
      $caregivers = publish_obituary_get_descent($family_friends, 'more_caregivers', 'caregiver_name', '');
      if (!empty($caregivers['main'])) {
        $form['adv_review']['step_4']['step_4_caregiver_name'] = array(
          '#markup' => $caregivers['main'],
          '#prefix' => '<div class="item-name caregiver-name"><div class="label-item">Caregivers</div>',
          '#suffix' => '</div>',
        );
      }
      $others = publish_obituary_get_descent($family_friends, 'more_others', 'other_name', '');
      if (!empty($others['main'])) { 
        $form['adv_review']['step_4']['step_4_other_name'] = array(
          '#markup' => $others['main'],
          '#prefix' => '<div class="item-name other-name"><div class="label-item">Others</div>',
          '#suffix' => '</div>',
        );
      }
    }
      
    $form['adv_review']['step_4']['step_4_close_tag'] = array(
      '#markup' => '',
      '#suffix' => '</div></div>',
    );

    $form['adv_review']['step_5']['step_5_title'] = array(
        '#markup' => '<h3>Funeral Details</h3>',
        '#prefix' => '<div class="step-item step-5-funeral-details">',
        '#suffix' => '<div class="funeral-value">',
    );
    if (!$p_onsite) {
      $funeral_det = $form_state['multistep_values']['funeral_details'];
      if ($funeral_det['template_choosed']==1) {
        $dec_religion = $form_state['multistep_values']['deceased_details']['religion'];
        if ($dec_religion != 'Sikh') {
          $second_street = !empty($funeral_det['wake_street_2']) ? '<div>' . $funeral_det['wake_street_2'] . '</div>' : '';
          $form['adv_review']['step_5']['step_5_funeral_wakelocation'] = array(
            '#markup' => '<div>'. $funeral_det['wake_street_1'] . '</div>'. $second_street, // . '<div>' . $adv_det['zip'] . '</div><div>' . $country_name . '</div>',
            '#prefix' => '<div class="funeral-wakelocation"><div class="label-item">Wake Location</div>',
            '#suffix' => '</div>',
          );
        }
        
        if ($dec_religion=='Christian' || $dec_religion=='Catholic') {
          $night_services='';
          foreach ($funeral_det['more_nightserv'] as $key => $value) {
            if (is_array($value)) {
              if (!empty($value['night_services'])) {
                $night_services .= $key == '0' ? date_format(date_create($value['night_services']), 'd F Y \A\t H:i') : ', ' . date_format(date_create($value['night_services']), 'd F Y \A\t H:i');
              } 
            }
          }

          $night_title = $dec_religion=='Christian' ? 'Night Services' : 'Night Prayers';
          $form['adv_review']['step_5']['step_5_funeral_nightservice'] = array(
            '#markup' => $night_services,
            '#prefix' => '<div class="funeral_nightservice"><div class="label-item">'. $night_title .'</div>',
            '#suffix' => '</div>',
          );
        }

        if ($dec_religion == 'Soka') {
          $form['adv_review']['step_5']['step_5_funeral_ssamemorial'] = array(
            '#markup' => date_format(date_create($funeral_det['ssa_memorial']), 'd F Y \A\t H:i'),
            '#prefix' => '<div class="funeral_ssamemorial"><div class="label-item">SSA Memorial Service on</div>',
            '#suffix' => '</div>',
          );

          $form['adv_review']['step_5']['step_5_funeral_service'] = array(
            '#markup' => date_format(date_create($funeral_det['funeral_service']), 'd F Y \A\t H:i'),
            '#prefix' => '<div class="funeral_service"><div class="label-item">Funeral Service on</div>',
            '#suffix' => '</div>',
          );
          /*
          $form['adv_review']['step_5']['step_5_funeral_leavesat'] = array(
            '#markup' => date_format(date_create($funeral_det['cortege_leaves_at']), 'H:i'),
            '#prefix' => '<div class="funeral_leavesat"><div class="label-item">Cortege Leaves at</div>',
            '#suffix' => '</div>',
          );
          */
        }
        
        switch ($dec_religion) {
          case 'Soka':
            $leaves_title = 'Cortege Leaves at';
            break;
          
          default:
            $leaves_title = 'Cortege Leaves on';
            break;
        }
        $form['adv_review']['step_5']['step_5_funeral_cortegeleaves'] = array(
          '#markup' => date_format(date_create($funeral_det['cortege_leaves']), 'd F Y \A\t H:i'),
          '#prefix' => '<div class="funeral_cortegeleaves"><div class="label-item">'. $leaves_title .'</div>',
          '#suffix' => '</div>',
        );

        if ($dec_religion == 'Sikh') {
          /*
          $form['adv_review']['step_5']['step_5_funeral_busesleaves'] = array(
            '#markup' => date_format(date_create($funeral_det['buses_leaves']), 'd F Y \A\t H:i'),
            '#prefix' => '<div class="funeral_busesleaves"><div class="label-item">Buses Leaves on</div>',
            '#suffix' => '</div>',
          );
          */

          $from_street2 = !empty($funeral_det['from_address_2']) ? '<div>' . $funeral_det['from_address_2'] . '</div>' : '';
          //$country_from = $countries[$funeral_det['from_country']];
          $form['adv_review']['step_5']['step_5_funeral_from'] = array(
            '#markup' => '<div>'. $funeral_det['from_address_1'] .'</div>'. $from_street2, //'<div>'. $funeral_det['from_zip'] .'</div><div>'. $country_from .'</div>',
            '#prefix' => '<div class="funeral_from"><div class="label-item">From</div>',
            '#suffix' => '</div>',
          );
          $and_street2 = !empty($funeral_det['and_address_2']) ? '<div>' . $funeral_det['and_address_2'] . '</div>' : '';
          //$country_and = $countries[$funeral_det['and_country']];
          $form['adv_review']['step_5']['step_5_funeral_and'] = array(
            '#markup' => '<div>'. $funeral_det['and_address_1'] .'</div>'. $and_street2, //'<div>'. $funeral_det['and_zip'] .'</div>', //'<div>'. $country_and .'</div>',
            '#prefix' => '<div class="funeral_and"><div class="label-item">And</div>',
            '#suffix' => '</div>',
          );
        }

        $form['adv_review']['step_5']['step_5_funeral_cortegefor'] = array(
          '#markup' => $funeral_det['cortege_for'],
          '#prefix' => '<div class="funeral_cortegefor"><div class="label-item">For</div>',
          '#suffix' => '</div>',
        );
        $form['adv_review']['step_5']['step_5_funeral_cortegeat'] = array(
          '#markup' => $funeral_det['cortege_at'],
          '#prefix' => '<div class="funeral_cortegeat"><div class="label-item">At</div>',
          '#suffix' => '</div>',
        );
        $form['adv_review']['step_5']['step_5_funeral_cortegetime'] = array(
          '#markup' => date_format(date_create($funeral_det['cortege_time']), 'H:i'),
          '#prefix' => '<div class="funeral_cortegetime"><div class="label-item">At Time</div>',
          '#suffix' => '</div>',
        );
        if ($dec_religion=='Christian' || $dec_religion=='Catholic') {
          $bible_verses = !empty($funeral_det['bible_verse']) ? '<p>'. publish_obituary_get_bible_word($funeral_det['bible_verse']) .'</p>' : '';
          $bible_verses .= !empty($funeral_det['custom_verse']) ? '<p>'. $funeral_det['custom_verse'] .'</p>' : '';
          $form['adv_review']['step_5']['step_5_funeral_bibleverse'] = array(
            '#markup' => $bible_verses,
            '#prefix' => '<div class="funeral_bibleverse"><div class="label-item">Bible Verses</div>',
            '#suffix' => '</div>',
          );
        }
      } else {
        $form['adv_review']['step_5']['step_5_funeral_owntemplate'] = array(
          '#markup' => $funeral_det['own_template']['value'],
          '#prefix' => '<div class="funeral_owntemplate"><div class="label-item"></div>',
          '#suffix' => '</div>',
        );
      }
    }
    $form['adv_review']['step_5']['step_5_close_tag'] = array(
      '#markup' => '',
      '#suffix' => '</div></div>',
    );
  }

  if ($form_state['kof'] == 'acknowledgement') {
    $acknow_det = $form_state['multistep_values']['acknowledgement_details'];
    $dec_religion = $form_state['multistep_values']['deceased_details']['religion'];
    $remarks_title = $dec_religion=='Hindu' ? 'Please treat this as a personal invitation' : 'Other remarks';
    
    $form['adv_review']['step_5']['step_5_title'] = array(
        '#markup' => '<h3>Acknowledgement Details</h3>',
        '#prefix' => '<div class="step-item step-5-acknowledgement-details">',
        '#suffix' => '<div class="acknowledgement-value">',
    );
    $form['adv_review']['step_5']['step_5_acknowledgement_message'] = array(
        '#markup' => $acknow_det['message'],
        '#prefix' => '<div class="acknowledgement-message"><div class="label-item">Acknowledgement</div>',
        '#suffix' => '</div>',
    );
    $form['adv_review']['step_5']['step_5_acknowledgement_thanks'] = array(
        '#markup' => $acknow_det['special_thanks'],
        '#prefix' => '<div class="acknowledgement_thanks"><div class="label-item">Special Thanks to</div>',
        '#suffix' => '</div>',
    );

    if ($dec_religion=='Hindu') {
      // Karumathi
      $karumathi_date = !empty($acknow_det['karumathi_date']) ? date_format(date_create($acknow_det['karumathi_date']), 'd F Y') : '';
      $form['adv_review']['step_5']['step_5_karumathi_date'] = array(
        '#markup' => $karumathi_date,
        '#prefix' => '<div class="acknowledgement_karumathi_date"><div class="label-item">Karumathi prayers will be held on the night of</div>',
        '#suffix' => '</div>',
      );
      $form['adv_review']['step_5']['step_5_karumathi_address'] = array(
        '#markup' => $acknow_det['karumathi_address'],
        '#prefix' => '<div class="acknowledgement_karumathi_address"><div class="label-item">Address</div>',
        '#suffix' => '</div>',
      );
  
      // Athma Shanthi
      $athma_date = !empty($acknow_det['athma_shanthi_date']) ? date_format(date_create($acknow_det['athma_shanthi_date']), 'd F Y') : '';
      $form['adv_review']['step_5']['step_5_athma_shanthi_date'] = array(
        '#markup' => $athma_date,
        '#prefix' => '<div class="acknowledgement_athma_shanthi_date"><div class="label-item">Athma Shanthi prayers will be held on</div>',
        '#suffix' => '</div>',
      );
      $form['adv_review']['step_5']['step_5_athma_shanthi_temple'] = array(
        '#markup' => $acknow_det['athma_shanthi_temple'],
        '#prefix' => '<div class="acknowledgement_athma_shanthi_temple"><div class="label-item">At Temple </div>',
        '#suffix' => '</div>',
      );
      $athma_time = !empty($acknow_det['athma_shanthi_time']) ? date_format(date_create($acknow_det['athma_shanthi_time']), 'H:i') : '';
      $form['adv_review']['step_5']['step_5_athma_shanthi_time'] = array(
        '#markup' => $athma_time,
        '#prefix' => '<div class="acknowledgement_athma_shanthi_time"><div class="label-item">At Time </div>',
        '#suffix' => '</div>',
      );
    }

    $form['adv_review']['step_5']['step_5_acknowledgement_remarks'] = array(
        '#markup' => $acknow_det['other_remarks'],
        '#prefix' => '<div class="acknowledgement_remarks"><div class="label-item">'. $remarks_title .'</div>',
        '#suffix' => '</div>',
    );
    $form['adv_review']['step_5']['step_5_close_tag'] = array(
      '#markup' => '',
      '#suffix' => '</div></div>',
    );
  }

  if ($form_state['kof'] == 'condolence') {
    $condo_det = $form_state['multistep_values']['condolence_details'];
    $form['adv_review']['step_5']['step_5_title'] = array(
      '#markup' => '<h3>Condolence Details</h3>',
      '#prefix' => '<div class="step-item step-5-condolence-details">',
      '#suffix' => '<div class="condolence-value">',
    );
    $form['adv_review']['step_5']['step_5_condolence_message'] = array(
      '#markup' => $condo_det['message']['value'],
      '#prefix' => '<div class="condolence-message"><div class="label-item">Condolence Message</div>',
      '#suffix' => '</div>',
    );
    $form['adv_review']['step_5']['step_5_condolence_thanks'] = array(
      '#markup' => $condo_det['condo_from']['value'],
      '#prefix' => '<div class="condolence_thanks"><div class="label-item">From</div>',
      '#suffix' => '</div>',
    );
    $form['adv_review']['step_5']['step_5_condolence_remarks'] = array(
      '#markup' => $condo_det['other_remarks'],
      '#prefix' => '<div class="condolence_remarks"><div class="label-item">Other remarks</div>',
      '#suffix' => '</div>',
    );
    $form['adv_review']['step_5']['step_5_close_tag'] = array(
      '#markup' => '',
      '#suffix' => '</div></div>',
    );
  }

  if ($form_state['kof'] == 'in-memoriam') {
    $memo_det = $form_state['multistep_values']['memoriam_details'];
    $memo_message = !empty($memo_det['custom_message']) ? $memo_det['custom_message'] : publish_obituary_get_memo_message_word($memo_det['select_message']);
    
    $rememberer='';
    foreach ($memo_det['more_rememberer'] as $key => $value) {
      if (is_array($value)) {
        if (!empty($value['rememberer_name'])) $rememberer .= '<div>' . $value['rememberer_name'] . '</div>';
      }
    }
    
    $form['adv_review']['step_5'] = array(
      'step_5_title' => array(
        '#markup' => '<h3>In Memoriam Details</h3>',
        '#prefix' => '<div class="step-item step-5-memoriam-details">',
      ),
      /*
      'step_5_memoriam_anniversary' => array(
        '#markup' => $memo_det['anniversary'] .'<br>'. $memo_det['anniversary_info'],
        '#prefix' => '<div class="memoriam-value"><div class="memoriam-anniversary"><div class="label-item">Year of Anniversary</div>',
        '#suffix' => '</div>',
      ),
      */
      'step_5_memoriam_message' => array(
        '#markup' => $memo_message,
        '#prefix' => '<div class="memoriam-message"><div class="label-item">Message</div>',
        '#suffix' => '</div>',
      ),
      'step_5_memoriam_rememberer' => array(
        '#markup' => $rememberer,
        '#prefix' => '<div class="memoriam-rememberer"><div class="label-item">Dearly missed and fondly remembered by</div>',
        '#suffix' => '</div></div></div>',
      ),
    );
  }

  if ($form_state['kof']!='acknowledgement') {
    $form['adv_review']['step_6']['step_6_title'] = array(
        '#markup' => '<h3>Upload Forms and Photo</h3>',
        '#prefix' => '<div class="step-item step-6-upload-certifphoto">',
        '#suffix' => '<div class="upload-value">',
    );
    if (!$p_onsite) {
      $deceased_photo = file_load($form_state['multistep_values']['certificate_photo']['deceased_photo']);
      $death_certific = file_load($form_state['multistep_values']['certificate_photo']['death_certificate']);
      $indemnity_form = file_load($form_state['multistep_values']['certificate_photo']['indemnity_upload']);
      $form['adv_review']['step_6']['step_6_upload_photo'] = array(
          '#markup' => $deceased_photo->filename,
          '#prefix' => '<div class="upload-deceasedphoto"><div class="label-item">Deceased Photo</div>',
          '#suffix' => '</div>',
      );
      if ($form_state['kof']=='condolence') {
        $deceased_logo = file_load($form_state['multistep_values']['certificate_photo']['company_logo']);
        $form['adv_review']['step_6']['step_6_upload_logo'] = array(
          '#markup' => $deceased_logo->filename,
          '#prefix' => '<div class="upload-value"><div class="upload-complogo"><div class="label-item">Company Logo</div>',
          '#suffix' => '</div>',
        );
      }
      $form['adv_review']['step_6']['step_6_upload_certificate'] = array(
        '#markup' => $death_certific->filename,
        '#prefix' => '<div class="upload_deathcertificate"><div class="label-item">Death Certificate</div>',
        '#suffix' => '</div>',
      );
      $form['adv_review']['step_6']['step_6_upload_indemnity'] = array(
        '#markup' => isset($indemnity_form->filename) ? $indemnity_form->filename : '',
        '#prefix' => '<div class="upload_indemnityform"><div class="label-item">Indemnity Form</div>',
        '#suffix' => '</div>',
      );
    }
    $form['adv_review']['step_6']['step_6_close_tag'] = array(
      '#markup' => '',
      '#suffix' => '</div></div>',
    );
  }
  
  $form['back'] = array(
    '#type' => 'submit',
    '#value' => t('Back'),
  );
  
  $form['next'] = array(
    '#type' => 'submit',
    '#value' => t('Complete'),
  );
    
  return $form;
}

/**
 * Form to edit Obituary.
 * 
 * @param type $form
 * @param type $form_state
 * @return type 
 */
function publish_obituary_edit_form($form, &$form_state, $node, $no_js_use=FALSE) {

  $form['#attributes']['enctype'] = 'multipart/form-data';

  $form['nid'] = array(
    '#type' => 'hidden',
    '#value' => $node->nid,
  );

  $form['deceased_photo'] = array(
    '#type' => 'managed_file',
    '#title' => 'Deceased Photo',
    '#upload_location' => 'public://',
    //'#required' => TRUE,
    '#upload_validators'  => array('file_validate_extensions' => array('png gif jpg jpeg')),
    '#default_value' => field_get_items('node', $node, 'field_obituary_deceased_photo')[0]['fid'],
  );

  $form['deceased_name'] = array(
    '#type' => 'textfield',
    '#title' => 'Deceased Name',
    '#attributes' => array('placeholder' => 'Deceased Name'),
    //'#required' => TRUE,
    '#default_value' => $node->title,
  );

  $form['deceased_age'] = array(
    '#type' => 'textfield',
    '#title' => 'Deceased Age',
    '#attributes' => array('placeholder' => 'Deceased Age'),
    //'#required' => TRUE,
    '#default_value' => field_get_items('node', $node, 'field_obituary_age')[0]['value'],
  );
  
  $departed_date = format_date(field_get_items('node', $node, 'field_obituary_departed')[0]['value'], 'custom', 'Y-m-d H:i:s');
  $form['departed_date'] = array(
    '#title' => 'Departed Date',
    '#type' => 'date_text',
    '#date_format' => 'd F Y',
    //'#required' => TRUE,
    '#default_value' => $departed_date,
  );

  $def_religion = field_get_items('node', $node, 'field_obituary_religion')[0]['value'];
  $form['deceased_religion'] = array(
    '#type' => 'select',
    '#title' => 'Religion',
    '#options' => array(
      'christian' => 'Christian',
      'catholic' => 'Catholic',
      'buddhist' => 'Buddhist',
      'taoist' => 'Taoist',
      'hindu' => 'Hindu',
      'sikh' => 'Sikh',
      'soka' => 'Soka',
      'free thinker' => 'Free thinker',
      'others' => 'Others',
    ),
    //'#required' => TRUE,
    '#default_value' => $def_religion,
  );

  $personality = field_get_items('node', $node, 'body')[0]['value'];
  if (substr($personality, 0, 3) == '<p>') {
    $personality = strip_tags($personality);
  }
  $form['deceased_personality'] = array(
    '#title' => 'Deceased Personality',
    '#type' => 'textarea',
    '#rows' => 7,
    //'#required' => TRUE,
    '#attributes' => array('placeholder' => 'Describe deceased personality'),
    '#default_value' => $personality,
  );

  $form['funeral_details'] = array(
    '#type' => 'fieldset',
    '#title' => 'Funeral Details',
    //'#collapsible' => TRUE,
    //'#collapsed' => TRUE,
  );
  $funeral_template = field_get_items('node', $node, 'field_funeral_template')[0]['value'];
  $form['funeral_details']['template'] = array(
    '#type' => 'hidden',
    '#value' => $funeral_template,
  );
  if ($funeral_template==1) {
    if ($def_religion != 'sikh') {
      $form['funeral_details']['wake_address_one'] = array(
        '#type' => 'hidden',
        '#value' => field_get_items('node', $node, 'field_wake_street_address_1')[0]['value'],
      );

      $form['funeral_details']['wake_address'] = array(
        '#type' => 'fieldset',
      );

      $form['funeral_details']['wake_address']['street_one'] = array(
        '#type' => 'textfield',
        '#title' => 'Wake Address',
        '#attributes' => array('placeholder' => 'Street Address 1'),
        //'#required' => TRUE,  //edited for own textarea
        '#default_value' => field_get_items('node', $node, 'field_wake_street_address_1')[0]['value'],
      );
      
      $form['funeral_details']['wake_address']['street_two'] = array(
        '#type' => 'textfield',
        '#attributes' => array('placeholder' => 'Street Address 2'),
        '#default_value' => field_get_items('node', $node, 'field_wake_street_address_2')[0]['value'],
      );
    }

    $leaves_on = format_date(field_get_items('node', $node, 'field_cortage_leaves_on')[0]['value'], 'custom', 'Y-m-d H:i:s');
    $form['funeral_details']['cortege_leaves_on'] = array(
      '#title' => 'Cortege Leaves on',
      '#type' => 'date_text',
      '#date_format' => 'd F Y H:i',
      //'#required' => TRUE,  //edited for own textarea
      '#default_value' => $leaves_on,
    );

    $cortege_for = field_get_items('node', $node, 'field_cortage_leaves_for')[0]['value'];
    $form['funeral_details']['cortege_for'] = array(
      '#type' => 'select', 
      '#title' => 'For',
      '#options' => array(''=>'- Select -') + publish_obituary_funeral_for_user(ucfirst($def_religion)),
      //'#required' => TRUE,  //edited for own textarea
      '#default_value' => $cortege_for,
    );

    // Preparing variable for At options
    if (!empty($cortege_for)) {
      $at_basic = publish_obituary_funeral_at(ucfirst($def_religion), ucfirst($cortege_for));
      $at_options = array(''=>'- Select -') + $at_basic;
    }

    $form['funeral_details']['cortege_at'] = array(
      '#title' => 'At',
      '#type' => 'select',
      '#options' => $at_options,
      //'#required' => TRUE,  //edited for own textarea
      '#default_value' => field_get_items('node', $node, 'field_cortage_leaves_at')[0]['value'],
    );

    $leaves_time = format_date(field_get_items('node', $node, 'field_cortage_leaves_at_time')[0]['value'], 'custom', 'Y-m-d H:i:s');
    $form['funeral_details']['cortege_time'] = array(
      '#title' => 'At Time',
      '#type' => 'date_text',
      '#date_format' => 'H:i',
      //'#required' => TRUE,  //edited for own textarea
      '#default_value' => $leaves_time,
    );

    $remarks = field_get_items('node', $node, 'field_obituary_remarks')[0]['value'];
    if (substr($remarks, 0, 3) == '<p>') {
      $remarks = strip_tags($remarks);
    }
    $form['funeral_details']['other_remarks'] = array(
      '#title' => 'Other Remarks',
      '#type' => 'textarea',
      '#rows' => 7,
      '#default_value' => $remarks,
    );

    $dec_religion = field_get_items('node', $node, 'field_obituary_religion')[0]['value'];
    if ($dec_religion=='christian' || $dec_religion=='catholic') {
      $night_title = $dec_religion=='catholic' ? 'Night Prayers' : 'Night Services';
      /**/
      $night_field = field_get_items('node', $node, 'field_night_services_prayers');
      $night_date = array();

      foreach ($night_field as $nk => $nv) {
        $night_obj = field_collection_item_load($nv);
        $night_date[$nk] = format_date(field_get_items('field_collection_item', $night_obj, 'field_night_serv_pray_date')[0]['value'], 'custom', 'Y-m-d H:i:s');
      }
      
      /** Start add more ajax for night service */
      $form['funeral_details']['more_nightserv'] = array(
        '#type' => 'fieldset',
        '#tree' => TRUE,
        //'#required' => TRUE,  //edited for own textarea
        '#prefix' => '<div id="more-nightserv">',
        '#suffix' => '</div>',
      );
    
      if (empty($form_state['nsnum_names'])) {
        $form_state['nsnum_names'] = count($night_date);
      }
    
      for ($i = 0; $i < $form_state['nsnum_names']; $i++) {
        $form['funeral_details']['more_nightserv'][$i]['night_services'] = array(
          '#type' => 'date_text',
          '#title' => $i == 0 ? $night_title : '',
          '#date_format' => 'd F Y H:i',
          '#attributes' => array(
            'multiple' => 'multiple',
          ),
          //'#required' => $i == 0 ? TRUE : FALSE,
          '#required' => FALSE,
          '#default_value' => $i < count($night_date) ? $night_date[$i] : NULL,
        );
      }
    
      $form['funeral_details']['more_nightserv']['add_nightserv'] = array(
        '#type' => 'submit',
        '#value' => '+',
        '#name' => 'add_more_nightservi',
        '#submit' => array('nightservi_add_more_add_one'),
        '#ajax' => array(
          'callback' => 'nightservi_add_more_callback',
          'wrapper' => 'more-nightserv',
        ),
      );
    
      if ($no_js_use) {
        if (!empty($form['more_nightserv']['remove_nightserv']['#ajax'])) {
          unset($form['more_nightserv']['remove_nightserv']['#ajax']);
        }
        unset($form['more_nightserv']['add_nightserv']['#ajax']);
      }
      /** End add more ajax for night service */
    }
  } else {
    $form['funeral_details']['own_template'] = array(
      //'#title' => 'Other Remarks',
      '#type' => 'text_format',
      '#format' => 'custom_html',
      '#rows' => 7,
      '#default_value' => field_get_items('node', $node, 'field_custom_funeral')[0]['value'],
    );
  }

  $form['family_friends'] = array(
    '#type' => 'fieldset',
    '#title' => 'Family and Friends',
    //'#collapsible' => TRUE,
    //'#collapsed' => TRUE,
  );

  $spouse_field = field_get_items('node', $node, 'field_obituary_spouse');
  $spouse_obj = field_collection_item_load($spouse_field[0]['value']);
  $spouse_as = field_get_items('field_collection_item', $spouse_obj, 'field_husband_wife')[0]['value'];
  $spouse_name = field_get_items('field_collection_item', $spouse_obj, 'field_spouse_name')[0]['value'];
  
  $form['family_friends']['spouse_as'] = array(
    '#type' => 'select',
    '#title' => 'Spouse',
    '#options' => array(
      'husband' => 'Husband',
      'wife' => 'Wife',
    ),
    '#default_value' => $spouse_as,
  );

  $form['family_friends']['spouse_name'] = array(
    '#type' => 'textfield',
    //'#attributes' => array('placeholder' => 'Name'),
    '#default_value' => $spouse_name,
  );

  /**
   * Form element as ajax callback return for more fathers
   */
  $father_pair = publish_obituary_family_fields($node, 'field_obituary_father', 'field_father_name', 'field_mother_name');
  $form['family_friends']['more_fathers'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-fathers">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['fnum_names'])) {
    $form_state['fnum_names'] = !empty($father_pair) ? count($father_pair) : 1;
  }

  for ($i = 0; $i < $form_state['fnum_names']; $i++) {
    $form['family_friends']['more_fathers'][$i]['father_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Father' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Son Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => $i < count($father_pair) ? $father_pair[$i]['main'] : NULL,
    );

    $form['family_friends']['more_fathers'][$i]['mother_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Mother' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Daughter-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => $i < count($father_pair) ? $father_pair[$i]['pair'] : NULL,
    );
  }

  $form['family_friends']['more_fathers']['add_father'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_fathere',
    '#submit' => array('fatherse_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'fatherse_add_more_callback',
      'wrapper' => 'more-fathers',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_fathers']['remove_father']['#ajax'])) {
       unset($form['more_fathers']['remove_father']['#ajax']);
    }
    unset($form['more_fathers']['add_father']['#ajax']);
  }
  /** End add more ajax for father */
  
  /**
   * Form element as ajax callback return for more sons
   */
  $sons_pair = publish_obituary_family_fields($node, 'field_obituary_sons', 'field_sons_name', 'field_daughters_in_law_name');
  $form['family_friends']['more_sons'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-sons">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['pnum_names'])) {
    $form_state['pnum_names'] = count($sons_pair);
  }

  for ($i = 0; $i < $form_state['pnum_names']; $i++) {
    $form['family_friends']['more_sons'][$i]['son_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Sons' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Son Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => $i < count($sons_pair) ? $sons_pair[$i]['main'] : NULL,
    );

    $form['family_friends']['more_sons'][$i]['daughter_law_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Daughters-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Daughter-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => $i < count($sons_pair) ? $sons_pair[$i]['pair'] : NULL,
    );
  }

  $form['family_friends']['more_sons']['add_son'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_sone',
    '#submit' => array('sonse_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'sonse_add_more_callback',
      'wrapper' => 'more-sons',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_sons']['remove_son']['#ajax'])) {
       unset($form['more_sons']['remove_son']['#ajax']);
    }
    unset($form['more_sons']['add_son']['#ajax']);
  }
  /** End add more ajax for sons */

  /**
   * Form element as ajax callback return for more daughters
   */
  $daughters_pair = publish_obituary_family_fields($node, 'field_obituary_daughters', 'field_daughters_name', 'field_sons_in_law_name');
  $form['family_friends']['more_daughters'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-daughters">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['dnum_names'])) {
    $form_state['dnum_names'] = count($daughters_pair);
  }
	
  for ($i = 0; $i < $form_state['dnum_names']; $i++) {
    $form['family_friends']['more_daughters'][$i]['daughter_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Daughters' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Daughter Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => $i < count($daughters_pair) ? $daughters_pair[$i]['main'] : NULL,
    );
    $form['family_friends']['more_daughters'][$i]['sons_law_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Sons-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Son-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => $i < count($daughters_pair) ? $daughters_pair[$i]['pair'] : NULL,
    );
  }
  
  $form['family_friends']['more_daughters']['add_daughter'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_daughtere',
    '#submit' => array('daughterse_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'daughterse_add_more_callback',
      'wrapper' => 'more-daughters',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_daughters']['remove_daughter']['#ajax'])) {
       unset($form['more_daughters']['remove_daughter']['#ajax']);
    }
    unset($form['more_daughters']['add_daughter']['#ajax']);
  }
  /** End add more ajax for daughter */

  /**
   * Form element as ajax callback return for more grandson
   */
  $grandsons_pair = publish_obituary_family_fields($node, 'field_obituary_grandsons', '	field_grandsons_name', '	field_granddaughters_in_law_name');
  $form['family_friends']['more_grandsons'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-grandsons">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['gsnum_names'])) {
    $form_state['gsnum_names'] = count($grandsons_pair);
  }
	
  for ($i = 0; $i < $form_state['gsnum_names']; $i++) {
    $form['family_friends']['more_grandsons'][$i]['grandson_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Grandsons' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Grandson Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => $i < count($grandsons_pair) ? $grandsons_pair[$i]['main'] : NULL,
    );
    $form['family_friends']['more_grandsons'][$i]['granddaughter_law_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Granddaughters-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Granddaughter-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => $i < count($grandsons_pair) ? $grandsons_pair[$i]['pair'] : NULL,
    );
  }
  
  $form['family_friends']['more_grandsons']['add_grandson'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_grandsone',
    '#submit' => array('grandsonse_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'grandsonse_add_more_callback',
      'wrapper' => 'more-grandsons',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_grandsons']['remove_grandson']['#ajax'])) {
       unset($form['more_grandsons']['remove_grandson']['#ajax']);
    }
    unset($form['more_grandsons']['add_grandson']['#ajax']);
  }
  /** End add more ajax for grandson */

  /**
   * Form element as ajax callback return for more granddaughter
   */
  $granddaughters_pair = publish_obituary_family_fields($node, 'field_obituary_granddaughters', 'field_granddaughters_name', 'field_grandsons_in_law_name');
  $form['family_friends']['more_granddaughters'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-granddaughters">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['gdnum_names'])) {
    $form_state['gdnum_names'] = count($granddaughters_pair);
  }
	
  for ($i = 0; $i < $form_state['gdnum_names']; $i++) {
    $form['family_friends']['more_granddaughters'][$i]['granddaughter_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Granddaughters' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Granddaughter Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => $i < count($granddaughters_pair) ? $granddaughters_pair[$i]['main'] : NULL,
    );
    $form['family_friends']['more_granddaughters'][$i]['grandson_law_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Grandsons-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Grandson-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => $i < count($granddaughters_pair) ? $granddaughters_pair[$i]['pair'] : NULL,
    );
  }
  
  $form['family_friends']['more_granddaughters']['add_granddaughter'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_granddaughtere',
    '#submit' => array('granddaughterse_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'granddaughterse_add_more_callback',
      'wrapper' => 'more-granddaughters',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_granddaughters']['remove_granddaughter']['#ajax'])) {
       unset($form['more_granddaughters']['remove_granddaughter']['#ajax']);
    }
    unset($form['more_granddaughters']['add_granddaughter']['#ajax']);
  }
  /** End add more ajax for granddaughter */

    /**
   * Form element as ajax callback return for more great grandson
   */
  $greatgrandsons_pair = publish_obituary_family_fields($node, 'field_obituary_great_grandsons', 'field_great_grandsons_name', 'field_great_granddaughters_name');
  $form['family_friends']['more_greatgrandsons'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-greatgrandsons">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['ggsnum_names'])) {
    $form_state['ggsnum_names'] = count($greatgrandsons_pair);
  }
	
  for ($i = 0; $i < $form_state['ggsnum_names']; $i++) {
    $form['family_friends']['more_greatgrandsons'][$i]['greatgrandson_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Great Grandsons' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Great Grandson Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => $i < count($greatgrandsons_pair) ? $greatgrandsons_pair[$i]['main'] : NULL,
    );
    $form['family_friends']['more_greatgrandsons'][$i]['greatgranddaughter'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Great Granddaughters' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Great Granddaughter Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => $i < count($greatgrandsons_pair) ? $greatgrandsons_pair[$i]['pair'] : NULL,
    );
  }
  
  $form['family_friends']['more_greatgrandsons']['add_greatgrandson'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_greatgrandsone',
    '#submit' => array('greatgrandsonse_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'greatgrandsonse_add_more_callback',
      'wrapper' => 'more-greatgrandsons',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_greatgrandsons']['remove_greatgrandson']['#ajax'])) {
       unset($form['more_greatgrandsons']['remove_greatgrandson']['#ajax']);
    }
    unset($form['more_greatgrandsons']['add_greatgrandson']['#ajax']);
  }
  /** End add more ajax for great grandson */

  /**
   * Form element as ajax callback return for more brother
   */
  $brothers_pair = publish_obituary_family_fields($node, 'field_obituary_brothers', 'field_brothers_name', 'field_sister_in_law_name');
  $form['family_friends']['more_brothers'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-brothers">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['btnum_names'])) {
    $form_state['btnum_names'] = count($brothers_pair);
  }
	
  for ($i = 0; $i < $form_state['btnum_names']; $i++) {
    $form['family_friends']['more_brothers'][$i]['brother_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Brothers' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Brother Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => $i < count($brothers_pair) ? $brothers_pair[$i]['main'] : NULL,
    );
    $form['family_friends']['more_brothers'][$i]['sister_law_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Sisters-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Sister-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => $i < count($brothers_pair) ? $brothers_pair[$i]['pair'] : NULL,
    );
  }
  
  $form['family_friends']['more_brothers']['add_brother'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_brothere',
    '#submit' => array('brotherse_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'brotherse_add_more_callback',
      'wrapper' => 'more-brothers',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_brothers']['remove_brother']['#ajax'])) {
       unset($form['more_brothers']['remove_brother']['#ajax']);
    }
    unset($form['more_brothers']['add_brother']['#ajax']);
  }
  /** End add more ajax for brother */

  /**
   * Form element as ajax callback return for more sister
   */
  $sisters_pair = publish_obituary_family_fields($node, 'field_obituary_sisters', 'field_sisters_name', 'field_brothers_in_law_name');
  $form['family_friends']['more_sisters'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-sisters">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['stnum_names'])) {
    $form_state['stnum_names'] = count($sisters_pair);
  }
	
  for ($i = 0; $i < $form_state['stnum_names']; $i++) {
    $form['family_friends']['more_sisters'][$i]['sister_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Sisters' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Sister Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => $i < count($sisters_pair) ? $sisters_pair[$i]['main'] : NULL,
    );
    $form['family_friends']['more_sisters'][$i]['brother_law_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Brothers-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Brother-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => $i < count($sisters_pair) ? $sisters_pair[$i]['pair'] : NULL,
    );
  }
  
  $form['family_friends']['more_sisters']['add_sister'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_sistere',
    '#submit' => array('sisterse_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'sisterse_add_more_callback',
      'wrapper' => 'more-sisters',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_sisters']['remove_sister']['#ajax'])) {
       unset($form['more_sisters']['remove_sister']['#ajax']);
    }
    unset($form['more_sisters']['add_sister']['#ajax']);
  }
  /** End add more ajax for sister */  

  /**
   * Form element as ajax callback return for more father in law
   */
  $fatherlaw_pair = publish_obituary_family_fields($node, 'field_obituary_father_in_law', 'field_father_in_law_name', 'field_mother_in_law_name');
  $form['family_friends']['more_fatherlaws'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-fatherlaws">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['flnum_names'])) {
    $form_state['flnum_names'] = !empty($fatherlaw_pair) ? count($fatherlaw_pair) : 1;
  }

  for ($i = 0; $i < $form_state['flnum_names']; $i++) {
    $form['family_friends']['more_fatherlaws'][$i]['fatherlaw_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Father-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Son Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => $i < count($fatherlaw_pair) ? $fatherlaw_pair[$i]['main'] : NULL,
    );

    $form['family_friends']['more_fatherlaws'][$i]['motherlaw_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Mother-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Daughter-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => $i < count($fatherlaw_pair) ? $fatherlaw_pair[$i]['pair'] : NULL,
    );
  }

  $form['family_friends']['more_fatherlaws']['add_fatherlaw'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_fatherlawe',
    '#submit' => array('fatherlawse_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'fatherlawse_add_more_callback',
      'wrapper' => 'more-fatherlaws',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_fatherlaws']['remove_fatherlaw']['#ajax'])) {
       unset($form['more_fatherlaws']['remove_fatherlaw']['#ajax']);
    }
    unset($form['more_fatherlaws']['add_fatherlaw']['#ajax']);
  }
  /** End add more ajax for father in law */

  /**
   * Form element as ajax callback return for more brothers in law
   */
  $brotherslaw_pair = publish_obituary_family_fields($node, 'field_obituary_brothers_in_law', 'field_brothers_law_main_name', 'field_sisters_law_pair_name');
  $form['family_friends']['more_brotherslaw'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-brotherslaw">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['btlnum_names'])) {
    $form_state['btlnum_names'] = count($brotherslaw_pair);
  }
	
  for ($i = 0; $i < $form_state['btlnum_names']; $i++) {
    $form['family_friends']['more_brotherslaw'][$i]['brotherlawmain_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Brothers-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Brother Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => $i < count($brotherslaw_pair) ? $brotherslaw_pair[$i]['main'] : NULL,
    );
    $form['family_friends']['more_brotherslaw'][$i]['sisterlawpair_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Sisters-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Sister-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => $i < count($brotherslaw_pair) ? $brotherslaw_pair[$i]['pair'] : NULL,
    );
  }
  
  $form['family_friends']['more_brotherslaw']['add_brotherlaw'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_brotherlawe',
    '#submit' => array('brotherslawe_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'brotherslawe_add_more_callback',
      'wrapper' => 'more-brotherslaw',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_brotherslaw']['remove_brotherlaw']['#ajax'])) {
       unset($form['more_brotherslaw']['remove_brotherlaw']['#ajax']);
    }
    unset($form['more_brotherslaw']['add_brotherlaw']['#ajax']);
  }
  /** End add more ajax for brothers in law */

  /**
   * Form element as ajax callback return for more sisters in law
   */
  $sisterslaw_pair = publish_obituary_family_fields($node, 'field_obituary_sisters_in_law', 'field_sisters_law_main_name', 'field_brothers_law_pair_name');
  $form['family_friends']['more_sisterslaw'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-sisterslaw">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['stlnum_names'])) {
    $form_state['stlnum_names'] = count($sisterslaw_pair);
  }
	
  for ($i = 0; $i < $form_state['stlnum_names']; $i++) {
    $form['family_friends']['more_sisterslaw'][$i]['sisterlawmain_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Sisters-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Sister Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => $i < count($sisterslaw_pair) ? $sisterslaw_pair[$i]['main'] : NULL,
    );
    $form['family_friends']['more_sisterslaw'][$i]['brotherlawpair_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Brothers-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Brother-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => $i < count($sisterslaw_pair) ? $sisterslaw_pair[$i]['pair'] : NULL,
    );
  }
  
  $form['family_friends']['more_sisterslaw']['add_sisterlaw'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_sisterlawe',
    '#submit' => array('sisterslawe_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'sisterslawe_add_more_callback',
      'wrapper' => 'more-sisterslaw',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_sisterslaw']['remove_sisterlaw']['#ajax'])) {
       unset($form['more_sisterslaw']['remove_sisterlaw']['#ajax']);
    }
    unset($form['more_sisterslaw']['add_sisterlaw']['#ajax']);
  }
  /** End add more ajax for sisters in law */

  /**
   * Form element as ajax callback return for more nephew
   */
  $nephews_pair = publish_obituary_family_fields($node, 'field_obituary_nephews', 'field_nephews_name', 'field_nieces_in_law_name');
  $form['family_friends']['more_nephews'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-nephews">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['npnum_names'])) {
    $form_state['npnum_names'] = count($nephews_pair);
  }
	
  for ($i = 0; $i < $form_state['npnum_names']; $i++) {
    $form['family_friends']['more_nephews'][$i]['nephew_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Nephews' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Nephew Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => $i < count($nephews_pair) ? $nephews_pair[$i]['main'] : NULL,
    );
    $form['family_friends']['more_nephews'][$i]['niece_law_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Nieces-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Niece-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => $i < count($nephews_pair) ? $nephews_pair[$i]['pair'] : NULL,
    );
  }
  
  $form['family_friends']['more_nephews']['add_nephew'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_nephewe',
    '#submit' => array('nephewse_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'nephewse_add_more_callback',
      'wrapper' => 'more-nephews',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_nephews']['remove_nephew']['#ajax'])) {
       unset($form['more_nephews']['remove_nephew']['#ajax']);
    }
    unset($form['more_nephews']['add_nephew']['#ajax']);
  }
  /** End add more ajax for nephew */

  /**
   * Form element as ajax callback return for more niece
   */
  $nieces_pair = publish_obituary_family_fields($node, 'field_obituary_nieces', 'field_nieces_name', 'field_nephews_in_law_name');
  $form['family_friends']['more_nieces'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-nieces">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['ncnum_names'])) {
    $form_state['ncnum_names'] = count($nieces_pair);
  }
	
  for ($i = 0; $i < $form_state['ncnum_names']; $i++) {
    $form['family_friends']['more_nieces'][$i]['niece_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Nieces' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Niece Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => $i < count($nieces_pair) ? $nieces_pair[$i]['main'] : NULL,
    );
    $form['family_friends']['more_nieces'][$i]['nephew_law_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Nephews-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Nephew-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => $i < count($nieces_pair) ? $nieces_pair[$i]['pair'] : NULL,
    );
  }
  
  $form['family_friends']['more_nieces']['add_niece'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_niecee',
    '#submit' => array('niecese_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'niecese_add_more_callback',
      'wrapper' => 'more-nieces',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_nieces']['remove_niece']['#ajax'])) {
       unset($form['more_nieces']['remove_niece']['#ajax']);
    }
    unset($form['more_nieces']['add_niece']['#ajax']);
  }
  /** End add more ajax for niece */

  /**
   * Form element as ajax callback return for more grandnephew
   */
  $grandnephews_pair = publish_obituary_family_fields($node, 'field_obituary_grandnephews', 'field_grandnephews_name', 'field_grandnieces_name');
  $form['family_friends']['more_grandnephews'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-grandnephews">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['gnpnum_names'])) {
    $form_state['gnpnum_names'] = count($grandnephews_pair);
  }
	
  for ($i = 0; $i < $form_state['gnpnum_names']; $i++) {
    $form['family_friends']['more_grandnephews'][$i]['grandnephew_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Grandnephews' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Grandnephew Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => $i < count($grandnephews_pair) ? $grandnephews_pair[$i]['main'] : NULL,
    );
    $form['family_friends']['more_grandnephews'][$i]['grandniece_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Grandnieces-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Grandniece Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => $i < count($grandnephews_pair) ? $grandnephews_pair[$i]['pair'] : NULL,
    );
  }
  
  $form['family_friends']['more_grandnephews']['add_grandnephew'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_grandnephewe',
    '#submit' => array('grandnephewse_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'grandnephewse_add_more_callback',
      'wrapper' => 'more-grandnephews',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_grandnephews']['remove_grandnephew']['#ajax'])) {
       unset($form['more_grandnephews']['remove_grandnephew']['#ajax']);
    }
    unset($form['more_grandnephews']['add_grandnephew']['#ajax']);
  }
  /** End add more ajax for grandnephew */

  /**
   * Form element as ajax callback return for more grandniece
   */
  $grandnieces_pair = publish_obituary_family_fields($node, 'field_obituary_grandnieces', 'field_grandnieces_real_name', 'field_grandnephews_in_law_name');
  $form['family_friends']['more_grandnieces'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-grandnieces">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['gcpnum_names'])) {
    $form_state['gcpnum_names'] = !empty($grandnieces_pair) ? count($grandnieces_pair) : 1;
  }
	
  for ($i = 0; $i < $form_state['gcpnum_names']; $i++) {
    $form['family_friends']['more_grandnieces'][$i]['grandniece_real_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Grandnieces' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Grandnephew Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => $i < count($grandnieces_pair) ? $grandnieces_pair[$i]['main'] : NULL,
    );
    $form['family_friends']['more_grandnieces'][$i]['grandnephew_law_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Grandnephews-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Grandniece Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => $i < count($grandnieces_pair) ? $grandnieces_pair[$i]['pair'] : NULL,
    );
  }
  
  $form['family_friends']['more_grandnieces']['add_grandniece'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_grandniecee',
    '#submit' => array('grandniecese_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'grandniecese_add_more_callback',
      'wrapper' => 'more-grandnieces',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_grandnieces']['remove_grandniece']['#ajax'])) {
       unset($form['more_grandnieces']['remove_grandniece']['#ajax']);
    }
    unset($form['more_grandnieces']['add_grandniece']['#ajax']);
  }
  /** End add more ajax for grandniece */

  /**
   * Form element as ajax callback return for more great grandnephew
   */
  $greatgrandnephews_pair = publish_obituary_family_fields($node, 'field_obituary_great_grandnephew', 'field_great_grandnephews_name', 'field_great_grandnieces_name');
  $form['family_friends']['more_greatgrandnephews'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-greatgrandnephews">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['ggnpnum_names'])) {
    $form_state['ggnpnum_names'] = !empty($greatgrandnephews_pair) ? count($greatgrandnephews_pair) : 1;
  }
	
  for ($i = 0; $i < $form_state['ggnpnum_names']; $i++) {
    $form['family_friends']['more_greatgrandnephews'][$i]['greatgrandnephew_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Great Grandnephews' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Grandnephew Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => $i < count($greatgrandnephews_pair) ? $greatgrandnephews_pair[$i]['main'] : NULL,
    );
    $form['family_friends']['more_greatgrandnephews'][$i]['greatgrandniece_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Great Grandnieces' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Grandniece Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => $i < count($greatgrandnephews_pair) ? $greatgrandnephews_pair[$i]['pair'] : NULL,
    );
  }
  
  $form['family_friends']['more_greatgrandnephews']['add_greatgrandnephew'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_greatgrandnephewe',
    '#submit' => array('greatgrandnephewse_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'greatgrandnephewse_add_more_callback',
      'wrapper' => 'more-greatgrandnephews',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_greatgrandnephews']['remove_greatgrandnephew']['#ajax'])) {
       unset($form['more_greatgrandnephews']['remove_greatgrandnephew']['#ajax']);
    }
    unset($form['more_greatgrandnephews']['add_greatgrandnephew']['#ajax']);
  }
  /** End add more ajax for great grandnephew */

  /**
   * Form element as ajax callback return for more uncle
   */
  $uncles_pair = publish_obituary_family_fields($node, 'field_obituary_uncles', 'field_uncle_name', 'field_uncles_spouses');
  $form['family_friends']['more_uncles'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-uncles">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['unnum_names'])) {
    $form_state['unnum_names'] = !empty($uncles_pair) ? count($uncles_pair) : 1;
  }

  for ($i = 0; $i < $form_state['unnum_names']; $i++) {
    $form['family_friends']['more_uncles'][$i]['uncle_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Uncles' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'God Son Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => $i < count($uncles_pair) ? $uncles_pair[$i]['main'] : NULL,
    );

    $form['family_friends']['more_uncles'][$i]['uncle_spouse_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Spouses' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'God Daughter-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => $i < count($uncles_pair) ? $uncles_pair[$i]['pair'] : NULL,
    );
  }

  $form['family_friends']['more_uncles']['add_uncle'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_unclee',
    '#submit' => array('unclese_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'unclese_add_more_callback',
      'wrapper' => 'more-uncles',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_uncles']['remove_uncle']['#ajax'])) {
       unset($form['more_uncles']['remove_uncle']['#ajax']);
    }
    unset($form['more_uncles']['add_uncle']['#ajax']);
  }
  /** End add more ajax for uncle */

  /**
   * Form element as ajax callback return for more auntie
   */
  $aunties_pair = publish_obituary_family_fields($node, 'field_obituary_aunties', 'field_aunties_name', 'field_aunties_spouses_name');
  $form['family_friends']['more_aunties'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-aunties">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['aunum_names'])) {
    $form_state['aunum_names'] = !empty($aunties_pair) ? count($aunties_pair) : 1;
  }

  for ($i = 0; $i < $form_state['aunum_names']; $i++) {
    $form['family_friends']['more_aunties'][$i]['auntie_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Aunties' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'God Son Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => $i < count($aunties_pair) ? $aunties_pair[$i]['main'] : NULL,
    );

    $form['family_friends']['more_aunties'][$i]['auntie_spouse_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Spouses' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'God Daughter-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => $i < count($aunties_pair) ? $aunties_pair[$i]['pair'] : NULL,
    );
  }

  $form['family_friends']['more_aunties']['add_auntie'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_auntiee',
    '#submit' => array('auntiese_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'auntiese_add_more_callback',
      'wrapper' => 'more-aunties',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_aunties']['remove_auntie']['#ajax'])) {
       unset($form['more_aunties']['remove_auntie']['#ajax']);
    }
    unset($form['more_aunties']['add_auntie']['#ajax']);
  }
  /** End add more ajax for auntie */

  /**
   * Form element as ajax callback return for more cousin
   */
  $cousins_pair = publish_obituary_family_fields($node, 'field_obituary_cousins', 'field_cousins_name', 'field_cousins_in_law_name');
  $form['family_friends']['more_cousins'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-cousins">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['csnum_names'])) {
    $form_state['csnum_names'] = !empty($cousins_pair) ? count($cousins_pair) : 1;
  }

  for ($i = 0; $i < $form_state['csnum_names']; $i++) {
    $form['family_friends']['more_cousins'][$i]['cousin_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Cousins' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'God Son Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => $i < count($cousins_pair) ? $cousins_pair[$i]['main'] : NULL,
    );

    $form['family_friends']['more_cousins'][$i]['cousin_law_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Cousins-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'God Daughter-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => $i < count($cousins_pair) ? $cousins_pair[$i]['pair'] : NULL,
    );
  }

  $form['family_friends']['more_cousins']['add_cousin'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_cousine',
    '#submit' => array('cousinse_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'cousinse_add_more_callback',
      'wrapper' => 'more-cousins',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_cousins']['remove_cousin']['#ajax'])) {
       unset($form['more_cousins']['remove_cousin']['#ajax']);
    }
    unset($form['more_cousins']['add_cousin']['#ajax']);
  }
  /** End add more ajax for cousin */

  /**
   * Form element as ajax callback return for more god sons
   */
  $godsons_pair = publish_obituary_family_fields($node, 'field_obituary_god_sons', 'field_god_sons_name', 'field_god_daughters_in_law_name');
  $form['family_friends']['more_godsons'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-godsons">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['gosnum_names'])) {
    $form_state['gosnum_names'] = count($godsons_pair);
  }

  for ($i = 0; $i < $form_state['gosnum_names']; $i++) {
    $form['family_friends']['more_godsons'][$i]['godson_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'God Sons' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'God Son Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => $i < count($godsons_pair) ? $godsons_pair[$i]['main'] : NULL,
    );

    $form['family_friends']['more_godsons'][$i]['goddaughter_law_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'God Daughters-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'God Daughter-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => $i < count($godsons_pair) ? $godsons_pair[$i]['pair'] : NULL,
    );
  }

  $form['family_friends']['more_godsons']['add_godson'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_godsone',
    '#submit' => array('godsonse_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'godsonse_add_more_callback',
      'wrapper' => 'more-godsons',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_godsons']['remove_godson']['#ajax'])) {
       unset($form['more_godsons']['remove_godson']['#ajax']);
    }
    unset($form['more_godsons']['add_godson']['#ajax']);
  }
  /** End add more ajax for sons */

  /**
   * Form element as ajax callback return for more god daughters
   */
  $goddaughters_pair = publish_obituary_family_fields($node, 'field_obituary_god_daughters', 'field_god_daughters_name', 'field_god_sons_in_law_name');
  $form['family_friends']['more_goddaughters'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-goddaughters">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['godnum_names'])) {
    $form_state['godnum_names'] = count($goddaughters_pair);
  }
	
  for ($i = 0; $i < $form_state['godnum_names']; $i++) {
    $form['family_friends']['more_goddaughters'][$i]['goddaughter_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'God Daughters' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'God Daughter Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => $i < count($goddaughters_pair) ? $goddaughters_pair[$i]['main'] : NULL,
    );
    $form['family_friends']['more_goddaughters'][$i]['godsons_law_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'God Sons-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'God Son-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => $i < count($goddaughters_pair) ? $goddaughters_pair[$i]['pair'] : NULL,
    );
  }
  
  $form['family_friends']['more_goddaughters']['add_goddaughter'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_goddaughtere',
    '#submit' => array('goddaughterse_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'goddaughterse_add_more_callback',
      'wrapper' => 'more-goddaughters',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_goddaughters']['remove_goddaughter']['#ajax'])) {
       unset($form['more_goddaughters']['remove_goddaughter']['#ajax']);
    }
    unset($form['more_goddaughters']['add_goddaughter']['#ajax']);
  }
  /** End add more ajax for god daughter */

  /**
   * Form element as ajax callback return for more god grandson
   */
  $godgrandsons_pair = publish_obituary_family_fields($node, 'field_obituary_god_grandsons', 'field_god_grandsons_name', 'field_god_granddaughters_in_law_');
  $form['family_friends']['more_godgrandsons'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-godgrandsons">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['gogsnum_names'])) {
    $form_state['gogsnum_names'] = count($godgrandsons_pair);
  }
	
  for ($i = 0; $i < $form_state['gogsnum_names']; $i++) {
    $form['family_friends']['more_godgrandsons'][$i]['godgrandson_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'God Grandsons' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'God Grandson Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => $i < count($godgrandsons_pair) ? $godgrandsons_pair[$i]['main'] : NULL,
    );
    $form['family_friends']['more_godgrandsons'][$i]['godgranddaughter_law_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'God Granddaughters-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'God Granddaughter-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => $i < count($godgrandsons_pair) ? $godgrandsons_pair[$i]['pair'] : NULL,
    );
  }
  
  $form['family_friends']['more_godgrandsons']['add_godgrandson'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_godgrandsone',
    '#submit' => array('godgrandsonse_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'godgrandsonse_add_more_callback',
      'wrapper' => 'more-godgrandsons',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_godgrandsons']['remove_godgrandson']['#ajax'])) {
       unset($form['more_godgrandsons']['remove_godgrandson']['#ajax']);
    }
    unset($form['more_godgrandsons']['add_godgrandson']['#ajax']);
  }
  /** End add more ajax for god grandson */

  /**
   * Form element as ajax callback return for more god granddaughter
   */
  $godgranddaughters_pair = publish_obituary_family_fields($node, 'field_obituary_god_granddaugh', 'field_god_granddaughters_name', 'field_god_grandsons_in_law_name');
  $form['family_friends']['more_godgranddaughters'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-godgranddaughters">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['ggdnum_names'])) {
    $form_state['ggdnum_names'] = count($godgranddaughters_pair);
  }
	
  for ($i = 0; $i < $form_state['ggdnum_names']; $i++) {
    $form['family_friends']['more_godgranddaughters'][$i]['godgranddaughter_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'God Granddaughters' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'God Granddaughter Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => $i < count($godgranddaughters_pair) ? $godgranddaughters_pair[$i]['main'] : NULL,
    );
    $form['family_friends']['more_godgranddaughters'][$i]['godgrandson_law_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'God Grandsons-in-law' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'God Grandson-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => $i < count($godgranddaughters_pair) ? $godgranddaughters_pair[$i]['pair'] : NULL,
    );
  }
  
  $form['family_friends']['more_godgranddaughters']['add_godgranddaughter'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_godgranddaughtere',
    '#submit' => array('godgranddaughterse_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'godgranddaughterse_add_more_callback',
      'wrapper' => 'more-godgranddaughters',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_godgranddaughters']['remove_godgranddaughter']['#ajax'])) {
       unset($form['more_godgranddaughters']['remove_godgranddaughter']['#ajax']);
    }
    unset($form['more_godgranddaughters']['add_godgranddaughter']['#ajax']);
  }
  /** End add more ajax for god granddaughter */

  /**
   * Form element as ajax callback return for more god great grandson
   */
  $godgreatgrandsons_pair = publish_obituary_family_fields($node, 'field_obituary_god_great_grandso', 'field_god_great_grandsons_name', 'god_great_granddaughters_n');
  $form['family_friends']['more_godgreatgrandsons'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-godgreatgrandsons">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['gggsnum_names'])) {
    $form_state['gggsnum_names'] = !empty($godgreatgrandsons_pair) ? count($godgreatgrandsons_pair) : 1;
  }
	
  for ($i = 0; $i < $form_state['gggsnum_names']; $i++) {
    $form['family_friends']['more_godgreatgrandsons'][$i]['godgreatgrandson_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'God Great Grandsons' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'God Grandson Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => $i < count($godgreatgrandsons_pair) ? $godgreatgrandsons_pair[$i]['main'] : NULL,
    );
    $form['family_friends']['more_godgreatgrandsons'][$i]['godgreatgranddaughter_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'God Great Granddaughters' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'God Granddaughter-in-law Name',
      ),
      '#suffix' => '</div>',
      '#default_value' => $i < count($godgreatgrandsons_pair) ? $godgreatgrandsons_pair[$i]['pair'] : NULL,
    );
  }
  
  $form['family_friends']['more_godgreatgrandsons']['add_godgreatgrandson'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_godgreatgrandsone',
    '#submit' => array('godgreatgrandsonse_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'godgreatgrandsonse_add_more_callback',
      'wrapper' => 'more-godgreatgrandsons',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_godgreatgrandsons']['remove_godgreatgrandson']['#ajax'])) {
       unset($form['more_godgreatgrandsons']['remove_godgreatgrandson']['#ajax']);
    }
    unset($form['more_godgreatgrandsons']['add_godgreatgrandson']['#ajax']);
  }
  /** End add more ajax for god great grandson */

  /**
   * Form element as ajax callback return for more caregiver
   */
  $caregivers_pair = publish_obituary_family_fields($node, 'field_obituary_caregivers', 'field_caregivers_name', '');
  $form['family_friends']['more_caregivers'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-caregivers">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['cgvnum_names'])) {
    $form_state['cgvnum_names'] = count($caregivers_pair);
  }

  for ($i = 0; $i < $form_state['cgvnum_names']; $i++) {
    $form['family_friends']['more_caregivers'][$i]['caregiver_name'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Caregivers' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Caregiver Name',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#suffix' => '</div>',
      '#default_value' => $i < count($caregivers_pair) ? $caregivers_pair[$i]['main'] : NULL,
    );
  }

  $form['family_friends']['more_caregivers']['add_caregiver'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_caregivere',
    '#submit' => array('caregiverse_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'caregiverse_add_more_callback',
      'wrapper' => 'more-caregivers',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_caregivers']['remove_caregiver']['#ajax'])) {
       unset($form['more_caregivers']['remove_caregiver']['#ajax']);
    }
    unset($form['more_caregivers']['add_caregiver']['#ajax']);
  }
  /** End add more ajax for caregiver */

  /**
   * Form element as ajax callback return for others
   */
  $others_pair = publish_obituary_family_fields($node, 'field_obituary_other', 'field_other_title', 'field_other_name');
  $form['family_friends']['more_others'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="more-others">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['othnum_names'])) {
    $form_state['othnum_names'] = count($others_pair);
  }
	
  for ($i = 0; $i < $form_state['othnum_names']; $i++) {
    $form['family_friends']['more_others'][$i]['other_title'] = array(
      '#type' => 'textfield',
      '#title' => $i == 0 ? 'Others' : '',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Title (e.g. Dog)',
      ),
      '#prefix' => '<div class="more-family-wrapper">',
      '#default_value' => $i < count($others_pair) ? $others_pair[$i]['main'] : NULL,
    );
    $form['family_friends']['more_others'][$i]['other_name'] = array(
      '#type' => 'textfield',
      '#attributes' => array(
        'multiple' => 'multiple',
        //'placeholder' => 'Name (e.g. Buddy)',
      ),
      '#suffix' => '</div>',
      '#default_value' => $i < count($others_pair) ? $others_pair[$i]['pair'] : NULL,
    );
  }
  
  $form['family_friends']['more_others']['add_other'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#name' => 'add_more_othere',
    '#submit' => array('otherse_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'otherse_add_more_callback',
      'wrapper' => 'more-others',
    ),
  );

  if ($no_js_use) {
    if (!empty($form['more_others']['remove_other']['#ajax'])) {
       unset($form['more_others']['remove_other']['#ajax']);
    }
    unset($form['more_others']['add_other']['#ajax']);
  }
  /** End add more ajax for others*/
  
  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
  );

  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Add more button submit and ajax callback for night service
 */
function nightservi_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['nsnum_names'])) {
    $form_state['nsnum_names'] = 0;
    $form_state['nsnum_names']++;
  }

  $form_state['nsnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function nightservi_add_more_callback($form, $form_state) {
  return $form['funeral_details']['more_nightserv'];
}

/**
 * Add more button submit and ajax callback for father
 */
function fatherse_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['fnum_names'])) {
      $form_state['fnum_names'] = 0;
      $form_state['fnum_names']++;
  }

  $form_state['fnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function fatherse_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_fathers'];
}
/**
 * Add more button submit and ajax callback for son
 */
function sonse_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['pnum_names'])) {
      $form_state['pnum_names'] = 0;
      $form_state['pnum_names']++;
  }

  $form_state['pnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function sonse_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_sons'];
}
/**
 * Add more button submit and ajax callback for daughter
 */
function daughterse_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['dnum_names'])) {
      $form_state['dnum_names'] = 0;
      $form_state['dnum_names']++;
  }

  $form_state['dnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function daughterse_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_daughters'];
}
/**
 * Add more button submit and ajax callback for grandson
 */
function grandsonse_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['gsnum_names'])) {
      $form_state['gsnum_names'] = 0;
      $form_state['gsnum_names']++;
  }

  $form_state['gsnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function grandsonse_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_grandsons'];
}
/**
 * Add more button submit and ajax callback for granddaughter
 */
function granddaughterse_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['gdnum_names'])) {
      $form_state['gdnum_names'] = 0;
      $form_state['gdnum_names']++;
  }

  $form_state['gdnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function granddaughterse_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_granddaughters'];
}
/**
 * Add more button submit and ajax callback for great grandson
 */
function greatgrandsonse_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['ggsnum_names'])) {
      $form_state['ggsnum_names'] = 0;
      $form_state['ggsnum_names']++;
  }

  $form_state['ggsnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function greatgrandsonse_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_greatgrandsons'];
}
/**
 * Add more button submit and ajax callback for brother
 */
function brotherse_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['btnum_names'])) {
      $form_state['btnum_names'] = 0;
      $form_state['btnum_names']++;
  }

  $form_state['btnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function brotherse_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_brothers'];
}
/**
 * Add more button submit and ajax callback for sister
 */
function sisterse_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['stnum_names'])) {
      $form_state['stnum_names'] = 0;
      $form_state['stnum_names']++;
  }

  $form_state['stnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function sisterse_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_sisters'];
}
/**
 * Add more button submit and ajax callback for father-in-law
 */
function fatherlawse_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['flnum_names'])) {
      $form_state['flnum_names'] = 0;
      $form_state['flnum_names']++;
  }

  $form_state['flnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function fatherlawse_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_fatherlaws'];
}
/**
 * Add more button submit and ajax callback for brothers in law
 */
function brotherslawe_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['btlnum_names'])) {
      $form_state['btlnum_names'] = 0;
      $form_state['btlnum_names']++;
  }

  $form_state['btlnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function brotherslawe_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_brotherslaw'];
}
/**
 * Add more button submit and ajax callback for sisters in law
 */
function sisterslawe_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['stlnum_names'])) {
      $form_state['stlnum_names'] = 0;
      $form_state['stlnum_names']++;
  }

  $form_state['stlnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function sisterslawe_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_sisterslaw'];
}
/**
 * Add more button submit and ajax callback for nephew
 */
function nephewse_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['npnum_names'])) {
      $form_state['npnum_names'] = 0;
      $form_state['npnum_names']++;
  }

  $form_state['npnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function nephewse_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_nephews'];
}
/**
 * Add more button submit and ajax callback for niece
 */
function niecese_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['ncnum_names'])) {
      $form_state['ncnum_names'] = 0;
      $form_state['ncnum_names']++;
  }

  $form_state['ncnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function niecese_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_nieces'];
}
/**
 * Add more button submit and ajax callback for grandnephew
 */
function grandnephewse_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['gnpnum_names'])) {
      $form_state['gnpnum_names'] = 0;
      $form_state['gnpnum_names']++;
  }

  $form_state['gnpnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function grandnephewse_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_grandnephews'];
}
/**
 * Add more button submit and ajax callback for grandniece
 */
function grandniecese_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['gcpnum_names'])) {
      $form_state['gcpnum_names'] = 0;
      $form_state['gcpnum_names']++;
  }

  $form_state['gcpnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function grandniecese_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_grandnieces'];
}
/**
 * Add more button submit and ajax callback for great grandnephew
 */
function greatgrandnephewse_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['ggnpnum_names'])) {
      $form_state['ggnpnum_names'] = 0;
      $form_state['ggnpnum_names']++;
  }

  $form_state['ggnpnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function greatgrandnephewse_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_greatgrandnephews'];
}
/**
 * Add more button submit and ajax callback for uncle
 */
function unclese_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['unnum_names'])) {
      $form_state['unnum_names'] = 0;
      $form_state['unnum_names']++;
  }

  $form_state['unnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function unclese_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_uncles'];
}
/**
 * Add more button submit and ajax callback for auntie
 */
function auntiese_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['aunum_names'])) {
      $form_state['aunum_names'] = 0;
      $form_state['aunum_names']++;
  }

  $form_state['aunum_names']++;
  $form_state['rebuild'] = TRUE;
}

function auntiese_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_aunties'];
}
/**
 * Add more button submit and ajax callback for cousin
 */
function cousinse_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['csnum_names'])) {
      $form_state['csnum_names'] = 0;
      $form_state['csnum_names']++;
  }

  $form_state['csnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function cousinse_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_cousins'];
}
/**
 * Add more button submit and ajax callback for god son
 */
function godsonse_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['gosnum_names'])) {
      $form_state['gosnum_names'] = 0;
      $form_state['gosnum_names']++;
  }

  $form_state['gosnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function godsonse_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_godsons'];
}
/**
 * Add more button submit and ajax callback for god daughter
 */
function goddaughterse_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['godnum_names'])) {
      $form_state['godnum_names'] = 0;
      $form_state['godnum_names']++;
  }

  $form_state['godnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function goddaughterse_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_goddaughters'];
}
/**
 * Add more button submit and ajax callback for god grandson
 */
function godgrandsonse_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['gogsnum_names'])) {
      $form_state['gogsnum_names'] = 0;
      $form_state['gogsnum_names']++;
  }

  $form_state['gogsnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function godgrandsonse_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_godgrandsons'];
}
/**
 * Add more button submit and ajax callback for god granddaughter
 */
function godgranddaughterse_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['ggdnum_names'])) {
      $form_state['ggdnum_names'] = 0;
      $form_state['ggdnum_names']++;
  }

  $form_state['ggdnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function godgranddaughterse_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_godgranddaughters'];
}

/**
 * Add more button submit and ajax callback for god great grandson
 */
function godgreatgrandsonse_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['gggsnum_names'])) {
      $form_state['gggsnum_names'] = 0;
      $form_state['gggsnum_names']++;
  }

  $form_state['gggsnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function godgreatgrandsonse_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_godgreatgrandsons'];
}
/**
 * Add more button submit and ajax callback for caregiver
 */
function caregiverse_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['cgvnum_names'])) {
      $form_state['cgvnum_names'] = 0;
      $form_state['cgvnum_names']++;
  }

  $form_state['cgvnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function caregiverse_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_caregivers'];
}
/**
 * Add more button submit and ajax callback for other
 */
function otherse_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['othnum_names'])) {
      $form_state['othnum_names'] = 0;
      $form_state['othnum_names']++;
  }

  $form_state['othnum_names']++;
  $form_state['rebuild'] = TRUE;
}

function otherse_add_more_callback($form, $form_state) {
  return $form['family_friends']['more_others'];
}

/**
 * Obituary form setting
 */
function publish_obituary_setting_form($form, &$form_state) {
  $form['publication_details'] = array(
    '#type' => 'fieldset',
    '#title' => 'Publication Details',
    '#collapsible' => TRUE,
    //'#collapsed' => TRUE,
  );
  $form['publication_details']['public_media'] = array(
    '#title' => 'Media List',
    '#type' => 'textarea',
    '#description' => 'Enter one Media per line include Size Rate and Color Charge (in percent).<br/>Format: key|Media Name|size_rate|color_charge.<br/><span>Do not use zero (0) for the key!</span>',
    '#default_value' => variable_get('public_media'),
  );
  $form['publication_details']['public_gst'] = array(
    '#title' => 'GST Charge',
    '#type' => 'textfield',
    '#description' => 'GST charge in percent',
    '#default_value' => variable_get('public_gst'),
  );
  $form['publication_details']['public_common_sizes'] = array(
    '#title' => 'Common Sizes List',
    '#type' => 'textarea',
    '#description' => 'Enter one Size per line.<br/>Format: key|Size label.<br/><span>Make sure the key format is cm-col!</span>',
    '#default_value' => variable_get('public_common_sizes'),
  );
  $form['publication_details']['public_width'] = array(
    '#title' => 'Custom Width List',
    '#type' => 'textarea',
    '#description' => 'Enter one Width per line.<br/>Format: key|Width label.<br/><span>Make sure the key is width number!</span>',
    '#default_value' => variable_get('public_width'),
  );
  $form['publication_details']['public_columns'] = array(
    '#title' => 'Custom Column list',
    '#type' => 'textarea',
    '#description' => 'Enter one Column per line.<br/>Format: key|Column label.<br/><span>Make sure the key is column number!</span>',
    '#default_value' => variable_get('public_columns'),
  );

  $form['funeral_details'] = array(
    '#type' => 'fieldset',
    '#title' => 'Funeral Details',
    '#collapsible' => TRUE,
    //'#collapsed' => TRUE,
  );
  $form['funeral_details']['funeral_for_at'] = array(
    '#title' => 'Funeral For and At List',
    '#type' => 'textarea',
    '#description' => 'Enter one At per line include it For and the Religion.<br/>Format: religion|for|at.',
    '#default_value' => variable_get('funeral_for_at'),
  );
  $form['funeral_details']['funeral_bible'] = array(
    '#title' => 'Bible Verses List',
    '#type' => 'textarea',
    '#description' => 'Enter one Verse per line with key.<br/>Format: key|verse_word.<br/><span>Do not use zero (0) for the key!</span>',
    '#default_value' => variable_get('funeral_bible'),
  );

  $form['obituaries_thanks'] = array(
    '#title' => 'Thanks message',
    '#type' => 'textarea',
    '#default_value' => variable_get('thank_message'),
  );

  $form['next'] = array(
    '#type' => 'submit',
    '#value' => t('Submit')
  );

  return $form;
}

/**
 * Memoriam form setting
 */
function publish_memoriam_setting_form($form, &$form_state) {
  $form['memoriam_details'] = array(
    '#type' => 'fieldset',
    '#title' => 'In Memoriam Details',
    '#collapsible' => TRUE,
    //'#collapsed' => TRUE,
  );
  $form['memoriam_details']['memoriam_messages'] = array(
    '#title' => 'Messages List',
    '#type' => 'textarea',
    '#description' => 'Enter one Message per line the key.<br/>Format: key|message.<br/><span>Do not use zero (0) for the key!</span>',
    '#default_value' => variable_get('memoriam_messages'),
  );

  $form['next'] = array(
    '#type' => 'submit',
    '#value' => t('Submit')
  );

  return $form;
}


/**
 * Function to get media list 
 */
function publish_obituary_get_media_list() {
  $raw_media = publish_obituary_get_media_all();
  $media_list = array();
  foreach ($raw_media as $rm_key => $rm_value) {
    $media_list[$rm_key] = $rm_value['media_name'];
  }
  $media_list = array(''=>'- Select -') + $media_list;
  return $media_list;
}

/**
 * Function to get media val by key
 */
function publish_obituary_get_media_val($key) {
  $raw_media = publish_obituary_get_media_all();
  $media_val = array();
  foreach ($raw_media as $rm_key => $rm_value) {
    if ($rm_key == $key) {
      $media_val = $rm_value;
    }
  }
  return $media_val;
}

/**
 * Function to restructure the array and get all media value 
 */
function publish_obituary_get_media_all() {
  $raw_media = preg_split('/\r\n|[\r\n]/', variable_get('public_media'));
  $media_arr = array();
  foreach ($raw_media as $rm_key => $rm_value) {
    $raw_val = explode('|', $rm_value);
    $media_arr[$raw_val[0]] = array(
      'media_name' => $raw_val[1],
      'size_rates' => $raw_val[2],
      'color_rates' => $raw_val[3],
    );
  }
  return $media_arr;
}

/**
 * Function to get common sizes list 
 */
function publish_obituary_common_sizes_list() {
  $common_list = publish_obituary_common_sizes_all();
  $common_list = array(''=>'- Select -') + $common_list;
  return $common_list;
}

/**
 * Function to get common sizes by key
 */
function publish_obituary_common_sizes_label($common_size_val) {
  $raw_common = publish_obituary_common_sizes_all();
  $common_val = array();
  foreach ($raw_common as $rc_key => $rc_value) {
    if ($rc_key == $common_size_val) {
      $common_val = $rc_value;
    }
  }
  
  return $common_val;
}

/**
 * Function to restructure the array and get the common sizes value
 */
function publish_obituary_common_sizes_all() {
  $raw_common_sizes = preg_split('/\r\n|[\r\n]/', variable_get('public_common_sizes'));
  $common_arr = array();
  foreach ($raw_common_sizes as $rcl_key => $rcl_value) {
    $raw_common_val = explode('|', $rcl_value);
    $common_arr[$raw_common_val[0]] = $raw_common_val[1];
  }
  return $common_arr;
}

/**
 * Function to get custom size height list 
 */
function publish_obituary_custom_height_list() {
  $custom_height_list = publish_obituary_custom_height_all();
  $custom_height_list = array(''=>'- Select -') + $custom_height_list;

  return $custom_height_list;
}

/**
 * Function to restructure the array and get the height/widht sizes value
 */
function publish_obituary_custom_height_all() {
  $raw_custom_height = preg_split('/\r\n|[\r\n]/', variable_get('public_width'));
  $height_arr = array();
  foreach ($raw_custom_height as $rch_key => $rch_value) {
    $raw_height_val = explode('|', $rch_value);
    $height_arr[$raw_height_val[0]] = $raw_height_val[1];
  }
  return $height_arr;
}

/**
 * Function to get custom size column list 
 */
function publish_obituary_custom_column_list() {
  $custom_column_list = publish_obituary_custom_column_all();
  $custom_column_list = array(''=>'- Select -') + $custom_column_list;
  
  return $custom_column_list;
}

/**
 * Function to restructure the array and get the column sizes value
 */
function publish_obituary_custom_column_all() {
  $raw_custom_column = preg_split('/\r\n|[\r\n]/', variable_get('public_columns'));
  $column_arr = array();
  foreach ($raw_custom_column as $rcc_key => $rcc_value) {
    $raw_column_val = explode('|', $rcc_value);
    $column_arr[$raw_column_val[0]] = $raw_column_val[1];
  }
  return $column_arr;
}

/**
 * Function to get common sizes by key
 */
function publish_obituary_custom_sizes_label($custom_height_val, $custom_column_val) {
  $raw_height = publish_obituary_custom_height_all();
  $height_val = array();
  foreach ($raw_height as $rh_key => $rh_value) {
    if ($rh_key == $custom_height_val) {
      $height_val = $rh_value;
    }
  }
  
  $raw_column = publish_obituary_custom_column_all();
  $column_val = array();
  foreach ($raw_column as $rcl_key => $rcl_value) {
    if ($rcl_key == $custom_column_val) {
      $column_val = $rcl_value;
    }
  }

  return array('height' => $height_val, 'column' => $column_val);
}

/**
 * Function to get the choosed size to show
 */
function publish_obituary_public_size($custom_height, $custom_column, $common_size) {
  if (!empty($custom_height) || !empty($custom_column)) {
    $size_item = publish_obituary_custom_sizes_label($custom_height, $custom_column);
    $size = $size_item['height'] .' x '. $size_item['column'];
  } else {
    $size = publish_obituary_common_sizes_label($common_size);
  }
  return $size;
}

/**
 * Function to get Funeral For by religion
 */
function publish_obituary_funeral_for($religion) {
  $raw_for = publish_obituary_funeral_for_at($religion);
  $rel_for_list = array();
  foreach ($raw_for as $rf_key => $rf_value) {
    $rel_for_list[$rf_value['for']] = $rf_value['for'];
  }
  return $rel_for_list;
}

/**
 * Function to get Funeral For by religion for user edit form
 */
function publish_obituary_funeral_for_user($religion) {
  $raw_for = publish_obituary_funeral_for_at($religion);
  $rel_for_list = array();
  foreach ($raw_for as $rf_key => $rf_value) {
    $rel_for_list[strtolower($rf_value['for'])] = $rf_value['for'];
  }
  return $rel_for_list;
}

/**
 * Function to get Funeral At by religion
 */
function publish_obituary_funeral_at($religion, $f_for) {
  $raw_at = publish_obituary_funeral_for_at($religion);
  $rel_at_list = array();
  foreach ($raw_at as $ra_key => $ra_value) {
    if ($ra_value['for']==$f_for) {
      $rel_at_list[$ra_value['at']] = $ra_value['at'];
    }
  }
  return $rel_at_list;
}

/**
 * Function to get Funeral For and At by religion
 */
function publish_obituary_funeral_for_at($religion) {
  $raw_for_at = preg_split('/\r\n|[\r\n]/', variable_get('funeral_for_at'));
  $rel_for_at = array();
  foreach ($raw_for_at as $fa_key => $fa_value) {
    $for_at_val = explode('|', $fa_value);
    if ($for_at_val[0] == $religion) {
      $rel_for_at[] = array(
        'for' => $for_at_val[1],
        'at' => $for_at_val[2],
      );
    }
  }
  return $rel_for_at;
}

/**
 * function to get bible word by key
 */
function publish_obituary_get_bible_word($key) {
  $raw_bible = publish_obituary_funeral_bible();
  $bible_val = array();
  foreach ($raw_bible as $rb_key => $rb_value) {
    if ($rb_key == $key) {
      $bible_val = $rb_value;
    }
  }

  return $bible_val;
}

/**
 * Function to get bible verses list
 */
function publish_obituary_funeral_bible() {
  $raw_bible = preg_split('/\r\n|[\r\n]/', variable_get('funeral_bible'));
  $bible_list = array();
  foreach ($raw_bible as $rb_key => $rb_value) {
    $bible_val = explode('|', $rb_value);
    $bible_list[$bible_val[0]]=$bible_val[1];
  }
  return $bible_list;
}

/**
 * function to get memoriam message word by key
 */
function publish_obituary_get_memo_message_word($key) {
  $raw_memo_message = publish_obituary_memo_message_list() ;
  $memo_message_val = array();
  foreach ($raw_memo_message as $rmm_key => $rmm_value) {
    if ($rmm_key == $key) {
      $memo_message_val = $rmm_value;
    }
  }

  return $memo_message_val;
}

/**
 * Function to get all memoriam message
 */
function publish_obituary_memo_message_list() {
  $raw_message = preg_split('/\r\n|[\r\n]/', trim(variable_get('memoriam_messages')));
  $message_all = array();
  foreach ($raw_message as $rm_key => $rm_value) {
    $message_val = explode('|', $rm_value);
    $message_all[$message_val[0]]=$message_val[1];
  }
  return $message_all;
}

/**
 * Function to get family value from previous step for review
 */
function publish_obituary_get_descent($family_friends, $desc, $first, $second) {
  $main = $pair = '';
  foreach ($family_friends[$desc] as $key => $value) {
    if (is_array($value)) {
      if (!empty($value[$first])) $main .= '<div>' . $value[$first] . '</div>';
      if (!empty($second) && !empty($value[$second])) $pair .= '<div>' . $value[$second] . '</div>';
    }
  }
  return array('main'=>$main, 'pair'=>$pair);
}

/**
 * Function to get family value from obituary entity for edit
 */
function publish_obituary_family_fields($node, $parent_field, $main_field, $pair_field) {
  $fams_field = field_get_items('node', $node, $parent_field);
  $fams_item = array();
  foreach ($fams_field as $sk => $sv) {
    $fams_obj = field_collection_item_load($sv);
    if (!empty($pair_field)) {
      $fams_item[] = array(
        'main' => field_get_items('field_collection_item', $fams_obj, $main_field)[0]['value'],
        'pair' => field_get_items('field_collection_item', $fams_obj, $pair_field)[0]['value'],
      );
    } else {
      $fams_item[] = array(
        'main' => field_get_items('field_collection_item', $fams_obj, $main_field)[0]['value'],
      );
    }
  }
  return $fams_item;
}