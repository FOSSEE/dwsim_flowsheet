<?php
/**
 * Implementation of hook_mail().
 */
function dwsim_flowsheet_mail($key, &$message, $params)
{
	global $user;
	$language = $message['language'];
	//$language = user_preferred_language($user);
	switch ($key)
	{
		case 'dwsim_flowsheet_proposal_received':
			/* initializing data */
			$query = db_select('dwsim_flowsheet_proposal');
			$query->fields('dwsim_flowsheet_proposal');
			$query->condition('id', $params['dwsim_flowsheet_proposal_received']['proposal_id']);
			$query->range(0, 1);
			$proposal_data = $query->execute()->fetchObject();
			$phpdate = time( $proposal_data->creation_date );
			$user_data = user_load($params['dwsim_flowsheet_proposal_received']['user_id']);
			if ($proposal_data->project_guide_name == "NULL" || $proposal_data->project_guide_name == "")
			{
				$project_guide_name = "Not Entered";
			} //$proposal_data->project_guide_name == NULL
			else
			{
				$project_guide_name = $proposal_data->project_guide_name;
			}
			if ($proposal_data->project_guide_email_id == "NULL" || $proposal_data->project_guide_email_id == "")
			{
				$project_guide_email_id = "Not Entered";
			} //$proposal_data->project_guide_email_id == NULL
			else
			{
				$project_guide_email_id = $proposal_data->project_guide_email_id;
			}

			$message['headers'] = $params['dwsim_flowsheet_proposal_received']['headers'];
			$message['subject'] = t('[!site_name][Flowsheeting Project] Your DWSIM Flowsheeting Project proposal has been received', array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				'body' => t('
Dear !user_name,

Thank you for submitting proposal for the DWSIM Flowsheeting Project. The details of your proposal are as below.

Full Name: ' . $proposal_data->name_title . ' ' . $proposal_data->contributor_name . '
Email: ' . $user_data->mail . '
University/Institute: ' . $proposal_data->university . '
City: ' . $proposal_data->city . '
Project Title: ' . $proposal_data->project_title . '
Name of compound for which process development is carried out: ' . $proposal_data->process_development_compound_name . '

Your proposal is under review. You will be notified via email about the approval/disapproval within ten working days.


Best Wishes,

!site_name Team,
FOSSEE,IIT Bombay', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user_data->name
				), array(
					'language' => $language->language
				))
			);
			break;
		case 'dwsim_flowsheet_proposal_disapproved':
			/* initializing data */
			$query = db_select('dwsim_flowsheet_proposal');
			$query->fields('dwsim_flowsheet_proposal');
			$query->condition('id', $params['dwsim_flowsheet_proposal_disapproved']['proposal_id']);
			$query->range(0, 1);
			$proposal_data = $query->execute()->fetchObject();
			$phpdate = time( $proposal_data->creation_date );
			$user_data = user_load($params['dwsim_flowsheet_proposal_disapproved']['user_id']);
			if ($proposal_data->project_guide_name == "NULL" || $proposal_data->project_guide_name == "")
			{
				$project_guide_name = "Not Entered";
			} //$proposal_data->project_guide_name == NULL
			else
			{
				$project_guide_name = $proposal_data->project_guide_name;
			}
			if ($proposal_data->project_guide_email_id == "NULL" || $proposal_data->project_guide_email_id == "")
			{
				$project_guide_email_id = "Not Entered";
			} //$proposal_data->project_guide_email_id == NULL
			else
			{
				$project_guide_email_id = $proposal_data->project_guide_email_id;
			}
			$message['headers'] = $params['dwsim_flowsheet_proposal_disapproved']['headers'];
			$message['subject'] = t('[!site_name][Flowsheeting Project] Your DWSIM Flowsheeting Project proposal has been disapproved', array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				'body' => t('
Dear !user_name,

We regret to inform you that your DWSIM Flowsheeting Project proposal with following details have been disapproved.

Full Name: ' . $proposal_data->name_title . ' ' . $proposal_data->contributor_name . '
Project Title: ' . $proposal_data->project_title . '
Name of compound for which process development is carried out: ' . $proposal_data->process_development_compound_name . '
Reason(s) / Feedback: ' . $proposal_data->dissapproval_reason . '

Please note that any files that were uploaded along with the proposal form will be deleted from all our database(s).

Thank you for participating in the DWSIM Flowsheeting Project. You are welcome to submit a new proposal.


Best Wishes,

!site_name Team,
FOSSEE,IIT Bombay', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user_data->name
				), array(
					'language' => $language->language
				))
			);
			break;
		case 'dwsim_flowsheet_proposal_approved':
			/* initializing data */
			$query = db_select('dwsim_flowsheet_proposal');
			$query->fields('dwsim_flowsheet_proposal');
			$query->condition('id', $params['dwsim_flowsheet_proposal_approved']['proposal_id']);
			$query->range(0, 1);
			$proposal_data = $query->execute()->fetchObject();
			$user_data = user_load($params['dwsim_flowsheet_proposal_approved']['user_id']);
			$url = 'http://dwsim.fossee.in/flowsheeting-project/coding-guidelines-flowsheeting';
			if ($proposal_data->project_guide_name == "NULL" || $proposal_data->project_guide_name == "")
			{
				$project_guide_name = "Not Entered";
			} //$proposal_data->project_guide_name == NULL
			else
			{
				$project_guide_name = $proposal_data->project_guide_name;
			}
			if ($proposal_data->project_guide_email_id == "NULL" || $proposal_data->project_guide_email_id == "")
			{
				$project_guide_email_id = "Not Entered";
			} //$proposal_data->project_guide_email_id == NULL
			else
			{
				$project_guide_email_id = $proposal_data->project_guide_email_id;
			}
			$message['headers'] = $params['dwsim_flowsheet_proposal_approved']['headers'];
			$message['subject'] = t('[!site_name][Flowsheeting Project] Your DWSIM Flowsheeting Project proposal has been approved', array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				'body' => t('
Dear !user_name,

Congratulations! Your proposal for DWSIM Flowsheeting Project with the following details is approved.

Full Name : ' . $proposal_data->name_title . ' ' . $proposal_data->contributor_name . '
Project Title  : ' . $proposal_data->project_title . '
Name of compound for which process development is carried out  : ' . $proposal_data->process_development_compound_name . '

Please ensure that ALL the guidelines for coding at http://dwsim.fossee.in/flowsheeting-project/coding-guidelines-flowsheeting are strictly followed. Refer http://dwsim.fossee.in/flowsheeting-project/flowsheeting-internship for the further procedure for submission of the project. Kindly ensure that the flowsheet and abstract are submitted within 1 month.

You can upload your abstract using abstract submission interface.

Best Wishes,

!site_name Team,
FOSSEE,IIT Bombay', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user_data->name
				), array(
					'language' => $language->language
				))
			);
			break;
		case 'dwsim_flowsheet_proposal_completed':
			/* initializing data */
			$query = db_select('dwsim_flowsheet_proposal');
			$query->fields('dwsim_flowsheet_proposal');
			$query->condition('id', $params['dwsim_flowsheet_proposal_completed']['proposal_id']);
			$query->range(0, 1);
			$proposal_data = $query->execute()->fetchObject();
			$user_data = user_load($params['dwsim_flowsheet_proposal_completed']['user_id']);
			if ($proposal_data->project_guide_name == "NULL" || $proposal_data->project_guide_name == "")
			{
				$project_guide_name = "Not Entered";
			} //$proposal_data->project_guide_name == NULL
			else
			{
				$project_guide_name = $proposal_data->project_guide_name;
			}
			if ($proposal_data->project_guide_email_id == "NULL" || $proposal_data->project_guide_email_id == "")
			{
				$project_guide_email_id = "Not Entered";
			} //$proposal_data->project_guide_email_id == NULL
			else
			{
				$project_guide_email_id = $proposal_data->project_guide_email_id;
			}
			$message['headers'] = $params['dwsim_flowsheet_proposal_completed']['headers'];
			$message['subject'] = t('[!site_name][Flowsheeting Project] Your DWSIM Flowsheeting Project proposal has been completed', array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				'body' => t('
Dear !user_name,

Your DWSIM flowsheet and abstract on the following process have been published successfully.

Full Name : ' . $proposal_data->name_title . ' ' . $proposal_data->contributor_name . '
Project Title  : ' . $proposal_data->project_title . '

Kindly access this link to download the same.
http://dwsim.fossee.in/flowsheeting-project/completed-flowsheet

You can now download your e-certificate after logging into the DWSIM website: dwsim.fossee.in.  On the left-hand side panel, go to User Dashboard > Flowsheet Certificates > Download. In case of any difficulties, kindly let us know.

Thank you for participating in the DWSIM Flowsheeting Project. You are welcome to propose a new flowsheet.


Best Wishes,

!site_name Team,
FOSSEE,IIT Bombay', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user_data->name
				), array(
					'language' => $language->language
				))
			);
			break;
		case 'dwsim_flowsheet_proposal_deleted':
			/* initializing data */
			$query = db_select('dwsim_flowsheet_proposal');
			$query->fields('dwsim_flowsheet_proposal');
			$query->condition('id', $params['dwsim_flowsheet_proposal_deleted']['proposal_id']);
			$query->range(0, 1);
			$proposal_data = $query->execute()->fetchObject();
			$user_data = user_load($params['dwsim_flowsheet_proposal_deleted']['user_id']);
			
			if ($proposal_data->project_guide_name == "NULL" || $proposal_data->project_guide_name == "")
			{
				$project_guide_name = "Not Entered";
			} //$proposal_data->project_guide_name == NULL
			else
			{
				$project_guide_name = $proposal_data->project_guide_name;
			}
			if ($proposal_data->project_guide_email_id == "NULL" || $proposal_data->project_guide_email_id == "")
			{
				$project_guide_email_id = "Not Entered";
			} //$proposal_data->project_guide_email_id == NULL
			else
			{
				$project_guide_email_id = $proposal_data->project_guide_email_id;
			}
			$message['headers'] = $params['dwsim_flowsheet_proposal_deleted']['headers'];
			$message['subject'] = t('[!site_name][Flowsheeting Project] Your DWSIM Flowsheeting Project proposal has been deleted', array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				'body' => t('
Dear !user_name,

Your DWSIM Flowsheeting Project proposal with the following details has been deleted :

Full Name : ' . $proposal_data->name_title . ' ' . $proposal_data->contributor_name . '
Email : ' . $user_data->mail . '
Month and Year of Award of Degree : ' . $proposal_data->month_year_of_degree . '
University/Institute : ' . $proposal_data->university . '
City : ' . $proposal_data->city . '
State : ' . $proposal_data->state . '
Country : ' . $proposal_data->country . '

Project Guide  : ' . $project_guide_name . '
Project Guide Email : ' . $project_guide_email_id . '

DWSIM Version : ' . $proposal_data->version . '

Project Title  : ' . $proposal_data->project_title . '

Name of compound for which process development is carried out  : ' . $proposal_data->process_development_compound_name . '



You can propose a new flowsheet project.

Best Wishes,

!site_name Team,
FOSSEE,IIT Bombay', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user_data->name
				), array(
					'language' => $language->language
				))
			);
			break;
		/**************************************************************/
		case 'abstract_uploaded':
			// $solution_q = db_query("SELECT * FROM {dwsim_flowsheet_solution} WHERE id = %d LIMIT 1", $params['abstract_uploaded']['solution_id']);
			$query = db_select('dwsim_flowsheet_proposal');
			$query->fields('dwsim_flowsheet_proposal');
			$query->condition('id', $params['abstract_uploaded']['proposal_id']);
			$query->range(0, 1);
			$proposal_data = $query->execute()->fetchObject();
			
			$user_data = user_load($params['abstract_uploaded']['user_id']);
			$message['subject'] = t('[!site_name][Flowsheeting Project] You have uploaded DWSIM flowsheet abstract', array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['headers'] = $params['abstract_uploaded']['headers'];
			$message['body'] = array(
				'body' => t('
Dear !user_name,

You have successfully uploaded your DWSIM flowsheet and abstract with the following details.

Full Name : ' . $proposal_data->name_title . ' ' . $proposal_data->contributor_name . '
Project Title  : ' . $proposal_data->project_title . '
Name of compound for which process development is carried out  : ' . $proposal_data->process_development_compound_name . '

You shall be notified via email after the flowsheet and abstract are reviewed and approved / disapproved. This may take upto 30 working days.

Best Wishes,

!site_name Team,
FOSSEE,IIT Bombay', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user_data->name
				), array(
					'language' => $language->language
				))
			);
			break;
		case 'solution_approved':
			//$solution_q = db_query("SELECT * FROM {dwsim_flowsheet_solution} WHERE id = %d LIMIT 1", $params['solution_approved']['solution_id']);
			$query = db_select('dwsim_flowsheet_solution');
			$query->fields('dwsim_flowsheet_solution');
			$query->condition('id', $params['solution_approved']['solution_id']);
			$query->range(0, 1);
			$solution_q = $query->execute();
			$solution_data = $solution_q->fetchObject();
			$query = db_select('dwsim_flowsheet_experiment');
			$query->fields('dwsim_flowsheet_experiment');
			$query->condition('id', $solution_data->experiment_id);
			$query->range(0, 1);
			$experiment_q = $query->execute();
			$experiment_data = $experiment_q->fetchObject();
			$user_data = user_load($params['solution_approved']['user_id']);
			$message['headers'] = $params['solution_approved']['headers'];
			$message['subject'] = t('[!site_name] Your uploaded DWSIM flowsheet solution has been approved', array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				'body' => t('
Dear !user_name,

Your following solution has been approved:

Experiment Title : ' . $experiment_data->title . '

Solution number : ' . $solution_data->code_number . '
Caption : ' . $solution_data->caption . '

Please ensure that ALL the codes follow guidelines at http://dwsim.fossee.in/dwsim-flowsheet-project/dwsim-flowsheet-guidelines

Best Wishes,

!site_name Team,
FOSSEE,IIT Bombay', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user_data->name
				), array(
					'language' => $language->language
				))
			);
			break;
		case 'solution_disapproved':
			$user_data = user_load($params['solution_disapproved']['user_id']);
			$message['headers'] = $params['solution_disapproved']['headers'];
			$message['subject'] = t('[!site_name] Your uploaded DWSIM flowsheet solution has been disapproved', array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				'body' => t('
Dear !user_name,

Your following solution has been disapproved:

Solution number : ' . $params['solution_disapproved']['solution_number'] . '
Caption : ' . $params['solution_disapproved']['solution_caption'] . '

Reason for dis-approval : ' . $params['solution_disapproved']['message'] . '


Best Wishes,

!site_name Team,
FOSSEE,IIT Bombay', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user_data->name
				), array(
					'language' => $language->language
				))
			);
			break;
		case 'solution_deleted_user':
			$user_data = user_load($params['solution_deleted_user']['user_id']);
			$message['headers'] = $params['solution_deleted_user']['headers'];
			$message['subject'] = t('[!site_name] User has deleted pending DWSIM flowsheet solution', array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				'body' => t('
Dear !user_name,

Your following pending solution has been deleted :

Title of the Lab : ' . $params['solution_deleted_user']['lab_title'] . '
Title of the Experiment : ' . $params['solution_deleted_user']['experiment_title'] . '

Solution number : ' . $params['solution_deleted_user']['solution_number'] . '
Caption : ' . $params['solution_deleted_user']['solution_caption'] . '


Best Wishes,

!site_name Team,
FOSSEE,IIT Bombay', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user_data->name
				), array(
					'language' => $language->language
				))
			);
			break;
		case 'dependency_uploaded':
			$user_data = user_load($params['dependency_uploaded']['user_id']);
			$dependency_files = implode(',', $params['dependency_uploaded']['dependency_names']);
			$message['headers'] = $params['dependency_uploaded']['headers'];
			$message['subject'] = t('[!site_name] You have uploaded dependency file', array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				'body' => t('
Dear !user_name,

You have uploaded following dependency files :
    ' . $dependency_files . '

Please ensure that ALL the codes follow guidelines at http://dwsim.fossee.in/dwsim-flowsheet-project/dwsim-flowsheet-guidelines

Best Wishes,

!site_name Team,
FOSSEE,IIT Bombay', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user_data->name
				), array(
					'language' => $language->language
				))
			);
			break;
		case 'standard':
			$message['subject'] = $params['standard']['subject'];
			$message['body'] = $params['standard']['body'];
			$message['headers'] = $params['standard']['headers'];
			break;
	} //$key
}
