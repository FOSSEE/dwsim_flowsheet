<?php

//Proposal tab for reviewing approved proposals
function dwsim_flowsheet_approved_tab()
{
	$page_content = "";
	$result = db_query("SELECT * from dwsim_flowsheet_proposal where id not in (select proposal_id from dwsim_flowsheet_submitted_abstracts) AND approval_status = 1 order by approval_date desc");
	if ($result->rowCount() == 0)
	{
		$page_content .= "Approved Proposals under Flowsheeting Project<hr>";
	} //$result->rowCount() == 0
	else
	{
		$page_content .= "Approved Proposals under Flowsheeting Project: " . $result->rowCount() . "<hr>";
		$preference_rows = array();
		$i = 1;
		while ($row = $result->fetchObject())
		{
			$approval_date = date("d-M-Y", $row->approval_date);
			$preference_rows[] = array(
				$i,
				$row->project_title,
				$row->contributor_name,
				$row->university,
				$approval_date
			);
			$i++;
		} 
		$preference_header = array(
			'No',
			'Flowsheet Project',
			'Contributor Name',
			'University / Institute',
			'Date of Approval'
		);
		$page_content .= theme('table', array(
			'header' => $preference_header,
			'rows' => $preference_rows
		));
	}
	return $page_content;
}


///////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

//Proposal tab for reviewing Code submission of the uploaded proposals  

function dwsim_flowsheet_uploaded_tab()
{
	$page_content = "";
	$result = db_query("SELECT dfp.project_title, dfp.contributor_name, dfp.id, dfp.university, dfa.abstract_upload_date, dfa.abstract_approval_status from dwsim_flowsheet_proposal as dfp JOIN dwsim_flowsheet_submitted_abstracts as dfa on dfa.proposal_id = dfp.id where dfp.id in (select proposal_id from dwsim_flowsheet_submitted_abstracts) AND approval_status = 1");

	if ($result->rowCount() == 0)
	{
		$page_content .= "Uploaded Proposals under Flowsheeting Project<hr>";
	}
	else
	{
		$page_content .= "Uploaded Proposals under Flowsheeting Project: " . $result->rowCount() . "<hr>";
		$preference_rows = array();
		$i = 1;
		while ($row = $result->fetchObject())
		{
			$abstract_upload_date = date("d-M-Y", $row->abstract_upload_date);
			$preference_rows[] = array(
				$i,
				$row->project_title,
				$row->contributor_name,
				$row->university,
				$abstract_upload_date
			);
			$i++;
		}
		$preference_header = array(
			'No',
			'Flowsheet Project',
			'Contributor Name',
			'University / Institute',
			'Date of file submission'
		);
		$page_content .= theme('table', array(
			'header' => $preference_header,
			'rows' => $preference_rows
		));
	}
	return $page_content;
}


///////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

//Proposal tab for reviewing Completed proposals

function dwsim_flowsheet_completed_tab_form($form, $form_state)
{
	$options_first = _flowsheet_details_year_wise();
	$selected = isset($form_state['values']['howmany_select']) ? $form_state['values']['howmany_select'] : key($options_first);
	$form['howmany_select'] = array(
    '#title' => t('Sorting projects according to year:'),
    '#type' => 'select',
    '#options' => _flowsheet_details_year_wise(),
    /*'#options' => array(
    	'Please select...' => 'Please select...',
    	'2017' => '2017',
    	'2018' => '2018', 
    	'2019' => '2019', 
    	'2020' => '2020', 
    	'2021' => '2021'),*/
    '#default_value' => $selected,
    '#ajax' => array(
      'callback' => 'ajax_example_autocheckboxes_callback',
    ),
    '#suffix' => '<div id="ajax-selected-flowsheet"></div>'

	  );
	return $form;
}


function ajax_example_autocheckboxes_callback($form, $form_state)
{
	$commands = array();
	$flowsheet_project_default_value = $form_state['values']['howmany_select'];
	if ($flowsheet_project_default_value != 0)
	{
		$form['howmany_select']['#options'] =  array('Please select...' => 'Please select...');
		$commands[] = ajax_command_html('#ajax-selected-flowsheet', _flowsheet_details($flowsheet_project_default_value));
		$commands[] = ajax_command_replace('#ajax_selected_flowsheet_action', drupal_render($form['howmany_select']));
		
	} 
	else
	{
		$form['howmany_select']['#options'] =  array('Please select...' => 'Please select...');
		$commands[] = ajax_command_data('#ajax_selected_flowsheet', 'form_state_value_select', $form_state['values']['howmany_select']);
	}
	return array(
		'#type' => 'ajax',
		'#commands' => $commands
	);
}

function _flowsheet_details_year_wise()
  {
    $flowsheet_years = array(
		'0' => 'Please select...'
	);
    $result = db_query("SELECT from_unixtime(actual_completion_date, '%Y ') from dwsim_flowsheet_proposal WHERE approval_status = 3");
    
    /*$query = db_select('dwsim_flowsheet_proposal');
    $query->fields('dwsim_flowsheet_proposal');
    $query->condition('from_unixtime(actual_completion_date)');
    $query->condition('approval_status', 3);*/
    //$query->orderBy('id', 'DESC');
    //$year_wise_list = $query->execute();
    while ($year_wise_list_data = $result->fetchObject())
      {
        $flowsheet_years[$year_wise_list_data->id] = $year_wise_list_data->actual_completion_date;
      }
    return $flowsheet_years;
  }


function _flowsheet_details($flowsheet_proposal_id)
{
	$output = "";
	$output = "Click <a href='/flowsheeting-project/download-proposals'>here</a> to download the Proposals of the participants <br>";
	$result = db_query("SELECT * from {dwsim_flowsheet_proposal} WHERE approval_status = 3 and from_unixtime(actual_completion_date, '%Y') = :year",
		array(
							':year' => $flowsheet_proposal_id)
				 );
	
	if ($result->rowCount() == 0)
	{	
		// $output = "Click <a href='/flowsheeting-project/download-proposals/'>here</a> to download the Proposals of the participants";
		$output .= "Work has been completed for the following flow sheets.";

	} 
	else
	{
		
		$output .= "Work has been completed for the following flow sheets: " . $result->rowCount() ;
		$preference_rows = array();
		$i = 1;
		while ($row = $result->fetchObject())
		{
			$completion_date = date("d-M-Y", $row->actual_completion_date);
			$preference_rows[] = array(
				$i,
				l($row->project_title, "flowsheeting-project/dwsim-flowsheet-run/" . $row->id, array('attributes' => array('title' => 'This is a zip file containing a pdf (abstract) and a dwxml/dwxmz file which is the DWSIM flow sheet which is to be viewed by right clicking on the file and opening with DWSIM.'))),
				$row->contributor_name,
				$row->university,
				$completion_date
			);
			$i++;
		} 
		$preference_header = array(
			'No',
			'Flowsheet Project',
			'Contributor Name',
			'University / Institute',
			'Date of Completion'
		);
		$output .= theme('table', array(
			'header' => $preference_header,
			'rows' => $preference_rows
		));
	}
	return $output;
}


///////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

//Proposal tab for Downloading proposals

function dwsim_flowsheet_download_proposals_tab()
{
		$output= "";
		

}