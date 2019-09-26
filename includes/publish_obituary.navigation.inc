<?php 

/**
 * @file
 * Handles the navigation of the publish obituary form
 * All hooks are in the .module file.
 */


/**
 * Generate a header which informs the user of which stage they're on.
 * 
 * @param type $form
 * @param type $form_state
 * @return type 
 */
function publish_obituary_get_header($form, &$form_state) {
  
  $adv_choosed = array_key_exists('multistep_values', $form_state) ? $form_state['multistep_values']['kind_of_advertisement']['adv_choosed'] : 2;
  $form_state['stage'] = isset($form_state['stage']) ? $form_state['stage'] : 1;  
  switch ($form_state['kof']) {
    case 'death-announcement':
      $form_stages1 = array(
        'kind_of_advertisement' => 1,
        'publication_details' => 2,
        'advertiser_details' => 3,
        'deceased_details' => 4,
        'family_friends' => 5,
        'funeral_details' => 6,
        'certificate_photo' => 7,
        'adv_review' => 8,
      ); 
      $form_stages2 = array(
        'kind_of_advertisement' => 1,
        'advertiser_details' => 2,
        'deceased_details' => 3,
        'family_friends' => 4,
        'funeral_details' => 5,
        'certificate_photo' => 6,
        'adv_review' => 7,
      ); 

      $stages1 = array(
        1 => array('data' => '<span>1</span><span>Advertisement Type</span>'),
        2 => array('data' => '<span>2</span><span>Publication Details</span>'),
        3 => array('data' => '<span>3</span><span>Advertiser Details</span>'),
        4 => array('data' => '<span>4</span><span>Deceased Details</span>'),
        5 => array('data' => '<span>5</span><span>Family and Friends</span>'),
        6 => array('data' => '<span>6</span><span>Funeral Details</span>'),
        7 => array('data' => '<span>7</span><span>Upload Forms and Photo</span>'),
        8 => array('data' => '<span>8</span><span>Review</span>'),
      );
      
      $stages2 = array(
        1 => array('data' => '<span>1</span><span>Advertisement Type</span>'),
        2 => array('data' => '<span>2</span><span>Advertiser Details</span>'),
        3 => array('data' => '<span>3</span><span>Deceased Details</span>'),
        4 => array('data' => '<span>4</span><span>Family and Friends</span>'),
        5 => array('data' => '<span>5</span><span>Funeral Details</span>'),
        6 => array('data' => '<span>6</span><span>Upload Forms and Photo</span>'),
        7 => array('data' => '<span>7</span><span>Review</span>'),
      );
      break;

    case 'acknowledgement':
      $form_stages1 = array(
        'kind_of_advertisement' => 1,
        'publication_details' => 2,
        'advertiser_details' => 3,
        'deceased_details' => 4,
        'acknowledgement_details' => 5,
        //'certificate_photo' => 6,
        'adv_review' => 6,
      ); 
      $form_stages2 = array(
        'kind_of_advertisement' => 1,
        'advertiser_details' => 2,
        'deceased_details' => 3,
        'acknowledgement_details' => 4,
        //'certificate_photo' => 5,
        'adv_review' => 5,
      );

      $stages1 = array(
        1 => array('data' => '<span>1</span><span>Advertisement Type</span>'),
        2 => array('data' => '<span>2</span><span>Publication Details</span>'),
        3 => array('data' => '<span>3</span><span>Advertiser Details</span>'),
        4 => array('data' => '<span>4</span><span>Deceased Details</span>'),
        5 => array('data' => '<span>5</span><span>Acknowledgement Details</span>'),
        //6 => array('data' => '<span>6</span><span>Upload Forms and Photo</span>'),
        6 => array('data' => '<span>6</span><span>Review</span>'),
      );
      
      $stages2 = array(
        1 => array('data' => '<span>1</span><span>Advertisement Type</span>'),
        2 => array('data' => '<span>2</span><span>Advertiser Details</span>'),
        3 => array('data' => '<span>3</span><span>Deceased Details</span>'),
        4 => array('data' => '<span>4</span><span>Acknowledgement Details</span>'),
        //5 => array('data' => '<span>5</span><span>Upload Forms and Photo</span>'),
        5 => array('data' => '<span>5</span><span>Review</span>'),
      );
      break;
    
    case 'condolence':
      $form_stages1 = array(
        'kind_of_advertisement' => 1,
        'publication_details' => 2,
        'advertiser_details' => 3,
        'deceased_details' => 4,
        'condolence_details' => 5,
        'certificate_photo' => 6,
        'adv_review' => 7,
      ); 
      $form_stages2 = array(
        'kind_of_advertisement' => 1,
        'advertiser_details' => 2,
        'deceased_details' => 3,
        'condolence_details' => 4,
        'certificate_photo' => 5,
        'adv_review' => 6,
      );

      $stages1 = array(
        1 => array('data' => '<span>1</span><span>Advertisement Type</span>'),
        2 => array('data' => '<span>2</span><span>Publication Details</span>'),
        3 => array('data' => '<span>3</span><span>Advertiser Details</span>'),
        4 => array('data' => '<span>4</span><span>Deceased Details</span>'),
        5 => array('data' => '<span>5</span><span>Condolence Details</span>'),
        6 => array('data' => '<span>6</span><span>Upload Forms and Photo</span>'),
        7 => array('data' => '<span>7</span><span>Review</span>'),
      );
      
      $stages2 = array(
        1 => array('data' => '<span>1</span><span>Advertisement Type</span>'),
        2 => array('data' => '<span>2</span><span>Advertiser Details</span>'),
        3 => array('data' => '<span>3</span><span>Deceased Details</span>'),
        4 => array('data' => '<span>4</span><span>Condolence Details</span>'),
        5 => array('data' => '<span>5</span><span>Upload Forms and Photo</span>'),
        6 => array('data' => '<span>6</span><span>Review</span>'),
      );
      break;

    case 'in-memoriam':
      $form_stages1 = array(
        'kind_of_advertisement' => 1,
        'publication_details' => 2,
        'advertiser_details' => 3,
        'deceased_details' => 4,
        'memoriam_details' => 5,
        'certificate_photo' => 6,
        'adv_review' => 7,
      ); 
      $form_stages2 = array(
        'kind_of_advertisement' => 1,
        'advertiser_details' => 2,
        'deceased_details' => 3,
        'memoriam_details' => 4,
        'certificate_photo' => 5,
        'adv_review' => 6,
      );

      $stages1 = array(
        1 => array('data' => '<span>1</span><span>Advertisement Type</span>'),
        2 => array('data' => '<span>2</span><span>Publication Details</span>'),
        3 => array('data' => '<span>3</span><span>Advertiser Details</span>'),
        4 => array('data' => '<span>4</span><span>Deceased Details</span>'),
        5 => array('data' => '<span>5</span><span>In Memoriam Details</span>'),
        6 => array('data' => '<span>6</span><span>Upload Forms and Photo</span>'),
        7 => array('data' => '<span>7</span><span>Review</span>'),
      );
      
      $stages2 = array(
        1 => array('data' => '<span>1</span><span>Advertisement Type</span>'),
        2 => array('data' => '<span>2</span><span>Advertiser Details</span>'),
        3 => array('data' => '<span>3</span><span>Deceased Details</span>'),
        4 => array('data' => '<span>4</span><span>In Memoriam Details</span>'),
        5 => array('data' => '<span>5</span><span>Upload Forms and Photo</span>'),
        6 => array('data' => '<span>6</span><span>Review</span>'),
      );
      break;
  }
  
  $form_stages = $adv_choosed == 1 ? $form_stages1 : $form_stages2;
  if (isset($form_stages[$form_state['stage']])) {
   $current_step = $form_stages[$form_state['stage']]; 
  }
  else {
   $current_step = 1;
  }
  
  $stages = $adv_choosed == 1 ? $stages1 : $stages2;
  
  $form_state['active_stage'] = $current_step;
  if (isset($adv_choosed)) {
    $form_state['count_stage'] = count($stages);
  } else {
    $form_state['count_stage'] = $form_state['kof'] == 'death-announcement' ? 7 : 6;
  }

  for ($i=1; $i<=$current_step; $i++) { 
    $stages[$i]['class'] = array('active');
  }
  //$stages[$current_step]['class'] = array('active');
  
  $stages_list = theme('item_list', array('items' => $stages));
  
  $form['header'] = array(
    '#type' => 'fieldset',
    '#title' => '',
    '#value' => $stages_list,
  );

  return $form;
}


/**
 * Given the current stage the user is on, calculate what the next step would be
 * 
 * @param type $form_state
 * @return string 
 */
function publish_obituary_move_to_next_stage($form, &$form_state) {
  $adv_choosed = $form_state['multistep_values']['kind_of_advertisement']['adv_choosed'];
  switch ($form_state['stage']) {
    case 'kind_of_advertisement':
      $f_stage = $adv_choosed == 1 ? 'publication_details' : 'advertiser_details';
      return $f_stage;
      break;

    case 'publication_details':
      return 'advertiser_details';
      break;

    case 'advertiser_details':
      if (array_key_exists('publication_details', $form_state['multistep_values']) && $form_state['multistep_values']['publication_details']['pub_choosed'] == 1) {
        $p_stage = 'adv_review';
      } else {
        $p_stage = 'deceased_details';
      }
      //$p_stage = $form_state['multistep_values']['publication_details']['pub_choosed'] == 1 ? 'adv_review' : 'deceased_details';
      return $p_stage;
      break;
     
    case 'deceased_details':
      if ($form_state['kof'] == 'acknowledgement') {
        $f_stage = 'acknowledgement_details';
      } elseif ($form_state['kof'] == 'condolence') {
        $f_stage = 'condolence_details';
      } elseif ($form_state['kof'] == 'in-memoriam') {
        $f_stage = 'memoriam_details';
      } else {
        $f_stage = 'family_friends';
      }
      return $f_stage;
      break;

    case 'family_friends':
      return 'funeral_details';
      break;

    case 'funeral_details':
    //case 'acknowledgement_details':
    case 'condolence_details':
    case 'memoriam_details':
      return 'certificate_photo';
      break;
    
    case 'acknowledgement_details':
    case 'certificate_photo':
      return 'adv_review';
      break;
  }

}


/**
 * Given the current stage the user is on, calculate what the previous step
 * would be
 * 
 * @param type $form_state
 * @return string 
 */

function publish_obituary_move_to_previous_stage($form, &$form_state) {
  $adv_choosed = $form_state['multistep_values']['kind_of_advertisement']['adv_choosed'];

  switch ($form_state['stage']) {
    case 'advertiser_details':
      $f_stage = $adv_choosed == 1 ? 'publication_details' : 'kind_of_advertisement';
      return $f_stage;
      break;

    case 'publication_details':
      return 'kind_of_advertisement';
      break;

    case 'deceased_details':
      return 'advertiser_details';
      break;

    case 'family_friends':
    case 'acknowledgement_details':
    case 'condolence_details':
    case 'memoriam_details';
      return 'deceased_details';
      break;

    case 'funeral_details':
      return 'family_friends';
      break;

    case 'certificate_photo':
      if ($form_state['kof'] == 'acknowledgement') { //Actually it not used, cause acknowledgement no certificate_photo step
        $f_stage = 'acknowledgement_details';
      } elseif ($form_state['kof'] == 'condolence') {
        $f_stage = 'condolence_details';
      } elseif ($form_state['kof'] == 'in-memoriam') {
        $f_stage = 'memoriam_details';
      } else {
        $f_stage = 'funeral_details';
      }
      return $f_stage;
      break;

    case 'adv_review':
      if (array_key_exists('publication_details', $form_state['multistep_values']) && $form_state['multistep_values']['publication_details']['pub_choosed'] == 1) {
        $p_stage = 'advertiser_details';
      } else {
        if ($form_state['kof'] == 'acknowledgement') {
          $p_stage = 'acknowledgement_details';
        } else {
          $p_stage = 'certificate_photo';
        }
      }
      return $p_stage;
      break;
  }
    
}