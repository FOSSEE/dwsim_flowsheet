<?php
// $Id$
/*
Approval Status :
0 - Pending
1 - Approved
2 - Dis-Approved
3 - Solved
Solution Status :
0 - Pending
1 - Approved
2 - Dis-Approved
Solution Display :
0 - No
1 - Yes

Tables :
dwsim_flowsheet_solution : approval_status
0 - Pending
1 - Approved
2 - Disapproved (delete it)
*/
function dwsim_flowsheet_proposal_form($form, &$form_state)
  {
    global $user;
    /************************ start approve book details ************************/
    if ($user->uid == 0) {
        $msg =  drupal_set_message(t('It is mandatory to ' . l('login', 'user') . ' on this website to access the lab proposal form. If you are new user please create a new account first.'), 'error');
         //drupal_goto('dwsim-flowsheet-project');
         drupal_goto('user');
         return $msg;
    }
    $query = db_select('dwsim_flowsheet_proposal');
    $query->fields('dwsim_flowsheet_proposal');
    $query->condition('uid', $user->uid);
    $query->orderBy('id', 'DESC');
    $query->range(0, 1);
    $proposal_q = $query->execute();
    $proposal_data = $proposal_q->fetchObject();
    if ($proposal_data)
      {
        if ($proposal_data->approval_status == 0 || $proposal_data->approval_status == 1)
          {
            drupal_set_message(t('We have already received your proposal.'), 'status');
            drupal_goto('');
            return;
          }
      }
    $form['#attributes'] = array(
        'enctype' => "multipart/form-data"
    );
    $form['name_title'] = array(
        '#type' => 'select',
        '#title' => t('Title'),
        '#options' => array(
            'Dr' => 'Dr',
            'Prof' => 'Prof',
            'Mr' => 'Mr',
            'Mrs' => 'Mrs',
            'Ms' => 'Ms'
        ),
        '#required' => TRUE
    );
    $form['student_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Name of the student'),
        '#size' => 250,
        '#attributes' => array(
            'placeholder' => t('Enter your full name')
        ),
        '#maxlength' => 250,
        '#required' => TRUE
    );
    $form['month_year_of_degree'] = array(
        '#type' => 'date_popup',
        '#title' => t('Month and year of award of degree'),
        '#date_label_position' => '',
        '#description' => '',
        '#default_value' => '',
        '#date_format' => 'M-Y',
        '#date_increment' => 0,
        '#date_year_range' => '1960:+0',
        '#datepicker_options' => array(
            'maxDate' => 0
        ),
    );
    $form['student_email_id'] = array(
        '#type' => 'textfield',
        '#title' => t('Email'),
        '#size' => 30,
        '#value' => $user->mail,
        '#disabled' => TRUE
    );
       $form['project_guide_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Project guide'),
        '#size' => 250,
        '#attributes' => array(
            'placeholder' => t('Enter full name of project guide')
        ),
        '#maxlength' => 250,
        '#required' => TRUE
    );
    $form['project_guide_email_id'] = array(
        '#type' => 'textfield',
        '#title' => t('Project guide email'),
        '#size' => 30,
    );
    $form['university'] = array(
        '#type' => 'textfield',
        '#title' => t('University/ Institute'),
        '#size' => 80,
        '#maxlength' => 200,
        '#required' => TRUE,
        '#attributes' => array(
            'placeholder' => 'Insert full name of your institute/ university.... '
        )
    );
    $form['country'] = array(
        '#type' => 'select',
        '#title' => t('Country'),
        '#options' => array(
            'India' => 'India',
            'Others' => 'Others'
        ),
        '#required' => TRUE,
        '#tree' => TRUE,
        '#validated' => TRUE
    );
    $form['other_country'] = array(
        '#type' => 'textfield',
        '#title' => t('Other than India'),
        '#size' => 100,
        '#attributes' => array(
            'placeholder' => t('Enter your country name')
        ),
        '#states' => array(
            'visible' => array(
                ':input[name="country"]' => array(
                    'value' => 'Others'
                )
            )
        )
    );
    $form['other_state'] = array(
        '#type' => 'textfield',
        '#title' => t('State other than India'),
        '#size' => 100,
        '#attributes' => array(
            'placeholder' => t('Enter your state/region name')
        ),
        '#states' => array(
            'visible' => array(
                ':input[name="country"]' => array(
                    'value' => 'Others'
                )
            )
        )
    );
    $form['other_city'] = array(
        '#type' => 'textfield',
        '#title' => t('City other than India'),
        '#size' => 100,
        '#attributes' => array(
            'placeholder' => t('Enter your city name')
        ),
        '#states' => array(
            'visible' => array(
                ':input[name="country"]' => array(
                    'value' => 'Others'
                )
            )
        )
    );
    $form['all_state'] = array(
        '#type' => 'select',
        '#title' => t('State'),
        '#options' => _df_list_of_states(),
        '#validated' => TRUE,
        '#states' => array(
            'visible' => array(
                ':input[name="country"]' => array(
                    'value' => 'India'
                )
            )
        )
    );
    $form['city'] = array(
        '#type' => 'select',
        '#title' => t('City'),
        '#options' => _df_list_of_cities(),
        '#states' => array(
            'visible' => array(
                ':input[name="country"]' => array(
                    'value' => 'India'
                )
            )
        )
    );
    $form['pincode'] = array(
        '#type' => 'textfield',
        '#title' => t('Pincode'),
        '#size' => 30,
        '#maxlength' => 6,
        '#required' => False,
        '#attributes' => array(
            'placeholder' => 'Enter pincode....'
        )
    );
    /***************************************************************************/
    $form['hr'] = array(
        '#type' => 'item',
        '#markup' => '<hr>'
    );
   
    $form['project_title'] = array(
        '#type' => 'textarea',
        '#title' => t('Project Title'),
        '#size' => 250,
        '#description' => t('Maximum character limit is 250'),
        '#required' => TRUE
    );
     $form['version'] = array(
        '#type' => 'hidden',
        
        '#value' => 'Not available',
        
    );
    $form['version'] = array(
        '#type' => 'select',
        '#title' => t('Version'),
        '#options' => _df_list_of_software_version(),
        '#required' => TRUE
    );
    $form['older'] = array(
        '#type' => 'textfield',
        '#title' => t('Other Version'),
        '#size' => 30,
        '#maxlength' => 50,
        //'#required' => TRUE,
        '#description' => t('Specify the Older version used'),
        '#states' => array(
            'visible' => array(
                ':input[name="version"]' => array(
                    'value' => 'olderversion'
                )
            )
        )
    );
    $form['compound_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Name of compound for which process development is carried out'),
        '#size' => 50,
        '#description' => t('Maximum character limit is 50'),
        '#required' => TRUE
    );
    $form['compound_name'] = array(
        '#type' => 'select',
        '#title' => t('List of compounds from DWSIM Database used in process flowsheet'),
        '#multiple' => TRUE,
        '#options' => _df_list_of_dwsim_compound(),
        '#required' => TRUE
    );
    $form['ucompound'] = array(
		'#type' => 'checkbox',
		'#title' => t('Upload user defind compound'),
		'#options' => array("1" => "Upload user defind compound"),
	);
     $form['user_defined_compounds_used_in_process_flowsheetcompound_name'] = array(
        '#type' => 'textarea',
        '#title' => t('List of user defined compounds used in process flowsheet'),
        '#states' => array(
			'visible' => array(
				':input[name="ucompound"]' => array(
					'checked' => True
				)
			)
		)
    );
    
    $form['upload_u_compound'] = array(
		'#type' => 'fieldset',
		'#title' => t('Upload user defind compound'),
		'#collapsible' => FALSE,
		'#collapsed' => FALSE,
		'#states' => array(
			'visible' => array(
				':input[name="ucompound"]' => array(
					'checked' => True
				)
			)
		)
	);
	$form['upload_u_compound']['upload_user_compound'] = array(
		'#type' => 'file',
		'#title' => t('Upload user defind compound'),
		'#size' => 48,
		'#description' => t('Separate filenames with underscore. No spaces or any special characters allowed in filename.') . '<br />' . t('<span style="color:red;">Allowed file extensions : ') . variable_get('dwsim_flowsheet_user_defind_compound_source_extensions', '') . '</span>',
		'#states' => array(
			'visible' => array(
				':input[name="ucompound"]' => array(
					'checked' => True
				)
			)
		)
	);
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit')
    );
    return $form;
  }
function dwsim_flowsheet_proposal_form_validate($form, &$form_state)
  {
    if (!preg_match('/^[0-9\ \+]{0,15}$/', $form_state['values']['contact_ph']))
      {
        form_set_error('contact_ph', t('Invalid contact phone number'));
      }
    if ($form_state['values']['country'] == 'Others')
      {
        if ($form_state['values']['other_country'] == '')
          {
            form_set_error('other_country', t('Enter country name'));
            // $form_state['values']['country'] = $form_state['values']['other_country'];
          }
        else
          {
            $form_state['values']['country'] = $form_state['values']['other_country'];
          }
        if ($form_state['values']['other_state'] == '')
          {
            form_set_error('other_state', t('Enter state name'));
            // $form_state['values']['country'] = $form_state['values']['other_country'];
          }
        else
          {
            $form_state['values']['all_state'] = $form_state['values']['other_state'];
          }
        if ($form_state['values']['other_city'] == '')
          {
            form_set_error('other_city', t('Enter city name'));
            // $form_state['values']['country'] = $form_state['values']['other_country'];
          }
        else
          {
            $form_state['values']['city'] = $form_state['values']['other_city'];
          }
      }
    else
      {
        if ($form_state['values']['country'] == '')
          {
            form_set_error('country', t('Select country name'));
            // $form_state['values']['country'] = $form_state['values']['other_country'];
          }
        if ($form_state['values']['all_state'] == '')
          {
            form_set_error('all_state', t('Select state name'));
            // $form_state['values']['country'] = $form_state['values']['other_country'];
          }
        if ($form_state['values']['city'] == '')
          {
            form_set_error('city', t('Select city name'));
            // $form_state['values']['country'] = $form_state['values']['other_country'];
          }
      }
    if ($form_state['values']['version'] == 'olderversion')
      {
        if ($form_state['values']['older'] == '')
          {
            form_set_error('older', t('Please provide valid version'));
          }
      }
    return;
  }
function dwsim_flowsheet_proposal_form_submit($form, &$form_state)
  {
    global $user;
    if (!$user->uid)
      {
        drupal_set_message('It is mandatory to login on this website to access the proposal form', 'error');
        return;
      }

    if ($form_state['values']['version'] == 'olderversion')
      {
        $form_state['values']['version'] = $form_state['values']['older'];
      }
    /* inserting the user proposal */
    $v = $form_state["values"];
    $lab_title = $v['lab_title'];
    $proposar_name = $v['name_title']. ' ' . $v['name'];
    $university = $v['university'];    
    $directory_name = _df_dir_name($lab_title, $proposar_name, $university);
    $result = "INSERT INTO {dwsim_flowsheet_proposal} 
    (uid, approver_uid, name_title, name, contact_ph, department, university, city, pincode, state, country, version, lab_title, approval_status, solution_status, solution_provider_uid, solution_display, creation_date, approval_date, solution_date, solution_provider_name_title, solution_provider_name, solution_provider_contact_ph, solution_provider_department, solution_provider_university, directory_name) VALUES
    (:uid, :approver_uid, :name_title, :name, :contact_ph, :department, :university, :city, :pincode, :state, :country,
     :version, :lab_title, :approval_status, :solution_status, :solution_provider_uid, :solution_display, :creation_date, 
     :approval_date, :solution_date, :solution_provider_name_title, :solution_provider_name,
      :solution_provider_contact_ph, :solution_provider_department, :solution_provider_university, :directory_name)";
    $args = array(
        ":uid" => $user->uid,
        ":approver_uid" => 0,
        ":name_title" => $v['name_title'],
        ":name" => $v['name'],
        ":contact_ph" => $v['contact_ph'],
        ":department" => $v['department'],
        ":university" => $v['university'],
        ":city" => $v['city'],
        ":pincode" => $v['pincode'],
        ":state" => $v['all_state'],
        ":country" => $v['country'],
        ":version" => $form_state['values']['version'],
        ":lab_title" => $v['lab_title'],
        ":approval_status" => 0,
        ":solution_status" => $solution_status,
        ":solution_provider_uid" => $solution_provider_uid,
        ":solution_display" => $solution_display,
        ":creation_date" => time(),
        ":approval_date" => 0,
        ":solution_date" => 0,
        ":solution_provider_name_title" => $solution_provider_name_title,
        ":solution_provider_name" => $solution_provider_name,
        ":solution_provider_contact_ph" => $solution_provider_contact_ph,
        ":solution_provider_department" => $solution_provider_department,
        ":solution_provider_university" => $solution_provider_university,
        ":directory_name" => $directory_name
    );
    $proposal_id = db_query($result, $args, array(
        'return' => Database::RETURN_INSERT_ID
    ));
    if (!$proposal_id)
      {
        drupal_set_message(t('Error receiving your proposal. Please try again.'), 'error');
        return;
      }

    /* sending email */
    $email_to = $user->mail;
    $from = variable_get('dwsim_flowsheet_from_email', '');
    $bcc = variable_get('dwsim_flowsheet_emails', '');
    $cc = variable_get('dwsim_flowsheet_cc_emails', '');
    $param['proposal_received']['proposal_id'] = $proposal_id;
    $param['proposal_received']['user_id'] = $user->uid;
    $param['proposal_received']['headers'] = array(
        'From' => $from,
        'MIME-Version' => '1.0',
        'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
        'Content-Transfer-Encoding' => '8Bit',
        'X-Mailer' => 'Drupal',
        'Cc' => $cc,
        'Bcc' => $bcc
    );
    if (!drupal_mail('dwsim_flowsheet', 'proposal_received', $email_to, user_preferred_language($user), $param, $from, TRUE))
        drupal_set_message('Error sending email message.', 'error');
    drupal_set_message(t('We have received you Lab migration proposal. We will get back to you soon.'), 'status');
    drupal_goto('');
  }
function _df_list_of_states()
  {
    $states = array(0 => '-Select-');
    $query = db_select('list_states_of_india');
    $query->fields('list_states_of_india');
    //$query->orderBy('', '');
    $states_list = $query->execute();
    while ($states_list_data = $states_list->fetchObject())
      {
        $states[$states_list_data->state] = $states_list_data->state;
      }
    return $states;
  }
function _df_list_of_cities()
  {
    $city = array(0 => '-Select-');
    $query = db_select('list_cities_of_india');
    $query->fields('list_cities_of_india');
    $query->orderBy('city', 'ASC');
    $city_list = $query->execute();
    while ($city_list_data = $city_list->fetchObject())
      {
        $city[$city_list_data->city] = $city_list_data->city;
      }
    return $city;
  }
function _df_list_of_departments()
  {
    $department = array();
    $query = db_select('list_of_departments');
    $query->fields('list_of_departments');
    $query->orderBy('id', 'DESC');
    $department_list = $query->execute();
    while ($department_list_data = $department_list->fetchObject())
      {
        $department[$department_list_data->department] = $department_list_data->department;
      }
    return $department;
  }
function _df_list_of_software_version()
  {
    $software_version = array();
    $query = db_select('dwsim_software_version');
    $query->fields('dwsim_software_version');
    $query->orderBy('id', 'DESC');
    $software_version_list = $query->execute();
    while ($software_version_list_data = $software_version_list->fetchObject())
      {
        $software_version[$software_version_list_data->dwsim_version] = $software_version_list_data->dwsim_version;
      }
    return $software_version;
  }
function _df_dir_name($lab, $name, $university)
  {
    $lab_title = ucname($lab);
    $proposar_name = ucname($lab);
    $university_name = ucname($university);
    $dir_name = $lab_title . " " . "by". " " . $proposar_name . ' ' . $university_name;
    $directory_name = str_replace("__", "_", str_replace(" ", "_", $dir_name));
    return $directory_name;
  }
