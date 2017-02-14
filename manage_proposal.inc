<?php
// $Id$
function dwsim_flowsheet_proposal_pending()
{
	/* get pending proposals to be approved */
	$pending_rows = array();
	//$pending_q = db_query("SELECT * FROM {dwsim_flowsheet_proposal} WHERE approval_status = 0 ORDER BY id DESC");
	$query = db_select('dwsim_flowsheet_proposal');
	$query->fields('dwsim_flowsheet_proposal');
	$query->condition('approval_status', 0);
	$query->orderBy('id', 'DESC');
	$pending_q = $query->execute();
	while ($pending_data = $pending_q->fetchObject())
	{
		$pending_rows[$pending_data->id] = array(
			date('d-m-Y', $pending_data->creation_date),
			l($pending_data->name_title . ' ' . $pending_data->student_name, 'user/' . $pending_data->uid),
			$pending_data->project_title,
			l('Approve', 'flowsheeting-project/manage-proposal/approve/' . $pending_data->id) . ' | ' . l('Edit', 'flowsheeting-project/manage-proposal/edit/' . $pending_data->id)
		);
	} //$pending_data = $pending_q->fetchObject()
	/* check if there are any pending proposals */
	if (!$pending_rows)
	{
		drupal_set_message(t('There are no pending proposals.'), 'status');
		return '';
	} //!$pending_rows
	$pending_header = array(
		'Date of Submission',
		'Student Name',
		'Title of the Flowsheet Project',
		'Action'
	);
	//$output = theme_table($pending_header, $pending_rows);
	$output = theme('table', array(
		'header' => $pending_header,
		'rows' => $pending_rows
	));
	return $output;
}
function dwsim_flowsheet_proposal_pending_solution()
{
	/* get pending proposals to be approved */
	$pending_rows = array();
	//$pending_q = db_query("SELECT * FROM {dwsim_flowsheet_proposal} WHERE approval_status = 1 ORDER BY id DESC");
	$query = db_select('dwsim_flowsheet_proposal');
	$query->fields('dwsim_flowsheet_proposal');
	$query->condition('approval_status', 1);
	$query->orderBy('id', 'DESC');
	$pending_q = $query->execute();
	while ($pending_data = $pending_q->fetchObject())
	{
		$pending_rows[$pending_data->id] = array(
			date('d-m-Y', $pending_data->creation_date),
			date('d-m-Y', $pending_data->approval_date),
			l($pending_data->name, 'user/' . $pending_data->uid),
			$pending_data->project_title,
			$pending_data->department,
			l('Status', 'flowsheeting-project/manage-proposal/status/' . $pending_data->id)
		);
	} //$pending_data = $pending_q->fetchObject()
	/* check if there are any pending proposals */
	if (!$pending_rows)
	{
		drupal_set_message(t('There are no proposals pending for solutions.'), 'status');
		return '';
	} //!$pending_rows
	$pending_header = array(
		'Date of Submission',
		'Date of Approval',
		'Student name',
		'Title of the Flowsheet Project',
		'Department',
		'Action'
	);
	$output = theme('table', array(
		'header' => $pending_header,
		'rows' => $pending_rows
	));
	return $output;
}
function dwsim_flowsheet_proposal_all()
{
	/* get pending proposals to be approved */
	$proposal_rows = array();
	//$proposal_q = db_query("SELECT * FROM {dwsim_flowsheet_proposal} ORDER BY id DESC");
	$query = db_select('dwsim_flowsheet_proposal');
	$query->fields('dwsim_flowsheet_proposal');
	$query->orderBy('id', 'DESC');
	$proposal_q = $query->execute();
	while ($proposal_data = $proposal_q->fetchObject())
	{
		$approval_status = '';
		switch ($proposal_data->approval_status)
		{
			case 0:
				$approval_status = 'Pending';
				break;
			case 1:
				$approval_status = 'Approved';
				break;
			case 2:
				$approval_status = 'Dis-approved';
				break;
			case 3:
				$approval_status = 'Solved';
				break;
			default:
				$approval_status = 'Unknown';
				break;
		} //$proposal_data->approval_status
		$proposal_rows[] = array(
			date('d-m-Y', $proposal_data->creation_date),
			l($proposal_data->student_name, 'user/' . $proposal_data->uid),
			$proposal_data->project_title,
			$approval_status,
			l('Status', 'flowsheeting-project/manage-proposal/status/' . $proposal_data->id) . ' | ' . l('Edit', 'flowsheeting-project/manage-proposal/edit/' . $proposal_data->id)
		);
	} //$proposal_data = $proposal_q->fetchObject()
	/* check if there are any pending proposals */
	if (!$proposal_rows)
	{
		drupal_set_message(t('There are no proposals.'), 'status');
		return '';
	} //!$proposal_rows
	$proposal_header = array(
		'Date of Submission',
		'Student Name',
		'Title of the Lab',
		'Status',
		'Action'
	);
	$output = theme('table', array(
		'header' => $proposal_header,
		'rows' => $proposal_rows
	));
	return $output;
}
/******************************************************************************/
/************************** PROPOSAL APPROVAL FORM ****************************/
/******************************************************************************/
function dwsim_flowsheet_proposal_approval_form($form, &$form_state)
{
	global $user;
	/* get current proposal */
	$proposal_id = (int) arg(3);
	//$proposal_q = db_query("SELECT * FROM {dwsim_flowsheet_proposal} WHERE id = %d", $proposal_id);
	$query = db_select('dwsim_flowsheet_proposal');
	$query->fields('dwsim_flowsheet_proposal');
	$query->condition('id', $proposal_id);
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
			drupal_goto('flowsheeting-project/manage-proposal');
			return;
		}
	} //$proposal_q
	else
	{
		drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
		drupal_goto('flowsheeting-project/manage-proposal');
		return;
	}
	$form['student_name'] = array(
		'#type' => 'item',
		'#markup' => l($proposal_data->name_title . ' ' . $proposal_data->student_name, 'user/' . $proposal_data->uid),
		'#title' => t('Student name')
	);
	$form['student_email_id'] = array(
		'#title' => t('Student Email'),
		'#type' => 'item',
		'#markup' => user_load($proposal_data->uid)->mail,
		'#title' => t('Email')
	);
	$form['month_year_of_degree'] = array(
		'#type' => 'date_popup',
		'#title' => t('Month and year of award of degree'),
		'#date_label_position' => '',
		'#description' => '',
		'#default_value' => $proposal_data->month_year_of_degree,
		'#date_format' => 'M-Y',
		'#date_increment' => 0,
		'#date_year_range' => '1960:+0',
		'#datepicker_options' => array(
			'maxDate' => 0
		),
		'#disabled' => TRUE
	);
	$form['university'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->university,
		'#title' => t('University/Institute')
	);
	$form['country'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->country,
		'#title' => t('Country')
	);
	$form['all_state'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->state,
		'#title' => t('State')
	);
	$form['city'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->city,
		'#title' => t('City')
	);
	$form['pincode'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->pincode,
		'#title' => t('Pincode/Postal code')
	);
	$form['version'] = array(
		'#type' => 'item',
		'#title' => t('DWSIM version'),
		'#markup' => $proposal_data->version
	);
	$form['project_guide_name'] = array(
		'#type' => 'item',
		'#title' => t('Project guide'),
		'#markup' => $proposal_data->project_guide_name
	);
	$form['project_guide_email_id'] = array(
		'#type' => 'item',
		'#title' => t('Project guide email'),
		'#markup' => $proposal_data->project_guide_email_id
	);
	$form['project_title'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->project_title,
		'#title' => t('Title of the Flowsheet Project')
	);
	$form['process_development_compound_name'] = array(
		'#type' => 'item',
		'#title' => t('Name of compound for which process development is carried out'),
		'#markup' => $proposal_data->process_development_compound_name
	);
	$form['dwsim_database_compound_name'] = array(
		'#type' => 'item',
		'#title' => t('List of compounds from DWSIM Database used in process flowsheet'),
		'#markup' => $proposal_data->dwsim_database_compound_name
	);
	if ($proposal_data->user_defined_compounds_used_in_process != "" || $proposal_data->user_defined_compounds_used_in_process != NULL)
	{
		$form['user_defined_compounds_used_in_process_flowsheetcompound_name'] = array(
			'#type' => 'item',
			'#title' => t('List of user defined compounds used in process flowsheet'),
			'#markup' => $proposal_data->user_defined_compounds_used_in_process
		);
	} //$proposal_data->user_defined_compounds_used_in_process != "" || $proposal_data->user_defined_compounds_used_in_process != NULL
	else
	{
		$form['user_defined_compounds_used_in_process_flowsheetcompound_name'] = array(
			'#type' => 'item',
			'#title' => t('List of user defined compounds used in process flowsheet'),
			'#markup' => "Not entered"
		);
	}
	if ($proposal_data->user_defined_compound_filepath != "")
	{
		$form['user_defined_compound_filepath'] = array(
			'#type' => 'item',
			'#title' => t('Uploaded the user defined compound '),
			'#markup' => l('Download user defined compound list', 'flowsheeting-project/download/user-defined-compound-file/' . $proposal_id) . "<br><br>"
		);
	} //$proposal_data->user_defined_compound_filepath != ""
	else
	{
		$form['user_defined_compound_filepath'] = array(
			'#type' => 'item',
			'#title' => t('Uploaded the user defined compound '),
			'#markup' => "Not uploaded<br><br>"
		);
	}
	$form['approval'] = array(
		'#type' => 'radios',
		'#title' => t('DWSIM flowsheeting proposal'),
		'#options' => array(
			'1' => 'Approve',
			'2' => 'Disapprove'
		),
		'#required' => TRUE
	);
	$form['message'] = array(
		'#type' => 'textarea',
		'#title' => t('Reason for disapproval'),
		'#attributes' => array(
			'placeholder' => t('Enter reason for disapproval in minimum 30 characters '),
			'cols' => 50,
			'rows' => 4
		),
		'#states' => array(
			'visible' => array(
				':input[name="approval"]' => array(
					'value' => '2'
				)
			)
		)
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
function dwsim_flowsheet_proposal_approval_form_validate($form, &$form_state)
{
	if ($form_state['values']['approval'] == 2)
	{
		if ($form_state['values']['message'] == '')
		{
			form_set_error('message', t('Reason for disapproval could not be empty'));
		} //$form_state['values']['message'] == ''
	} //$form_state['values']['approval'] == 2
}
function dwsim_flowsheet_proposal_approval_form_submit($form, &$form_state)
{
	global $user;
	/* get current proposal */
	$proposal_id = (int) arg(3);
	// $proposal_q = db_query("SELECT * FROM {dwsim_flowsheet_proposal} WHERE id = %d", $proposal_id);
	$query = db_select('dwsim_flowsheet_proposal');
	$query->fields('dwsim_flowsheet_proposal');
	$query->condition('id', $proposal_id);
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
			drupal_goto('flowsheeting-project/manage-proposal');
			return;
		}
	} //$proposal_q
	else
	{
		drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
		drupal_goto('flowsheeting-project/manage-proposal');
		return;
	}
	if ($form_state['values']['approval'] == 1)
	{
		$query = "UPDATE {dwsim_flowsheet_proposal} SET approver_uid = :uid, approval_date = :date, approval_status = 1 WHERE id = :proposal_id";
		$args = array(
			":uid" => $user->uid,
			":date" => time(),
			":proposal_id" => $proposal_id
		);
		db_query($query, $args);
		/* sending email */
		$user_data = user_load($proposal_data->uid);
		$email_to = $user_data->mail;
		$from = variable_get('dwsim_flowsheet_from_email', '');
		$bcc = $user->mail . ', ' . variable_get('dwsim_flowsheet_emails', '');
		$cc = variable_get('dwsim_flowsheet_cc_emails', '');
		$params['dwsim_flowsheet_proposal_approved']['proposal_id'] = $proposal_id;
		$params['dwsim_flowsheet_proposal_approved']['user_id'] = $proposal_data->uid;
		$params['dwsim_flowsheet_proposal_approved']['headers'] = array(
			'From' => $from,
			'MIME-Version' => '1.0',
			'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
			'Content-Transfer-Encoding' => '8Bit',
			'X-Mailer' => 'Drupal',
			'Cc' => $cc,
			'Bcc' => $bcc
		);
		if (!drupal_mail('dwsim_flowsheet', 'dwsim_flowsheet_proposal_approved', $email_to, language_default(), $params, $from, TRUE))
			drupal_set_message('Error sending email message.', 'error');
		drupal_set_message('DWSIM flowsheeting proposal No. ' . $proposal_id . ' approved. User has been notified of the approval.', 'status');
		drupal_goto('flowsheeting-project/manage-proposal');
		return;
	} //$form_state['values']['approval'] == 1
	else if ($form_state['values']['approval'] == 2)
	{
		$query = "UPDATE {dwsim_flowsheet_proposal} SET approver_uid = :uid, approval_date = :date, approval_status = 2, message = :message WHERE id = :proposal_id";
		$args = array(
			":uid" => $user->uid,
			":date" => time(),
			":message" => $form_state['values']['message'],
			":proposal_id" => $proposal_id
		);
		$result = db_query($query, $args);
		/* sending email */
		$user_data = user_load($proposal_data->uid);
		$email_to = $user_data->mail;
		$from = variable_get('dwsim_flowsheet_from_email', '');
		$bcc = $user->mail . ', ' . variable_get('dwsim_flowsheet_emails', '');
		$cc = variable_get('dwsim_flowsheet_cc_emails', '');
		$params['dwsim_flowsheetproposal_disapproved']['proposal_id'] = $proposal_id;
		$params['dwsim_flowsheetproposal_disapproved']['user_id'] = $proposal_data->uid;
		$params['dwsim_flowsheetproposal_disapproved']['headers'] = array(
			'From' => $from,
			'MIME-Version' => '1.0',
			'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
			'Content-Transfer-Encoding' => '8Bit',
			'X-Mailer' => 'Drupal',
			'Cc' => $cc,
			'Bcc' => $bcc
		);
		if (!drupal_mail('dwsim_flowsheet', 'dwsim_flowsheet_proposal_disapproved', $email_to, language_default(), $params, $from, TRUE))
			drupal_set_message('Error sending email message.', 'error');
		drupal_set_message('DWSIM flowsheeting proposal No. ' . $proposal_id . ' dis-approved. User has been notified of the dis-approval.', 'error');
		drupal_goto('flowsheeting-project/manage-proposal');
		return;
	} //$form_state['values']['approval'] == 2
}
/******************************************************************************/
/*************************** PROPOSAL STATUS FORM *****************************/
/******************************************************************************/
function dwsim_flowsheet_proposal_status_form($form, &$form_state)
{
	global $user;
	/* get current proposal */
	$proposal_id = (int) arg(3);
	//$proposal_q = db_query("SELECT * FROM {dwsim_flowsheet_proposal} WHERE id = %d", $proposal_id);
	$query = db_select('dwsim_flowsheet_proposal');
	$query->fields('dwsim_flowsheet_proposal');
	$query->condition('id', $proposal_id);
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
			drupal_goto('flowsheeting-project/manage-proposal');
			return;
		}
	} //$proposal_q
	else
	{
		drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
		drupal_goto('flowsheeting-project/manage-proposal');
		return;
	}
	$form['student_name'] = array(
		'#type' => 'item',
		'#markup' => l($proposal_data->name_title . ' ' . $proposal_data->student_name, 'user/' . $proposal_data->uid),
		'#title' => t('Student name')
	);
	$form['student_email_id'] = array(
		'#title' => t('Student Email'),
		'#type' => 'item',
		'#markup' => user_load($proposal_data->uid)->mail,
		'#title' => t('Email')
	);
	$form['month_year_of_degree'] = array(
		'#type' => 'date_popup',
		'#title' => t('Month and year of award of degree'),
		'#date_label_position' => '',
		'#description' => '',
		'#default_value' => $proposal_data->month_year_of_degree,
		'#date_format' => 'M-Y',
		'#date_increment' => 0,
		'#date_year_range' => '1960:+0',
		'#datepicker_options' => array(
			'maxDate' => 0
		),
		'#disabled' => TRUE
	);
	$form['university'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->university,
		'#title' => t('University/Institute')
	);
	$form['country'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->country,
		'#title' => t('Country')
	);
	$form['all_state'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->state,
		'#title' => t('State')
	);
	$form['city'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->city,
		'#title' => t('City')
	);
	$form['pincode'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->pincode,
		'#title' => t('Pincode/Postal code')
	);
	$form['version'] = array(
		'#type' => 'item',
		'#title' => t('DWSIM version'),
		'#markup' => $proposal_data->version
	);
	$form['project_guide_name'] = array(
		'#type' => 'item',
		'#title' => t('Project guide'),
		'#markup' => $proposal_data->project_guide_name
	);
	$form['project_guide_email_id'] = array(
		'#type' => 'item',
		'#title' => t('Project guide email'),
		'#markup' => $proposal_data->project_guide_email_id
	);
	$form['project_title'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->project_title,
		'#title' => t('Title of the Flowsheet Project')
	);
	$form['process_development_compound_name'] = array(
		'#type' => 'item',
		'#title' => t('Name of compound for which process development is carried out'),
		'#markup' => $proposal_data->process_development_compound_name
	);
	$form['dwsim_database_compound_name'] = array(
		'#type' => 'item',
		'#title' => t('List of compounds from DWSIM Database used in process flowsheet'),
		'#markup' => $proposal_data->dwsim_database_compound_name
	);
	if ($proposal_data->user_defined_compounds_used_in_process != "" || $proposal_data->user_defined_compounds_used_in_process != NULL)
	{
		$form['user_defined_compounds_used_in_process_flowsheetcompound_name'] = array(
			'#type' => 'item',
			'#title' => t('List of user defined compounds used in process flowsheet'),
			'#markup' => $proposal_data->user_defined_compounds_used_in_process
		);
	} //$proposal_data->user_defined_compounds_used_in_process != "" || $proposal_data->user_defined_compounds_used_in_process != NULL
	else
	{
		$form['user_defined_compounds_used_in_process_flowsheetcompound_name'] = array(
			'#type' => 'item',
			'#title' => t('List of user defined compounds used in process flowsheet'),
			'#markup' => "Not entered"
		);
	}
	if ($proposal_data->user_defined_compound_filepath != "")
	{
		$form['user_defined_compound_filepath'] = array(
			'#type' => 'item',
			'#title' => t('Uploaded the user defined compound '),
			'#markup' => l('Download user defined compound list', 'flowsheeting-project/download/user-defined-compound-file/' . $proposal_id) . "<br><br>"
		);
	} //$proposal_data->user_defined_compound_filepath != ""
	else
	{
		$form['user_defined_compound_filepath'] = array(
			'#type' => 'item',
			'#title' => t('Uploaded the user defined compound '),
			'#markup' => "Not uploaded<br><br>"
		);
	}
	$proposal_status = '';
	switch ($proposal_data->approval_status)
	{
		case 0:
			$proposal_status = t('Pending');
			break;
		case 1:
			$proposal_status = t('Approved');
			break;
		case 2:
			$proposal_status = t('Dis-approved');
			break;
		case 3:
			$proposal_status = t('Completed');
			break;
		default:
			$proposal_status = t('Unkown');
			break;
	} //$proposal_data->approval_status
	$form['proposal_status'] = array(
		'#type' => 'item',
		'#markup' => $proposal_status,
		'#title' => t('Proposal Status')
	);
	if ($proposal_data->approval_status == 0)
	{
		$form['approve'] = array(
			'#type' => 'item',
			'#markup' => l('Click here', 'flowsheeting-project/manage-proposal/approve/' . $proposal_id),
			'#title' => t('Approve')
		);
	} //$proposal_data->approval_status == 0
	if ($proposal_data->approval_status == 1)
	{
		$form['completed'] = array(
			'#type' => 'checkbox',
			'#title' => t('Completed'),
			'#description' => t('Check if user has provided all experiment solutions.')
		);
	} //$proposal_data->approval_status == 1
	if ($proposal_data->approval_status == 2)
	{
		$form['message'] = array(
			'#type' => 'item',
			'#markup' => $proposal_data->message,
			'#title' => t('Reason for disapproval')
		);
	} //$proposal_data->approval_status == 2
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);
	$form['cancel'] = array(
		'#type' => 'markup',
		'#markup' => l(t('Cancel'), 'flowsheeting-project/manage-proposal/all')
	);
	return $form;
}
function dwsim_flowsheet_proposal_status_form_submit($form, &$form_state)
{
	global $user;
	/* get current proposal */
	$proposal_id = (int) arg(3);
	//$proposal_q = db_query("SELECT * FROM {dwsim_flowsheet_proposal} WHERE id = %d", $proposal_id);
	$query = db_select('dwsim_flowsheet_proposal');
	$query->fields('dwsim_flowsheet_proposal');
	$query->condition('id', $proposal_id);
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
			drupal_goto('flowsheeting-project/manage-proposal');
			return;
		}
	} //$proposal_q
	else
	{
		drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
		drupal_goto('flowsheeting-project/manage-proposal');
		return;
	}
	/* set the book status to completed */
	if ($form_state['values']['completed'] == 1)
	{
		$up_query = "UPDATE dwsim_flowsheet_proposal SET approval_status = :approval_status , expected_completion_date = :expected_completion_date WHERE id = :proposal_id";
		$args = array(
			":approval_status" => '3',
			":proposal_id" => $proposal_id,
			":expected_completion_date" => time()
		);
		$result = db_query($up_query, $args);
		CreateReadmeFileLabMigration($proposal_id);
		if (!$result)
		{
			drupal_set_message('Error in update status', 'error');
			return;
		} //!$result
		/* sending email */
		$user_data = user_load($proposal_data->uid);
		$email_to = $user_data->mail;
		$from = variable_get('dwsim_flowsheet_from_email', '');
		$bcc = $user->mail . ', ' . variable_get('dwsim_flowsheet_emails', '');
		$cc = variable_get('dwsim_flowsheet_cc_emails', '');
		$params['proposal_completed']['proposal_id'] = $proposal_id;
		$params['proposal_completed']['user_id'] = $proposal_data->uid;
		$params['proposal_completed']['headers'] = array(
			'From' => $from,
			'MIME-Version' => '1.0',
			'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
			'Content-Transfer-Encoding' => '8Bit',
			'X-Mailer' => 'Drupal',
			'Cc' => $cc,
			'Bcc' => $bcc
		);
		if (!drupal_mail('dwsim_flowsheet', 'proposal_completed', $email_to, language_default(), $params, $from, TRUE))
			drupal_set_message('Error sending email message.', 'error');
		/*$email_to = $user->mail . ', ' . variable_get('dwsim_flowsheet_emails', '');;
		if (!drupal_mail('dwsim_flowsheet', 'proposal_completed', $email_to , language_default(), $params, variable_get('dwsim_flowsheet_from_email', NULL), TRUE))
		drupal_set_message('Error sending email message.', 'error');*/
		drupal_set_message('Congratulations! DWSIM flowsheeting proposal has been marked as completed. User has been notified of the completion.', 'status');
	} //$form_state['values']['completed'] == 1
	drupal_goto('flowsheeting-project/manage-proposal');
	return;
}
/******************************************************************************/
/**************************** PROPOSAL EDIT FORM ******************************/
/******************************************************************************/
function dwsim_flowsheet_proposal_edit_form($form, &$form_state)
{
	global $user;
	/* get current proposal */
	$proposal_id = (int) arg(3);
	//$proposal_q = db_query("SELECT * FROM {dwsim_flowsheet_proposal} WHERE id = %d", $proposal_id);
	$query = db_select('dwsim_flowsheet_proposal');
	$query->fields('dwsim_flowsheet_proposal');
	$query->condition('id', $proposal_id);
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
			drupal_goto('flowsheeting-project/manage-proposal');
			return;
		}
	} //$proposal_q
	else
	{
		drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
		drupal_goto('flowsheeting-project/manage-proposal');
		return;
	}
	$user_data = user_load($proposal_data->uid);
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
		'#required' => TRUE,
		'#default_value' => $proposal_data->name_title
	);
	$form['student_name'] = array(
		'#type' => 'textfield',
		'#title' => t('Name of the Proposer'),
		'#size' => 30,
		'#maxlength' => 50,
		'#required' => TRUE,
		'#default_value' => $proposal_data->student_name
	);
	$form['student_email_id'] = array(
		'#type' => 'item',
		'#title' => t('Email'),
		'#markup' => $user_data->mail
	);
	$form['month_year_of_degree'] = array(
		'#type' => 'date_popup',
		'#title' => t('Month and year of award of degree'),
		'#date_label_position' => '',
		'#description' => '',
		'#default_value' => $proposal_data->month_year_of_degree,
		'#date_format' => 'M-Y',
		'#date_increment' => 0,
		'#date_year_range' => '1960:+0',
		'#datepicker_options' => array(
			'maxDate' => 0
		),
		'#required' => TRUE
	);
	$form['university'] = array(
		'#type' => 'textfield',
		'#title' => t('University/Institute'),
		'#size' => 30,
		'#maxlength' => 50,
		'#required' => TRUE,
		'#default_value' => $proposal_data->university
	);
	$form['country'] = array(
		'#type' => 'select',
		'#title' => t('Country'),
		'#options' => array(
			'India' => 'India',
			'Others' => 'Others'
		),
		'#default_value' => $proposal_data->country,
		'#required' => TRUE,
		'#tree' => TRUE,
		'#validated' => TRUE
	);
	$form['other_country'] = array(
		'#type' => 'textfield',
		'#title' => t('Other than India'),
		'#size' => 100,
		'#default_value' => $proposal_data->country,
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
		'#default_value' => $proposal_data->state,
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
		'#default_value' => $proposal_data->city,
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
		'#default_value' => $proposal_data->state,
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
		'#default_value' => $proposal_data->city,
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
		'#default_value' => $proposal_data->pincode,
		'#attributes' => array(
			'placeholder' => 'Insert pincode of your city/ village....'
		)
	);
	$form['project_title'] = array(
		'#type' => 'textfield',
		'#title' => t('Title of the Flowsheet Project'),
		'#size' => 30,
		'#maxlength' => 50,
		'#required' => TRUE,
		'#default_value' => $proposal_data->project_title
	);
	$form['delete_proposal'] = array(
		'#type' => 'checkbox',
		'#title' => t('Delete Proposal')
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
function dwsim_flowsheet_proposal_edit_form_validate($form, &$form_state)
{
	$proposal_id = (int) arg(3);
	/* check before delete proposal */
	if ($form_state['values']['delete_proposal'] == 1)
	{
		//$experiment_q = db_query("SELECT * FROM {dwsim_flowsheet_experiment} WHERE proposal_id = %d", $proposal_id);
		$query = db_select('dwsim_flowsheet_experiment');
		$query->fields('dwsim_flowsheet_experiment');
		$query->condition('proposal_id', $proposal_id);
		$experiment_q = $query->execute();
		while ($experiment_data = $experiment_q->fetchObject())
		{
			//$solution_q = db_query("SELECT * FROM {dwsim_flowsheet_solution} WHERE experiment_id = %d", $experiment_data->id);
			$query = db_select('dwsim_flowsheet_solution');
			$query->fields('dwsim_flowsheet_solution');
			$query->condition('experiment_id', $experiment_data->id);
			$solution_q = $query->execute();
			if ($solution_q->fetchObject())
			{
				form_set_error('', t('Cannot delete proposal since there are solutions already uploaded. Use the "Bulk Manage" interface to delete this proposal'));
			} //$solution_q->fetchObject()
		} //$experiment_data = $experiment_q->fetchObject()
	} //$form_state['values']['delete_proposal'] == 1
	return;
}
function dwsim_flowsheet_proposal_edit_form_submit($form, &$form_state)
{
	global $user;
	/* get current proposal */
	$proposal_id = (int) arg(3);
	// $proposal_q = db_query("SELECT * FROM {dwsim_flowsheet_proposal} WHERE id = %d", $proposal_id);
	$query = db_select('dwsim_flowsheet_proposal');
	$query->fields('dwsim_flowsheet_proposal');
	$query->condition('id', $proposal_id);
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
			drupal_goto('flowsheeting-project/manage-proposal');
			return;
		}
	} //$proposal_q
	else
	{
		drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
		drupal_goto('flowsheeting-project/manage-proposal');
		return;
	}
	/* delete proposal */
	if ($form_state['values']['delete_proposal'] == 1)
	{
		//db_query("DELETE FROM {dwsim_flowsheet_proposal} WHERE id = %d", $proposal_id);
		$query = db_delete('dwsim_flowsheet_proposal');
		$query->condition('id', $proposal_id);
		$num_deleted = $query->execute();
		//db_query("DELETE FROM {dwsim_flowsheet_experiment} WHERE proposal_id = %d", $proposal_id);
		$query = db_delete('dwsim_flowsheet_experiment');
		$query->condition('proposal_id', $proposal_id);
		$num_deleted = $query->execute();
		drupal_set_message(t('Proposal Delete'), 'status');
		drupal_goto('flowsheeting-project/manage-proposal');
		return;
	} //$form_state['values']['delete_proposal'] == 1
	if ($form_state['values']['open_solution'] == 1)
	{
		// $query = "UPDATE {dwsim_flowsheet_proposal} SET solution_provider_uid = :solution_provider_uid, solution_status = :solution_status, solution_provider_name_title = '', solution_provider_name = '', solution_provider_contact_ph = '', solution_provider_department = '', solution_provider_university = '' WHERE id = :proposal_id";
		// $args= array(
		//    ":solution_provider_uid" => 0, 
		//    ":solution_status" => 0,
		//    ":proposal_id" => $proposal_id,
		// );
		// $result = db_query($query, $args);
		$result = db_update('dwsim_flowsheet_proposal')->fields(array(
			'solution_provider_uid' => 0,
			'solution_status' => 0,
			'solution_provider_name_title' => '',
			'solution_provider_name' => '',
			'solution_provider_contact_ph' => '',
			'solution_provider_department' => '',
			'solution_provider_university' => ''
		))->condition('id', $proposal_id)->execute();
		if (!$result)
		{
			drupal_set_message(t('Solution already open for everyone.'), 'error');
			return;
		} //!$result
	} //$form_state['values']['open_solution'] == 1
	$solution_display = 0;
	if ($form_state['values']['solution_display'] == 1)
	{
		$solution_display = 1;
	} //$form_state['values']['solution_display'] == 1
	else
	{
		$solution_display = 0;
	}
	/* update proposal */
	$v = $form_state['values'];
	//$query = "UPDATE {dwsim_flowsheet_proposal} SET name_title = :name_title, name = :name, contact_ph = :contact_ph, department = :department, university = :unversity, project_title = :project_title, solution_display = :solution_display WHERE id = :id";
	// $args= array(    
	//    ":name_title" => $v['name_title'],
	//    ":name" => $v['student_name'],
	//    "contact_ph" => $v['contact_ph'],
	//    ":department" => $v['department'],
	//    ":university" => $v['university'],
	//    ":project_title" => $v['project_title'],
	//    ":solution_display" => $solution_display,
	//    ":id" => $proposal_id,
	//  );
	$project_title = $v['project_title'];
	$proposar_name = $v['name_title'] . ' ' . $v['student_name'];
	$university = $v['university'];
	$directory_names = _df_dir_name($project_title, $proposar_name, $university);
	if (LM_RenameDir($proposal_id, $directory_names))
	{
		$directory_name = $directory_names;
	} //LM_RenameDir($proposal_id, $directory_names)
	else
	{
		return;
	}
	$query = db_update('dwsim_flowsheet_proposal')->fields(array(
		'name_title' => $v['name_title'],
		'student_name' => $v['student_name'],
		'department' => $v['department'],
		'university' => $v['university'],
		'city' => $v['city'],
		'pincode' => $v['pincode'],
		'state' => $v['all_state'],
		'project_title' => $v['project_title'],
		'solution_display' => $solution_display,
		'directory_name' => $directory_name
	))->condition('id', $proposal_id);
	$result1 = $query->execute();
	//$result=db_query($query, $args);
	/* updating existing experiments */
	$query = db_select('dwsim_flowsheet_experiment');
	$query->fields('dwsim_flowsheet_experiment');
	$query->condition('proposal_id', $proposal_id);
	$query->orderBy('id', 'ASC');
	$experiment_q = $query->execute();
	for ($counter = 1; $counter <= 15; $counter++)
	{
		$experiment_data = $experiment_q->fetchObject();
		if ($experiment_data)
		{
			$experiment_field_name = 'lab_experiment_update' . $experiment_data->id;
			$experiment_description = 'lab_experiment_description_update' . $experiment_data->id;
			if (strlen(trim($form_state['values'][$experiment_field_name])) >= 1)
			{
				$query = "UPDATE {dwsim_flowsheet_experiment} SET title = :title, description= :description WHERE id = :id";
				$args = array(
					":title" => trim($form_state['values'][$experiment_field_name]),
					":description" => trim($form_state['values'][$experiment_description]),
					":id" => $experiment_data->id
				);
				$result2 = db_query($query, $args);
				if (!$result2)
				{
					drupal_set_message(t('Could not update Title of the Experiment : ') . trim($form_state['values'][$experiment_field_name]), 'error');
				} //!$result2
			} //strlen(trim($form_state['values'][$experiment_field_name])) >= 1
			else
			{
				$query = "DELETE FROM {dwsim_flowsheet_experiment} WHERE id = :id LIMIT 1";
				$args = array(
					":id" => $experiment_data->id
				);
				$result3 = db_query($query, $args);
			}
		} //$experiment_data
	} //$counter = 1; $counter <= 15; $counter++
	/* foreach ($form_state['values']['lab_experiment']['update'] as $update_id => $update_value) {
	if (strlen(trim($update_value)) >= 1) {
	$description= $form_state['values']['lab_experiment_description']['update']; 
	$query = "UPDATE {dwsim_flowsheet_experiment} SET title = :title and description=:description WHERE id = :id";
	$args = array(
	":title"=>  trim($update_value),
	":description"=>trim($description),
	":id"=> $update_id,
	);
	$result2 = db_query($query, $args);
	if (!$result2)
	{
	drupal_set_message(t('Could not update Title of the Experiment : ') . trim($update_value), 'error');
	}
	} else {
	$query = "DELETE FROM {dwsim_flowsheet_experiment} WHERE id = :id LIMIT 1";
	$args = array( 
	":id" => $update_id
	);
	$result3 = db_query($query, $args);
	}
	}*/
	/* inserting new experiments */
	$query = db_select('dwsim_flowsheet_experiment');
	$query->fields('dwsim_flowsheet_experiment');
	$query->condition('proposal_id', $proposal_id);
	$query->orderBy('number', 'DESC');
	$query->range(0, 1);
	$number_q = $query->execute();
	if ($number_data = $number_q->fetchObject())
	{
		$number = (int) $number_data->number;
		$number++;
	} //$number_data = $number_q->fetchObject()
	else
	{
		$number = 1;
	}
	for ($counter = 1; $counter <= 15; $counter++)
	{
		$lab_experiment_insert = 'lab_experiment_insert' . $counter;
		//var_dump($form_state['values'][$lab_experiment_insert]);die;
		$lab_experiment_description_insert = 'lab_experiment_description_insert' . $counter;
		if (!empty($form_state['values'][$lab_experiment_insert]))
		{
			//var_dump($form_state['values'][$lab_experiment_insert]);die;
			$query = "INSERT INTO {dwsim_flowsheet_experiment} (proposal_id, number, title, description) VALUES (:proposal_id, :number, :title, :description)";
			$args = array(
				":proposal_id" => $proposal_id,
				":number" => $number,
				":title" => trim($form_state['values'][$lab_experiment_insert]),
				":description" => trim($form_state['values'][$lab_experiment_description_insert])
			);
			$result4 = db_query($query, $args);
			if (!$result4)
			{
				drupal_set_message(t('Could not insert Title of the Experiment : ') . trim($form_state['values'][$lab_experiment_insert]), 'error');
			} //!$result4
			else
			{
				$number++;
			}
		} //!empty($form_state['values'][$lab_experiment_insert])
	} //$counter = 1; $counter <= 15; $counter++
	/* $query = db_select('dwsim_flowsheet_experiment');
	$query->fields('dwsim_flowsheet_experiment');
	$query->condition('proposal_id', $proposal_id);
	$query->orderBy('number', 'DESC');
	$query->range(0, 1);
	$number_q = $query->execute();
	if ($number_data = $number_q->fetchObject()) {
	$number = (int)$number_data->number;
	$number++;
	} else {
	$number = 1;
	}
	$insertvalue = array($insert_id => $insert_value);
	$lab_experimentinsert = $form_state['values']['lab_experiment']['insert'];
	$lab_exp_descriptioninsert=$form_state['values']['lab_experiment_description']['insert'];
	if (is_array($lab_experimentinsert) || is_object($lab_experimentinsert))
	{  
	foreach ($lab_experimentinsert as $insertvalue) {
	//foreach ($form_state['values']['lab_experiment']['insert'] as $insert_id => $insert_value) {
	if (strlen(trim($insert_value)) >= 1) {
	$query = "INSERT INTO {dwsim_flowsheet_experiment} (proposal_id, number, title, description) VALUES :proposal_id, :number, :title, :description";
	$args = array(
	":proposal_id" => $proposal_id, 
	":number" => $number, 
	":title" => trim($insert_value),
	":description"=>""
	);
	$result4 = db_query($query, $args);
	if (!$result4)
	{
	drupal_set_message(t('Could not insert Title of the Experiment : ') . trim($insert_value), 'error');
	} else {
	$number++;
	}
	}
	}
	}*/
	drupal_set_message(t('Proposal Updated'), 'status');
}
function CreateReadmeFileDWSIMFlowsheetingProject($proposal_id)
{
	$result = db_query("
                        SELECT * from dwsim_flowsheet_proposal WHERE id = :proposal_id", array(
		":proposal_id" => $proposal_id
	));
	$proposal_data = $result->fetchObject();
	$root_path = dwsim_flowsheet_path();
	$readme_file = fopen($root_path . $proposal_data->directory_name . "/README.txt", "w") or die("Unable to open file!");
	$txt = "";
	$txt .= "About the lab";
	$txt .= "\n" . "\n";
	$txt .= "Title Of The Flowsheet Project: " . $proposal_data->project_title . "\n";
	$txt .= "Proposar Name: " . $proposal_data->name_title . " " . $proposal_data->student_name . "\n";
	$txt .= "University: " . $proposal_data->university . "\n";
	$txt .= "\n" . "\n";
	$txt .= "DWSIM Flowsheet Project By FOSSEE, IIT Bombay" . "\n";
	fwrite($readme_file, $txt);
	fclose($readme_file);
	return $txt;
}
function DF_RenameDir($proposal_id, $dir_name)
{
	$proposal_id = $proposal_id;
	$dir_name = $dir_name;
	$query = db_query("SELECT directory_name,id FROM dwsim_flowsheet_proposal WHERE id = :proposal_id", array(
		':proposal_id' => $proposal_id
	));
	$result = $query->fetchObject();
	if ($result != NULL)
	{
		$files = scandir(dwsim_flowsheet_path());
		$files_id_dir = dwsim_flowsheet_path() . $result->id;
		//var_dump($files);die;
		$file_dir = dwsim_flowsheet_path() . $result->directory_name;
		if (is_dir($file_dir))
		{
			$new_directory_name = rename(dwsim_flowsheet_path() . $result->directory_name, dwsim_flowsheet_path() . $dir_name);
			return $new_directory_name;
		} //is_dir($file_dir)
		else if (is_dir($files_id_dir))
		{
			$new_directory_name = rename(dwsim_flowsheet_path() . $result->id, dwsim_flowsheet_path() . $dir_name);
			return $new_directory_name;
		} //is_dir($files_id_dir)
		else
		{
			drupal_set_message('Directory not available for rename.');
			return;
		}
	} //$result != NULL
	else
	{
		drupal_set_message('Project names directory not present in databse');
		return;
	}
	//var_dump($files);die;
	/* if ($files != NULL)
	{
	$new_directory_name = rename(dwsim_flowsheet_path() . $result->directory_name, dwsim_flowsheet_path() . $dir_name) or drupal_set_message("Unable to rename folder");
	}
	else
	{
	$new_directory_name = 'Can not rename the directory. Directory not present';
	}*/
	return;
}
