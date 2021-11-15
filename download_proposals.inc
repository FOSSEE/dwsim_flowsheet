<?php

function dwsim_flowsheet_download_completed_proposals()
{
	$output = "";
	$output .= "Click ".l("here","/flowsheeting-project/download-proposals"). " to download the Proposals of the participants" ."<h4>";

	return $output;

}