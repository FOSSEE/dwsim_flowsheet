<?php
// $Id$
/*
Approval Status :
0 - Pending
1 - Approved
2 - Dis-Approved
3 - Completed
Solution Status :
0 - Pending
1 - Approved
2 - Dis-Approved
Solution Display :
0 - No
1 - Yes


is_completed
0 - in progress
1 - Completed
Tables :
dwsim_flowsheet_solution : approval_status
0 - Pending
1 - Approved
2 - Disapproved (delete it)
*/
function dwsim_flowsheet_proposal_form($form, &$form_state, $no_js_use = FALSE)
{
	global $user;
	/************************ start approve book details ************************/
	if ($user->uid == 0)
	{
		$msg = drupal_set_message(t('It is mandatory to ' . l('login', 'user') . ' on this website to access the flowsheet proposal form. If you are new user please create a new account first.'), 'error');
		//drupal_goto('dwsim-flowsheet-project');
		drupal_goto('user');
		return $msg;
	} //$user->uid == 0
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
		} //$proposal_data->approval_status == 0 || $proposal_data->approval_status == 1
	} //$proposal_data
	$imp = t('<span style="color: red;">*This is a mandatory field</span>');
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
	$form['contributor_name'] = array(
		'#type' => 'textfield',
		'#title' => t('Name of the contributor'),
		'#size' => 250,
		'#attributes' => array(
			'placeholder' => t('Enter your full name.....')
		),
		'#maxlength' => 250,
		'#required' => TRUE
	);
	$form['contributor_contact_no'] = array(
		'#type' => 'textfield',
		'#title' => t('Contact No.'),
		'#size' => 10,
		'#attributes' => array(
			'placeholder' => t('Enter your contact number')
		),
		'#maxlength' => 250
	);
	$form['gender'] = array(
		'#type' => 'select',
		'#title' => t('Gender'),
		'#options' => array(
			'Male' => 'Male',
			'Female' => 'Female',
			'Other' => 'Other'
		),
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
		'#date_year_range' => '1960: +22',
		
		'#required' => TRUE
	);
	$form['contributor_email_id'] = array(
		'#type' => 'textfield',
		'#title' => t('Email'),
		'#size' => 30,
		'#value' => $user->mail,
		'#disabled' => TRUE
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
	$form['project_guide_name'] = array(
		'#type' => 'textfield',
		'#title' => t('Project guide'),
		'#size' => 250,
		'#attributes' => array(
			'placeholder' => t('Enter full name of project guide')
		),
		'#maxlength' => 250
	);
	$form['project_guide_email_id'] = array(
		'#type' => 'textfield',
		'#title' => t('Project guide email'),
		'#size' => 30
	);
	$form['project_guide_university'] = array(
		'#type' => 'textfield',
		'#title' => t('Project Guide University/ Institute'),
		'#size' => 80,
		'#maxlength' => 200,
		'#attributes' => array(
			'placeholder' => 'Insert full name of the institute/ university of your project guide.... '
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
		),
		'#description' => $imp,
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
		),
		'#description' => $imp,
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
		),
		'#description' => $imp,
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
		),
		'#description' => $imp
	);
	$form['city'] = array(
            '#type' => 'select',
            '#title' => t('City'),
            '#options' => _df_list_of_cities(),
            '#validated' => TRUE,
            '#states' => array(
                'visible' => array(
                    ':input[name="country"]' => array(
                        'value' => 'India'
                    )
                )
            ),
            '#description' => $imp,
	);
	$form['pincode'] = array(
		'#type' => 'textfield',
		'#title' => t('Pincode'),
		'#size' => 30,
		'#maxlength' => 6,
		'#required' => False,
		'#attributes' => array(
			'placeholder' => 'Enter pincode....'
		),
		'#required' => TRUE
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
	$form['reference'] = array(
		'#type' => 'textfield',
		'#title' => t('Reference'),
		'#maxsize' => 250,
		'#required' => TRUE,
		'#attributes' => array(
			'placeholder' => 'Enter reference'
		)
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
		'#description' => t('Specify the Older version used as format "DWSIM v2.0"'),
		'#states' => array(
			'visible' => array(
				':input[name="version"]' => array(
					'value' => 'Old version'
				)
			)
		)
	);
	$form['process_development_compound_name'] = array(
		'#type' => 'textfield',
		'#title' => t('Name of compound for which process development is carried out'),
		'#size' => 50,
		'#description' => t('Mention the compound name as shown:
Ex: Ethanol'),
		'#required' => TRUE
	);
	$form['process_development_compound_cas_no'] = array(
		'#type' => 'textfield',
		'#title' => t('CAS number for compound which process development is carried out'),
		'#size' => 50,
		'#description' => t('Mention the compound CAS No as shown:
Ex: 64-17-5'),
		'#required' => TRUE
	);
	$form['dwsim_database_compound_name'] = array(
		'#type' => 'select',
		'#title' => t('List of compounds from DWSIM Database used in process flowsheet'),
		'#multiple' => TRUE,
		'#size' => '20',
		'#description' => t('Select  all the compounds used in flowsheet which are available in above DWSIM compound list [You can select multiple options by holding ctrl + left key of mouse]'),
		'#options' => _df_list_of_dwsim_compound(),
		'#required' => TRUE
	);
	$form['ucompound'] = array(
		'#type' => 'checkbox',
		'#title' => t('Is user defined compound used?')
	);
	/*$form['user_defined_compounds_used_in_process_flowsheetcompound_name'] = array(
	'#type' => 'textarea',
	'#title' => t('List of user defined compounds used in process flowsheet'),
	'#states' => array(
	'visible' => array(
	':input[name="ucompound"]' => array(
	'checked' => True
	)
	)
	)
	);*/
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
	$form['upload_u_compound']['udc_field1_fieldset'] = array(
		'#type' => 'fieldset',
		'#tree' => TRUE,
		'#prefix' => '<div id="udc-field1-fieldset-wrapper">',
		'#suffix' => '</div>',
		'#states' => array(
			'visible' => array(
				':input[name="ucompound"]' => array(
					'checked' => True
				)
			)
		)
	);
	if (empty($form_state['user_defined_compound_num']))
	{
		$form_state['user_defined_compound_num'] = 1;
	} //empty($form_state['user_defined_compound_num'])
	$udc_temp1 = 0;
	for ($udc_i = 0; $udc_i < $form_state['user_defined_compound_num']; $udc_i++)
	{
		$udc_temp1 = $udc_i;
		$form['upload_u_compound']['udc_field1_fieldset'][$udc_i]["compound"] = array(
			"#type" => "textfield",
			"#title" => "Name of User defined compound " . ($udc_temp1 + 1),
			"#default_value" => ""
		);
		$form['upload_u_compound']['udc_field1_fieldset'][$udc_i]["cas_no"] = array(
			"#type" => "textfield",
			"#title" => "CAS Number of User defined compound " . ($udc_temp1 + 1),
			"#default_value" => ""
		);
	} //$i = 0; $i < $form_state['step1_num_compound']; $i++
	$form['upload_u_compound']['udc_field1_fieldset']["udc_compound_count"] = array(
		"#type" => "hidden",
		"#value" => $udc_temp1
	);
	$form['upload_u_compound']['udc_field1_fieldset']['add_compound'] = array(
		'#type' => 'submit',
		'#value' => t('Add more compound'),
		'#limit_validation_errors' => array(),
		'#submit' => array(
			'udc_compound_add_more_add_one'
		),
		'#ajax' => array(
			'callback' => 'udc_compound_add_more_callback',
			'wrapper' => array(
				'udc-field1-fieldset-wrapper'
			)
		)
	);
	if ($form_state['user_defined_compound_num'] > 1)
	{
		$form['upload_u_compound']['udc_field1_fieldset']['remove_compound'] = array(
			'#type' => 'submit',
			'#value' => t('Remove compound'),
			'#limit_validation_errors' => array(),
			'#submit' => array(
				'udc_compound_add_more_remove_one'
			),
			'#ajax' => array(
				'callback' => 'udc_compound_add_more_callback',
				'wrapper' => array(
					'udc-field1-fieldset-wrapper'
				)
			)
		);
	} //$form_state['step1_num_compound'] > 1
	if ($no_js_use)
	{
		if (!empty($form['upload_u_compound']['udc_field1_fieldset']['remove_compound']['#ajax']))
		{
			unset($form['upload_u_compound']['udc_field1_fieldset']['remove_compound']['#ajax']);
		} //!empty($form['udc_field1_fieldset']['remove_compound']['#ajax'])
		unset($form['upload_u_compound']['udc_field1_fieldset']['add_compound']['#ajax']);
	} //$no_js_use
	$form['upload_u_compound']['upload_user_compound'] = array(
		'#type' => 'file',
		'#title' => t('Upload user defined compound'),
		'#description' => t('Separate filenames with underscore. No spaces or any special characters allowed in filename.') . '<br />' . t('<span style="color:red;">Allowed file extensions : ') . variable_get('dwsim_flowsheet_user_defind_compound_source_extensions', '') . '</span>',
		'#states' => array(
			'visible' => array(
				':input[name="ucompound"]' => array(
					'checked' => True
				)
			)
		)
	);
	$form['term_condition'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Terms And Conditions'),
		'#options' => array(
			'status' => t('<a href="/term-and-conditions" target="_blank">I agree to the Terms and Conditions</a>')
		),
		'#required' => TRUE
	);
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);
	return $form;
}
/***************************************/
function udc_compound_add_more_add_one($form, &$form_state)
{
	$form_state['user_defined_compound_num']++;
	$form_state['rebuild'] = TRUE;
	//$form_state['no_redirect'] = TRUE;
}
function udc_compound_add_more_remove_one($form, &$form_state)
{
	if ($form_state['user_defined_compound_num'] > 1)
	{
		$form_state['user_defined_compound_num']--;
	} //$form_state['user_defined_compound_num'] > 1
	$form_state['rebuild'] = TRUE;
}
function udc_compound_add_more_callback($form, &$form_state)
{
	return $form['upload_u_compound']['udc_field1_fieldset'];
}
/***************************************/
function dwsim_flowsheet_proposal_form_validate($form, &$form_state)
{
	$project_title = $form_state['values']['project_title'];
	$query = db_select('dwsim_flowsheet_proposal');
	$query->fields('dwsim_flowsheet_proposal');
	$query->condition('project_title', $project_title);
	$result = $query->execute()->rowCount();
	if ($result >= 1)
	{
		form_set_error('project_title', t('Project title name already exists'));
		return;
	}
	if ($form_state['values']['term_condition'] == '1')
	{
		form_set_error('term_condition', t('Please check the terms and conditions'));
		// $form_state['values']['country'] = $form_state['values']['other_country'];
	} //$form_state['values']['term_condition'] == '1'
	if ($form_state['values']['country'] == 'Others')
	{
		if ($form_state['values']['other_country'] == '')
		{
			form_set_error('other_country', t('Enter country name'));
			// $form_state['values']['country'] = $form_state['values']['other_country'];
		} //$form_state['values']['other_country'] == ''
		else
		{
			$form_state['values']['country'] = $form_state['values']['other_country'];
		}
		if ($form_state['values']['other_state'] == '')
		{
			form_set_error('other_state', t('Enter state name'));
			// $form_state['values']['country'] = $form_state['values']['other_country'];
		} //$form_state['values']['other_state'] == ''
		else
		{
			$form_state['values']['all_state'] = $form_state['values']['other_state'];
		}
		if ($form_state['values']['other_city'] == '')
		{
			form_set_error('other_city', t('Enter city name'));
			// $form_state['values']['country'] = $form_state['values']['other_country'];
		} //$form_state['values']['other_city'] == ''
		else
		{
			$form_state['values']['city'] = $form_state['values']['other_city'];
		}
	} //$form_state['values']['country'] == 'Others'
	else
	{
		if ($form_state['values']['country'] == '0')
		{
			form_set_error('country', t('Select country name'));
			// $form_state['values']['country'] = $form_state['values']['other_country'];
		} //$form_state['values']['country'] == ''
		if ($form_state['values']['all_state'] == '0')
		{
			form_set_error('all_state', t('Select state name'));
			// $form_state['values']['country'] = $form_state['values']['other_country'];
		} //$form_state['values']['all_state'] == ''
		if ($form_state['values']['city'] == '0')
		{
			form_set_error('city', t('Select city name'));
			// $form_state['values']['country'] = $form_state['values']['other_country'];
		} //$form_state['values']['city'] == ''
	}
	//Validation for project title
	$form_state['values']['project_title'] = trim($form_state['values']['project_title']);
	if ($form_state['values']['project_title'] != '')
	{
		if (strlen($form_state['values']['project_title']) > 250)
		{
			form_set_error('project_title', t('Maximum charater limit is 250 charaters only, please check the length of the project title'));
		} //strlen($form_state['values']['project_title']) > 250
		else if (strlen($form_state['values']['project_title']) < 10)
		{
			form_set_error('project_title', t('Minimum charater limit is 10 charaters, please check the length of the project title'));
			
		}
		else if(preg_match('/[\^£$%&*()}{@#~?><>.:;`|=_+¬]/', $form_state['values']['project_title']))
			form_set_error('project_title', t('Special characters are not allowed for project title'));
	 //strlen($form_state['values']['project_title']) < 10
	} //$form_state['values']['project_title'] != ''
	else
	{
		form_set_error('project_title', t('Project title shoud not be empty'));
	}
	// validation for Name of compound for which process development is carried out
	$form_state['values']['process_development_compound_name'] = trim($form_state['values']['process_development_compound_name']);
	if ($form_state['values']['process_development_compound_name'] != '')
	{
		if (strlen($form_state['values']['process_development_compound_name']) >= 50)
		{
			form_set_error('process_development_compound_name', t('Maximum charater limit is 50 charaters only, please check the length'));
		} //strlen($form_state['values']['process_development_compound_name']) >= 50
		else if (strlen($form_state['values']['process_development_compound_name']) < 1)
		{
			form_set_error('process_development_compound_name', t('Minimum charater limit is 1 charaters, please check the length'));
		} //strlen($form_state['values']['process_development_compound_name']) < 1
	} //$form_state['values']['process_development_compound_name'] != ''
	else
	{
		form_set_error('process_development_compound_name', t('Field should not be empty'));
	}
	$form_state['values']['process_development_compound_cas_no'] = trim($form_state['values']['process_development_compound_cas_no']);
	if ($form_state['values']['process_development_compound_cas_no'] != '')
	{
		if (strlen($form_state['values']['process_development_compound_cas_no']) < 1)
		{
			form_set_error('process_development_compound_cas_no', t('Minimum charater limit is 1 charaters, please check the length'));
		} //strlen($form_state['values']['process_development_compound_cas_no']) < 1
	} //$form_state['values']['process_development_compound_cas_no'] != ''
	else
	{
		form_set_error('process_development_compound_cas_no', t('CAS number field should not be empty'));
	}
	if ($form_state['values']['version'] == 'Old version')
	{
		if ($form_state['values']['older'] == '')
		{
			form_set_error('older', t('Please provide valid version'));
		} //$form_state['values']['older'] == ''
	} //$form_state['values']['version'] == 'Old version'
	if ($form_state['values']['dwsim_database_compound_name'])
	{
		$dwsim_database_compound_name = implode("| ", $_POST['dwsim_database_compound_name']);
		$form_state['values']['dwsim_database_compound_name'] = trim($dwsim_database_compound_name);
	} //$form_state['values']['dwsim_database_compound_name']
	if ($form_state['values']['ucompound'] == 1)
	{
		if (isset($_FILES['files']))
		{
			/* check if atleast one source or result file is uploaded */
			if (!($_FILES['files']['name']['upload_user_compound']))
				form_set_error('upload_user_compound', t('Please upload a file.'));
			/* check for valid filename extensions */
			foreach ($_FILES['files']['name'] as $file_form_name => $file_name)
			{
				if ($file_name)
				{
					/* checking file type */
					if (strstr($file_form_name, 'user_compound'))
						$file_type = 'S';
					else
						$file_type = 'U';
					$allowed_extensions_str = '';
					switch ($file_type)
					{
						case 'S':
							$allowed_extensions_str = variable_get('dwsim_flowsheet_user_defind_compound_source_extensions', '');
							break;
					} //$file_type
					$allowed_extensions = explode(',', $allowed_extensions_str);
					$fnames = explode('.', strtolower($_FILES['files']['name'][$file_form_name]));
					$temp_extension = end($fnames);
					if (!in_array($temp_extension, $allowed_extensions))
						form_set_error($file_form_name, t('Only file with ' . $allowed_extensions_str . ' extensions can be uploaded.'));
					if ($_FILES['files']['size'][$file_form_name] <= 0)
						form_set_error($file_form_name, t('File size cannot be zero.'));
					/* check if valid file name */
					if (!textbook_companion_check_valid_filename($_FILES['files']['name'][$file_form_name]))
						form_set_error($file_form_name, t('Invalid file name specified. Only alphabets and numbers are allowed as a valid filename.'));
				} //$file_name
			} //$_FILES['files']['name'] as $file_form_name => $file_name
		} //isset($_FILES['files'])
	} //$form_state['values']['ucompound'] == 1
	return;
}
function dwsim_flowsheet_proposal_form_submit($form, &$form_state)
{
	global $user;
	if (!$user->uid)
	{
		drupal_set_message('It is mandatory to login on this website to access the proposal form', 'error');
		return;
	} //!$user->uid
	if ($form_state['values']['version'] == 'Old version')
	{
		$form_state['values']['version'] = trim($form_state['values']['older']);
	} //$form_state['values']['version'] == 'Old version'
	/* inserting the user proposal */
	$v = $form_state["values"];
	$project_title = trim($v['project_title']);
	$proposar_name = $v['name_title'] . ' ' . $v['contributor_name'];
	$university = $v['university'];
	$month_year_of_degree = $v['month_year_of_degree'];
	$directory_name = _df_dir_name($project_title, $proposar_name);
	$result = "INSERT INTO {dwsim_flowsheet_proposal} 
    (
    uid, 
    approver_uid,
    name_title, 
    contributor_name,
    contact_no,
    gender,
    month_year_of_degree, 
    university,
    city, 
    pincode, 
    state, 
    country,
    version, 
    project_guide_name,
    project_guide_email_id,
    project_guide_university,
    project_title, 
    process_development_compound_name, 
    process_development_compound_cas_number,
    dwsim_database_compound_name,
    approval_status,
    is_completed, 
    dissapproval_reason,
    creation_date, 
    approval_date, 
    directory_name,
    user_defined_compound_filepath,
    reference
    ) VALUES
    (
    :uid, 
    :approver_uid, 
    :name_title, 
    :contributor_name, 
    :contact_no,
    :gender,
    :month_year_of_degree, 
    :university, 
    :city, 
    :pincode, 
    :state, 
    :country,
    :version, 
    :project_guide_name,
    :project_guide_email_id,
    :project_guide_university,
    :project_title, 
    :process_development_compound_name, 
    :process_development_compound_cas_number,
    :dwsim_database_compound_name,
    :approval_status,
    :is_completed, 
    :dissapproval_reason,
    :creation_date, 
    :approval_date, 
    :directory_name,
    :user_defined_compound_filepath,
    :reference
    )";
	$args = array(
		":uid" => $user->uid,
		":approver_uid" => 0,
		":name_title" => $v['name_title'],
		":contributor_name" => _df_sentence_case(trim($v['contributor_name'])),
		":contact_no" => $v['contributor_contact_no'],
		":gender" => $v['gender'],
		":month_year_of_degree" => $month_year_of_degree,
		":university" => _df_sentence_case($v['university']),
		":city" => $v['city'],
		":pincode" => $v['pincode'],
		":state" => $v['all_state'],
		":country" => $v['country'],
		":version" => $v['version'],
		":project_guide_name" => _df_sentence_case($v['project_guide_name']),
		":project_guide_email_id" => trim($v['project_guide_email_id']),
		":project_guide_university" => trim($v['project_guide_university']),
		":project_title" => $v['project_title'],
		":process_development_compound_name" => _df_sentence_case($v['process_development_compound_name']),
		":process_development_compound_cas_number" => $v['process_development_compound_cas_no'],
		":dwsim_database_compound_name" => trim($v['dwsim_database_compound_name']),
		":approval_status" => 0,
		":is_completed" => 0,
		":dissapproval_reason" => "NULL",
		":creation_date" => time(),
		":approval_date" => 0,
		":directory_name" => $directory_name,
		":user_defined_compound_filepath" => "NULL",
		":reference" => $v['reference']
	);
	//var_dump($args);die;
	$proposal_id = db_query($result, $args, array(
		'return' => Database::RETURN_INSERT_ID
	));
	if ($form_state['values']['ucompound'] == 1)
	{
		/* For adding compounds */
		$compounds = 0;
		for ($i = 0; $i <= $v['udc_field1_fieldset']["udc_compound_count"]; $i++)
		{
			if ($v['udc_field1_fieldset'][$i]["compound"] != "" && $v['udc_field1_fieldset'][$i]["cas_no"] != "")
			{
				$compoundquery = "
	INSERT INTO dwsim_flowsheet_user_defined_compound
	(proposal_id, user_defined_compound, cas_no, compound_type)
	VALUES
	(:proposal_id, :user_defined_compound, :cas_no, :compound_type)
	";
				$compoundargs = array(
					":proposal_id" => $proposal_id,
					":user_defined_compound" => trim($v['udc_field1_fieldset'][$i]["compound"]),
					":cas_no" => trim($v['udc_field1_fieldset'][$i]["cas_no"]),
					":compound_type" => 'U'
				);
				/* storing the row id in $result */
				$compoundresult = db_query($compoundquery, $compoundargs, array(
					'return' => Database::RETURN_INSERT_ID
				));
				if ($compoundresult != 0)
				{
					$compounds++;
				} //$compoundresult != 0
			} //$v['udc_field1_fieldset'][$i]["compound"] != ""
		} //$i = 0; $i <= $v['udc_field1_fieldset']["udc_compound_count"]; $i++
		$root_path = dwsim_flowsheet_document_path();
		$dest_path1 = $directory_name . '/';
		$dest_path2 = 'user_defined_compound/';
		$dest_path = $dest_path1 . 'user_defined_compound/';
		$db_path = 'user_defined_compound/';
		//var_dump($root_path . $dest_path1);die;
		if (!is_dir($root_path . $dest_path1))
			mkdir($root_path . $dest_path1);
		if (!is_dir($root_path . $dest_path))
			mkdir($root_path . $dest_path);
		/* uploading files */
		foreach ($_FILES['files']['name'] as $file_form_name => $file_name)
		{
			if ($file_name)
			{
				/* checking file type */
				$file_type = 'S';
				if (file_exists($root_path . $dest_path . $_FILES['files']['name'][$file_form_name]))
				{
					// drupal_set_message(t("Error uploading file. File !filename already exists.", array('!filename' => $_FILES['files']['name'][$file_form_name])), 'error');
					unlink($root_path . $dest_path . $_FILES['files']['name'][$file_form_name]);
				} //file_exists($root_path . $dest_path . $_FILES['files']['name'][$file_form_name])
				/* uploading file */
				if (move_uploaded_file($_FILES['files']['tmp_name'][$file_form_name], $root_path . $dest_path . $_FILES['files']['name'][$file_form_name]))
				{
					$query = "UPDATE {dwsim_flowsheet_proposal} SET user_defined_compound_filepath = :user_defined_compound_filepath WHERE id = :id";
					$args = array(
						":user_defined_compound_filepath" => $dest_path2 . $_FILES['files']['name'][$file_form_name],
						":id" => $proposal_id
					);
					$updateresult = db_query($query, $args);
					drupal_set_message($file_name . ' uploaded successfully.', 'status');
				} //move_uploaded_file($_FILES['files']['tmp_name'][$file_form_name], $root_path . $dest_path . $_FILES['files']['name'][$file_form_name])
				else
				{
					drupal_set_message('Error uploading file : ' . $db_path . '/' . $file_name, 'error');
				}
			} //$file_name
		} //$_FILES['files']['name'] as $file_form_name => $file_name
	} //$form_state['values']['ucompound'] == 1
	else
	{
		$file_name = "Not Uploaded";
	}
	if (!$proposal_id)
	{
		drupal_set_message(t('Error receiving your proposal. Please try again.'), 'error');
		return;
	} //!$proposal_id
	/* sending email */
	$email_to = $user->mail;
	$form = variable_get('dwsim_flowsheet_from_email', '');
	$bcc = variable_get('dwsim_flowsheet_emails', '');
	$cc = variable_get('dwsim_flowsheet_cc_emails', '');
	$params['dwsim_flowsheet_proposal_received']['proposal_id'] = $proposal_id;
	$params['dwsim_flowsheet_proposal_received']['user_id'] = $user->uid;
	$params['dwsim_flowsheet_proposal_received']['headers'] = array(
		'From' => $form,
		'MIME-Version' => '1.0',
		'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
		'Content-Transfer-Encoding' => '8Bit',
		'X-Mailer' => 'Drupal',
		'Cc' => $cc,
		'Bcc' => $bcc
	);
	if (!drupal_mail('dwsim_flowsheet', 'dwsim_flowsheet_proposal_received', $email_to, user_preferred_language($user), $params, $form, TRUE))
		drupal_set_message('Error sending email message.', 'error');
	drupal_set_message(t('We have received your DWSIM Flowsheeting proposal. We will get back to you soon.'), 'status');
	drupal_goto('');
}
