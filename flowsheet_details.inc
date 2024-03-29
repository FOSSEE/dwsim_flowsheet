<?php
// DWSIM Flowsheet display completed proposals
function dwsim_flowsheet_completed_proposals_all()
{
	$output = "";
	$query = db_select('dwsim_flowsheet_proposal');
	$query->fields('dwsim_flowsheet_proposal');
	$query->condition('approval_status', 3);
	$query->orderBy('actual_completion_date', 'DESC');
	//$query->condition('is_completed', 1);
	$result = $query->execute();
	if ($result->rowCount() == 0)
	{
		$output .= "Work has been completed for the following flow sheets. We welcome your contributions. For more details, please visit ".l("https://dwsim.fossee.in/flowsheeting-project","https://dwsim.fossee.in/flowsheeting-project", array('attributes' => array('target' => '_blank')))."<br>"."<h4>"."If you are looking for flowsheeting project ideas, ".l("click here","https://dwsim.fossee.in/flowsheeting-ideas",array('attributes' => array('class' => array('flash_content'), 'target' => '_blank')))."</h4>"."<hr>";

	} //$result->rowCount() == 0
	else
	{
		$output .= "Total number of completed flowsheets: " . $result->rowCount() . "<br>";
		$output .= "Work has been completed for the following flow sheets. We welcome your contributions. For more details, please visit ".l("https://dwsim.fossee.in/flowsheeting-project","https://dwsim.fossee.in/flowsheeting-project", array('attributes' => array('target' => '_blank')))."<br>"."<h4>"."If you are looking for flowsheeting project ideas, ".l("click here","https://dwsim.fossee.in/flowsheeting-ideas",array('attributes' => array('class' => array('flash_content'), 'target' => '_blank')))."</h4>"."<hr>";
		$preference_rows = array();
		$i = $result->rowCount();
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
			$i--;
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
// DWSIM Flowsheet display in progress proposals
function dwsim_flowsheet_progress_all()
{
	$page_content = "";
	$query = db_select('dwsim_flowsheet_proposal');
	$query->fields('dwsim_flowsheet_proposal');
	$query->condition('approval_status', 1);
	$query->condition('is_completed', 0);
	$result = $query->execute();
	if ($result->rowCount() == 0)
	{
		$page_content .= "Work is in progress for the following flowsheets under Flowsheeting Project<hr>";
	} //$result->rowCount() == 0
	else
	{
		$page_content .= "Work is in progress for the following flowsheets under Flowsheeting Project<hr>";;
		$preference_rows = array();
		$i = $result->rowCount();
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
			$i--;
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