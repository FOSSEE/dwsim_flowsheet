<?php
// $Id$
function lab_migration_download_full_experiment()
  {
    $experiment_id = arg(3);
    $root_path = lab_migration_path();
    $APPROVE_PATH = 'APPROVED/';
    $PENDING_PATH = 'PENDING/';
    /* get solution data */
    //$experiment_q = db_query("SELECT * FROM {lab_migration_experiment} WHERE id = %d", $experiment_id);
    $query = db_select('lab_migration_experiment');
    $query->fields('lab_migration_experiment');
    $query->condition('id', $experiment_id);
    $experiment_q = $query->execute();
    $experiment_data = $experiment_q->fetchObject();
    $EXP_PATH = 'EXP' . $experiment_data->number . '/';
    /* zip filename */
    $zip_filename = $root_path . 'zip-' . time() . '-' . rand(0, 999999) . '.zip';
    /* creating zip archive on the server */
    $zip = new ZipArchive;
    $zip->open($zip_filename, ZipArchive::CREATE);
    /* approved solutions */
    //$solution_q = db_query("SELECT * FROM {lab_migration_solution} WHERE experiment_id = %d AND approval_status = 1", $experiment_id);
    $query = db_select('lab_migration_solution');
    $query->fields('lab_migration_solution');
    $query->condition('experiment_id', $experiment_id);
    $query->condition('approval_status', 1);
    $solution_q = $query->execute();
    while ($solution_row = $solution_q->fetchObject())
      {
        $CODE_PATH = 'CODE' . $solution_row->code_number . '/';
        //$solution_files_q = db_query("SELECT * FROM {lab_migration_solution_files} WHERE solution_id = %d", $solution_row->id);
        /*$query = db_select('lab_migration_solution_files');
        $query->fields('lab_migration_solution_files');
        $query->condition('solution_id', $solution_row->id);
        $solution_files_q = $query->execute();*/
        $solution_files_q = db_query("SELECT lmsf.*, lmp.directory_name FROM lab_migration_solution_files lmsf JOIN lab_migration_solution lms JOIN lab_migration_experiment lme JOIN lab_migration_proposal lmp WHERE lms.id = lmsf.solution_id AND lme.id = lms.experiment_id AND lmp.id = lme.proposal_id AND lmsf.id = :solution_id", array(':solution_id' => $solution_row->id));
        //$solution_dependency_files_q = db_query("SELECT * FROM {lab_migration_solution_dependency} WHERE solution_id = %d", $solution_row->id);
        $query = db_select('lab_migration_solution_dependency');
        $query->fields('lab_migration_solution_dependency');
        $query->condition('solution_id', $solution_row->id);
        $solution_dependency_files_q = $query->execute();
        while ($solution_files_row = $solution_files_q->fetchObject())
          {
            $zip->addFile($root_path . $solution_files_row->directory_name. '/' . $solution_files_row->filepath, $APPROVE_PATH . $EXP_PATH . $CODE_PATH . $solution_files_row->filename);
          }
        /* dependency files */
        while ($solution_dependency_files_row = $solution_dependency_files_q->fetchObject())
          {
            // $dependency_file_data = (db_query("SELECT * FROM {lab_migration_dependency_files} WHERE id = %d LIMIT 1", $solution_dependency_files_row->dependency_id))->fetchObject();
            $query = db_select('lab_migration_dependency_files');
            $query->fields('lab_migration_dependency_files');
            $query->condition('id', $solution_dependency_files_row->dependency_id);
            $query->range(0, 1);
            $dependency_file_data = $query->execute()->fetchObject();
            if ($dependency_file_data)
                $zip->addFile($root_path . $dependency_file_data->filepath, $APPROVE_PATH . $EXP_PATH . $CODE_PATH . 'DEPENDENCIES/' . $dependency_file_data->filename);
          }
      }
    /* unapproved solutions */
    // $solution_q = db_query("SELECT * FROM {lab_migration_solution} WHERE experiment_id = %d AND approval_status = 0", $experiment_id);
    $query = db_select('lab_migration_solution');
    $query->fields('lab_migration_solution');
    $query->condition('experiment_id', $experiment_id);
    $query->condition('approval_status', 0);
    $solution_q = $query->execute();
    while ($solution_row = $solution_q->fetchObject())
      {
        $CODE_PATH = 'CODE' . $solution_row->code_number . '/';
        //$solution_files_q = db_query("SELECT * FROM {lab_migration_solution_files} WHERE solution_id = %d", $solution_row->id);
        /*$query = db_select('lab_migration_solution_files');
        $query->fields('lab_migration_solution_files');
        $query->condition('solution_id', $solution_row->id);
        $solution_files_q = $query->execute();*/
        $solution_files_q = db_query("SELECT lmsf.*, lmp.directory_name FROM lab_migration_solution_files lmsf JOIN lab_migration_solution lms JOIN lab_migration_experiment lme JOIN lab_migration_proposal lmp WHERE lms.id = lmsf.solution_id AND lme.id = lms.experiment_id AND lmp.id = lme.proposal_id AND lmsf.id = :solution_id", array(':solution_id' => $solution_row->id));
        
        //$solution_dependency_files_q = db_query("SELECT * FROM {lab_migration_solution_dependency} WHERE solution_id = %d", $solution_row->id);
        $query = db_select('lab_migration_solution_dependency');
        $query->fields('lab_migration_solution_dependency');
        $query->condition('solution_id', $solution_row->id);
        $solution_dependency_files_q = $query->execute();
        while ($solution_files_row = $solution_files_q->fetchObject())
          {
            $zip->addFile($root_path . $solution_files_row->directory_name . '/' . $solution_files_row->filepath, $PENDING_PATH . $EXP_PATH . $CODE_PATH . $solution_files_row->filename);
          }
        /* dependency files */
        while ($solution_dependency_files_row = $solution_dependency_files_q->fetchObject())
          {
            // $dependency_file_data = (db_query("SELECT * FROM {lab_migration_dependency_files} WHERE id = %d LIMIT 1", $solution_dependency_files_row->dependency_id))->fetchObject();
            $query = db_select('lab_migration_dependency_files');
            $query->fields('lab_migration_dependency_files');
            $query->condition('id', $solution_dependency_files_row->dependency_id);
            $query->range(0, 1);
            $dependency_file_data = $query->execute()->fetchObject();
            if ($dependency_file_data)
                $zip->addFile($root_path . $dependency_file_data->filepath, $PENDING_PATH . $EXP_PATH . $CODE_PATH . 'DEPENDENCIES/' . $dependency_file_data->filename);
          }
      }
    $zip_file_count = $zip->numFiles;
    $zip->close();
    if ($zip_file_count > 0)
      {
        /* download zip file */
        header('Content-Type: application/zip');
        header('Content-disposition: attachment; filename="EXP' . $experiment_data->number . '.zip"');
        header('Content-Length: ' . filesize($zip_filename));
        readfile($zip_filename);
        unlink($zip_filename);
      }
    else
      {
        drupal_set_message("There are no solutions in this experiment to download", 'error');
        drupal_goto('lab-migration/code-approval/bulk');
      }
  }
function lab_migration_download_full_lab()
  {
    $lab_id = arg(3);
    var_dump($lab_id);
    //die;
    $root_path = lab_migration_path();
    $APPROVE_PATH = 'APPROVED/';
    $PENDING_PATH = 'PENDING/';
    /* get solution data */
    //$lab_q = db_query("SELECT * FROM {lab_migration_proposal} WHERE id = %d", $lab_id);
    $query = db_select('lab_migration_proposal');
    $query->fields('lab_migration_proposal');
    $query->condition('id', $lab_id);
    $lab_q = $query->execute();
    $lab_data = $lab_q->fetchObject();
    $LAB_PATH = $lab_data->lab_title . '/';
    /* zip filename */
    $zip_filename = $root_path . 'zip-' . time() . '-' . rand(0, 999999) . '.zip';
    /* creating zip archive on the server */
    $zip = new ZipArchive;
    $zip->open($zip_filename, ZipArchive::CREATE);
    /* approved solutions */
    //$experiment_q = db_query("SELECT * FROM {lab_migration_experiment} WHERE proposal_id = %d", $lab_id);
    $query = db_select('lab_migration_experiment');
    $query->fields('lab_migration_experiment');
    $query->condition('proposal_id', $lab_id);
    $experiment_q = $query->execute();
    while ($experiment_row = $experiment_q->fetchObject())
      {
        $EXP_PATH = 'EXP' . $experiment_row->number . '/';
        //$solution_q = db_query("SELECT * FROM {lab_migration_solution} WHERE experiment_id = %d AND approval_status = 1", $experiment_row->id);
        $query = db_select('lab_migration_solution');
        $query->fields('lab_migration_solution');
        $query->condition('experiment_id', $experiment_row->id);
        $query->condition('approval_status', 1);
        $solution_q = $query->execute();
        while ($solution_row = $solution_q->fetchObject())
          {
            $CODE_PATH = 'CODE' . $solution_row->code_number . '/';
            //$solution_files_q = db_query("SELECT * FROM {lab_migration_solution_files} WHERE solution_id = %d", $solution_row->id);
            /*$query = db_select('lab_migration_solution_files');
            $query->fields('lab_migration_solution_files');
            $query->condition('solution_id', $solution_row->id);
            $solution_files_q = $query->execute();*/
            $solution_files_q = db_query("SELECT lmsf.*, lmp.directory_name FROM lab_migration_solution_files lmsf JOIN lab_migration_solution lms JOIN lab_migration_experiment lme JOIN lab_migration_proposal lmp WHERE lms.id = lmsf.solution_id AND lme.id = lms.experiment_id AND lmp.id = lme.proposal_id AND lmsf.id = :solution_id", array(':solution_id' => $solution_row->id));
            //$solution_dependency_files_q = db_query("SELECT * FROM {lab_migration_solution_dependency} WHERE solution_id = %d", $solution_row->id);
            $query = db_select('lab_migration_solution_dependency');
            $query->fields('lab_migration_solution_dependency');
            $query->condition('solution_id', $solution_row->id);
            $solution_dependency_files_q = $query->execute();
            while ($solution_files_row = $solution_files_q->fetchObject())
              {
                $zip->addFile($root_path . $solution_files_row->directory_name. '/' . $solution_files_row->filepath, $APPROVE_PATH . $EXP_PATH . $CODE_PATH . $solution_files_row->filename);
              }
            /* dependency files */
            while ($solution_dependency_files_row = $solution_dependency_files_q->fetchObject())
              {
                //$dependency_file_data = (db_query("SELECT * FROM {lab_migration_dependency_files} WHERE id = %d LIMIT 1", $solution_dependency_files_row->dependency_id))->fetchObject();
                $query = db_select('lab_migration_dependency_files');
                $query->fields('lab_migration_dependency_files');
                $query->condition('id', $solution_dependency_files_row->dependency_id);
                $query->range(0, 1);
                $dependency_file_data = $query->execute()->fetchObject();
                if ($dependency_file_data)
                    $zip->addFile($root_path . $dependency_file_data->filepath, $APPROVE_PATH . $EXP_PATH . $CODE_PATH . 'DEPENDENCIES/' . $dependency_file_data->filename);
              }
          }
        /* unapproved solutions */
        //$solution_q = db_query("SELECT * FROM {lab_migration_solution} WHERE experiment_id = %d AND approval_status = 0", $experiment_row->id);
        $query = db_select('lab_migration_solution');
        $query->fields('lab_migration_solution');
        $query->condition('experiment_id', $experiment_row->id);
        $query->condition('approval_status', 0);
        $solution_q = $query->execute();
        while ($solution_row = $solution_q->fetchObject())
          {
            $CODE_PATH = 'CODE' . $solution_row->code_number . '/';
            //$solution_files_q = db_query("SELECT * FROM {lab_migration_solution_files} WHERE solution_id = %d", $solution_row->id);
           /* $query = db_select('lab_migration_solution_files');
            $query->fields('lab_migration_solution_files');
            $query->condition('solution_id', $solution_row->id);
            $solution_files_q = $query->execute();*/
            $solution_files_q = db_query("SELECT lmsf.*, lmp.directory_name FROM lab_migration_solution_files lmsf JOIN lab_migration_solution lms JOIN lab_migration_experiment lme JOIN lab_migration_proposal lmp WHERE lms.id = lmsf.solution_id AND lme.id = lms.experiment_id AND lmp.id = lme.proposal_id AND lmsf.id = :solution_id", array(':solution_id' => $solution_row->id));
            
            //$solution_dependency_files_q = db_query("SELECT * FROM {lab_migration_solution_dependency} WHERE solution_id = %d", $solution_row->id);
            $query = db_select('lab_migration_solution_dependency');
            $query->fields('lab_migration_solution_dependency');
            $query->condition('solution_id', $solution_row->id);
            $solution_dependency_files_q = $query->execute();
            while ($solution_files_row = $solution_files_q->fetchObject())
              {
                $zip->addFile($root_path .$solution_files_row->directory_name . '/' . $solution_files_row->filepath, $LAB_PATH . $PENDING_PATH . $EXP_PATH . $CODE_PATH . $solution_files_row->filename);
              }
            /* dependency files */
            while ($solution_dependency_files_row = $solution_dependency_files_q->fetchObject())
              {
                //$dependency_file_data = (db_query("SELECT * FROM {lab_migration_dependency_files} WHERE id = %d LIMIT 1", $solution_dependency_files_row->dependency_id))->fetchObject();
                $query = db_select('lab_migration_dependency_files');
                $query->fields('lab_migration_dependency_files');
                $query->condition('id', $solution_dependency_files_row->dependency_id);
                $query->range(0, 1);
                $dependency_file_data = $query->execute()->fetchObject();
                if ($dependency_file_data)
                    $zip->addFile($root_path . $dependency_file_data->filepath, $LAB_PATH . $PENDING_PATH . $EXP_PATH . $CODE_PATH . 'DEPENDENCIES/' . $dependency_file_data->filename);
              }
          }
      }
    $zip_file_count = $zip->numFiles;
    $zip->close();
    if ($zip_file_count > 0)
      {
        /* download zip file */
        ob_clean();
        //flush();
        header('Content-Type: application/zip');
        header('Content-disposition: attachment; filename="' . $lab_data->lab_title . '.zip"');
        header('Content-Length: ' . filesize($zip_filename));
        readfile($zip_filename);
        unlink($zip_filename);
      }
    else
      {
        drupal_set_message("There are no solutions in this lab to download", 'error');
        drupal_goto('lab-migration/code-approval/bulk');
      }
  }
