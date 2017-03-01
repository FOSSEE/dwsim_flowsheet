<?php
/******************************************************************************/
/****************************** DELETION FUNCTIONS ****************************/
/******************************************************************************/
function dwsim_flowsheet_delete_solution($solution_id)
  {
    global $user;
    $root_path = dwsim_flowsheet_path();
    $status = TRUE;
    //$solution_q = db_query("SELECT * FROM {dwsim_flowsheet_solution} WHERE id = %d", $solution_id);
    $query = db_select('dwsim_flowsheet_solution');
    $query->fields('dwsim_flowsheet_solution');
    $query->condition('id', $solution_id);
    $solution_q = $query->execute();
    $solution_data = $solution_q->fetchObject();
    if (!$solution_data)
      {
        drupal_set_message(t('Invalid solution.'), 'error');
        return FALSE;
      }
    //$experiment_q = db_query("SELECT * FROM {dwsim_flowsheet_experiment} WHERE id = %d", $solution_data->experiment_id);
    $experiment_q = db_query("SELECT lme.*, lmp.id, lmp.directory_name FROM dwsim_flowsheet_experiment lme JOIN dwsim_flowsheet_proposal lmp WHERE lmp.id = lme.proposal_id AND lme.id = :experiment_id",array(':experiment_id' => $solution_data->experiment_id));
    $experiment_data = $experiment_q->fetchObject();
    if (!$experiment_data)
      {
        drupal_set_message(t('Invalid experiment.'), 'error');
        return FALSE;
      }
    /* deleting solution files */
    //$solution_files_q = db_query("SELECT * FROM {dwsim_flowsheet_solution_files} WHERE solution_id = %d", $solution_id);
    $solution_files_q = db_query("SELECT lmsf.*, lmp.directory_name FROM dwsim_flowsheet_solution_files lmsf JOIN dwsim_flowsheet_solution lms JOIN dwsim_flowsheet_experiment lme JOIN dwsim_flowsheet_proposal lmp WHERE lms.id = lmsf.solution_id AND lme.id = lms.experiment_id AND lmp.id = lme.proposal_id AND lmsf.id = :solution_id", array(':solution_id' => $solution_id));
    /*$query = db_select('dwsim_flowsheet_solution_files');
    $query->fields('dwsim_flowsheet_solution_files');
    $query->condition('solution_id', $solution_id);
    $solution_files_q = $query->execute();*/
    while ($solution_files_data = $solution_files_q->fetchObject())
      {
        if (!file_exists($root_path .$solution_files_data->directory_name. '/'. $solution_files_data->filepath))
          {
            $status = FALSE;
            drupal_set_message(t('Error deleting !file. File does not exists.', array(
                '!file' => $solution_files_data->directory_name. '/' . $solution_files_data->filepath
            )), 'error');
            continue;
          }
        /* removing solution file */
        if (!unlink($root_path . $solution_files_data->directory_name. '/'. $solution_files_data->filepath))
          {
            $status = FALSE;
            drupal_set_message(t('Error deleting !file', array(
                '!file' => $solution_files_data->directory_name. '/' . $solution_files_data->filepath
            )), 'error');
            /* sending email to admins */
            $email_to = variable_get('dwsim_flowsheet_emails', '');
            $from = variable_get('dwsim_flowsheet_from_email', '');
            $bcc = "";
            $cc = variable_get('dwsim_flowsheet_cc_emails', '');
            $param['standard']['subject'] = "[ERROR] Error deleting example file";
            $param['standard']['body'] = "Error deleting solution files by " . $user->uid . " at " . $_SERVER['HTTP_HOST'] . $_SERVER['REQUEST_URI'] . " :
        solution id : " . $solution_id . "
        file id : " . $solution_files_data->id . "
        file path : " . $solution_files_data->directory_name. '/'. $solution_files_data->filepath . "
	PDF path : " . $PdfStatus;
            $param['standard']['headers'] = array(
                'From' => $from,
                'MIME-Version' => '1.0',
                'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
                'Content-Transfer-Encoding' => '8Bit',
                'X-Mailer' => 'Drupal',
                'Cc' => $cc,
                'Bcc' => $bcc
            );
            if (!drupal_mail('dwsim_flowsheet', 'standard', $email_to, language_default(), $param, $from, TRUE))
                drupal_set_message('Error sending email message.', 'error');
          }
        else
          {
            /* deleting example files database entries */
            db_delete('dwsim_flowsheet_solution_files')->condition('id', $solution_files_data->id)->execute();
          }
      }
    if (!$status)
        return FALSE;
    /* removing code folder */
    $ex_path = $experiment_data->directory_name . '/EXP' . $experiment_data->number . '/CODE' . $solution_data->code_number;
    $dir_path = $root_path . $ex_path;
    if (is_dir($dir_path))
      {
        if (!rmdir($dir_path))
          {
            drupal_set_message(t('Error deleting folder !folder', array(
                '!folder' => $dir_path
            )), 'error');
            /* sending email to admins */
            $email_to = variable_get('dwsim_flowsheet_emails', '');
            $from = variable_get('dwsim_flowsheet_from_email', '');
            $bcc = "";
            $cc = variable_get('dwsim_flowsheet_cc_emails', '');
            $param['standard']['subject'] = "[ERROR] Error deleting folder";
            $param['standard']['body'] = "Error deleting folder " . $dir_path . " by " . $user->uid . " at " . $_SERVER['HTTP_HOST'] . $_SERVER['REQUEST_URI'];
            $param['standard']['headers'] = array(
                'From' => $from,
                'MIME-Version' => '1.0',
                'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
                'Content-Transfer-Encoding' => '8Bit',
                'X-Mailer' => 'Drupal',
                'Cc' => $cc,
                'Bcc' => $bcc
            );
            if (!drupal_mail('dwsim_flowsheet', 'standard', $email_to, language_default(), $param, $from, TRUE))
                drupal_set_message('Error sending email message.', 'error');
            return FALSE;
          }
      }
    else
      {
        drupal_set_message(t('Cannot delete solution folder. !folder does not exists.', array(
            '!folder' => $dir_path
        )), 'error');
        return FALSE;
      }
    /* deleting solution dependency and solution database entries */
    db_delete('dwsim_flowsheet_solution_dependency')->condition('solution_id', $solution_id)->execute();
    db_delete('dwsim_flowsheet_solution')->condition('id', $solution_id)->execute();
    return $status;
  }
function dwsim_flowsheet_delete_experiment($experiment_id)
  {
    $status = TRUE;
    $root_path = dwsim_flowsheet_path();
    //$experiment_q = db_query("SELECT * FROM {dwsim_flowsheet_experiment} WHERE id = %d", $experiment_id);
    $query = db_select('dwsim_flowsheet_experiment');
    $query->fields('dwsim_flowsheet_experiment');
    $query->condition('id', $experiment_id);
    $experiment_q = $query->execute();
    $experiment_data = $experiment_q->fetchObject();
    if (!$experiment_data)
      {
        drupal_set_message('Invalid experiment.', 'error');
        return FALSE;
      }
    /* deleting solutions */
    //$solution_q = db_query("SELECT * FROM {dwsim_flowsheet_solution} WHERE experiment_id = %d", $experiment_id);  
    $query = db_select('dwsim_flowsheet_solution');
    $query->fields('dwsim_flowsheet_solution');
    $query->condition('experiment_id', $experiment_id);
    $solution_q = $query->execute();
    $delete_exp_folder = FALSE;
    while ($solution_data = $solution_q->fetchObject())
      {
        $delete_exp_folder = TRUE;
        if (!dwsim_flowsheet_delete_solution($solution_data->id))
            $status = FALSE;
      }
    if (!$delete_exp_folder)
      {
        return TRUE;
      }
    if ($status)
      {
        $dir_path = $root_path . $experiment_data->directory_name . '/EXP' . $experiment_data->number;
        if (is_dir($dir_path))
          {
            $res = rmdir($dir_path);
            if (!$res)
              {
                drupal_set_message(t('Error deleting experiment folder !folder', array(
                    '!folder' => $dir_path
                )), 'error');
                /* sending email to admins */
                $email_to = variable_get('dwsim_flowsheet_emails', '');
                $from = variable_get('dwsim_flowsheet_from_email', '');
                $bcc = "";
                $cc = variable_get('dwsim_flowsheet_cc_emails', '');
                $param['standard']['subject'] = "[ERROR] Error deleting experiment folder";
                $param['standard']['body'] = "Error deleting folder " . $dir_path;
                $param['standard']['headers'] = array(
                    'From' => $from,
                    'MIME-Version' => '1.0',
                    'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
                    'Content-Transfer-Encoding' => '8Bit',
                    'X-Mailer' => 'Drupal',
                    'Cc' => $cc,
                    'Bcc' => $bcc
                );
                if (!drupal_mail('dwsim_flowsheet', 'standard', $email_to, language_default(), $param, $from, TRUE))
                    drupal_set_message('Error sending email message.', 'error');
                return FALSE;
              }
            else
              {
                return TRUE;
              }
          }
        else
          {
            drupal_set_message(t('Cannot delete experiment folder. !folder does not exists.', array(
                '!folder' => $dir_path
            )), 'error');
            return FALSE;
          }
      }
    return FALSE;
  }
function dwsim_flowsheet_delete_lab($lab_id)
  {
    $status = TRUE;
    $root_path = dwsim_flowsheet_path();
    //$proposal_q = db_query("SELECT * FROM {dwsim_flowsheet_proposal} WHERE id = %d", $lab_id);
    $query = db_select('dwsim_flowsheet_proposal');
    $query->fields('dwsim_flowsheet_proposal');
    $query->condition('id', $lab_id);
    $proposal_q = $query->execute();
    $proposal_data = $proposal_q->fetchObject();
    if (!$proposal_data)
      {
        drupal_set_message('Invalid Lab.', 'error');
        return FALSE;
      }
    /* delete experiments */
    //$experiment_q = db_query("SELECT * FROM {dwsim_flowsheet_experiment} WHERE proposal_id = %d", $proposal_data->id);
    $query = db_select('dwsim_flowsheet_experiment');
    $query->fields('dwsim_flowsheet_experiment');
    $query->condition('proposal_id', $proposal_data->id);
    $experiment_q = $query->execute();
    while ($experiment_data = $experiment_q->fetchObject())
      {
        if (!dwsim_flowsheet_delete_experiment($experiment_data->id))
          {
            $status = FALSE;
          }
      }
    return $status;
  }
function dwsim_flowsheet_del_lab_pdf($lab_id)
  {
    $root_path = dwsim_flowsheet_path();
    $dir_path = $root_path . "latex/";
    $pdf_filename = "lab_" . $lab_id . ".pdf";
    if (file_exists($dir_path . $pdf_filename))
        unlink($dir_path . $pdf_filename);
  }
function dwsim_flowsheet_delete_dependency($dependency_id)
  {
    global $user;
    $root_path = dwsim_flowsheet_path();
    $status = TRUE;
    //$dependency_files_q = db_query("SELECT * FROM {dwsim_flowsheet_dependency_files} WHERE id = %d", $dependency_id);
    $query = db_select('dwsim_flowsheet_dependency_files');
    $query->fields('dwsim_flowsheet_dependency_files');
    $query->condition('id', $dependency_id);
    $dependency_files_q = $query->execute();
    $dependency_files_data = $dependency_files_q->fetchObject();
    if (!$dependency_files_data)
      {
        drupal_set_message(t('Invalid dependency.'), 'error');
        return FALSE;
      }
    if (!file_exists($root_path . $dependency_files_data->filepath))
      {
        drupal_set_message(t('Error deleting !file. File does not exists.', array(
            '!file' => $dependency_files_data->filepath
        )), 'error');
        return FALSE;
      }
    /* removing dependency file */
    if (!unlink($root_path . $dependency_files_data->filepath))
      {
        $status = FALSE;
        drupal_set_message(t('Error deleting !file', array(
            '!file' => $dependency_files_data->filepath
        )), 'error');
        /* sending email to admins */
        $email_to = variable_get('dwsim_flowsheet_emails', '');
        $from = variable_get('dwsim_flowsheet_from_email', '');
        $bcc = "";
        $cc = variable_get('dwsim_flowsheet_cc_emails', '');
        $param['standard']['subject'] = "[ERROR] Error deleting dependency file";
        $param['standard']['body'] = "Error deleting dependency files by " . $user->uid . " at " . $_SERVER['HTTP_HOST'] . $_SERVER['REQUEST_URI'] . " :
			dependency id : " . $dependency_id . "
			file id : " . $dependency_files_data->id . "
			file path : " . $dependency_files_data->filepath;
        $param['standard']['headers'] = array(
            'From' => $from,
            'MIME-Version' => '1.0',
            'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
            'Content-Transfer-Encoding' => '8Bit',
            'X-Mailer' => 'Drupal',
            'Cc' => $cc,
            'Bcc' => $bcc
        );
        if (!drupal_mail('dwsim_flowsheet', 'standard', $email_to, language_default(), $param, $from, TRUE))
            drupal_set_message('Error sending email message.', 'error');
      }
    else
      {
        /* deleting dependency files database entries */
        db_delete('dwsim_flowsheet_dependency_files')->condition('id', $dependency_id)->execute();
      }
    return $status;
  }
