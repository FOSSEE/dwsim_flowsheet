<?php

function dwsim_flowsheet_abstract_bulk_approval_form($form, &$form_state)
{
	$options_first = _bulk_list_of_flowsheet_project();
	$selected = isset($form_state['values']['flowsheet_project']) ? $form_state['values']['flowsheet_project'] : key($options_first);
	$form = array();
	$form['flowsheet_project'] = array(
		'#type' => 'select',
		'#title' => t('Title of the flowsheeting project'),
		'#options' => _bulk_list_of_flowsheet_project(),
		'#default_value' => $selected,
		'#ajax' => array(
			
			'callback' => 'ajax_bulk_flowsheet_abstract_details_callback'
		),
		'#suffix' => '<div id="ajax_selected_flowsheet"></div><div id="ajax_selected_flowsheet_pdf"></div>'
	);
	$form['flowsheet_actions'] = array(
		'#type' => 'select',
		'#title' => t('Please select action for Flowsheeting project'),
		'#options' => _bulk_list_flowsheet_actions(),
		'#default_value' => 0,
		'#prefix' => '<div id="ajax_selected_flowsheet_action" style="color:red;">',
		'#suffix' => '</div>',
		'#states' => array(
			'invisible' => array(
				':input[name="flowsheet_project"]' => array(
					'value' => 0
				)
			)
		)
	);
	$form['message'] = array(
		'#type' => 'textarea',
		'#title' => t('If Dis-Approved please specify reason for Dis-Approval'),
		'#prefix' => '<div id= "message_submit">',
		'#states' => array(
			'visible' => array(
				array(
					':input[name="flowsheet_actions"]' => array(
						'value' => 3
					)
				),
				'or',
				array(
					':input[name="flowsheet_actions"]' => array(
						'value' => 4
					)
				)
			)
		)
	);
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
		'#states' => array(
			'invisible' => array(
				':input[name="lab"]' => array(
					'value' => 0
				)
			)
		)
	);
	return $form;
}
function ajax_bulk_flowsheet_abstract_details_callback($form, $form_state)
{
	$commands = array();
	$flowsheet_project_default_value = $form_state['values']['flowsheet_project'];
	if ($flowsheet_project_default_value != 0)
	{
		$commands[] = ajax_command_html('#ajax_selected_flowsheet', _flowsheet_details($flowsheet_project_default_value));
		$form['flowsheet_actions']['#options'] = _bulk_list_flowsheet_actions();
		//$form['lab_experiment_list']['#options'] = _ajax_bulk_get_experiment_list($lab_default_value);
		// $commands[] = ajax_command_data('#ajax_selected_flowsheet', 'form_state_value_select', $form_state['values']['lab_experiment_list']);
		// $commands[] = ajax_command_replace('#ajax_selected_experiment', drupal_render($form['lab_experiment_list']));
		$commands[] = ajax_command_replace('#ajax_selected_flowsheet_action', drupal_render($form['flowsheet_actions']));
	} //$flowsheet_project_default_value != 0
	else
	{
		$commands[] = ajax_command_html('#ajax_selected_flowsheet', '');
		$commands[] = ajax_command_data('#ajax_selected_flowsheet', 'form_state_value_select', $form_state['values']['flowsheet_project']);
	}
	return array(
		'#type' => 'ajax',
		'#commands' => $commands
	);
}
/************************************************************/
function dwsim_flowsheet_abstract_bulk_approval_form_submit($form, &$form_state)
{
	global $user;
	$msg = '';
	$root_path = dwsim_flowsheet_document_path();
	if ($form_state['clicked_button']['#value'] == 'Submit')
	{
		if ($form_state['values']['flowsheet_project'])
		// dwsim_flowsheet_abstract_del_lab_pdf($form_state['values']['flowsheet_project']);
			if (user_access('dwsim flowsheet bulk manage abstract'))
			{
				$query = db_select('dwsim_flowsheet_proposal');
				$query->fields('dwsim_flowsheet_proposal');
				$query->condition('id', $form_state['values']['flowsheet_project']);
				$user_query = $query->execute();
				$user_info = $user_query->fetchObject();
				$user_data = user_load($user_info->uid);
				if ($form_state['values']['flowsheet_actions'] == 1)
				{
					// approving entire project //
					$query = db_select('dwsim_flowsheet_submitted_abstracts');
					$query->fields('dwsim_flowsheet_submitted_abstracts');
					$query->condition('proposal_id', $form_state['values']['flowsheet_project']);
					$abstracts_q = $query->execute();
					$experiment_list = '';
					while ($abstract_data = $abstracts_q->fetchObject())
					{
						db_query("UPDATE {dwsim_flowsheet_submitted_abstracts} SET abstract_approval_status = 1, approver_uid = :approver_uid WHERE id = :id", array(
							':approver_uid' => $user->uid,
							':id' => $abstract_data->id
						));
						db_query("UPDATE {dwsim_flowsheet_submitted_abstracts_file} SET file_approval_status = 1, approvar_uid = :approver_uid WHERE submitted_abstract_id = :submitted_abstract_id", array(
							':approver_uid' => $user->uid,
							':submitted_abstract_id' => $abstract_data->id
						));
					} //$abstract_data = $abstracts_q->fetchObject()
					drupal_set_message(t('Approved Flosheeting project.'), 'status');
					// email 
					$email_subject = t('[!site_name][Flowsheeting Project] Your uploaded flowsheeting project have been approved', array(
						'!site_name' => variable_get('site_name', '')
					));
					$email_body = array(
						0 => t('

Dear !user_name,

Congratulations!
Your DWSIM flowsheet and abstract with the following details have been approved.

Full Name: ' . $user_info->name_title . ' ' . $user_info->contributor_name . '
Project Title: ' . $user_info->project_title . '
Name of compound for which process development is carried out  : ' . $user_info->process_development_compound_name . '

Kindly send us the internship forms as early as possible for processing your honorarium on time. In case you have already sent these forms, please share the the consignment number or tracking id with us.

Note: It will take upto 30 days from the time we receive your forms, to process your honorarium.


Best Wishes,

!site_name Team,
FOSSEE,IIT Bombay', array(
							'!site_name' => variable_get('site_name', ''),
							'!user_name' => $user_data->name
						))
					);
					/** sending email when everything done **/
					$email_to = $user_data->mail;
					$from = variable_get('dwsim_flowsheet_from_email', '');
					$bcc = variable_get('dwsim_flowsheet_emails', '');
					$cc = variable_get('dwsim_flowsheet_cc_emails', '');
					$params['standard']['subject'] = $email_subject;
					$params['standard']['body'] = $email_body;
					$params['standard']['headers'] = array(
						'From' => $from,
						'MIME-Version' => '1.0',
						'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
						'Content-Transfer-Encoding' => '8Bit',
						'X-Mailer' => 'Drupal',
						'Cc' => $cc,
						'Bcc' => $bcc
					);
					if (!drupal_mail('dwsim_flowsheet', 'standard', $email_to, language_default(), $params, $from, TRUE))
					{
						$msg = drupal_set_message('Error sending email message.', 'error');
					} //!drupal_mail('dwsim_flowsheet', 'standard', $email_to, language_default(), $params, $from, TRUE)
				} //$form_state['values']['flowsheet_actions'] == 1
				elseif ($form_state['values']['flowsheet_actions'] == 2)
				{
					//pending review entire project 
					$query = db_select('dwsim_flowsheet_submitted_abstracts');
					$query->fields('dwsim_flowsheet_submitted_abstracts');
					$query->condition('proposal_id', $form_state['values']['flowsheet_project']);
					$abstracts_q = $query->execute();
					$experiment_list = '';
					while ($abstract_data = $abstracts_q->fetchObject())
					{
						db_query("UPDATE {dwsim_flowsheet_submitted_abstracts} SET abstract_approval_status = 0, approver_uid = :approver_uid WHERE id = :id", array(
							':approver_uid' => $user->uid,
							':id' => $abstract_data->id
						));
						db_query("UPDATE {dwsim_flowsheet_submitted_abstracts_file} SET file_approval_status = 0, approvar_uid = :approver_uid WHERE submitted_abstract_id = :submitted_abstract_id", array(
							':approver_uid' => $user->uid,
							':submitted_abstract_id' => $abstract_data->id
						));
					} //$abstract_data = $abstracts_q->fetchObject()
					drupal_set_message(t('Approved Flosheeting project.'), 'status');
					// email 
					$email_subject = t('[!site_name][Flowsheeting Project] Your uploaded flowsheeting project have been marked as pending', array(
						'!site_name' => variable_get('site_name', '')
					));
					$email_body = array(
						0 => t('

Dear !user_name,

Your all the uploaded flowsheeting project with Title : ' . $user_info->project_title . ' have been marked as pending to be reviewed.
 
You will be able to see the flowsheeting project after approved by one of our reviewers.

Best Wishes,

!site_name Team,
FOSSEE,IIT Bombay', array(
							'!site_name' => variable_get('site_name', ''),
							'!user_name' => $user_data->name
						))
					);
					/** sending email when everything done **/
					$email_to = $user_data->mail;
					$from = variable_get('dwsim_flowsheet_from_email', '');
					$bcc = variable_get('dwsim_flowsheet_emails', '');
					$cc = variable_get('dwsim_flowsheet_cc_emails', '');
					$params['standard']['subject'] = $email_subject;
					$params['standard']['body'] = $email_body;
					$params['standard']['headers'] = array(
						'From' => $from,
						'MIME-Version' => '1.0',
						'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
						'Content-Transfer-Encoding' => '8Bit',
						'X-Mailer' => 'Drupal',
						'Cc' => $cc,
						'Bcc' => $bcc
					);
					if (!drupal_mail('dwsim_flowsheet', 'standard', $email_to, language_default(), $params, $from, TRUE))
					{
						drupal_set_message('Error sending email message.', 'error');
					} //!drupal_mail('dwsim_flowsheet', 'standard', $email_to, language_default(), $params, $from, TRUE)
				} //$form_state['values']['flowsheet_actions'] == 2
				elseif ($form_state['values']['flowsheet_actions'] == 3) //disapprove and delete entire flowsheeting project
				{
					if (strlen(trim($form_state['values']['message'])) <= 30)
					{
						form_set_error('message', t(''));
						$msg = drupal_set_message("Please mention the reason for disapproval. Minimum 30 character required", 'error');
						return $msg;
					} //strlen(trim($form_state['values']['message'])) <= 30
					if (!user_access('dwsim flowsheet bulk delete abstract'))
					{
						$msg = drupal_set_message(t('You do not have permission to Bulk Dis-Approved and Deleted Entire Lab.'), 'error');
						return $msg;
					} //!user_access('flowsheet bulk delete code')
					if (dwsim_flowsheet_abstract_delete_project($form_state['values']['flowsheet_project'])) //////
					{
						drupal_set_message(t('Dis-Approved and Deleted Entire Flowsheeting project.'), 'status');
					} //dwsim_flowsheet_abstract_delete_project($form_state['values']['flowsheet_project'])
					else
					{
						drupal_set_message(t('Error Dis-Approving and Deleting Entire flowsheeting project.'), 'error');
					}
					// email 
					$email_subject = t('[!site_name][Flowsheeting Project] Your uploaded flowsheeting project have been marked as dis-approved', array(
						'!site_name' => variable_get('site_name', '')
					));
					$email_body = array(
						0 => t('

Dear !user_name,

We regret to inform you that your DWSIM flowsheet and abstract with the following details have been disapproved:

Full Name: ' . $user_info->name_title . ' ' . $user_info->contributor_name . '
Project Title: ' . $user_info->project_title . '
Name of compound for which process development is carried out  : ' . $user_info->process_development_compound_name . '
Reason for dis-approval: ' . $form_state['values']['message'] . '

Best Wishes,

!site_name Team,
FOSSEE,IIT Bombay', array(
							'!site_name' => variable_get('site_name', ''),
							'!user_name' => $user_data->name
						))
					);
				} //$form_state['values']['flowsheet_actions'] == 3
				elseif ($form_state['values']['flowsheet_actions'] == 4)
				{
					if (strlen(trim($form_state['values']['message'])) <= 30)
					{
						form_set_error('message', t(''));
						$msg = drupal_set_message("Please mention the reason for disapproval/deletion. Minimum 30 character required", 'error');
						return $msg;
					} //strlen(trim($form_state['values']['message'])) <= 30
					$query = db_select('dwsim_flowsheet_abstract_experiment');
					$query->fields('dwsim_flowsheet_abstract_experiment');
					$query->condition('proposal_id', $form_state['values']['lab']);
					$query->orderBy('number', 'ASC');
					$experiment_q = $query->execute();
					$experiment_list = '';
					while ($experiment_data = $experiment_q->fetchObject())
					{
						$experiment_list .= '<p>' . $experiment_data->number . ') ' . $experiment_data->title . '<br> Description :  ' . $experiment_data->description . '<br>';
						$experiment_list .= ' ';
						$experiment_list .= '</p>';
					} //$experiment_data = $experiment_q->fetchObject()
					if (!user_access('lab migration bulk delete code'))
					{
						$msg = drupal_set_message(t('You do not have permission to Bulk Delete Entire Lab Including Proposal.'), 'error');
						return $msg;
					} //!user_access('lab migration bulk delete code')
					// check if dependency files are present 
					$dep_q = db_query("SELECT * FROM {dwsim_flowsheet_abstract_dependency_files} WHERE proposal_id = :proposal_id", array(
						":proposal_id" => $form_state['values']['lab']
					));
					if ($dep_data = $dep_q->fetchObject())
					{
						$msg = drupal_set_message(t("Cannot delete lab since it has dependency files that can be used by others. First delete the dependency files before deleting the lab."), 'error');
						return $msg ;
					} //$dep_data = $dep_q->fetchObject()
					if (dwsim_flowsheet_abstract_delete_lab($form_state['values']['lab']))
					{
						drupal_set_message(t('Dis-Approved and Deleted Entire Lab solutions.'), 'status');
						$query = db_select('dwsim_flowsheet_abstract_experiment');
						$query->fields('dwsim_flowsheet_abstract_experiment');
						$query->condition('proposal_id', $form_state['values']['lab']);
						$experiment_q = $query->execute()->fetchObject();
						$dir_path = $root_path . $experiment_q->directory_name;
						if (is_dir($dir_path))
						{
							$res = rmdir($dir_path);
							if (!$res)
							{
								$msg = drupal_set_message(t("Cannot delete Lab directory : " . $dir_path . ". Please contact administrator."), 'error');
								return $msg;
							} //!$res
						} //is_dir($dir_path)
						else
						{
							drupal_set_message(t("Lab directory not present : " . $dir_path . ". Skipping deleting lab directory."), 'status');
						}
						//deleting full proposal 
						//$proposal_q = db_query("SELECT * FROM {dwsim_flowsheet_abstract_proposal} WHERE id = %d", $form_state['values']['lab']);
						$proposal_q = db_query("SELECT * FROM {dwsim_flowsheet_abstract_proposal} WHERE id = :id", array(
							":id" => $form_state['values']['lab']
						));
						$proposal_data = $proposal_q->fetchObject();
						$proposal_id = $proposal_data->id;
						db_query("DELETE FROM {dwsim_flowsheet_abstract_experiment} WHERE proposal_id = :proposal_id", array(
							":proposal_id" => $proposal_id
						));
						db_query("DELETE FROM {dwsim_flowsheet_abstract_proposal} WHERE id = :id", array(
							":id" => $proposal_id
						));
						drupal_set_message(t('Deleted Lab Proposal.'), 'status');
						//email 
						$email_subject = t('[!site_name] Your uploaded Lab Migration solutions including the Lab proposal have been deleted', array(
							'!site_name' => variable_get('site_name', '')
						));
						$email_body = array(
							0 => t('

Dear !user_name,

We regret to inform you that all the uploaded Experiments of your Lab with following details have been deleted permanently.

Title of Lab :' . $user_info->lab_title . '

List of experiments : ' . $experiment_list . '

Reason for dis-approval: ' . $form_state['values']['message'] . '

Best Wishes,

!site_name Team,
FOSSEE,IIT Bombay', array(
								'!site_name' => variable_get('site_name', ''),
								'!user_name' => $user_data->name
							))
						);
						// email 
						//  $email_subject = t('Your uploaded Lab Migration solutions including the Lab proposal have been deleted');
						$email_body = array(
							0 => t('Your all the uploaded solutions including the Lab proposal have been deleted permanently.')
						);
					} //dwsim_flowsheet_abstract_delete_lab($form_state['values']['lab'])
					else
					{
						$msg = drupal_set_message(t('Error Dis-Approving and Deleting Entire Lab.'), 'error');
					}
				} //$form_state['values']['flowsheet_actions'] == 4
				else
				{
					$msg = drupal_set_message(t('You do not have permission to bulk manage code.'), 'error');
				}
			} //user_access('flowsheet project bulk manage code')
		return $msg;
	} //$form_state['clicked_button']['#value'] == 'Submit'
}
/**********************************************************/
function _bulk_list_of_flowsheet_project()
{
	$project_titles = array(
		'0' => 'Please select...'
	);
	$query = db_select('dwsim_flowsheet_proposal');
	$query->fields('dwsim_flowsheet_proposal');
	$query->condition('approval_status', 1);
	$query->orderBy('project_title', 'ASC');
	$project_titles_q = $query->execute();
	while ($project_titles_data = $project_titles_q->fetchObject())
	{
		$project_titles[$project_titles_data->id] = $project_titles_data->project_title . ' (Proposed by ' . $project_titles_data->contributor_name . ')';
	} //$project_titles_data = $project_titles_q->fetchObject()
	return $project_titles;
}
function _bulk_list_flowsheet_actions()
{
	$flowsheet_actions = array(
		0 => 'Please select...'
	);
	$flowsheet_actions[1] = 'Approve Entire Flowsheeting Project';
	//$flowsheet_actions[2] = 'Pending Review Entire Flowsheeting Project';
	$flowsheet_actions[3] = 'Dis-Approve Entire Flowsheeting Project (This will delete Flowsheeting Project)';
	//$flowsheet_actions[4] = 'Delete Entire Flowsheeting Project Including Proposal';
	return $flowsheet_actions;
}
function _flowsheet_details($flowsheet_proposal_id)
{
	$return_html = "";
	$query_pro = db_select('dwsim_flowsheet_proposal');
	$query_pro->fields('dwsim_flowsheet_proposal');
	$query_pro->condition('id', $flowsheet_proposal_id);
	$abstracts_pro = $query_pro->execute()->fetchObject();
	$query_pdf = db_select('dwsim_flowsheet_submitted_abstracts_file');
	$query_pdf->fields('dwsim_flowsheet_submitted_abstracts_file');
	$query_pdf->condition('proposal_id', $flowsheet_proposal_id);
	$query_pdf->condition('filetype', 'A');
	$abstracts_pdf = $query_pdf->execute()->fetchObject();
	if ($abstracts_pdf == TRUE)
	{
		if ($abstracts_pdf->filename != "NULL" || $abstracts_pdf->filename != "")
		{
			$abstract_filename = $abstracts_pdf->filename;
		} //$abstracts_pdf->filename != "NULL" || $abstracts_pdf->filename != ""
		else
		{
			$abstract_filename = "File not uploaded";
		}
	} //$abstracts_pdf == TRUE
	else
	{
		$abstract_filename = "File not uploaded";
	}
	$query_process = db_select('dwsim_flowsheet_submitted_abstracts_file');
	$query_process->fields('dwsim_flowsheet_submitted_abstracts_file');
	$query_process->condition('proposal_id', $flowsheet_proposal_id);
	$query_process->condition('filetype', 'S');
	$abstracts_query_process = $query_process->execute()->fetchObject();
	$query = db_select('dwsim_flowsheet_submitted_abstracts');
	$query->fields('dwsim_flowsheet_submitted_abstracts');
	$query->condition('proposal_id', $flowsheet_proposal_id);
	$abstracts_q = $query->execute()->fetchObject();
	if ($abstracts_q)
	{
		if ($abstracts_q->is_submitted == 0)
		{
			//drupal_set_message(t('Abstract is not submmited yet.'), 'error', $repeat = FALSE);
			//return;
		} //$abstracts_q->is_submitted == 0
	} //$abstracts_q
	//var_dump($abstracts_query_process);die;
	if ($abstracts_query_process == TRUE)
	{
		if ($abstracts_query_process->filename != "NULL" || $abstracts_query_process->filename != "")
		{
			$abstracts_query_process_filename = $abstracts_query_process->filename;
		} //$abstracts_query_process->filename != "NULL" || $abstracts_query_process->filename != ""
		else
		{
			$abstracts_query_process_filename = "File not uploaded";
		}
		if ($abstracts_q->unit_operations_used_in_dwsim == '')
		{
			$unit_operations_used_in_dwsim = "Not entered";
		} //$abstracts_q->unit_operations_used_in_dwsim == ''
		else
		{
			$unit_operations_used_in_dwsim = $abstracts_q->unit_operations_used_in_dwsim;
		}
		if ($abstracts_q->thermodynamic_packages_used == '')
		{
			$thermodynamic_packages_used = "Not entered";
		} //$abstracts_q->thermodynamic_packages_used == ''
		else
		{
			$thermodynamic_packages_used = $abstracts_q->thermodynamic_packages_used;
		}
		if ($abstracts_q->logical_blocks_used == '')
		{
			$logical_blocks_used = "Not entered";
		} //$abstracts_q->logical_blocks_used == ''
		else
		{
			$logical_blocks_used = $abstracts_q->logical_blocks_used;
		}
	} //$abstracts_query_process == TRUE
	else
	{
		$url = l('Upload abstract', 'flowsheeting-project/abstract-code/upload');
		$unit_operations_used_in_dwsim = "Not entered";
		$thermodynamic_packages_used = "Not entered";
		$logical_blocks_used = "Not entered";
		$abstracts_query_process_filename = "File not uploaded";
	}
	$headers = array(
		"Name of compound for which process development is carried out",
		"CAS No."
	);
	$rows = array();
	$item = array(
		"{$abstracts_pro->process_development_compound_name}",
		"{$abstracts_pro->process_development_compound_cas_number}"
	);
	array_push($rows, $item);
	$prodata = theme('table', array(
		'header' => $headers,
		'rows' => $rows
	));
	$download_flowsheet = l('Download flowsheet project','flowsheeting-project/full-download/project/'.$flowsheet_proposal_id);
	$return_html .= '<strong>Proposer Name:</strong><br />' . $abstracts_pro->name_title . ' ' . $abstracts_pro->contributor_name . '<br /><br />';
	$return_html .= '<strong>Title of the Flowsheet Project:</strong><br />' . $abstracts_pro->project_title . '<br /><br />';
	$return_html .= '<strong>DWSIM version:</strong><br />' . $abstracts_pro->version . '<br /><br />';
	$return_html .= '<strong>Unit Operations used in DWSIM:</strong><br />' . $unit_operations_used_in_dwsim . '<br /><br />';
	$return_html .= '<strong>Thermodynamic Packages Used:</strong><br />' . $thermodynamic_packages_used . '<br /><br />';
	$return_html .= '<strong>Logical Blocks used:</strong><br />' . $logical_blocks_used . '<br /><br />';
	$return_html .= '<strong>Name of compound for which process development is carried out:</strong><br />' . $prodata . '<br />';
	$return_html .= '<strong>List of compounds from DWSIM Database used in process flowsheet:</strong><br />' . $abstracts_pro->dwsim_database_compound_name . '<br /><br />';
	$return_html .= '<strong>List of user defined compounds used in process flowsheet:</strong><br />' . _dwsim_flowsheet_list_of_user_defined_compound($abstracts_pro->id) . '<br />';
	$return_html .= '<strong>Uploaded an abstract (brief outline) of the project:</strong><br />' . $abstract_filename . '<br /><br />';
	$return_html .= '<strong>Upload the DWSIM flowsheet for the developed process:</strong><br />' . $abstracts_query_process_filename . '<br /><br />';
	$return_html .= $download_flowsheet;
	return $return_html;
}
