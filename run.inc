<?php
function dwsim_flowsheet_run_form($form, &$form_state)
{
	$options_first = _list_of_flowsheet();
	$url_flowsheet_id = (int) arg(2);
	$flowsheet_data = _flowsheet_information($url_flowsheet_id);
	if ($flowsheet_data == 'Not found') {
		$url_flowsheet_id = '';
	} //$flowsheet_data == 'Not found'
	if (!$url_flowsheet_id) {
		$selected = isset($form_state['values']['flowsheet']) ? $form_state['values']['flowsheet'] : key($options_first);
	} //!$url_flowsheet_id
	elseif ($url_flowsheet_id == '') {
		$selected = 0;
	} //$url_flowsheet_id == ''
	else {
		$selected = $url_flowsheet_id;
	}
	$form = array();
	$form['flowsheet'] = array(
		'#type' => 'select',
		'#title' => t('Title of the flowsheet'),
		'#options' => _list_of_flowsheet(),
		'#default_value' => $selected,
		'#ajax' => array(
			'callback' => 'dwsim_flowsheet_project_details_callback'
		)
	);
	if (!$url_flowsheet_id) {
		$form['flowsheet_details'] = array(
			'#type' => 'item',
			'#markup' => '<div id="ajax_flowsheet_details"></div>'
		);
		$form['selected_flowsheet'] = array(
			'#type' => 'item',
			'#markup' => '<div id="ajax_selected_flowsheet"></div>'
		);
	} //!$url_flowsheet_id
	else {
		$flowsheet_default_value = $url_flowsheet_id;
		$form['flowsheet_details'] = array(
			'#type' => 'item',
			'#markup' => '<div id="ajax_flowsheet_details">' . _flowsheet_details($flowsheet_default_value) . '</div>'
		);
		$form['selected_flowsheet'] = array(
			'#type' => 'item',
			'#markup' => '<div id="ajax_selected_flowsheet">' . l('Download Flowsheet', 'flowsheeting-project/full-download/project/' . $flowsheet_default_value,array('attributes' => array('title' => 'This is a zip file containing a pdf (abstract) and a dwxml/dwxmz file which is the DWSIM flow sheet which is to be viewed by right clicking on the file and opening with DWSIM.'))) . '</div>'
		);
	}
	return $form;
}
function dwsim_flowsheet_project_details_callback($form, $form_state)
{
	$commands = array();
	$flowsheet_default_value = $form_state['values']['flowsheet'];
	if ($flowsheet_default_value != 0) {
		$form['flowsheet_details']['#markup'] = _flowsheet_details($flowsheet_default_value);
		$flowsheet_details = _flowsheet_information($flowsheet_default_value);
		$provider = user_load($flowsheet_details->uid);
		if ($flowsheet_details->uid > 0) {
			$commands[] = ajax_command_html('#ajax_selected_flowsheet', l('Download Flowsheet', 'flowsheeting-project/full-download/project/' . $flowsheet_default_value,array('attributes' => array('title' => 'This is a zip file containing a pdf (abstract) and a dwxml/dwxmz file which is the DWSIM flow sheet which is to be viewed by right clicking on the file and opening with DWSIM.'))));
		} //$flowsheet_details->uid > 0
		else {
			$commands[] = ajax_command_html('#ajax_selected_flowsheet', '');
			$commands[] = ajax_command_html('#ajax_selected_flowsheet_dwsim', '');
		}
		$commands[] = ajax_command_html('#ajax_flowsheet_details', _flowsheet_details($flowsheet_default_value));
	} //$flowsheet_default_value != 0
	else {
		// $form['lab_experiment_list']['#options'] = _ajax_get_experiment_list();
		//  $commands[] = ajax_command_replace('#ajax_selected_experiment', drupal_render($form['lab_experiment_list']));
		$commands[] = ajax_command_html('#ajax_flowsheet_details', '');
		$commands[] = ajax_command_html('#ajax_selected_flowsheet', '');
		$commands[] = ajax_command_html('#ajax_selected_flowsheet_dwsim', '');
		$commands[] = ajax_command_data('#ajax_selected_flowsheet', 'form_state_value_select', $form_state['values']['flowsheet']);
	}
	return array(
		'#type' => 'ajax',
		'#commands' => $commands
	);
}
function bootstrap_table_format($headers, $rows)
{
	$thead = "";
	$tbody = "";
	foreach ($headers as $header) {
		$thead .= "<th>{$header}</th>";
	} //$headers as $header
	foreach ($rows as $row) {
		$tbody .= "<tr>";
		foreach ($row as $data) {
			$tbody .= "<td>{$data}</td>";
		} //$row as $data
		$tbody .= "</tr>";
	} //$rows as $row
	$table = "
            <table class='table table-bordered table-hover' style='margin-left:-140px'>
                <thead>{$thead}</thead>
                <tbody>{$tbody}</tbody>
            </table>
        ";
	return $table;
}
/*****************************************************/
function _list_of_flowsheet()
{
	$flowsheet_titles = array(
		'0' => 'Please select...'
	);
	//$lab_titles_q = db_query("SELECT * FROM {dwsim_flowsheet_proposal} WHERE solution_display = 1 ORDER BY lab_title ASC");
	$query = db_select('dwsim_flowsheet_proposal');
	$query->fields('dwsim_flowsheet_proposal');
	$query->condition('approval_status', 3);
	$query->orderBy('project_title', 'ASC');
	$flowsheet_titles_q = $query->execute();
	while ($flowsheet_titles_data = $flowsheet_titles_q->fetchObject()) {
		$flowsheet_titles[$flowsheet_titles_data->id] = $flowsheet_titles_data->project_title . ' (Proposed by ' . $flowsheet_titles_data->name_title . ' ' . $flowsheet_titles_data->contributor_name . ')';
	} //$flowsheet_titles_data = $flowsheet_titles_q->fetchObject()
	return $flowsheet_titles;
}
function _flowsheet_information($proposal_id)
{
	$query = db_select('dwsim_flowsheet_proposal');
	$query->fields('dwsim_flowsheet_proposal');
	$query->condition('id', $proposal_id);
	$query->condition('approval_status', 3);
	$flowhsheet_q = $query->execute();
	$flowsheet_data = $flowhsheet_q->fetchObject();
	if ($flowsheet_data) {
		return $flowsheet_data;
	} //$flowsheet_data
	else {
		return 'Not found';
	}
}
function _flowsheet_details($flowsheet_default_value)
{
	$flowsheet_details = _flowsheet_information($flowsheet_default_value);
	if ($flowsheet_default_value != 0) {
		$form['flowsheet_details']['#markup'] = '<span style="color: rgb(128, 0, 0);"><strong>About the Flowsheet</strong></span></td><td style="width: 35%;"><br />' . '<ul>' . '<li><strong>Proposer Name:</strong> ' . $flowsheet_details->name_title . ' ' . $flowsheet_details->contributor_name . '</li>' . '<li><strong>Title of the Flowhseet:</strong> ' . l($flowsheet_details->project_title,'flowsheeting-project/full-download/project/' . $flowsheet_default_value,array('attributes' => array('title' => 'This is a zip file containing a pdf (abstract) and a dwxml/dwxmz file which is the DWSIM flow sheet which is to be viewed by right clicking on the file and opening with DWSIM.'))) . '</li>' . '<li><strong>Institution:</strong> ' . $flowsheet_details->university . '</li>' . '<li><strong>Version:</strong> ' . $flowsheet_details->version . '</li>' . '<li><strong>Reference:</strong> ' . $flowsheet_details->reference . '</li>' . '</ul>';
		$details = $form['flowsheet_details']['#markup'];
		return $details;
	} //$flowsheet_default_value != 0
}
