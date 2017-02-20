<?php
// $Id$
function dwsim_flowsheet_abstract()
{
	global $user;
	$proposal_data = dwsim_flowsheet_get_proposal();
	if (!$proposal_data)
	{
		drupal_goto('');
		return;
	} //!$proposal_data
	$return_html = '<strong>Title of the Flowsheet Project:</strong><br />' . $proposal_data->project_title . '<br /><br />';
	$return_html .= '<strong>Proposer Name:</strong><br />' . $proposal_data->name_title . ' ' . $proposal_data->student_name . '<br /><br />';
	$return_html .= l('Upload abstract', 'flowsheeting-project/abstract-code/upload') . '<br />';
	/* get experiment list */
	$experiment_rows = array();
	//$experiment_q = db_query("SELECT * FROM {dwsim_flowsheet_experiment} WHERE proposal_id = %d ORDER BY number ASC", $proposal_data->id);
	$query = db_select('dwsim_flowsheet_experiment');
	$query->fields('dwsim_flowsheet_experiment');
	$query->condition('proposal_id', $proposal_data->id);
	$query->orderBy('number', 'ASC');
	$experiment_q = $query->execute();
	$experiment_header = array(
		'No. Uploaded File',
		'Type',
		'Status',
		'Actions'
	);
	// $return_html .= theme_table($experiment_header, $experiment_rows);
	$return_html .= theme('table', array(
		'header' => $experiment_header,
		'rows' => $experiment_rows
	));
	return $return_html;
}
function dwsim_flowsheet_upload_abstract_code_form($form, &$form_state)
{
	global $user;
	$form['#attributes'] = array(
		'enctype' => "multipart/form-data"
	);
	/* get current proposal */
	//$proposal_id = (int) arg(3);
	$uid = $user->uid;
	//$proposal_q = db_query("SELECT * FROM {dwsim_flowsheet_proposal} WHERE id = %d", $proposal_id);
	$query = db_select('dwsim_flowsheet_proposal');
	$query->fields('dwsim_flowsheet_proposal');
	$query->condition('uid', $uid);
	$query->condition('approval_status', '1');
	$proposal_q = $query->execute();
	if ($proposal_q)
	{
		if ($proposal_data = $proposal_q->fetchObject())
		{
			/* everything ok */
		} //$proposal_data = $proposal_q->fetchObject()
		else
		{
			drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
			drupal_goto('flowsheeting-project/abstract-code');
			return;
		}
	} //$proposal_q
	else
	{
		drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
		drupal_goto('flowsheeting-project/abstract-code');
		return;
	}
	$form['project_title'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->project_title,
		'#title' => t('Title of the Flowsheet Project')
	);
	$form['version'] = array(
		'#type' => 'item',
		'#title' => t('DWSIM version'),
		'#markup' => $proposal_data->version
	);
	$form['unit_operations_used_in_dwsim'] = array(
		'#type' => 'select',
		'#title' => t('Unit Operations used in DWSIM'),
		'#options' => _df_list_of_unit_operations(),
		'#required' => TRUE,
		'#multiple' => TRUE,
		'#description' => t('[You can select multiple options by holding ctrl + left key of mouse]')
	);
	$form['thermodynamic_packages_used'] = array(
		'#type' => 'select',
		'#title' => t('Thermodynamic Packages Used'),
		'#options' => _df_list_of_thermodynamic_packages(),
		'#required' => TRUE,
		'#multiple' => TRUE,
		'#description' => t('[You can select multiple options by holding ctrl + left key of mouse]')
	);
	$form['logical_blocks_used'] = array(
		'#type' => 'select',
		'#title' => t('Logical Blocks used (If any)'),
		'#options' => _df_list_of_logical_block(),
		'#required' => TRUE,
		'#multiple' => TRUE,
		'#description' => t('[You can select multiple options by holding ctrl + left key of mouse]')
	);
	$form['process_development_compound_name'] = array(
		'#type' => 'textfield',
		'#title' => t('Name of compound for which process development is carried out'),
		'#size' => 50,
		'#description' => t('Mention the compound name with CAS No as shown below:
Ex: Ethanol (64-17-5)')
	);
	$form['list_of_compounds_from_dwsim_database_used_in_process_flowsheet'] = array(
		'#type' => 'select',
		'#title' => t('List of compounds from DWSIM Database used in process flowsheet'),
		'#options' => _df_list_of_dwsim_compound(),
		'#multiple' => TRUE,
		'#description' => t('[You can select multiple options by holding ctrl + left key of mouse]')
	);
	$form['user_defined_compounds_used_in_process_flowsheetcompound_name'] = array(
		'#type' => 'textarea',
		'#title' => t('List of user defined compounds used in process flowsheet'),
		'#required' => FALSE,
		'#description' => 'List all user defined compounds with CAS No. as in format - Ex: Ethylene (74-85-1)'
	);
	$form['upload_an_abstract'] = array(
		'#type' => 'file',
		'#title' => t('Upload an abstract (brief outline) of the project.'),
		'#description' => t('Separate filenames with underscore. No spaces or any special characters allowed in filename.') . '<br />' . t('<span style="color:red;">Allowed file extensions : ') . variable_get('dwsim_flowsheet_abstract_upload_extensions', '') . '</span>'
	);
	$form['upload_flowsheet_developed_process'] = array(
		'#type' => 'file',
		'#title' => t('Upload the DWSIM flowsheet for the developed process.'),
		'#description' => t('Separate filenames with underscore. No spaces or any special characters allowed in filename.') . '<br />' . t('<span style="color:red;">Allowed file extensions : ') . variable_get('dwsim_flowsheet_flowsheet_developed_process_source_extensions', '') . '</span>'
	);
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);
	$form['cancel'] = array(
		'#type' => 'item',
		'#markup' => l(t('Cancel'), 'flowsheeting-project/manage-proposal')
	);
	return $form;
}
function dwsim_flowsheet_upload_abstract_code_form_validate($form, &$form_state)
{
	if ($form_state['values']['unit_operations_used_in_dwsim'])
	{
		$unit_operations_used_in_dwsim = implode(", ", $_POST['unit_operations_used_in_dwsim']);
		$form_state['values']['unit_operations_used_in_dwsim'] = $unit_operations_used_in_dwsim;
	} //$form_state['values']['unit_operations_used_in_dwsim']
	if ($form_state['values']['thermodynamic_packages_used'])
	{
		$thermodynamic_packages_used = implode(", ", $_POST['thermodynamic_packages_used']);
		$form_state['values']['thermodynamic_packages_used'] = $thermodynamic_packages_used;
	} //$form_state['values']['thermodynamic_packages_used']
	if ($form_state['values']['logical_blocks_used'])
	{
		$logical_blocks_used = implode(", ", $_POST['logical_blocks_used']);
		$form_state['values']['logical_blocks_used'] = $logical_blocks_used;
	} //$form_state['values']['logical_blocks_used']
	if ($form_state['values']['list_of_compounds_from_dwsim_database_used_in_process_flowsheet'])
	{
		$list_of_compounds_from_dwsim_database_used_in_process_flowsheet = implode(", ", $_POST['list_of_compounds_from_dwsim_database_used_in_process_flowsheet']);
		$form_state['values']['list_of_compounds_from_dwsim_database_used_in_process_flowsheet'] = $list_of_compounds_from_dwsim_database_used_in_process_flowsheet;
	} //$form_state['values']['list_of_compounds_from_dwsim_database_used_in_process_flowsheet']
	if (isset($_FILES['files']))
	{
		/* check if file is uploaded */
		if (!($_FILES['files']['name']['upload_flowsheet_developed_process']))
			form_set_error('upload_flowsheet_developed_process', t('Please upload the file.'));
		if (!($_FILES['files']['name']['upload_an_abstract']))
			form_set_error('upload_an_abstract', t('Please upload the file.'));
		/* check for valid filename extensions */
		foreach ($_FILES['files']['name'] as $file_form_name => $file_name)
		{
			if ($file_name)
			{
				/* checking file type */
				if (strstr($file_form_name, 'upload_flowsheet_developed_process'))
					$file_type = 'S';
				else if (strstr($file_form_name, 'upload_an_abstract'))
					$file_type = 'A';
				else
					$file_type = 'U';
				$allowed_extensions_str = '';
				switch ($file_type)
				{
					case 'S':
						$allowed_extensions_str = variable_get('dwsim_flowsheet_flowsheet_developed_process_source_extensions', '');
						break;
					case 'A':
						$allowed_extensions_str = variable_get('dwsim_flowsheet_abstract_upload_extensions', '');
						break;
				} //$file_type
				$allowed_extensions = explode(',', $allowed_extensions_str);
				$tmp_ext = explode('.', strtolower($_FILES['files']['name'][$file_form_name]));
				$temp_extension = end($tmp_ext);
				if (!in_array($temp_extension, $allowed_extensions))
					form_set_error($file_form_name, t('Only file with ' . $allowed_extensions_str . ' extensions can be uploaded.'));
				if ($_FILES['files']['size'][$file_form_name] <= 0)
					form_set_error($file_form_name, t('File size cannot be zero.'));
				/* check if valid file name */
				if (!dwsim_flowsheet_check_valid_filename($_FILES['files']['name'][$file_form_name]))
					form_set_error($file_form_name, t('Invalid file name specified. Only alphabets and numbers are allowed as a valid filename.'));
			} //$file_name
		} //$_FILES['files']['name'] as $file_form_name => $file_name
	} //isset($_FILES['files'])
	// drupal_add_js('jQuery(document).ready(function () { alert("Hello!"); });', 'inline');
	// drupal_static_reset('drupal_add_js') ;
}
function dwsim_flowsheet_upload_abstract_code_form_submit($form, &$form_state)
{
	global $user;
	$v = $form_state['values'];
	$root_path = dwsim_flowsheet_path();
	$proposal_data = dwsim_flowsheet_get_proposal();
	$proposal_id = $proposal_data->id;
	if (!$proposal_data)
	{
		drupal_goto('');
		return;
	} //!$proposal_data
	$proposal_id = $proposal_data->id;
	$proposal_drectory = $proposal_data->directory_name;
	/* create proposal folder if not present */
	$dest_path = $proposal_drectory . '/';
	if (!is_dir($root_path . $dest_path))
		mkdir($root_path . $dest_path);
	/* creating solution database entry */
	$query = "INSERT INTO {dwsim_flowsheet_submitted_abstracts} (proposal_id, approver_uid, abstract_approval_status, unit_operations_used_in_dwsim, thermodynamic_packages_used, logical_blocks_used, process_development_compound_name, list_of_compounds_from_dwsim_database_used_in_process_flowsheet, user_defined_compounds_used_in_process_flowsheet, abstract_upload_date, abstract_approval_date) VALUES (:proposal_id, :approver_uid, :abstract_approval_status, :unit_operations_used_in_dwsim, 
  :thermodynamic_packages_used, :logical_blocks_used, :process_development_compound_name, 
  :list_of_compounds_from_dwsim_database_used_in_process_flowsheet, 
  :user_defined_compounds_used_in_process_flowsheet, :abstract_upload_date, :abstract_approval_date)";
	$args = array(
		":proposal_id" => $proposal_id,
		":approver_uid" => 0,
		":abstract_approval_status" => 0,
		":unit_operations_used_in_dwsim" => $v['unit_operations_used_in_dwsim'],
		":thermodynamic_packages_used" => $v['thermodynamic_packages_used'],
		":logical_blocks_used" => $v['logical_blocks_used'],
		":process_development_compound_name" => $v['process_development_compound_name'],
		":list_of_compounds_from_dwsim_database_used_in_process_flowsheet" => $v['list_of_compounds_from_dwsim_database_used_in_process_flowsheet'],
		":user_defined_compounds_used_in_process_flowsheet" => $v['user_defined_compounds_used_in_process_flowsheetcompound_name'],
		":abstract_upload_date" => time(),
		":abstract_approval_date" => 0
	);
	$submitted_abstract_id = db_query($query, $args, array(
		'return' => Database::RETURN_INSERT_ID
	));
	/* uploading files */
	foreach ($_FILES['files']['name'] as $file_form_name => $file_name)
	{
		if ($file_name)
		{
			/* checking file type */
			if (strstr($file_form_name, 'source'))
				$file_type = 'S';
			else if (strstr($file_form_name, 'result'))
				$file_type = 'R';
			else if (strstr($file_form_name, 'xcos'))
				$file_type = 'X';
			else
				$file_type = 'U';
			if (file_exists($root_path . $dest_path . $_FILES['files']['name'][$file_form_name]))
			{
				drupal_set_message(t("Error uploading file. File !filename already exists.", array(
					'!filename' => $_FILES['files']['name'][$file_form_name]
				)), 'error');
				return;
			} //file_exists($root_path . $dest_path . $_FILES['files']['name'][$file_form_name])
			/* uploading file */
			if (move_uploaded_file($_FILES['files']['tmp_name'][$file_form_name], $root_path . $dest_path . $_FILES['files']['name'][$file_form_name]))
			{
				/* for uploaded files making an entry in the database */
				$query = "INSERT INTO {dwsim_flowsheet_submitted_abstracts_file} (submitted_abstract_id, proposal_id,approvar_uid, filename, filepath, filemime, filesize, filetype, timestamp)
          VALUES (:submitted_abstract_id, :proposal_id,:approvar_uid, :filename, :filepath, :filemime, :filesize, :filetype, :timestamp)";
				$args = array(
					":submitted_abstract_id" => $submitted_abstract_id,
					":proposal_id" => $proposal_id,
					":approvar_uid" => 0,
					":filename" => $_FILES['files']['name'][$file_form_name],
					":filepath" => $file_path . $_FILES['files']['name'][$file_form_name],
					":filemime" => mime_content_type($root_path . $dest_path . $_FILES['files']['name'][$file_form_name]),
					":filesize" => $_FILES['files']['size'][$file_form_name],
					":filetype" => $file_type,
					":timestamp" => time()
				);
				db_query($query, $args);
				drupal_set_message($file_name . ' uploaded successfully.', 'status');
			} //move_uploaded_file($_FILES['files']['tmp_name'][$file_form_name], $root_path . $dest_path . $_FILES['files']['name'][$file_form_name])
			else
			{
				drupal_set_message('Error uploading file : ' . $dest_path . $file_name, 'error');
			}
		} //$file_name
	} //$_FILES['files']['name'] as $file_form_name => $file_name
	drupal_set_message('Solution uploaded successfully.', 'status');
	/* sending email */
	$email_to = $user->mail;
	$from = variable_get('dwsim_flowsheet_from_email', '');
	$bcc = variable_get('dwsim_flowsheet_emails', '');
	$cc = variable_get('dwsim_flowsheet_cc_emails', '');
	$params['solution_uploaded']['solution_id'] = $solution_id;
	$params['solution_uploaded']['user_id'] = $user->uid;
	$params['solution_uploaded']['headers'] = array(
		'From' => $from,
		'MIME-Version' => '1.0',
		'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
		'Content-Transfer-Encoding' => '8Bit',
		'X-Mailer' => 'Drupal',
		'Cc' => $cc,
		'Bcc' => $bcc
	);
	if (!drupal_mail('dwsim_flowsheet', 'solution_uploaded', $email_to, language_default(), $params, $from, TRUE))
		drupal_set_message('Error sending email message.', 'error');
	drupal_goto('flowsheeting-project/abstract-code');
}