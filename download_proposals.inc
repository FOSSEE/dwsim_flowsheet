<?php

function dwsim_flowsheet_download_completed_proposals()
{
	$output = "";
	$output .= "Click ".l("here","/flowsheeting-project/download-proposals-all"). " to download the proposals of the participants" ."<h4>";

	return $output;

}