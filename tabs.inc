<?php

function dwsim_flowsheet_uploaded_tab()
{
	$page_content = "";
	$result =  db_query("SELECT dfp.project_title, dfp.contributor_name, dfp.university, dfp.approval_date
			   FROM dwsim_flowsheet_proposal dfp LEFT JOIN dwsim_flowsheet_submitted_abstracts dfsa 
			   ON dfp.id= dfsa.proposal_id WHERE approval_status = 1 
			   AND user_defined_compound_filepath = 'NULL' ORDER BY dfsa.abstract_upload_date ASC");
	//var_dump($result);die;

	//$result = $query->execute();
	if ($result->rowCount() == 0)
	{
		$page_content .= "Uploaded Proposals under Flowsheeting Project<hr>";
	} //$result->rowCount() == 0
	else
	{
		$page_content .= "Uploaded Proposals under Flowsheeting Project<hr>";;
		$preference_rows = array();
		$i = 1;
		while ($row = $result->fetchObject())
		{
			$approval_date = date("Y", $row->approval_date);
			$preference_rows[] = array(
				$i,
				$row->project_title,
				$row->contributor_name,
				$row->university,
				$approval_date
			);
			$i++;
		} //$row = $result->fetchObject()
		$preference_header = array(
			'No',
			'Flowsheet Project',
			'Contributor Name',
			'University / Institute',
			'Year'
		);
		$page_content .= theme('table', array(
			'header' => $preference_header,
			'rows' => $preference_rows
		));
	}
	return $page_content;
}







function dwsim_flowsheet_progress_tab()
{
	$page_content = "";
	$query = db_select('dwsim_flowsheet_proposal');
	$query->fields('dwsim_flowsheet_proposal');
	$query->condition('approval_status', 1);
	$query->condition('is_completed', 0);
	$query->orderBy('approval_date', 'DESC');
	$result = $query->execute();
	if ($result->rowCount() == 0)
	{
		$page_content .= "Approved Proposals under Flowsheeting Project<hr>";
	} //$result->rowCount() == 0
	else
	{
		$page_content .= "Approved Proposals under Flowsheeting Project<hr>";;
		$preference_rows = array();
		$i = 1;
		while ($row = $result->fetchObject())
		{
			$approval_date = date("Y", $row->approval_date);
			$preference_rows[] = array(
				$i,
				$row->project_title,
				$row->contributor_name,
				$row->university,
				$approval_date
			);
			$i++;
		} //$row = $result->fetchObject()
		$preference_header = array(
			'No',
			'Flowsheet Project',
			'Contributor Name',
			'University / Institute',
			'Year'
		);
		$page_content .= theme('table', array(
			'header' => $preference_header,
			'rows' => $preference_rows
		));
	}
	return $page_content;
}

function dwsim_flowsheet_completed_tab_form($form, $form_state){
	/*$options_first = _list_of_flowsheet_project();
	$selected = isset($form_state['values']['howmany_select']) ? $form_state['values']['howmany_select'] : key($options_first);*/
	$form['howmany_select'] = array(
    '#title' => t('Sorting projects according to year:'),
    '#type' => 'select',
    '#options' => array(2017 => 2017, 2018 => 2018, 2019 => 2019, 2020 => 2020, 2021 => 2021),
    //'#default_value' => $selected,
    '#ajax' => array(
      'callback' => 'ajax_example_autocheckboxes_callback',
      //'wrapper' => 'checkboxes-div',
      /*'method' => 'replace',
      'effect' => 'fade',*/
    ),
    '#suffix' => '<div id="ajax_selected_flowsheet"></div>'

	  );
	return $form;
}

function ajax_example_autocheckboxes_callback($form, $form_state)
{
	$commands = array();
	$flowsheet_project_default_value = $form_state['values']['howmany_select'];
	//var_dump($flowsheet_project_default_value);die;
	if ($flowsheet_project_default_value != 0)
	{
		$commands[] = ajax_command_replace('#ajax_selected_flowsheet', _flowsheet_details($flowsheet_project_default_value));
	} //$flowsheet_project_default_value != 0
	else
	{
		$commands[] = ajax_command_replace('#ajax_selected_flowsheet', '');
		$commands[] = ajax_command_data('#ajax_selected_flowsheet', 'form_state_value_select', $form_state['values']['howmany_select']);
	}
	return array(
		'#type' => 'ajax',
		'#commands' => $commands
	);
}


function _flowsheet_details($flowsheet_proposal_id){
	//var_dump($flowsheet_proposal_id);die;
	$output = "";
	$result = db_query("SELECT * from {dwsim_flowsheet_proposal} WHERE approval_status = 3 and from_unixtime(actual_completion_date, '%Y') = :year", array(
							':year' => $flowsheet_proposal_id));
	//$sql = $result->execute();
	//var_dump($result);die;
	if ($result->rowCount() == 0)
	{
		$output .= "Work has been completed for the following flow sheets.";

	} //$result->rowCount() == 0
	else
	{
		$output .= "Work has been completed for the following flow sheets";
		$preference_rows = array();
		$i = 1;
		while ($row = $result->fetchObject())
		{
			$completion_date = date("Y", $row->actual_completion_date);
			$preference_rows[] = array(
				$i,
				l($row->project_title, "flowsheeting-project/dwsim-flowsheet-run/" . $row->id, array('attributes' => array('title' => 'This is a zip file containing a pdf (abstract) and a dwxml/dwxmz file which is the DWSIM flow sheet which is to be viewed by right clicking on the file and opening with DWSIM.'))),
				$row->contributor_name,
				$row->university,
				$completion_date
			);
			$i++;
		} //$row = $result->fetchObject()
		$preference_header = array(
			'No',
			'Flowsheet Project',
			'Contributor Name',
			'University / Institute',
			'Year of Completion'
		);
		$output .= theme('table', array(
			'header' => $preference_header,
			'rows' => $preference_rows
		));
	}
	return $output;
}

function _list_of_flowsheet_project()
{
	$options_first = array(
		'0' => 'Please select...'
	);
	if(isset($form_state['values']['howmany_select'])) {
    		$rows = db_select('dwsim_flowsheet_proposal', 'actual_completion_date')
     		->fields('actual_completion_date')
      		->condition('approval_status', $form_state['values']['howmany_select'])
      		->execute()
      		->fetchAll();

    		$header = array();
    		$form['results']['#markup'] = theme('table', array('header' => $header, 'rows' => $rows));
			  }
			  else {
			    $form['results']['#markup'] = '<p>The results will appear here when you change the dropdown option.</p>';
			  }
}