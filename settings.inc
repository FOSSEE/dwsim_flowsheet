<?php
// $Id$
function dwsim_flowsheet_settings_form($form, $form_state)
  {
    $form['emails'] = array(
        '#type' => 'textfield',
        '#title' => t('(Bcc) Notification emails'),
        '#description' => t('Specify emails id for Bcc option of mail system with comma separated'),
        '#size' => 50,
        '#maxlength' => 255,
        '#required' => TRUE,
        '#default_value' => variable_get('dwsim_flowsheet_emails', '')
    );
    $form['cc_emails'] = array(
        '#type' => 'textfield',
        '#title' => t('(Cc) Notification emails'),
        '#description' => t('Specify emails id for Cc option of mail system with comma separated'),
        '#size' => 50,
        '#maxlength' => 255,
        '#required' => TRUE,
        '#default_value' => variable_get('dwsim_flowsheet_cc_emails', '')
    );
    $form['from_email'] = array(
        '#type' => 'textfield',
        '#title' => t('Outgoing from email address'),
        '#description' => t('Email address to be display in the from field of all outgoing messages'),
        '#size' => 50,
        '#maxlength' => 255,
        '#required' => TRUE,
        '#default_value' => variable_get('dwsim_flowsheet_from_email', '')
    );
    $form['extensions']['user_defind_compound_source'] = array(
        '#type' => 'textfield',
        '#title' => t('Allowed source file extensions for user defind compound'),
        '#description' => t('A comma separated list WITHOUT SPACE of source file extensions that are permitted to be uploaded on the server'),
        '#size' => 50,
        '#maxlength' => 255,
        '#required' => TRUE,
        '#default_value' => variable_get('dwsim_flowsheet_user_defind_compound_source_extensions', '')
    );
    $form['extensions']['dependency'] = array(
        '#type' => 'textfield',
        '#title' => t('Allowed dependency file extensions'),
        '#description' => t('A comma separated list WITHOUT SPACE of dependency file extensions that are permitted to be uploaded on the server'),
        '#size' => 50,
        '#maxlength' => 255,
        '#required' => TRUE,
        '#default_value' => variable_get('dwsim_flowsheet_dependency_extensions', '')
    );
    $form['extensions']['result'] = array(
        '#type' => 'textfield',
        '#title' => t('Allowed result file extensions'),
        '#description' => t('A comma separated list WITHOUT SPACE of result file extensions that are permitted to be uploaded on the server'),
        '#size' => 50,
        '#maxlength' => 255,
        '#required' => TRUE,
        '#default_value' => variable_get('dwsim_flowsheet_result_extensions', '')
    );
    $form['extensions']['xcos'] = array(
        '#type' => 'textfield',
        '#title' => t('Allowed xcos file extensions'),
        '#description' => t('A comma separated list WITHOUT SPACE of xcos file extensions that are permitted to be uploaded on the server'),
        '#size' => 50,
        '#maxlength' => 255,
        '#required' => TRUE,
        '#default_value' => variable_get('dwsim_flowsheet_xcos_extensions', '')
    );
    $form['extensions']['pdf'] = array(
        '#type' => 'textfield',
        '#title' => t('Allowed pdf file extensions'),
        '#description' => t('A comma separated list WITHOUT SPACE of pdf file extensions that are permitted to be uploaded on the server'),
        '#size' => 50,
        '#maxlength' => 255,
        '#required' => TRUE,
        '#default_value' => variable_get('dwsim_flowsheet_pdf_extensions', '')
    );
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit')
    );
    return $form;
  }
function dwsim_flowsheet_settings_form_validate($form, &$form_state)
  {
    return;
  }
function dwsim_flowsheet_settings_form_submit($form, &$form_state)
  {
    variable_set('dwsim_flowsheet_emails', $form_state['values']['emails']);
    variable_set('dwsim_flowsheet_cc_emails', $form_state['values']['cc_emails']);
    variable_set('dwsim_flowsheet_from_email', $form_state['values']['from_email']);
    variable_set('dwsim_flowsheet_user_defind_compound_source_extensions', $form_state['values']['user_defind_compound_source']);
    variable_set('dwsim_flowsheet_dependency_extensions', $form_state['values']['dependency']);
    variable_set('dwsim_flowsheet_result_extensions', $form_state['values']['result']);
    variable_set('dwsim_flowsheet_xcos_extensions', $form_state['values']['xcos']);
    variable_set('dwsim_flowsheet_pdf_extensions', $form_state['values']['pdf']);
    drupal_set_message(t('Settings updated'), 'status');
  }
