<?php
// $Id$
function dwsim_flowsheet_download_full_project()
{
	global $user;
	$flowsheet_id = arg(3);
	$root_path = dwsim_flowsheet_path();
	$query = db_select('dwsim_flowsheet_proposal');
	$query->fields('dwsim_flowsheet_proposal');
	$query->condition('id', $flowsheet_id);
	$flowsheet_q = $query->execute();
	$flowsheet_data = $flowsheet_q->fetchObject();
	$FLOWSHEET_PATH = $flowsheet_data->directory_name . '/';
	/* zip filename */
	$zip_filename = $root_path . 'zip-' . time() . '-' . rand(0, 999999) . '.zip';
	/* creating zip archive on the server */
	$zip = new ZipArchive();
	$zip->open($zip_filename, ZipArchive::CREATE);
	$query = db_select('dwsim_flowsheet_proposal');
	$query->fields('dwsim_flowsheet_proposal');
	$query->condition('id', $flowsheet_id);
	$flowsheet_udc_q = $query->execute();
	while ($flowsheet_udc_row = $flowsheet_udc_q->fetchObject()) {
		if ($flowsheet_udc_row->user_defined_compound_filepath || $flowsheet_udc_row->user_defined_compound_filepath != 'NULL') {
			$USER_DEFINED_PATH = 'user_defined_compound/';
			$zip->addFile($root_path . $FLOWSHEET_PATH . '/' . $flowsheet_udc_row->user_defined_compound_filepath, $FLOWSHEET_PATH . $USER_DEFINED_PATH . str_replace(' ', '_', basename($flowsheet_udc_row->user_defined_compound_filepath)));
		} //$flowsheet_udc_row->user_defined_compound_filepath || $flowsheet_udc_row->user_defined_compound_filepath != 'NULL'
	} //$flowsheet_udc_row = $flowsheet_udc_q->fetchObject()
	$query = db_select('dwsim_flowsheet_submitted_abstracts_file');
	$query->fields('dwsim_flowsheet_submitted_abstracts_file');
	$query->condition('proposal_id', $flowsheet_id);
	$flowsheet_f_q = $query->execute();
	while ($flowsheet_f_row = $flowsheet_f_q->fetchObject()) {
		$zip->addFile($root_path . $FLOWSHEET_PATH . '/' . $flowsheet_f_row->filepath, $FLOWSHEET_PATH . str_replace(' ', '_', basename($flowsheet_f_row->filename)));
	} //$flowsheet_f_row = $flowsheet_f_q->fetchObject()
	$zip_file_count = $zip->numFiles;
	$zip->close();
	if ($zip_file_count > 0) {
		if ($user->uid) {
			/* download zip file */
			header('Content-Type: application/zip');
			header('Content-disposition: attachment; filename="' . str_replace(' ', '_', $flowsheet_data->project_title) . '.zip"');
			header('Content-Length: ' . filesize($zip_filename));
			ob_end_flush();
			ob_clean();
			flush();
			readfile($zip_filename);
			unlink($zip_filename);
		} //$user->uid
		else {
			header('Content-Type: application/zip');
			header('Content-disposition: attachment; filename="' . str_replace(' ', '_', $flowsheet_data->project_title) . '.zip"');
			header('Content-Length: ' . filesize($zip_filename));
			header("Content-Transfer-Encoding: binary");
			header('Expires: 0');
			header('Pragma: no-cache');
			ob_end_flush();
			ob_clean();
			flush();
			readfile($zip_filename);
			unlink($zip_filename);
		}
	} //$zip_file_count > 0
	else {
		drupal_set_message("There are no flowsheet project in this proposal to download", 'error');
		drupal_goto('flowsheeting-project/full-download/project');
	}
}
function dwsim_flowsheet_download_completed_project()
{
	global $user;
	$flowsheet_id = arg(3);
	$root_path = dwsim_flowsheet_path();
	$query = db_select('dwsim_flowsheet_proposal');
	$query->fields('dwsim_flowsheet_proposal');
	$query->condition('id', $flowsheet_id);
	$flowsheet_q = $query->execute();
	$flowsheet_data = $flowsheet_q->fetchObject();
	$FLOWSHEET_PATH = $flowsheet_data->directory_name . '/';
	/* zip filename */
	$zip_filename = $root_path . 'zip-' . time() . '-' . rand(0, 999999) . '.zip';
	/* creating zip archive on the server */
	$zip = new ZipArchive();
	$zip->open($zip_filename, ZipArchive::CREATE);
	$query = db_select('dwsim_flowsheet_proposal');
	$query->fields('dwsim_flowsheet_proposal');
	$query->condition('id', $flowsheet_id);
	$flowsheet_udc_q = $query->execute();
	while ($flowsheet_udc_row = $flowsheet_udc_q->fetchObject()) {
		if ($flowsheet_udc_row->user_defined_compound_filepath || $flowsheet_udc_row->user_defined_compound_filepath != 'NULL') {
			$USER_DEFINED_PATH = 'user_defined_compound/';
			$zip->addFile($root_path . $FLOWSHEET_PATH . '/' . $flowsheet_udc_row->user_defined_compound_filepath, $USER_DEFINED_PATH . str_replace(' ', '_', basename($flowsheet_udc_row->user_defined_compound_filepath)));
		} //$flowsheet_udc_row->user_defined_compound_filepath || $flowsheet_udc_row->user_defined_compound_filepath != 'NULL'
	} //$flowsheet_udc_row = $flowsheet_udc_q->fetchObject()
	$query = db_select('dwsim_flowsheet_submitted_abstracts_file');
	$query->fields('dwsim_flowsheet_submitted_abstracts_file');
	$query->condition('proposal_id', $flowsheet_id);
	$flowsheet_f_q = $query->execute();
	while ($flowsheet_f_row = $flowsheet_f_q->fetchObject()) {
		$zip->addFile($root_path . $FLOWSHEET_PATH . '/' . $flowsheet_f_row->filepath, str_replace(' ', '_', basename($flowsheet_f_row->filename)));
	} //$flowsheet_f_row = $flowsheet_f_q->fetchObject()
	$zip_file_count = $zip->numFiles;
	$zip->close();
	if ($zip_file_count > 0) {
		if ($user->uid) {
			/* download zip file */
			header('Content-Type: application/zip');
			header('Content-disposition: attachment; filename="' . str_replace(' ', '_', $flowsheet_data->project_title) . '.zip"');
			header('Content-Length: ' . filesize($zip_filename));
			ob_end_flush();
			ob_clean();
			flush();
			readfile($zip_filename);
			unlink($zip_filename);
		} //$user->uid
		else {
			header('Content-Type: application/zip');
			header('Content-disposition: attachment; filename="' . str_replace(' ', '_', $flowsheet_data->project_title) . '.zip"');
			header('Content-Length: ' . filesize($zip_filename));
			header("Content-Transfer-Encoding: binary");
			header('Expires: 0');
			header('Pragma: no-cache');
			ob_end_flush();
			ob_clean();
			flush();
			readfile($zip_filename);
			unlink($zip_filename);
		}
	} //$zip_file_count > 0
	else {
		drupal_set_message("There are no flowsheet project in this proposal to download", 'error');
		drupal_goto('flowsheeting-project/dwsim-flowsheet-run');
	}
}
