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
        case 'solution_proposal_approved':
            /* initializing data */
            //$proposal_q = db_query("SELECT * FROM {dwsim_flowsheet_proposal} WHERE id = %d LIMIT 1", $params['solution_proposal_approved']['proposal_id']);
            $query = db_select('dwsim_flowsheet_proposal');
            $query->fields('dwsim_flowsheet_proposal');
            $query->condition('id', $params['solution_proposal_approved']['proposal_id']);
            $query->range(0, 1);
            $proposal_q = $query->execute();
            $proposal_data = $proposal_q->fetchObject();
            if ($proposal_data->solution_display == 1)
              {
                $solution_display = 'Yes';
              }
            else
              {
                $solution_display = 'No';
              }
            // $experiment_q = db_query("SELECT * FROM {dwsim_flowsheet_experiment} WHERE proposal_id = %d ORDER BY number", $params['solution_proposal_approved']['proposal_id'], 1);
            $query = db_select('dwsim_flowsheet_experiment');
            $query->fields('dwsim_flowsheet_experiment');
            $query->condition('proposal_id', $params['solution_proposal_approved']['proposal_id']);
            $query->orderBy('number', 'ASC');
            $experiment_q = $query->execute();
            $experiment_list = '
      	';
            while ($experiment_data = $experiment_q->fetchObject())
              {
                $experiment_list .= '<p>' . $experiment_data->number . ') ' . $experiment_data->title . '<br> Description : ' . $experiment_data->description . '<br>';
                $experiment_list .= ' ';
                $experiment_list .= '</p>';
              }
            $user_data = user_load($params['solution_proposal_approved']['user_id']);
            $message['headers'] = $params['solution_proposal_approved']['headers'];
            $message['subject'] = t('[!site_name] DWSIM flowsheet Solution Proposal Approval', array(
                '!site_name' => variable_get('site_name', '')
            ), array(
                'language' => $language->language
            ));
            $message['body'] = array(
                0 => t('
Dear !user_name,

Your following DWSIM flowsheet solution proposal has been approved:

Full Name : ' . $proposal_data->solution_provider_name_title . ' ' . $proposal_data->solution_provider_name . '
Email : ' . $user_data->mail . '
Contact No. : ' . $proposal_data->solution_provider_contact_ph . '
Department/Branch : ' . $proposal_data->solution_provider_department . '
University/Institute : ' . $proposal_data->solution_provider_university . '
City : ' . $proposal_data->solution_provider_city . '
State : ' . $proposal_data->solution_provider_state . '

List of experiments : ' . $experiment_list . '

Please ensure that ALL the codes follow guidelines at http://dwsim.fossee.in/dwsim-flowsheet/dwsim-flowsheet-guidelines

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
        case 'solution_proposal_disapproved':
            $user_data = user_load($params['solution_proposal_disapproved']['user_id']);
            $proposal_id = $params['solution_proposal_disapproved']['proposal_id'];
            $query = db_select('dwsim_flowsheet_proposal');
            $query->fields('dwsim_flowsheet_proposal');
            $query->condition('id', $proposal_id);
            $query->range(0, 1);
            $proposal_q = $query->execute();
            $proposal_data = $proposal_q->fetchObject();
            
            $query = db_select('dwsim_flowsheet_experiment');
            $query->fields('dwsim_flowsheet_experiment');
            $query->condition('proposal_id', $proposal_id);
            $query->orderBy('number', 'ASC');
            $experiment_q = $query->execute();
            $experiment_list = '
      	';
            while ($experiment_data = $experiment_q->fetchObject())
              {
                $experiment_list .= '<p>' . $experiment_data->number . ') ' . $experiment_data->title . '<br> Description : ' . $experiment_data->description . '<br>';
                $experiment_list .= ' ';
                $experiment_list .= '</p>';
              }
            $message['headers'] = $params['solution_proposal_disapproved']['headers'];
            $message['subject'] = t('[!site_name] DWSIM flowsheet Solution Disapproval', array(
                '!site_name' => variable_get('site_name', '')
            ), array(
                'language' => $language->language
            ));
            $message['body'] = array(
                'body' => t('
Dear !user_name,

We regret to inform you that your DWSIM flowsheet solution proposal has been disapproved.

Title of Lab :' . $proposal_data->lab_title . '

List of experiments : ' . $experiment_list . '

Reason: ' . $params['solution_proposal_disapproved']['message'] . '

You are welcome to submit a new proposal.

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
        case 'proposal_received':
            /* initializing data */
            // $proposal_q = db_query("SELECT * FROM {dwsim_flowsheet_proposal} WHERE id = %d LIMIT 1", $params['proposal_received']['proposal_id']);
            // $proposal_data = $proposal_q->fetchObject();
            $query = db_select('dwsim_flowsheet_proposal');
            $query->fields('dwsim_flowsheet_proposal');
            $query->condition('id', $params['proposal_received']['proposal_id']);
            $query->range(0, 1);
            $proposal_data = $query->execute()->fetchObject();
            /* $samplecodefilename = "";
            if (strlen($proposal_data->samplefilepath) >= 5)
            {
            $samplecodefilename = substr($proposal_data->samplefilepath, strrpos($proposal_data->samplefilepath, '/') + 1);
            }
            else
            {
            $samplecodefilename = "Not provided";
            }*/
            if ($proposal_data->solution_display == 1)
              {
                $solution_display = 'Yes';
              }
            else
              {
                $solution_display = 'No';
              }
            if ($proposal_data->solution_provider_uid == 0)
              {
                $solution_provider_user = 'Open';
              }
            else if ($proposal_data->solution_provider_uid == $proposal_data->uid)
              {
                $solution_provider_user = 'Proposer';
              }
            else
              {
                $solution_provider_user = 'Unknown';
              }
            // $experiment_q = db_query("SELECT * FROM {dwsim_flowsheet_experiment} WHERE proposal_id = %d ORDER BY number",
            //  $params['proposal_received']['proposal_id'], 1);
            $query = db_select('dwsim_flowsheet_experiment');
            $query->fields('dwsim_flowsheet_experiment');
            $query->condition('proposal_id', $params['proposal_received']['proposal_id']);
            $query->orderBy('number', 'ASC');
            $experiment_q = $query->execute();
            $experiment_list = '
      	';
            while ($experiment_data = $experiment_q->fetchObject())
              {
                $experiment_list .= '<p>' . $experiment_data->number . ') ' . $experiment_data->title . '<br> Description : ' . $experiment_data->description . '<br>';
                $experiment_list .= ' ';
                $experiment_list .= '</p>';
              }
            $user_data = user_load($params['proposal_received']['user_id']);
            $message['headers'] = $params['proposal_received']['headers'];
            $message['subject'] = t('[!site_name] Your DWSIM flowsheet proposal has been received', array(
                '!site_name' => variable_get('site_name', '')
            ), array(
                'language' => $language->language
            ));
            $message['body'] = array(
                'body' => t('
Dear !user_name,

We have received your DWSIM flowsheet proposal with the following details:

Full Name : ' . $proposal_data->name_title . ' ' . $proposal_data->name . '
Email : ' . $user_data->mail . '
Contact No. : ' . $proposal_data->contact_ph . '
Department/Branch : ' . $proposal_data->department . '
University/Institute : ' . $proposal_data->university . '
City : ' . $proposal_data->city . '
State : ' . $proposal_data->state . '


Solution Provided By : ' . $solution_provider_user . '

List of experiments : ' . $experiment_list . '

Your proposal is under review. You will soon receive an email when same has been approved/disapproved.

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
        case 'proposal_disapproved':
            /* initializing data */
            // $proposal_q = db_query("SELECT * FROM {dwsim_flowsheet_proposal} WHERE id = %d LIMIT 1", $params['proposal_disapproved']['proposal_id']);
            //$proposal_data = $proposal_q->fetchObject();
            $query = db_select('dwsim_flowsheet_proposal');
            $query->fields('dwsim_flowsheet_proposal');
            $query->condition('id', $params['proposal_disapproved']['proposal_id']);
            $query->range(0, 1);
            $proposal_data = $query->execute()->fetchObject();
            $samplecodefilename = "";
            if (strlen($proposal_data->samplefilepath) >= 5)
              {
                $samplecodefilename = substr($proposal_data->samplefilepath, strrpos($proposal_data->samplefilepath, '/') + 1);
              }
            else
              {
                $samplecodefilename = "Not provided";
              }
            if ($proposal_data->solution_display == 1)
              {
                $solution_display = 'Yes';
              }
            else
              {
                $solution_display = 'No';
              }
            if ($proposal_data->solution_provider_uid == 0)
              {
                $solution_provider_user = 'Open';
              }
            else if ($proposal_data->solution_provider_uid == $proposal_data->uid)
              {
                $solution_provider_user = 'Proposer';
              }
            else
              {
                $solution_provider_user = 'Unknown';
              }
            // $experiment_q = db_query("SELECT * FROM {dwsim_flowsheet_experiment} WHERE proposal_id = %d ORDER BY number", $params['proposal_disapproved']['proposal_id'], 1);
            $query = db_select('dwsim_flowsheet_experiment');
            $query->fields('dwsim_flowsheet_experiment');
            $query->condition('proposal_id', $params['proposal_disapproved']['proposal_id']);
            $query->orderBy('number', 'ASC');
            $experiment_q = $query->execute();
            $experiment_list = '
      	';
            while ($experiment_data = $experiment_q->fetchObject())
              {
                $experiment_list .= '<p>' . $experiment_data->number . ') ' . $experiment_data->title . '<br> Description : ' . $experiment_data->description . '<br>';
                $experiment_list .= ' ';
                $experiment_list .= '</p>';
              }
            $user_data = user_load($params['proposal_disapproved']['user_id']);
            $message['headers'] = $params['proposal_disapproved']['headers'];
            $message['subject'] = t('[!site_name] Your DWSIM flowsheet proposal has been disapproved', array(
                '!site_name' => variable_get('site_name', '')
            ), array(
                'language' => $language->language
            ));
            $message['body'] = array(
                'body' => t('
Dear !user_name,

We regret to inform you that all the Experiments of your Lab with following details have been dis-approved.

Reason for disapproval: ' . $proposal_data->message . '

Full Name : ' . $proposal_data->name_title . ' ' . $proposal_data->name . '
Email : ' . $user_data->mail . '
Contact No. : ' . $proposal_data->contact_ph . '
Department/Branch : ' . $proposal_data->department . '
University/Institute : ' . $proposal_data->university . '
City : ' . $proposal_data->city . '
State : ' . $proposal_data->state . '

Solution Provided By : ' . $solution_provider_user . '

List of experiments : ' . $experiment_list . '

Uploaded Sample Code : ' . $samplecodefilename . '


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
        case 'proposal_approved':
            /* initializing data */
            // $proposal_q = db_query("SELECT * FROM {dwsim_flowsheet_proposal} WHERE id = %d LIMIT 1", $params['proposal_approved']['proposal_id']);
            $query = db_select('dwsim_flowsheet_proposal');
            $query->fields('dwsim_flowsheet_proposal');
            $query->condition('id', $params['proposal_approved']['proposal_id']);
            $query->range(0, 1);
            $proposal_q = $query->execute();
            $proposal_data = $proposal_q->fetchObject();
            /* $samplecodefilename = "";
            if (strlen($proposal_data->samplefilepath) >= 5)
            {
            $samplecodefilename = substr($proposal_data->samplefilepath, strrpos($proposal_data->samplefilepath, '/') + 1);
            }
            else
            {
            $samplecodefilename = "Not provided";
            }*/
            if ($proposal_data->solution_display == 1)
              {
                $solution_display = 'Yes';
              }
            else
              {
                $solution_display = 'No';
              }
            if ($proposal_data->solution_provider_uid == 0)
              {
                $solution_provider_user = 'Open';
              }
            else if ($proposal_data->solution_provider_uid == $proposal_data->uid)
              {
                $solution_provider_user = 'Proposer';
              }
            else
              {
                $solution_provider_user = 'Unknown';
              }
            //$experiment_q = db_query("SELECT * FROM {dwsim_flowsheet_experiment} WHERE proposal_id = %d ORDER BY number", $params['proposal_approved']['proposal_id'], 1);
            $query = db_select('dwsim_flowsheet_experiment');
            $query->fields('dwsim_flowsheet_experiment');
            $query->condition('proposal_id', $params['proposal_approved']['proposal_id']);
            $query->orderBy('number', 'ASC');
            $experiment_q = $query->execute();
            $experiment_list = '
      	';
            while ($experiment_data = $experiment_q->fetchObject())
              {
                $experiment_list .= '<p>' . $experiment_data->number . ') ' . $experiment_data->title . '<br> Description : ' . $experiment_data->description . '<br>';
                $experiment_list .= ' ';
                $experiment_list .= '</p>';
              }
            $user_data = user_load($params['proposal_approved']['user_id']);
            $message['headers'] = $params['proposal_approved']['headers'];
            $message['subject'] = t('[!site_name] Your DWSIM flowsheet proposal has been approved', array(
                '!site_name' => variable_get('site_name', '')
            ), array(
                'language' => $language->language
            ));
            $message['body'] = array(
                0 => t('
Dear !user_name,

Congratulations! Your DWSIM flowsheet proposal with the below details has been approved:

Full Name : ' . $proposal_data->name_title . ' ' . $proposal_data->name . '
Email : ' . $user_data->mail . '
Contact No. : ' . $proposal_data->contact_ph . '
Department/Branch : ' . $proposal_data->department . '
University/Institute : ' . $proposal_data->university . '
City : ' . $proposal_data->city . '
State : ' . $proposal_data->state . '


Solution Provided By : ' . $solution_provider_user . '

List of experiments : ' . $experiment_list . '

Please ensure that ALL the guidelines for coding are strictly followed:
http://dwsim.fossee.in/dwsim-flowsheet-project/dwsim-flowsheet-guidelines

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
        case 'proposal_completed':
            /* initializing data */
            //$proposal_q = db_query("SELECT * FROM {dwsim_flowsheet_proposal} WHERE id = %d LIMIT 1", $params['proposal_completed']['proposal_id']);
            $query = db_select('dwsim_flowsheet_proposal');
            $query->fields('dwsim_flowsheet_proposal');
            $query->condition('id', $params['proposal_completed']['proposal_id']);
            $query->range(0, 1);
            $proposal_q = $query->execute();
            $proposal_data = $proposal_q->fetchObject();
            if ($proposal_data->solution_display == 1)
              {
                $solution_display = 'Yes';
              }
            else
              {
                $solution_display = 'No';
              }
            if ($proposal_data->solution_provider_uid == 0)
              {
                $solution_provider_user = 'Open';
              }
            else if ($proposal_data->solution_provider_uid == $proposal_data->uid)
              {
                $solution_provider_user = 'Proposer';
              }
            else
              {
                $user_data = user_load($proposal_data->solution_provider_uid);
                if (!$user_data)
                  {
                    $solution_provider_user = 'Unknown';
                  }
              }
            //$experiment_q = db_query("SELECT * FROM {dwsim_flowsheet_experiment} WHERE proposal_id = %d ORDER BY number", $params['proposal_completed']['proposal_id'], 1);
            $query = db_select('dwsim_flowsheet_experiment');
            $query->fields('dwsim_flowsheet_experiment');
            $query->condition('proposal_id', $params['proposal_completed']['proposal_id']);
            $query->orderBy('number', 'ASC');
            $experiment_q = $query->execute();
            $experiment_list = '
      	';
            while ($experiment_data = $experiment_q->fetchObject())
              {
                $experiment_list .= '<p>' . $experiment_data->number . ') ' . $experiment_data->title . '<br> Description : ' . $experiment_data->description . '<br>';
                $experiment_list .= ' ';
                $experiment_list .= '</p>';
              }
            $user_data = user_load($params['proposal_completed']['user_id']);
            $message['headers'] = $params['proposal_completed']['headers'];
            $message['subject'] = t('[!site_name] Congratulations for completion of the DWSIM flowsheet', array(
                '!site_name' => variable_get('site_name', '')
            ), array(
                'language' => $language->language
            ));
            $message['body'] = array(
                'body' => t('
Dear !user_name,

Following DWSIM flowsheet has been completed sucessfully :

Full Name : ' . $proposal_data->name_title . ' ' . $proposal_data->name . '
Email : ' . $user_data->mail . '
Contact No. : ' . $proposal_data->contact_ph . '
Department/Branch : ' . $proposal_data->department . '
University/Institute : ' . $proposal_data->university . '
City : ' . $proposal_data->city . '
State : ' . $proposal_data->state . '


Solution Provided By : ' . $solution_provider_user . '

List of experiments : ' . $experiment_list . '

Your Lab solution is now available at following link to download.

http://dwsim.fossee.in/dwsim-flowsheet/dwsim-flowsheet-run/' . $proposal_data->id . '

Now you should be able to propose a new DWSIM flowsheet.

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
        case 'solution_proposal_received':
            /* initializing data */
            //$proposal_q = db_query("SELECT * FROM {dwsim_flowsheet_proposal} WHERE id = %d LIMIT 1", $params['solution_proposal_received']['proposal_id']);
            $query = db_select('dwsim_flowsheet_proposal');
            $query->fields('dwsim_flowsheet_proposal');
            $query->condition('id', $params['solution_proposal_received']['proposal_id']);
            $query->range(0, 1);
            $proposal_q = $query->execute();
            $proposal_data = $proposal_q->fetchObject();
            if ($proposal_data->solution_display == 1)
              {
                $solution_display = 'Yes';
              }
            else
              {
                $solution_display = 'No';
              }
            //$experiment_q = db_query("SELECT * FROM {dwsim_flowsheet_experiment} WHERE proposal_id = %d ORDER BY number", $params['solution_proposal_received']['proposal_id'], 1);
            $query = db_select('dwsim_flowsheet_experiment');
            $query->fields('dwsim_flowsheet_experiment');
            $query->condition('proposal_id', $params['solution_proposal_received']['proposal_id']);
            $query->orderBy('number', 'ASC');
            $experiment_q = $query->execute();
            $experiment_list = '
      	';
            while ($experiment_data = $experiment_q->fetchObject())
              {
                $experiment_list .= '<p>' . $experiment_data->number . ') ' . $experiment_data->title . '<br> Description : ' . $experiment_data->description . '<br>';
                $experiment_list .= ' ';
                $experiment_list .= '</p>';
              }
            $user_data = user_load($params['solution_proposal_received']['user_id']);
            $message['headers'] = $params['solution_proposal_received']['headers'];
            $message['subject'] = t('[!site_name] Your DWSIM flowsheet solution proposal has been received', array(
                '!site_name' => variable_get('site_name', '')
            ), array(
                'language' => $language->language
            ));
            $message['body'] = array(
                'body' => t('
Dear !user_name,

We have received your proposal for providing solution for DWSIM flowsheet with the following details:

List of experiments : ' . $experiment_list . '

Full Name : ' . $proposal_data->solution_provider_name_title . ' ' . $proposal_data->solution_provider_name . '
Email : ' . $user->mail . '
Contact No. : ' . $proposal_data->solution_provider_contact_ph . '
Department/Branch : ' . $proposal_data->department . '
University/Institute : ' . $proposal_data->solution_provider_university . '


Your proposal is under review. You will soon receive an email when the same has been approved / disapproved.

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
        case 'solution_uploaded':
            // $solution_q = db_query("SELECT * FROM {dwsim_flowsheet_solution} WHERE id = %d LIMIT 1", $params['solution_uploaded']['solution_id']);
            $query = db_select('dwsim_flowsheet_solution');
            $query->fields('dwsim_flowsheet_solution');
            $query->condition('id', $params['solution_uploaded']['solution_id']);
            $query->range(0, 1);
            $solution_q = $query->execute();
            $solution_data = $solution_q->fetchObject();
            $query = db_select('dwsim_flowsheet_experiment');
            $query->fields('dwsim_flowsheet_experiment');
            $query->condition('id', $solution_data->experiment_id);
            $query->range(0, 1);
            $experiment_q = $query->execute();
            $experiment_data = $experiment_q->fetchObject();
            $user_data = user_load($params['solution_uploaded']['user_id']);
            $message['subject'] = t('[!site_name] You have uploaded DWSIM flowsheet solution', array(
                '!site_name' => variable_get('site_name', '')
            ), array(
                'language' => $language->language
            ));
            $message['headers'] = $params['solution_uploaded']['headers'];
            $message['body'] = array(
                'body' => t('
Dear !user_name,

You have uploaded the following solution:

Experiment Title : ' . $experiment_data->title . '

Solution number : ' . $solution_data->code_number . '
Caption : ' . $solution_data->caption . '

The solution is under review. You will be notified when it has been approved.

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
    }
  }
