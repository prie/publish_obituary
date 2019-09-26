<?php
/**
 * Page callback for obituaries order list in admin page
 */
function publish_obituary_orders() {
  drupal_set_title('Obituaries Orders');

  return drupal_get_form('publish_obituary_orders_filter_form');
}

/**
 * Form called by obituary order list page
 */
function publish_obituary_orders_filter_form($form, &$form_state) {
  $form['filter_wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => 'Orders filter',
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['filter_wrapper']['filter_form'] = array(
    '#type' => 'select',
    '#options' => array(
      '' => '- Choose Service -',
      '1' => 'Death Announcement',
      '2' => 'Acknowledgement',
      '3' => 'Condolence',
      '4' => 'In Memoriam',
    ),
    '#attributes' => array('style' => array('height:30px;margin-right:7px;')),
    '#default_value' => isset($form_state['stored']['f_type']) ? $form_state['stored']['f_type'] : NULL,
  );
  $form['filter_wrapper']['button'] = array(
    '#type' => 'submit',
    '#value' => 'Apply',
  );

  $orders_per_page = 20;

  $header[] = array('data' => t('Deceased'), 'field' => 'oo.dec_name');
  $header[] = array('data' => t('Advert Type'), 'field' => 'oo.kind');
  $header[] = array('data' => t('Service'), 'field' => 'oo.form_type');
  $header[] = array('data' => t('Author'), 'field' => 'oo.uid');
  $header[] = array('data' => t('Created'), 'field' => 'oo.created', 'sort' => 'asc');
  $header[] = array('data' => t('Approved'), 'field' => 'oo.status');
  $header[] = array('data' => t('Operation'));
 
  $select = db_select('online_obituaries_orders', 'oo')->extend('PagerDefault')->extend('TableSort');
  $select->fields('oo', array('oid', 'uid', 'kind', 'form_type', 'deceased', 'status', 'created'));
  $select->condition('kind', 1, '=');
  if (isset($form_state['stored']['f_type']) && !empty($form_state['stored']['f_type'])) {
    $select->condition('form_type', $form_state['stored']['f_type'], '=');
  }
  $select->orderBy('created', 'DESC');
  $select->limit($orders_per_page);
  $select->orderByHeader($header);
  $queried_orders = $select->execute();
   
  $rows = array();
  foreach ($queried_orders as $order) {
    $deceased_names = unserialize($order->deceased)['deceased_name'];
    $f_service = get_service_name($order->form_type);
    if ($order->form_type==2 && $order->kind==2) {
      $obi_node = node_load(trim($deceased_names));
      $deceased_name = $obi_node->title .' - aknowledgement';
    } else {
      $deceased_name = $deceased_names;
    }
    
    $rows[] = array(
      check_plain($deceased_name),
      check_plain($order->kind == 1 ? 'Print' : 'Online'),
      check_plain($f_service),
      theme('username', array('account' => $order)),
      format_date($order->created, 'custom', 'd M Y - H:i'),
      $order->status == 0 ? 'No' : 'Yes',
      l('View details', 'obituaries-order/' . $order->oid . '/approve', array('attributes' => array('style' => 'background:#148aa5;color:#FFF;padding:3px 5px;border-radius:3px;font-size:12px;text-decoration:none;'))).' '.l('Delete', 'obituaries-order/' . $order->oid . '/delete', array('attributes' => array('style' => 'background:#DF3A3A;color:#FFF;padding:3px 5px;border-radius:3px;font-size:12px;text-decoration:none;'))),   
    );
      
  }
 
  $form['obituaries_orders_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No order available.'),
  );
  $form['obituaries_orders_pager'] = array('#theme' => 'pager');

  return $form;
}

/**
 * Form called by obituary order list page
 */
function publish_obituary_orders_filter_form_submit($form, &$form_state) {
  $form_state['stored']['f_type'] = $form_state['values']['filter_form'];
  $form_state['rebuild'] = TRUE; 
}

/**
 * Page callback for obituary order approve page
 */
function publish_obituary_order_approve($oid) {
  drupal_add_css(drupal_get_path('module', 'publish_obituary') .'/css/obituaries.css', 'module');

  $oborder = obituaries_order_load($oid);
  
  return drupal_get_form('publish_obituary_approve_form', $oborder);
}

/**
 * Form called by  approve page
 */
function publish_obituary_approve_form($form, &$form_state, $oborder) {
  require_once DRUPAL_ROOT . '/includes/locale.inc';
  
  $deceased = unserialize($oborder->deceased);
  if ($oborder->form_type==2 && $oborder->kind==2) {
    //$obi_node = node_load(trim($deceased['deceased_name']));
    $approve_title = $deceased['deceased_name'] .' - '. get_service_name($oborder->form_type);
  } else {
    $dec_name = empty($deceased['deceased_name']) ? 'Schedule an onsite visit' : $deceased['deceased_name'];
    $approve_title = $dec_name;
  }
  
  $form['pg_title'] = array(
    '#markup' => '<h3><em>Approve obituary</em> ' . $approve_title . '?</h3>',
  );

  $form['oid'] = array(
    '#type' => 'hidden',
    '#value' => $oborder->oid,
  );

  $form['form_type'] = array(
    '#type' => 'hidden',
    '#value' => $oborder->form_type,
  );

  $form['kind'] = array(
    '#type' => 'hidden',
    '#value' => $oborder->kind,
  );

  $form['status'] = array(
    '#type' => 'hidden',
    '#value' => $oborder->status,
  );
  
  $form['kind_of_advertisement'] = array(
    'koa_title' => array(
      '#markup' => '<h3>Advertisement Type</h3>',
      '#prefix' => '<div class="kind-of-advertisement">',
    ),
    'koa_value' => array(
      '#markup' => $oborder->kind == 1 ? '<div class="adv-choosed-print">Print</div>' : '<div class="adv-choosed-online">Online</div>',
      '#suffix' => '</div>',
    ),
  );

  if ($oborder->kind == 1) {
    $publication = unserialize($oborder->publication);
    $form['publication_details']['advertiser_title'] = array(
      '#markup' => '<h3>Publication Details</h3>',
      '#prefix' => '<div class="publication-details">',
      '#suffix' => '<div class="publication-value">',
    );

    if ($publication['pub_choosed'] == 1) {
      $form['publication_details']['kind_public'] = array(
        '#markup' => $publication['pub_choosed'] == 1 ? 'Schedule an onsite visit' : 'Create your own',
        '#prefix' => '<div class="pub-kind"><div class="label-item">Publication Choosed</div>',
        '#suffix' => '</div>',
      );
    } else {
      $form['publication_details']['pub_media'] = array(
        '#markup' => publish_obituary_get_media_val($publication['media'])['media_name'],
        '#prefix' => '<div class="pub-media"><div class="label-item">Media</div>',
        '#suffix' => '</div>',
      );
      $form['publication_details']['pub_date'] = array(
        '#markup' => format_date(strtotime($publication['pub_date']), 'custom', 'd F Y'),
        '#prefix' => '<div class="pub-date"><div class="label-item">Publication Date</div>',
        '#suffix' => '</div>',
      );
      $form['publication_details']['pub_style'] = array(
        '#markup' => $publication['style'] == 1 ? 'Full Color' : 'Black and White',
        '#prefix' => '<div class="pub-style"><div class="label-item">Style</div>',
        '#suffix' => '</div>',
      );
      $form['publication_details']['pub_size'] = array(
        '#markup' => publish_obituary_public_size($publication['height'], $publication['column'], $publication['common_sizes']),
        '#prefix' => '<div class="size-price-wrapper"><div class="pub-size"><div class="label-item">Size</div>',
        '#suffix' => '</div>',
      );
      $form['publication_details']['pub_price'] = array(
        '#markup' => '$ ' . $publication['price'],
        '#prefix' => '<div class="pub-price">',
        '#suffix' => '</div></div>',
      );
      $form['publication_details']['pub_quote'] = array(
        '#markup' => $publication['quote_me'] == 0 ? 'No' : 'Yes',
        '#prefix' => '<div class="pub-quote"><div class="label-item">Request for Quote</div>',
        '#suffix' => '</div>',
      );
    }

    $form['publication_details']['close_tag'] = array(
      '#markup' => '',
      '#suffix' => '</div></div>',
    );
  }
  
  $advertiser = unserialize($oborder->advertiser);
  $countries = country_get_list();
  $country_name = $countries[$advertiser['country']];
  $form['advertiser_details'] = array(
    'advertiser_title' => array(
      '#markup' => '<h3>Advertiser Details</h3>',
      '#prefix' => '<div class="advertiser-details">',
    ),
    'advertiser_name' => array(
      '#markup' => $advertiser['name'],
      '#prefix' => '<div class="advertiser-value"><div class="adv-name"><div class="label-item">Advertiser Name</div>',
      '#suffix' => '</div>',
    ),
    'adv_contact' => array(
      '#markup' => $advertiser['contact_number'],
      '#prefix' => '<div class="adv-contact"><div class="label-item">Contact Number</div>',
      '#suffix' => '</div>',
    ),
    'adv_email' => array(
      '#markup' => $advertiser['email'],
      '#prefix' => '<div class="adv-email"><div class="label-item">Email Address</div>',
      '#suffix' => '</div>',
    ),
    'bill_address' => array(
      '#markup' => $advertiser['company_name'] . ' ' . $advertiser['street_address_1'] . ' ' . $country_name . ' ' . $advertiser['zip'],
      '#prefix' => '<div class="adv-bill-address"><div class="label-item">Billing Address</div>',
      '#suffix' => '</div>',
    ),
    'adv_relation' => array(
      '#markup' => $advertiser['relationship'],
      '#prefix' => '<div class="adv-relation"><div class="label-item">Relationship to Deceased</div>',
      '#suffix' => '</div></div></div>',
    ),
  );

  $publication_onsite = isset($publication) && $publication['pub_choosed']==1 ? TRUE : FALSE;
  
  $form['deceased_details']['decease_title'] = array(
    '#markup' => '<h3>Deceased Details</h3>',
    '#prefix' => '<div class="deceased-details">',
    '#suffix' => '<div class="deceased-value">',
  );

  if (!$publication_onsite) {
    $form['deceased_details']['decease_name'] = array(
      '#markup' => $deceased['deceased_name'],
      '#prefix' => '<div class="decease-name"><div class="label-item">Deceased Name</div>',
      '#suffix' => '</div>',
    );
    $form['deceased_details']['decease_date'] = array(
      '#markup' => format_date(strtotime($deceased['departed_date']), 'custom', 'd F Y'),
      '#prefix' => '<div class="decease_depart_date"><div class="label-item">Departed Date</div>',
      '#suffix' => '</div>',
    );
    if ($oborder->form_type != 2) {
      $form['deceased_details']['decease_age'] = array(
        '#markup' => $deceased['age'],
        '#prefix' => '<div class="decease_age"><div class="label-item">Age</div>',
        '#suffix' => '</div>',
      );
    }
    $form['deceased_details']['decease_religion'] = array(
      '#markup' => $deceased['religion'],
      '#prefix' => '<div class="decease_religion"><div class="label-item">Religion</div>',
      '#suffix' => '</div>',
    );
  }
  
  $form['deceased_details']['close_tag'] = array(
    '#markup' => '',
    '#suffix' => '</div></div>',
  );
  
  if ($oborder->form_type == 1) {
    $family_friends = unserialize($oborder->family);
    $form['family_friend'] = array(
      'family_title' => array(
        '#markup' => '<h3>Family and Friends</h3>',
        '#prefix' => '<div class="family-friends">',
        '#suffix' => '<div class="family-friend-value">',
      ),
    );

    if (!$publication_onsite) {
      if (!empty($family_friends['spouse_name'])) {
        $form['family_friend']['spouse_name'] = array(
          '#markup' => $family_friends['spouse_name'],
          '#prefix' => '<div class="spouse-name"><div class="label-item">' . $family_friends['spouse_as'] . '</div>',
          '#suffix' => '</div>',
        );
      }
      $fathers = publish_obituary_get_descent($family_friends, 'more_fathers', 'father_name', 'mother_name');
      if (!empty($fathers['main'])) {
        $form['family_friend']['father_name'] = array(
          '#markup' => $fathers['main'],
          '#prefix' => '<div class="fathers-name"><div class="label-item">Father</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($fathers['pair'])) {
        $form['family_friend']['mother_name'] = array(
          '#markup' => $fathers['pair'],
          '#prefix' => '<div class="mother-name"><div class="label-item">Mother</div>',
          '#suffix' => '</div>',
        );
      }
      $sons = publish_obituary_get_descent($family_friends, 'more_sons', 'son_name', 'daughter_law_name');
      if (!empty($sons['main'])) {
        $form['family_friend']['son_name'] = array(
          '#markup' => $sons['main'],
          '#prefix' => '<div class="sons-name"><div class="label-item">Sons</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($sons['pair'])) {
        $form['family_friend']['daughterinlaw_name'] = array(
          '#markup' => $sons['pair'],
          '#prefix' => '<div class="daughterinlaw-name"><div class="label-item">Daughters-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $daughters = publish_obituary_get_descent($family_friends, 'more_daughters', 'daughter_name', 'sons_law_name');
      if (!empty($daughters['main'])) {
        $form['family_friend']['daughter_name'] = array(
          '#markup' => $daughters['main'],
          '#prefix' => '<div class="daughters-name"><div class="label-item">Daughters</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($daughters['pair'])) {
        $form['family_friend']['soninlaw_name'] = array(
          '#markup' => $daughters['pair'],
          '#prefix' => '<div class="soninlaw-name"><div class="label-item">Sons-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $grandsons = publish_obituary_get_descent($family_friends, 'more_grandsons', 'grandson_name', 'granddaughter_law_name');
      if (!empty($grandsons['main'])) {
        $form['family_friend']['grandson_name'] = array(
          '#markup' => $grandsons['main'],
          '#prefix' => '<div class="grandsons-name"><div class="label-item">Grandsons</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($grandsons['pair'])) {
        $form['family_friend']['granddaughterinlaw_name'] = array(
          '#markup' => $grandsons['pair'],
          '#prefix' => '<div class="granddaughterinlaw-name"><div class="label-item">Granddaughters-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $granddaughters = publish_obituary_get_descent($family_friends, 'more_granddaughters', 'granddaughter_name', 'grandson_law_name');
      if (!empty($granddaughters['main'])) {
        $form['family_friend']['granddaughter_name'] = array(
          '#markup' => $granddaughters['main'],
          '#prefix' => '<div class="granddaughters-name"><div class="label-item">Granddaughters</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($granddaughters['pair'])) {
        $form['family_friend']['grandsoninlaw_name'] = array(
          '#markup' => $granddaughters['pair'],
          '#prefix' => '<div class="grandsoninlaw-name"><div class="label-item">Grandsons-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $greatgrandsons = publish_obituary_get_descent($family_friends, 'more_greatgrandsons', 'greatgrandson_name', 'greatgranddaughter'); 
      if (!empty($greatgrandsons['main'])) {
        $form['family_friend']['greatgrandson_name'] = array(
          '#markup' => $greatgrandsons['main'],
          '#prefix' => '<div class="greatgrandsons-name"><div class="label-item">Great Grandsons</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($greatgrandsons['pair'])) {
        $form['family_friend']['greatgranddaughter_name'] = array(
          '#markup' => $greatgrandsons['pair'],
          '#prefix' => '<div class="greatgranddaughter-name"><div class="label-item">Great Granddaughters</div>',
          '#suffix' => '</div>',
        );
      }
      $brothers = publish_obituary_get_descent($family_friends, 'more_brothers', 'brother_name', 'sister_law_name');
      if (!empty($brothers['main'])) {
        $form['family_friend']['brother_name'] = array(
          '#markup' => $brothers['main'],
          '#prefix' => '<div class="brothers-name"><div class="label-item">Brothers</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($brothers['pair'])) {
        $form['family_friend']['sisterinlaw_name'] = array(
          '#markup' => $brothers['pair'],
          '#prefix' => '<div class="sisterinlaws-name"><div class="label-item">Sisters-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $sisters = publish_obituary_get_descent($family_friends, 'more_sisters', 'sister_name', 'brother_law_name');
      if (!empty($sisters['main'])) {
        $form['family_friend']['sister_name'] = array(
          '#markup' => $sisters['main'],
          '#prefix' => '<div class="sisters-name"><div class="label-item">Sisters</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($sisters['pair'])) {
        $form['family_friend']['brotherinlaw_name'] = array(
          '#markup' => $sisters['pair'],
          '#prefix' => '<div class="brotherinlaws-name"><div class="label-item">Brothers-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $fatherslaw = publish_obituary_get_descent($family_friends, 'more_fatherlaws', 'fatherlaw_name', 'motherlaw_name');
      if (!empty($fatherslaw['main'])) {
        $form['family_friend']['fatherlaw_name'] = array(
          '#markup' => $fatherslaw['main'],
          '#prefix' => '<div class="fatherslaw-name"><div class="label-item">Father-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($fatherslaw['pair'])) {
        $form['family_friend']['motherlaw_name'] = array(
          '#markup' => $fatherslaw['pair'],
          '#prefix' => '<div class="motherlaw-name"><div class="label-item">Mother-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $brotherslaw = publish_obituary_get_descent($family_friends, 'more_brotherslaw', 'brotherlawmain_name', 'sisterlawpair_name');
      if (!empty($brotherslaw['main'])) {
        $form['family_friend']['brotherlawmain_name'] = array(
          '#markup' => $brotherslaw['main'],
          '#prefix' => '<div class="brotherslawmain-name"><div class="label-item">Brothers-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($brotherslaw['pair'])) {
        $form['family_friend']['sisterlawpair_name'] = array(
          '#markup' => $brotherslaw['pair'],
          '#prefix' => '<div class="sisterslawpair-name"><div class="label-item">Sisters-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $sisterslaw = publish_obituary_get_descent($family_friends, 'more_sisterslaw', 'sisterlawmain_name', 'brotherlawpair_name');
      if (!empty($sisterslaw['main'])) {
        $form['family_friend']['sisterlawmain_name'] = array(
          '#markup' => $sisterslaw['main'],
          '#prefix' => '<div class="sisterslawmain-name"><div class="label-item">Sisters-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($sisterslaw['pair'])) {
        $form['family_friend']['brotherlawpair_name'] = array(
          '#markup' => $sisterslaw['pair'],
          '#prefix' => '<div class="brotherslawpair-name"><div class="label-item">Brothers-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $nephews = publish_obituary_get_descent($family_friends, 'more_nephews', 'nephew_name', 'niece_law_name');
      if (!empty($nephews['main'])) {
        $form['family_friend']['nephew_name'] = array(
          '#markup' => $nephews['main'],
          '#prefix' => '<div class="nephews-name"><div class="label-item">Nephews</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($nephews['pair'])) {
        $form['family_friend']['nieceinlaw_name'] = array(
          '#markup' => $nephews['pair'],
          '#prefix' => '<div class="nieceinlaws-name"><div class="label-item">Nieces-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $nieces = publish_obituary_get_descent($family_friends, 'more_nieces', 'niece_name', 'nephew_law_name');
      if (!empty($nieces['main'])) {
        $form['family_friend']['niece_name'] = array(
          '#markup' => $nieces['main'],
          '#prefix' => '<div class="niece-name"><div class="label-item">Nieces</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($nieces['pair'])) {
        $form['family_friend']['nephewinlaw_name'] = array(
          '#markup' => $nieces['pair'],
          '#prefix' => '<div class="nephewinlaws-name"><div class="label-item">Nephews-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $grandnephews = publish_obituary_get_descent($family_friends, 'more_grandnephews', 'grandnephew_name', 'grandniece_name');
      if (!empty($grandnephews['main'])) {
        $form['family_friend']['grandnephew_name'] = array(
          '#markup' => $grandnephews['main'],
          '#prefix' => '<div class="grandnephews-name"><div class="label-item">Grandnephews</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($grandnephews['pair'])) {
        $form['family_friend']['grandniece_name'] = array(
          '#markup' => $grandnephews['pair'],
          '#prefix' => '<div class="grandnieces-name"><div class="label-item">Grandnieces-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $grandnieces = publish_obituary_get_descent($family_friends, 'more_grandnieces', 'grandniece_real_name', 'grandnephew_law_name');
      if (!empty($grandnieces['main'])) {
        $form['family_friend']['grandniece_real_name'] = array(
          '#markup' => $grandnieces['main'],
          '#prefix' => '<div class="grandniece-real-name"><div class="label-item">Grandnieces</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($grandnieces['pair'])) {
        $form['family_friend']['grandnephewinlaw_name'] = array(
          '#markup' => $grandnieces['pair'],
          '#prefix' => '<div class="grandnephewinlaws-name"><div class="label-item">Grandnephews-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $greatgrandnephews = publish_obituary_get_descent($family_friends, 'more_greatgrandnephews', 'greatgrandnephew_name', 'greatgrandniece_name');
      if (!empty($greatgrandnephews['main'])) {
        $form['family_friend']['greatgrandnephew_name'] = array(
          '#markup' => $greatgrandnephews['main'],
          '#prefix' => '<div class="greatgrandnephews-name"><div class="label-item">Great Grandnephews</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($greatgrandnephews['pair'])) {
        $form['family_friend']['greatgrandniece_name'] = array(
          '#markup' => $greatgrandnephews['pair'],
          '#prefix' => '<div class="greatgrandnieces-name"><div class="label-item">Great Grandnieces</div>',
          '#suffix' => '</div>',
        );
      }
      $uncles = publish_obituary_get_descent($family_friends, 'more_uncles', 'uncle_name', 'uncle_spouse_name');
      if (!empty($uncles['main'])) {
        $form['family_friend']['uncle_name'] = array(
          '#markup' => $uncles['main'],
          '#prefix' => '<div class="uncles-name"><div class="label-item">Uncles</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($uncles['pair'])) {
        $form['family_friend']['uncle_spouse_name'] = array(
          '#markup' => $uncles['pair'],
          '#prefix' => '<div class="uncle-spouse-name"><div class="label-item">Spouses</div>',
          '#suffix' => '</div>',
        );
      }
      $aunties = publish_obituary_get_descent($family_friends, 'more_aunties', 'auntie_name', 'auntie_spouse_name');
      if (!empty($aunties['main'])) {
        $form['family_friend']['auntie_name'] = array(
          '#markup' => $aunties['main'],
          '#prefix' => '<div class="aunties-name"><div class="label-item">Aunties</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($aunties['pair'])) {
        $form['family_friend']['auntie_spouse_name'] = array(
          '#markup' => $aunties['pair'],
          '#prefix' => '<div class="auntie-spouse-name"><div class="label-item">Spouses</div>',
          '#suffix' => '</div>',
        );
      }
      $cousins = publish_obituary_get_descent($family_friends, 'more_cousins', 'cousin_name', 'cousin_law_name');
      if (!empty($cousins['main'])) {
        $form['family_friend']['cousin_name'] = array(
          '#markup' => $cousins['main'],
          '#prefix' => '<div class="cousins-name"><div class="label-item">Cousins</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($cousins['pair'])) {
        $form['family_friend']['cousinlaw_name'] = array(
          '#markup' => $cousins['pair'],
          '#prefix' => '<div class="cousinlaw-name"><div class="label-item">Cousins-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $godsons = publish_obituary_get_descent($family_friends, 'more_godsons', 'godson_name', 'goddaughter_law_name');
      if (!empty($godsons['main'])) {
        $form['family_friend']['godson_name'] = array(
          '#markup' => $godsons['main'],
          '#prefix' => '<div class="godsons-name"><div class="label-item">God Sons</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($godsons['pair'])) {
        $form['family_friend']['goddaughterinlaw_name'] = array(
          '#markup' => $godsons['pair'],
          '#prefix' => '<div class="goddaughterinlaw-name"><div class="label-item">God Daughters-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $goddaughters = publish_obituary_get_descent($family_friends, 'more_goddaughters', 'goddaughter_name', 'godsons_law_name');
      if (!empty($goddaughters['main'])) {
        $form['family_friend']['goddaughter_name'] = array(
          '#markup' => $goddaughters['main'],
          '#prefix' => '<div class="goddaughters-name"><div class="label-item">God Daughters</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($goddaughters['pair'])) {
        $form['family_friend']['godsoninlaw_name'] = array(
          '#markup' => $goddaughters['pair'],
          '#prefix' => '<div class="godsoninlaw-name"><div class="label-item">God Sons-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $godgrandsons = publish_obituary_get_descent($family_friends, 'more_godgrandsons', 'godgrandson_name', 'godgranddaughter_law_name');
      if (!empty($godgrandsons['main'])) {
        $form['family_friend']['godgrandson_name'] = array(
          '#markup' => $godgrandsons['main'],
          '#prefix' => '<div class="godgrandsons-name"><div class="label-item">God Grandsons</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($godgrandsons['pair'])) {
        $form['family_friend']['godgranddaughterinlaw_name'] = array(
          '#markup' => $godgrandsons['pair'],
          '#prefix' => '<div class="godgranddaughterinlaw-name"><div class="label-item">God Granddaughters-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $godgranddaughters = publish_obituary_get_descent($family_friends, 'more_godgranddaughters', 'godgranddaughter_name', 'godgrandson_law_name');
      if (!empty($godgranddaughters['main'])) {
        $form['family_friend']['godgranddaughter_name'] = array(
          '#markup' => $godgranddaughters['main'],
          '#prefix' => '<div class="godgranddaughters-name"><div class="label-item">God Granddaughters</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($godgranddaughters['pair'])) {
        $form['family_friend']['godgrandsoninlaw_name'] = array(
          '#markup' => $godgranddaughters['pair'],
          '#prefix' => '<div class="godgrandsoninlaw-name"><div class="label-item">God Grandsons-in-law</div>',
          '#suffix' => '</div>',
        );
      }
      $godgreatgrandsons = publish_obituary_get_descent($family_friends, 'more_godgreatgrandsons', 'godgreatgrandson_name', 'godgreatgranddaughter_name');
      if (!empty($godgreatgrandsons['main'])) {
        $form['family_friend']['godgreatgrandson_name'] = array(
          '#markup' => $godgreatgrandsons['main'],
          '#prefix' => '<div class="godgreatgrandsons-name"><div class="label-item">God Great Grandsons</div>',
          '#suffix' => '</div>',
        );
      }
      if (!empty($godgreatgrandsons['pair'])) {
        $form['family_friend']['godgreatgranddaughter_name'] = array(
          '#markup' => $godgreatgrandsons['pair'],
          '#prefix' => '<div class="godgreatgranddaughter-name"><div class="label-item">God Great Granddaughters</div>',
          '#suffix' => '</div>',
        );
      }
      $caregivers = publish_obituary_get_descent($family_friends, 'more_caregivers', 'caregiver_name', '');
      if (!empty($caregivers['main'])) {
        $form['family_friend']['caregiver_name'] = array(
          '#markup' => $caregivers['main'],
          '#prefix' => '<div class="caregiver-name"><div class="label-item">Caregivers</div>',
          '#suffix' => '</div>',
        );
      }
      $others = publish_obituary_get_descent($family_friends, 'more_others', 'other_name', '');
      if (!empty($others['main'])) {
        $form['family_friend']['other_name'] = array(
          '#markup' => $others['main'],
          '#prefix' => '<div class="other-name"><div class="label-item">Others</div>',
          '#suffix' => '</div>',
        );
      }
    }
    
    $form['family_friend']['close_tag'] = array(
      '#markup' => '',
      '#suffix' => '</div></div>',
    );
  }
  
  $serv_type = unserialize($oborder->type_details);
  if ($oborder->form_type == 1) {
    $form['funeral_details']['funeral_title'] = array(
      '#markup' => '<h3>Funeral Details</h3>',
      '#prefix' => '<div class="funeral-details">',
      '#suffix' => '<div class="funeral-value">',
    );

    if (!$publication_onsite) {
      if (!isset($serv_type['template_choosed']) || $serv_type['template_choosed']==1) {
        if ($deceased['religion']!='Sikh') {
          $w_street_2 = !empty($serv_type['wake_street_2']) ? '<div>'. $serv_type['wake_street_2'] .'</div>' : '';
          $form['funeral_details']['funeral_wakelocation'] = array(
            '#markup' => '<div>'. $serv_type['wake_street_1'] . '</div>' . $w_street_2, // '<div>' . $advertiser['zip'] . '</div><div>' . $country_name .'</div>',
            '#prefix' => '<div class="funeral-wakelocation"><div class="label-item">Wake Location</div>',
            '#suffix' => '</div>',
          );
        }
        
        if ($deceased['religion']=='Christian' || $deceased['religion']=='Catholic') {
          $night_serv = '';
          foreach ($serv_type['more_nightserv'] as $key => $value) {
            if (is_array($value)) {
              if (!empty($value['night_services'])) {
                $night_serv .= $key==0 ? format_date(strtotime($value['night_services']), 'custom', 'd F Y \A\t h:i A') : ','. format_date(strtotime($value['night_services']), 'custom', 'd F Y \A\t h:i A');
              }
            }
          }

          $night_title = $deceased['religion']=='Christian' ? 'Night Services' : 'Night Prayers';
          $form['funeral_details']['funeral_nightservice'] = array(
            '#markup' => $night_serv,
            '#prefix' => '<div class="funeral_nightservice"><div class="label-item">'. $night_title .'</div>',
            '#suffix' => '</div>',
          );
        }

        if ($deceased['religion']=='Soka') {
          $form['funeral_details']['funeral_ssamemorial'] = array(
            '#markup' => format_date(strtotime($serv_type['ssa_memorial']), 'custom', 'd F Y \A\t h:i A'),
            '#prefix' => '<div class="funeral_ssamemorial"><div class="label-item">SSA Memorial Service on</div>',
            '#suffix' => '</div>',
          );
          $form['funeral_details']['funeral_service'] = array(
            '#markup' => format_date(strtotime($serv_type['funeral_service']), 'custom', 'd F Y \A\t h:i A'),
            '#prefix' => '<div class="funeral_service"><div class="label-item">Funeral Service on</div>',
            '#suffix' => '</div>',
          );
          /*
          $form['funeral_details']['funeral_leavesat'] = array(
            '#markup' => format_date(strtotime($serv_type['cortege_leaves_at']), 'custom', 'h:i A'),
            '#prefix' => '<div class="funeral_leavesat"><div class="label-item">Cortege Leaves at</div>',
            '#suffix' => '</div>',
          );
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
        $form['funeral_details']['funeral_cortegeleaves'] = array(
          '#markup' => format_date(strtotime($serv_type['cortege_leaves']), 'custom', 'd F Y \A\t h:i A'),
          '#prefix' => '<div class="funeral_cortegeleaves"><div class="label-item">'. $leaves_title .'</div>',
          '#suffix' => '</div>',
        );

        if ($deceased['religion']=='Sikh') {
          /*
          $form['funeral_details']['funeral_busesleaves'] = array(
            '#markup' => format_date(strtotime($serv_type['buses_leaves']), 'custom', 'd F Y \A\t h:i A'),
            '#prefix' => '<div class="funeral_busesleaves"><div class="label-item">Buses Leaves on</div>',
            '#suffix' => '</div>',
          );
          */
          $from_street_2 = !empty($serv_type['from_address_2']) ? '<div>'. $serv_type['from_address_2'] .'</div>' : '';
        // $from_country = $countries[$serv_type['from_country']];
          $form['funeral_details']['funeral_from'] = array(
            '#markup' => '<div>'. $serv_type['from_address_1'] .'</div>'. $from_street_2, //'<div>'. $serv_type['from_zip'] .'</div><div>'. $from_country .'</div>',
            '#prefix' => '<div class="funeral_from"><div class="label-item">From</div>',
            '#suffix' => '</div>',
          );
          $and_street_2 = !empty($serv_type['and_address_2']) ? '<div>'. $serv_type['and_address_2'] .'</div>' : '';
          //$and_country = $countries[$serv_type['and_country']];
          $form['funeral_details']['funeral_and'] = array(
            '#markup' => '<div>'. $serv_type['and_address_1'] .'</div>'. $and_street_2, //'<div>'. $serv_type['and_zip'] .'</div><div>'. $and_country .'</div>',
            '#prefix' => '<div class="funeral_and"><div class="label-item">And</div>',
            '#suffix' => '</div>',
          );
        }
        
        $form['funeral_details']['funeral_cortegefor'] = array(
          '#markup' => $serv_type['cortege_for'],
          '#prefix' => '<div class="funeral_cortegefor"><div class="label-item">For</div>',
          '#suffix' => '</div>',
        );
        $form['funeral_details']['funeral_cortegeat'] = array(
          '#markup' => $serv_type['cortege_at'],
          '#prefix' => '<div class="funeral_cortegeat"><div class="label-item">At</div>',
          '#suffix' => '</div>',
        );
        $form['funeral_details']['funeral_cortegetime'] = array(
          '#markup' => format_date(strtotime($serv_type['cortege_time']), 'custom', 'h:i A'),
          '#prefix' => '<div class="funeral_cortegetime"><div class="label-item">At Time</div>',
          '#suffix' => '</div>',
        );
        if ($deceased['religion']=='Christian' || $deceased['religion']=='Catholic') {
          $bible_verses = !empty($serv_type['bible_verse']) ? '<p>'. publish_obituary_get_bible_word($serv_type['bible_verse']) .'</p>' : '';
          $bible_verses .= !empty($serv_type['custom_verse']) ? '<p>'. $serv_type['custom_verse'] .'</p>' : '';
          $form['funeral_details']['funeral_bibleverse'] = array(
            '#markup' => $bible_verses,
            '#prefix' => '<div class="funeral_bibleverse"><div class="label-item">Bible Verses</div>',
            '#suffix' => '</div>',
          );
        }
      } else {
        $form['funeral_details']['funeral_owntemplate'] = array(
          '#markup' => $serv_type['own_template']['value'],
          '#prefix' => '<div class="funeral_owntemplate"><div class="label-item"></div>',
          '#suffix' => '</div>',
        );
      }
    }

    $form['funeral_details']['close_tag'] = array(
      '#markup' => '',
      '#suffix' => '</div></div>',
    );
  }
  
  if ($oborder->form_type == 2) {
    $form['acknowledge_details']['acknowledge_title'] = array(
      '#markup' => '<h3>Acknowledgement Details</h3>',
      '#prefix' => '<div class="step-item step-5-acknowledgement-details">',
      '#suffix' => '<div class="acknowledgement-value">',
    );

    if (!$publication_onsite) {
      $form['acknowledge_details']['acknowledge_message'] = array(
        '#markup' => $serv_type['message'],
        '#prefix' => '<div class="acknowledgement-message"><div class="label-item">Acknowledgement</div>',
        '#suffix' => '</div>',
      );
      $form['acknowledge_details']['acknowledge_thanks'] = array(
        '#markup' => $serv_type['special_thanks'],
        '#prefix' => '<div class="acknowledgement_thanks"><div class="label-item">Special Thanks to</div>',
        '#suffix' => '</div>',
      );
      $form['acknowledge_details']['acknowledge_remarks'] = array(
        '#markup' => $serv_type['other_remarks'],
        '#prefix' => '<div class="acknowledgement_remarks"><div class="label-item">Other remarks</div>',
        '#suffix' => '</div>',
      );
    }
    
    $form['acknowledge_details']['close_tag'] = array(
      '#markup' => '',
      '#suffix' => '</div></div>',
    );
  }

  if ($oborder->form_type == 3) {
    $form['condolence_details']['condolence_title'] = array(
      '#markup' => '<h3>Condolence Details</h3>',
      '#prefix' => '<div class="step-item step-5-condolence-details">',
      '#suffix' => '<div class="condolence-value">',
    );

    if (!$publication_onsite) {
      $form['condolence_details']['condolence_message'] = array(
        '#markup' => $serv_type['message']['value'],
        '#prefix' => '<div class="condolence-message"><div class="label-item">Condolence Message</div>',
        '#suffix' => '</div>',
      );
      $form['condolence_details']['condolence_thanks'] = array(
        '#markup' => $serv_type['condo_from']['value'],
        '#prefix' => '<div class="condolence_thanks"><div class="label-item">From</div>',
        '#suffix' => '</div>',
      );
      $form['condolence_details']['condolence_remarks'] = array(
        '#markup' => $serv_type['other_remarks'],
        '#prefix' => '<div class="condolence_remarks"><div class="label-item">Other remarks</div>',
        '#suffix' => '</div>',
      );
    }
    
    $form['condolence_details']['close_tag'] = array(
      '#markup' => '',
      '#suffix' => '</div></div>',
    );
  }

  if ($oborder->form_type == 4) {
    $form['memoriam_details']['memoriam_title'] = array(
      '#markup' => '<h3>In Memoriam Details</h3>',
      '#prefix' => '<div class="step-item step-5-memoriam-details">',
      '#suffix' => '<div class="memoriam-value">',
    );

    if (!$publication_onsite) {
      $memo_message = !empty($serv_type['custom_message']) ? $serv_type['custom_message'] : publish_obituary_get_memo_message_word($serv_type['select_message']);
    
      $rememberer='';
      foreach ($serv_type['more_rememberer'] as $key => $value) {
        if (is_array($value)) {
          if (!empty($value['rememberer_name'])) $rememberer .= '<div>' . $value['rememberer_name'] . '</div>';
        }
      }
      /*
      $form['memoriam_details']['memoriam_anniversary'] = array(
        '#markup' => $serv_type['anniversary'] .'<br>'. $serv_type['anniversary_info'],
        '#prefix' => '<div class="memoriam-anniversary"><div class="label-item">Year of Anniversary</div>',
        '#suffix' => '</div>',
      );
      */
      $form['memoriam_details']['memoriam_message'] = array(
        '#markup' => $memo_message,
        '#prefix' => '<div class="memoriam-message"><div class="label-item">Message</div>',
        '#suffix' => '</div>',
      );
      $form['memoriam_details']['memoriam_rememberer'] = array(
        '#markup' => $rememberer,
        '#prefix' => '<div class="memoriam-rememberer"><div class="label-item">Dearly missed and fondly remembered by</div>',
        '#suffix' => '</div>',
      );
    }
    
    $form['memoriam_details']['close_tag'] = array(
      '#markup' => '',
      '#suffix' => '</div></div>',
    );
  }

  if ($oborder->form_type != 2) { // if not acknowledgement
    $form['upload_files']['upload_title'] = array(
        '#markup' => '<h3>Upload Forms and Photo</h3>',
        '#prefix' => '<div class="upload-files">',
        '#suffix' => '<div class="upload-value">',
    );

    if (!$publication_onsite) {
      $doc_upload = unserialize($oborder->upload);
      $deceased_photo = file_load($doc_upload['deceased_photo']);
      $show_photo = !empty($deceased_photo) ? '<img src="' . image_style_url('thumbnail', $deceased_photo->uri) . '">' : '';
      $death_certific = file_load($doc_upload['death_certificate']);
      $show_certific = !empty($death_certific) ? '<a href="' . file_create_url($death_certific->uri) . '">'. $death_certific->filename . '</a>' : '';
      $indemnity_form = file_load($doc_upload['indemnity_upload']);
      $show_indemnity = !empty($indemnity_form) ? '<a href="' . file_create_url($indemnity_form->uri) . '">'. $indemnity_form->filename . '</a>' : '';
    
      $form['upload_files']['upload_photo'] = array(
        '#markup' => $show_photo,
        '#prefix' => '<div class="upload-deceasedphoto"><div class="label-item">Deceased Photo</div>',
        '#suffix' => '</div>',
      );
      if ($oborder->form_type==3) {
        $deceased_logo = file_load($doc_upload['company_logo']);
        $show_logo = !empty($deceased_logo) ? '<img src="'. image_style_url('thumbnail', $deceased_logo->uri) .'">' : '';
        $form['upload_files']['upload_logo'] = array(
          '#markup' => $show_logo,
          '#prefix' => '<div class="upload-complogo"><div class="label-item">Company Logo</div>',
          '#suffix' => '</div>',
        );
      }
      $form['upload_files']['upload_certificate'] = array(
          '#markup' => $show_certific,
          '#prefix' => '<div class="upload_deathcertificate"><div class="label-item">Death Certificate</div>',
          '#suffix' => '</div>',
      );
      $form['upload_files']['upload_indemnity'] = array(
        '#markup' => $show_indemnity,
        '#prefix' => '<div class="upload_deathindemnity"><div class="label-item">Indemnity Form</div>',
        '#suffix' => '</div>',
      );
    }
    
    $form['upload_files']['close_tag'] = array(
      '#markup' => '',
      '#suffix' => '</div></div>',
    );
  }
  
  $form['approve'] = array(
    '#type' => 'checkbox',
    '#title' => t('Approved'),
    '#default_value' => $oborder->status,
  );

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  
  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
  );
  
  return $form;
}

function publish_obituary_approve_form_submit($form, &$form_state) {
  if ($form_state['triggering_element']['#value'] == 'Save') {
    publish_obituary_approve_action($form_state['values']['oid'], $form_state['values']['approve'], $form_state['values']['status']);
    
    $form_state['redirect'] = 'admin/commerce/orders/obituaries';
    //drupal_goto('admin/commerce/orders/obituaries'); 
  }

  if ($form_state['triggering_element']['#value'] == 'Cancel') {
    $form_state['redirect'] = 'admin/commerce/orders/obituaries';
  }
}

/**
 * Function to approve the obituaries order.
 */
function publish_obituary_approve_action($oid, $approve_act, $approve_def) {
  $order = db_update('online_obituaries_orders')
        ->fields(array('status' => $approve_act,))
        ->condition ('online_obituaries_orders.oid', $oid, '=')
        ->execute();

      if ($order && $approve_def == 0 && $approve_act == 1) {
      
        $obiorder = obituaries_order_load($oid);
        approved_obituary_create($obiorder);      
      }
}

/**
 * Page callback for obituary order delete page
 */
function publish_obituary_order_delete($oid) {
  $oborder = obituaries_order_load($oid);
  
  return drupal_get_form('publish_obituary_delete_form', $oborder);
}

/**
 * Form called by obituary order delete
 */
function publish_obituary_delete_form($form, &$form_state, $oborder) {
  $deceased = unserialize($oborder->deceased);
  $form['confirm'] = array(
    '#markup' => '<h3><em>Delete obituary</em> ' . $deceased['deceased_name'] . ' (' . $oborder->oid . ') order?</h3>',
  );
  $form['oid'] = array(
    '#type' => 'hidden',
    '#value' => $oborder->oid,
  );

  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
  );
  
  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
  );
  
  return $form;
}

function publish_obituary_delete_form_submit($form, &$form_state) {
  if ($form_state['triggering_element']['#value'] == 'Delete') {
    $o_delete = publish_obituary_order_db_delete($form_state['values']['oid']);

    if ($o_delete) { 
      drupal_set_message('Obituary order id #'.$form_state['values']['oid'].' has been deleted.', 'status');
    }
    $form_state['redirect'] = 'admin/commerce/orders/obituaries';
  }

  if ($form_state['triggering_element']['#value'] == 'Cancel') {
    $form_state['redirect'] = 'admin/commerce/orders/obituaries';
  }
}

function publish_obituary_order_db_delete($oid) {
  $order = db_delete('online_obituaries_orders')
    ->condition('online_obituaries_orders.oid', $oid, '=')
    ->execute();

  return $order;
}

/**
 * Page callback for obituary form settings page
 */
function publish_obituary_obituary_form_settings() {
  drupal_set_title('Death Announcement form settings');

  return drupal_get_form('publish_obituary_setting_form');
}

/**
 * Page callback for memoriam form settings page
 */
function publish_obituary_memoriam_form_settings() {
  drupal_set_title('In Memoriam form settings');

  return drupal_get_form('publish_memoriam_setting_form');
}

// Load online obituaries order data
function obituaries_order_load($oid) {

  $query = db_select('online_obituaries_orders', 'obd');
  $order_result = $query
    ->fields('obd')
    ->condition('obd.oid', $oid, '=')
    ->execute()
    ->fetchObject();

  return $order_result;
}

// Create obituary node based on online obituary order approved
function approved_obituary_create($obiorder) {
  //global $user;
  if ($obiorder->kind==1) $publication = unserialize($obiorder->publication);
  $advertiser = unserialize($obiorder->advertiser);
  $deceased = unserialize($obiorder->deceased);
  $serv_type = unserialize($obiorder->type_details);
  $doc_upload = unserialize($obiorder->upload);
  //dpm($deceased);
  $publication_onsite = isset($publication) && $publication['pub_choosed']==1 ? TRUE : FALSE;

  if ($obiorder->form_type==1) {
    $values = array(
      'type' => 'obituary',
      'uid' => $obiorder->uid,
      'status' => 0,
      'comment' => 2, //open
      'promote' => 0,
    );
    $obituary_title = !$publication_onsite ? $deceased['deceased_name'] : 'Schedule an onsite visit';
    $entity = entity_create('node', $values);
    $wrapper = entity_metadata_wrapper('node', $entity);
    $wrapper->title->set($obituary_title);
    if (!$publication_onsite) {
      $wrapper->body->set(array('value'=>$deceased['personality']));
      $dec_photo = file_load($doc_upload['deceased_photo']);
      if (!empty($dec_photo)) {
        $wrapper->field_obituary_deceased_photo->set(array('fid' => $doc_upload['deceased_photo']));
      }
      $wrapper->field_obituary_age->set($deceased['age']);
      $wrapper->field_obituary_departed->set(strtotime($deceased['departed_date']));
      $wrapper->field_obituary_religion->set(strtolower($deceased['religion']));
      $wrapper->field_funeral_template->set($serv_type['template_choosed']);
      if ($serv_type['template_choosed']==1) {
        if ($deceased['religion']!='Sikh') {
          $wrapper->field_wake_street_address_1->set($serv_type['wake_street_1']);
          $wrapper->field_wake_street_address_2->set($serv_type['wake_street_2']);
        }
        $wrapper->field_cortage_leaves_on->set(strtotime($serv_type['cortege_leaves']));
        $wrapper->field_cortage_leaves_for->set(strtolower($serv_type['cortege_for']));
        $wrapper->field_cortage_leaves_at->set($serv_type['cortege_at']);
        $wrapper->field_cortage_leaves_at_time->set(strtotime($serv_type['cortege_time']));
        $wrapper->field_obituary_remarks->set(array('value'=>$serv_type['other_remarks']));
        if ($deceased['religion']=='Christian' || $deceased['religion']=='Catholic') {
          $bible_verses = !empty($serv_type['bible_verse']) ? '<p>'. publish_obituary_get_bible_word($serv_type['bible_verse']) .'</p>' : '';
          $bible_verses .= !empty($serv_type['custom_verse']) ? '<p>'. $serv_type['custom_verse'] .'</p>' : '';
          $wrapper->field_obituary_bible_verses->set(array('value'=>$bible_verses));
        }
        if ($deceased['religion']=='Catholic') {
          $wrapper->field_funeral_mass->set($serv_type['cortege_massat']);
          $wrapper->field_funeral_mass_time->set(strtotime($serv_type['cortege_masstime']));
        }
        if ($deceased['religion']=='Sikh') {
          //$wrapper->field_bus_leaves->set(strtotime($serv_type['buses_leaves']));
          $wrapper->field_bus_from_street_1->set($serv_type['from_address_1']);
          $wrapper->field_bus_from_street_2->set($serv_type['from_address_2']);
          //$wrapper->field_bus_from_zip_code->set($serv_type['from_zip']);
          $wrapper->field_bus_and_street_1->set($serv_type['and_address_1']);
          $wrapper->field_bus_and_street_2->set($serv_type['and_address_2']);
          //$wrapper->field_bus_and_zip_code->set($serv_type['and_zip']);
          $wrapper->field_bhog_details->set(array('value'=>$serv_type['bhog']));
        }
        if ($deceased['religion']=='Soka') {
          $wrapper->field_ssa_memorial_service->set(strtotime($serv_type['ssa_memorial']));
          $wrapper->field_funeral_service->set(strtotime($serv_type['funeral_service']));
        }
      } else {
        if ($deceased['religion']=='Hindu' || $deceased['religion']=='Sikh') {
          $leave_long = strtotime("+3 days");
        } else {
          $leave_long = strtotime("+7 days");
        }
        $wrapper->field_cortage_leaves_on->set($leave_long);
        $wrapper->field_custom_funeral->set(array('value'=>$serv_type['own_template']['value']));
      }
      $dec_cert = file_load($doc_upload['death_certificate']);
      if (!empty($dec_cert)) {
        $wrapper->field_obituary_death_certificate->file->set(file_load($doc_upload['death_certificate']));
      }
    }
    $wrapper->field_kind_of_advertisement->set($obiorder->kind);
    $wrapper->save();
    //entity_save('node', $entity);
    
    if (!$publication_onsite) {
      $eid = $wrapper->getIdentifier();
      
      $node = node_load($eid);

      if ($serv_type['template_choosed']==1) {
        if ($deceased['religion']=='Christian' || $deceased['religion']=='Catholic') {
          $night_serv_date = array();
          foreach ($serv_type['more_nightserv'] as $key => $value) {
            if (is_array($value)) {
              if (!empty($value['night_services'])) {
                $night_serv_date['more_nightserv'][] = $value;
              }
            }
          }

          if (!empty($night_serv_date)) {
            foreach ($night_serv_date['more_nightserv'] as $key => $value) {
              $values = array(
                'field_name' => 'field_night_services_prayers',
                'field_night_serv_pray_date' => array(
                  LANGUAGE_NONE => array(array('value' => strtotime($value['night_services']))),
                ),
              );
              $entity = entity_create('field_collection_item', $values);
              $entity->setHostEntity('node', $node);
            }
            $entity->save();
          }
        }
      }
      
      $family_friends = unserialize($obiorder->family);
      //Fill spouse field collection.
      $values = array(
        'field_name' => 'field_obituary_spouse',
        'field_husband_wife' => array(
          LANGUAGE_NONE => array(array('value' => $family_friends['spouse_as'])),
        ),
        'field_spouse_name' => array(
          LANGUAGE_NONE => array(array('value' => $family_friends['spouse_name'])),
        ),
      );
      $entity = entity_create('field_collection_item', $values);
      $entity->setHostEntity('node', $node);
      $entity->save();
      
      approved_obituary_collection_add($node, $family_friends, 'more_fathers', 'field_obituary_father', 'field_father_name', 'field_mother_name', 'father_name', 'mother_name');
      approved_obituary_collection_add($node, $family_friends, 'more_sons', 'field_obituary_sons', 'field_sons_name', 'field_daughters_in_law_name', 'son_name', 'daughter_law_name');
      approved_obituary_collection_add($node, $family_friends, 'more_daughters', 'field_obituary_daughters', 'field_daughters_name', 'field_sons_in_law_name', 'daughter_name', 'sons_law_name');
      approved_obituary_collection_add($node, $family_friends, 'more_grandsons', 'field_obituary_grandsons', 'field_grandsons_name', 'field_granddaughters_in_law_name', 'grandson_name', 'granddaughter_law_name');
      approved_obituary_collection_add($node, $family_friends, 'more_granddaughters', 'field_obituary_granddaughters', 'field_granddaughters_name', 'field_grandsons_in_law_name', 'granddaughter_name', 'grandson_law_name');
      approved_obituary_collection_add($node, $family_friends, 'more_greatgrandsons', 'field_obituary_great_grandsons', 'field_great_grandsons_name', 'field_great_granddaughters_name', 'greatgrandson_name', 'greatgranddaughter');
      approved_obituary_collection_add($node, $family_friends, 'more_brothers', 'field_obituary_brothers', 'field_brothers_name', 'field_sister_in_law_name', 'brother_name', 'sister_law_name');
      approved_obituary_collection_add($node, $family_friends, 'more_sisters', 'field_obituary_sisters', 'field_sisters_name', 'field_brothers_in_law_name', 'sister_name', 'brother_law_name');
      approved_obituary_collection_add($node, $family_friends, 'more_fatherlaws', 'field_obituary_father_in_law', 'field_father_in_law_name', 'field_mother_in_law_name', 'fatherlaw_name', 'motherlaw_name');
      approved_obituary_collection_add($node, $family_friends, 'more_brotherslaw', 'field_obituary_brothers_in_law', 'field_brothers_law_main_name', 'field_sisters_law_pair_name', 'brotherlawmain_name', 'sisterlawpair_name');
      approved_obituary_collection_add($node, $family_friends, 'more_sisterslaw', 'field_obituary_sisters_in_law', 'field_sisters_law_main_name', 'field_brothers_law_pair_name', 'sisterlawmain_name', 'brotherlawpair_name');
      approved_obituary_collection_add($node, $family_friends, 'more_nephews', 'field_obituary_nephews', 'field_nephews_name', 'field_nieces_in_law_name', 'nephew_name', 'niece_law_name');
      approved_obituary_collection_add($node, $family_friends, 'more_nieces', 'field_obituary_nieces', 'field_nieces_name', 'field_nephews_in_law_name', 'niece_name', 'nephew_law_name');
      approved_obituary_collection_add($node, $family_friends, 'more_grandnephews', 'field_obituary_grandnephews', 'field_grandnewphews_name', 'field_grandnieces_name', 'grandnephew_name', 'grandniece_name');
      approved_obituary_collection_add($node, $family_friends, 'more_grandnieces', 'field_obituary_grandnieces', 'field_grandnieces_real_name', 'field_grandnephews_in_law_name', 'grandniece_real_name', 'grandnephew_law_name');
      approved_obituary_collection_add($node, $family_friends, 'more_greatgrandnephews', 'field_obituary_great_grandnephew', 'field_great_grandnephews_name', 'field_great_grandnieces_name', 'greatgrandnephew_name', 'greatgrandniece_name');
      approved_obituary_collection_add($node, $family_friends, 'more_uncles', 'field_obituary_uncles', 'field_uncle_name', 'field_uncles_spouses', 'uncle_name', 'uncle_spouse_name');
      approved_obituary_collection_add($node, $family_friends, 'more_aunties', 'field_obituary_aunties', 'field_aunties_name', 'field_aunties_spouses_name', 'auntie_name', 'auntie_spouse_name');
      approved_obituary_collection_add($node, $family_friends, 'more_cousins', 'field_obituary_cousins', 'field_cousins_name', 'field_cousins_in_law_name', 'cousin_name', 'cousin_law_name');
      approved_obituary_collection_add($node, $family_friends, 'more_godsons', 'field_obituary_god_sons', 'field_god_sons_name', 'field_god_daughters_in_law_name', 'godson_name', 'goddaughter_law_name');
      approved_obituary_collection_add($node, $family_friends, 'more_goddaughters', 'field_obituary_god_daughters', 'field_god_daughters_name', 'field_god_sons_in_law_name', 'goddaughter_name', 'godsons_law_name');
      approved_obituary_collection_add($node, $family_friends, 'more_godgrandsons', 'field_obituary_god_grandsons', 'field_god_grandsons_name', 'field_god_granddaughters_in_law_', 'godgrandson_name', 'godgranddaughter_law_name');
      approved_obituary_collection_add($node, $family_friends, 'more_godgranddaughters', 'field_obituary_god_granddaugh', 'field_god_granddaughters_name', 'field_god_grandsons_in_law_name', 'godgranddaughter_name', 'godgrandson_law_name');
      approved_obituary_collection_add($node, $family_friends, 'more_godgreatgrandsons', 'field_obituary_god_great_grandso', 'field_god_great_grandsons_name', 'field_god_great_granddaughters_n', 'godgreatgrandson_name', 'godgreatgranddaughter_name');
      approved_obituary_collection_add($node, $family_friends, 'more_caregivers', 'field_obituary_caregivers', 'field_caregivers_name', '', 'caregiver_name', '');
      approved_obituary_collection_add($node, $family_friends, 'more_others', 'field_obituary_other', 'field_other_title', 'field_other_name', 'other_title', 'other_name');
    }  
  } else {
    switch ($obiorder->form_type) {
      case '2':
        $ann_body = $serv_type['message'];
        $ann_serv = 'Acknowledgement';
        break;
      case '3':
        $ann_body = $serv_type['message']['value'];
        $ann_serv = 'Condolence';
        break;
      case '4':
        $ann_body = !empty($serv_type['custom_message']) ? $serv_type['custom_message'] : publish_obituary_get_memo_message_word($serv_type['select_message']);
        $ann_serv = 'In Memoriam';
        break;
    }
    /*
    if ($obiorder->form_type==2 && $obiorder->kind==2) {
      $obi_node = node_load(trim($deceased['deceased_name']));
      $deceased_name = $obi_node->title .' ('. $deceased['deceased_name'] .')';
    } else {
      $deceased_name = $deceased['deceased_name'];
    }
    */
    $deceased_name = $deceased['deceased_name'];
    $announce_title = !$publication_onsite ? $deceased_name : 'Schedule an onsite visit';

    $values = array(
      'type' => 'announcement',
      'uid' => $obiorder->uid,
      'status' => 1,
      'comment' => 1, //closed
      'promote' => 0,
    );
    
    $entity = entity_create('node', $values);
    $wrapper = entity_metadata_wrapper('node', $entity);
    $wrapper->title->set($announce_title);
    $wrapper->field_type_of_announcement->set($ann_serv);
    if (!$publication_onsite) {
      $wrapper->body->set(array('value'=>$ann_body));
      if ($obiorder->form_type!=2) {
        $wrapper->field_obituary_deceased_photo->set(array('fid' => $doc_upload['deceased_photo']));
      }
      $wrapper->field_obituary_departed->set(strtotime($deceased['departed_date']));
      $wrapper->field_announcement_remarks->set(array('value'=>$serv_type['other_remarks']));
      if ($obiorder->form_type==2) {
        $wrapper->field_acknowledge_family->set($serv_type['late_family']);
        $wrapper->field_acknowledge_special_thanks->set(array('value'=>$serv_type['special_thanks']));
        if ($deceased['religion']=='Hindu') {
          if (!empty($serv_type['karumathi_date'])) {
            $wrapper->field_karumathi_date->set(strtotime($serv_type['karumathi_date']));
          }
          $wrapper->field_karumathi_address->set($serv_type['karumathi_address']);
          if (!empty($serv_type['athma_shanthi_date'])) {
            $wrapper->field_athma_shanthi_date->set(strtotime($serv_type['athma_shanthi_date']));
          }
          $wrapper->field_athma_shanthi_temple->set($serv_type['athma_shanthi_temple']);
          if (!empty($serv_type['athma_shanthi_time'])) {
            $wrapper->field_athma_shanthi_time->set(strtotime($serv_type['athma_shanthi_time']));
          }
        }
      }
      if ($obiorder->form_type==3) {
        $client_logo = file_load($doc_upload['company_logo']);
        if (!empty($client_logo)) {
          $wrapper->field_condo_company_logo->set(array('fid' => $doc_upload['company_logo']));
        }
        $wrapper->field_condo_from->set(array('value'=>$serv_type['condo_from']['value']));
      }
      /*
      if ($obiorder->form_type==4) {
        $wrapper->field_year_of_anniversary->set($serv_type['anniversary']);
        $wrapper->field_info_of_anniversary->set($serv_type['anniversary_info']);
      }
      */
    }
    $wrapper->field_kind_of_advertisement->set($obiorder->kind);
    $wrapper->save();
    
    if (!$publication_onsite) {
      $eid = $wrapper->getIdentifier();
      
      if ($obiorder->form_type==4) {
        $node = node_load($eid);

        $l_remember = array();
        foreach ($serv_type['more_rememberer'] as $key => $value) {
          if (is_array($value)) {
            $l_remember['more_rememberer'][] = $value;
          }
        }

        foreach ($l_remember['more_rememberer'] as $key => $value) {
          $values = array(
            'field_name' => 'field_memoriam_dearly_missed',
            'field_rememberer_name' => array(
              LANGUAGE_NONE => array(array('value' => $value['rememberer_name'])),
            ),
          );
          $entity = entity_create('field_collection_item', $values);
          $entity->setHostEntity('node', $node);
        }
        $entity->save();
      }
    }
  }
  $congrate = !$publication_onsite ? $deceased['deceased_name'] . ' (' . $eid . ')' : 'Schedule an onsite visit';
  drupal_set_message('<em>Obituary</em> ' . $congrate . ' <em>Approved</em>', 'status');
  /**/
}

// Fill field collection of approved obituary node
function approved_obituary_collection_add($node, $family_friends, $fam_key, $field_name, $fci1, $fci2, $fv_key1, $fv_key2) {
  // Get family data in array only
  $family_mores = array();
  foreach ($family_friends as $key => $value) {
    if (is_array($value)) {
      foreach ($value as $key2 => $value2) {
        if (is_array($value2)) {
          $family_mores[$key][] = $value2;
        }
      }
    }
  }

  if ($fam_key == 'more_caregivers') {
    foreach ($family_mores[$fam_key] as $key => $value) {
      $values = array(
        'field_name' => $field_name,
        $fci1 => array(
          LANGUAGE_NONE => array(array('value' => $value[$fv_key1])),
        ),
      );
      $entity = entity_create('field_collection_item', $values);
      $entity->setHostEntity('node', $node);
    }
    $entity->save();
  } else {
    foreach ($family_mores[$fam_key] as $key => $value) {
      $values = array(
        'field_name' => $field_name,
        $fci1 => array(
          LANGUAGE_NONE => array(array('value' => $value[$fv_key1])),
        ),
        $fci2 => array(
          LANGUAGE_NONE => array(array('value' => $value[$fv_key2])),
        ),
      );
      $entity = entity_create('field_collection_item', $values);
      $entity->setHostEntity('node', $node);
    }
    $entity->save();
  }
}

/**
 * Get service name from value saved in database
 */
function get_service_name($sid) {
  switch ($sid) {
    case '2':
      $service_name = 'Acknowledgement';
      break;
    case '3':
      $service_name = 'Condolence';
      break;
    case '4':
      $service_name = 'In Memoriam';
      break;
    default:
      $service_name = 'Death Announcement';
      break;
  }
  return $service_name;
}

/**
 * Function to return type of obituaries by id
 */
function publish_obituary_form_type($oid) {
  //$obies_result = obituaries_order_load($oid);
  $query = db_select('online_obituaries_orders', 'obd');
  $obies_result = $query
    ->fields('obd', array('form_type'))
    ->condition('obd.oid', $oid, '=')
    ->execute()
    ->fetchAssoc();
  $f_service = get_service_name($obies_result['form_type']);

  return $f_service;
}