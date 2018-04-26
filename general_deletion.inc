<?php
/******************************************************************************/
/****************************** DELETION FUNCTIONS ****************************/
/******************************************************************************/
function dwsim_flowsheet_delete_abstract_file($abstract_id)
{
	$status = TRUE;
	$root_path = dwsim_flowsheet_path();
	$abstract_q = db_query("SELECT * from dwsim_flowsheet_proposal dfp
LEFT JOIN dwsim_flowsheet_submitted_abstracts dfsa ON dfp.id = dfsa.proposal_id
WHERE dfsa.id = :abstract_id", array(
		":abstract_id" => $abstract_id
	));
	$abstract_data = $abstract_q->fetchObject();
	if (!$abstract_data)
	{
		drupal_set_message('Invalid flowsheet project abstract.', 'error');
		return FALSE;
	} //!$abstract_data
	/* deleting solutions */
	$query = db_select('dwsim_flowsheet_submitted_abstracts_file');
	$query->fields('dwsim_flowsheet_submitted_abstracts_file');
	$query->condition('submitted_abstract_id', $abstract_id);
	$abstract_f_q = $query->execute();
	
	while ($abstract_f_data = $abstract_f_q->fetchObject())
	{
		unlink($root_path . $abstract_data->directory_name . '/' . $abstract_f_data->filepath);

	} 
	//$abstract_f_data = $abstract_f_q->fetchObject()
	//!$delete_project_folder
	/*if ($status)
	{
		$dir_path_udc = $root_path . $abstract_f_data->directory_name . '/user_defined_compound';
		if (is_dir($dir_path_udc))
		{
			unlink($root_path . $abstract_f_data->directory_name . '/' . $abstract_f_data->user_defined_compound_filepath);
			$res = rmdir($dir_path_udc);
			if (!$res)
			{
				drupal_set_message(t('Error in deleting user defiend folder !folder', array(
					'!folder' => $abstract_f_data->directory_name
				)), 'error');
				/* sending email to admins */
		/*		$email_to = variable_get('dwsim_flowsheet_emails', '');
				$from = variable_get('dwsim_flowsheet_from_email', '');
				$bcc = "";
				$cc = variable_get('dwsim_flowsheet_cc_emails', '');
				$params['standard']['subject'] = "[ERROR] Error deleting experiment folder";
				$params['standard']['body'] = "Error deleting folder " . $dir_path;
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
					drupal_set_message('Error sending email message.', 'error');
				return FALSE;
			} //!$res
			else
			{
				return TRUE;
			}
		} //is_dir($dir_path_udc)
	} */ //$status
	return True;
}
function dwsim_flowsheet_abstract_delete_project($proposal_id)
{
	$status = TRUE;
	$root_path = dwsim_flowsheet_path();
	//$proposal_q = db_query("SELECT * FROM {dwsim_flowsheet_proposal} WHERE id = %d", $lab_id);
	$query = db_select('dwsim_flowsheet_proposal');
	$query->fields('dwsim_flowsheet_proposal');
	$query->condition('id', $proposal_id);
	$proposal_q = $query->execute();
	$proposal_data = $proposal_q->fetchObject();
	if (!$proposal_data)
	{
		drupal_set_message('Invalid Flowsheeting Project.', 'error');
		return FALSE;
	} //!$proposal_data
	$query = db_select('dwsim_flowsheet_submitted_abstracts');
	$query->fields('dwsim_flowsheet_submitted_abstracts');
	$query->condition('proposal_id', $proposal_data->id);
	$abstract_q = $query->execute();
	while ($abstract_data = $abstract_q->fetchObject())
	{
		if (!dwsim_flowsheet_delete_abstract_file($abstract_data->id))
		{
			$status = FALSE;
		} //!dwsim_flowsheet_delete_abstract_file($abstract_data->id)
	} //$abstract_data = $abstract_q->fetchObject()
	$dir_path_udc = $root_path . $proposal_data->directory_name . '/user_defined_compound';
	if (is_dir($dir_path_udc))
	{
		unlink($root_path . $proposal_data->directory_name . '/' . $proposal_data->user_defined_compound_filepath);
		$res = rmdir($dir_path_udc);
	} //is_dir($dir_path_udc)
	$dir_main_path = $root_path . $proposal_data->directory_name;
	//var_dump($dir_main_path);die;
	if (is_dir($dir_main_path))
	{
		$res_main = rmdir($dir_main_path);
	} //is_dir($dir_main_path)
	db_delete('dwsim_flowsheet_proposal')->condition('id', $proposal_data->id)->execute();
	db_delete('dwsim_flowsheet_submitted_abstracts')->condition('proposal_id', $proposal_data->id)->execute();
	db_delete('dwsim_flowsheet_submitted_abstracts_file')->condition('id', $abstract_f_data->id)->execute();
	return $status;
}/*
function _dwsim_flowsheet_delete_abstract_file($abstract_id)
{
	global $user;
	$root_path = dwsim_flowsheet_path();
	$query = db_select('dwsim_flowsheet_proposal');
	$query->fields('dwsim_flowsheet_proposal');
	$query->condition('id', $abstract_id);
	$abstract_q = $query->execute();
	$abstract_data = $abstract_q->fetchObject();
	$abstract_q = db_query("SELECT * from dwsim_flowsheet_proposal dfp
LEFT JOIN dwsim_flowsheet_submitted_abstracts dfsa ON dfp.id = dfsa.proposal_id
WHERE dfsa.id = :abstract_id", array(
		":abstract_id" => $abstract_id
	));
	$abstract_data = $abstract_q->fetchObject();
	if (!$abstract_data)
	{
		drupal_set_message('Invalid flowsheet project abstract.', 'error');
		return FALSE;
	} //!$abstract_data
	/* deleting abstract files */
	/*$query = db_select('dwsim_flowsheet_submitted_abstracts_file');
	$query->fields('dwsim_flowsheet_submitted_abstracts_file');
	$query->condition('submitted_abstract_id', $abstract_id);
	$abstract_f_q = $query->execute();
	while ($abstract_f_data = $abstract_f_q->fetchObject())
	{
		if (!file_exists($root_path . $abstract_data->directory_name . '/' . $abstract_f_data->filepath))
		{
			$status = FALSE;
			drupal_set_message(t('Error deleting !file. File does not exists.', array(
				'!file' => $abstract_data->directory_name . '/' . $abstract_f_data->filepath
			)), 'error');
			continue;
		} //!file_exists($root_path . $abstract_data->directory_name . '/' . $abstract_f_data->filepath)
		/* removing solution file */
	/*	if (!unlink($root_path . $abstract_data->directory_name . '/' . $abstract_f_data->filepath))
		{
			$status = FALSE;
			drupal_set_message(t('Error deleting !file', array(
				'!file' => $abstract_data->directory_name . '/' . $abstract_f_data->filepath
			)), 'error');
			/* sending email to admins */
	/*		$from = variable_get('dwsim_flowsheet_from_email', '');
			$bcc = variable_get('dwsim_flowsheet_emails', '');
			$cc = variable_get('dwsim_flowsheet_cc_emails', '');
			$params['standard']['subject'] = "[ERROR] Error deleting example file";
			$params['standard']['body'] = "Error deleting solution files by " . $user->uid . " at " . $_SERVER['HTTP_HOST'] . $_SERVER['REQUEST_URI'] . " :
        file id : " . $abstract_data->id . "
        file path : " . $abstract_data->directory_name . '/' . $abstract_f_data->filepath;
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
				drupal_set_message('Error sending email message.', 'error');
		} //!unlink($root_path . $abstract_data->directory_name . '/' . $abstract_f_data->filepath)
		else
		{
			/* deleting example files database entries */
			//db_delete('dwsim_flowsheet_submitted_abstracts_file')->condition('id', $abstract_f_data->id)->execute();
/*		}
	} //$abstract_f_data = $abstract_f_q->fetchObject()
	if (!$status)
	{
		return FALSE;
	} //!$status
	return $status;
}
