<?php

class acecrewmodule {  
    // object instance
    private static $instance;
    
    private $currentJobId = null;
    private $currentClientId = null;
    private $nextJobCallId = null;
    
    private function __construct() { }
    
    private function __clone() { }
    
    public static function getInstance() {
        if (self::$instance === null) {
            self::$instance = new acecrewmodule();
        }       
        return self::$instance;
    }
        
    
    public function getCurrentJobId()
    {
        return $this->currentJobId;
    }

    public function getCurrentClientId()
    {
        return $this->currentClientId;
    }
     //saf added
    public function getNextJobCallId()
    {
        return $this->nextJobCallId;
    }
        
    public function setCurrentJobId($id)
    {
        $this->currentJobId = $id;
    }
    
    public function setCurrentClientId($id)
    {
        $this->currentClientId = $id;
    }
     //saf added
    public function setNextJobCallId($callId)
    {
        $this->nextJobCallId = $callId;
    }
    
     
}
//saf added
function _acecrew_get_jobclient_contacts(){
    $curClientId = acecrewmodule::getInstance()->getCurrentClientId();
    
    $client = node_load($curClientId);
    
    $contacts = array();
    foreach($client->field_client_contact as $contact){
        $contacts[$contact['item_id']] = $contact['value']['field_client_contact_name']['0']['value'];
    }
    return $contacts;
}
     
function _acecrew_get_assigned_job_sessions_iterator($nid){
    $query = "SELECT js.nid FROM {content_type_job_session} js
        JOIN {node} n ON js.field_session_job_nid_value = n.nid
        WHERE n.nid = '%d'";
    $query_result =  db_query($query, $nid);
    
    return $query_result;
}

function _acecrew_get_assigned_job_sessions($nid) {
    $query_result =  _acecrew_get_assigned_job_sessions_iterator($nid);
    
    $sessions = array();
    while ($row = db_fetch_object($query_result)) {
        $node = node_load(array('nid' => $row->nid));
        $node_rendered = node_view($node);
        $sessions[$row->nid]['content'] = $node_rendered;
        
        // required for Drupal 6
        module_load_include('inc', 'node', 'node.pages');
        
        // which nodeform you want
        $node_type = 'job_session';
        $form_id = $node_type . '_node_form';
        $form_state = array('type' => $node_type);
        
        $edit_form = drupal_get_form($form_id, $node);        
        
        $sessions[$row->nid]['form'] = $edit_form;
    }
    
    return $sessions;
}

function _acecrew_get_assigned_job_docs($nid){
    $docs = array();
    
    $query = "SELECT ctj.field_job_number_value AS job_id FROM {content_type_job} ctj
         WHERE ctj.nid = '%d' LIMIT 1";
    $query_result =  db_query($query, $nid);
    $row = db_fetch_object($query_result);
    $docs['job_id'] = $row->job_id;
    
    $docs['invoices'] = array();
    $query = "SELECT ji.job_id, ji.invoice_id, n.created  FROM {content_type_job} ctj
        JOIN {acecrew_job_invoices} ji ON ctj.field_job_number_value = ji.job_id
        JOIN {invoice_invoices} ii ON ji.invoice_id = ii.iid  
        JOIN {node} n ON ii.nid = n.nid WHERE ctj.nid = '%d'";
        
    $query_result =  db_query($query, $nid);
    
    while ($row = db_fetch_object($query_result)){
        $docs['invoices'][$row->invoice_id] = date('Y-m-d H:i:s', $row->created);
    }
    
    $docs['quotes'] = array();
    $query = "SELECT * FROM {acecrew_job_quotes} jq WHERE jq.job_id IN (SELECT ctj.field_job_number_value FROM {content_type_job} ctj WHERE ctj.nid = '%d') ORDER BY jq.created DESC";
    $query_result =  db_query($query, $nid);
    
    while ($row = db_fetch_object($query_result)){
        $docs['quotes'][$row->acecrew_job_quote_id] = date('Y-m-d H:i:s', $row->created);
    }
    
    return $docs;
}

//clear invoice module buffer
function _acecrew_clear_invoice_items($uid){
    $query = "DELETE FROM {invoice_items} 
        WHERE uid = '%d'
        AND invoice_id = '0'";
    $query_result =  db_query($query, $uid);
}

//insert item in invoice module buffer
function _acecrew_add_invoice_item($description, $quantity, $unit_cost, $uid, $weight = 0){
    $vat = variable_get('invoice_vat', 0);
    $query = "INSERT INTO {invoice_items} 
        (description, quantity, unitcost, vat, weight, invoice_id, uid, created) VALUES
        ('%s','%d','%f','%f','%d','%d','%d', NOW())";
    $query_result =  db_query($query, $description, $quantity, $unit_cost, $vat, $weight, 0, $uid);
    
}

//set relate between job and invoice
function _acecrew_add_invoice_job_relate($invoice_number, $job_id){
    $query = "INSERT INTO {acecrew_job_invoices} 
        (job_id, invoice_id) VALUES
        ('%d','%d')";
    $query_result =  db_query($query, $job_id, $invoice_number);
}

//set relate between job and quote
function _acecrew_add_quote_job_relate($job_id){
    $date = mktime();
    $query = "INSERT INTO {acecrew_job_quotes} (job_id, created) VALUES ('%d', '%d')";
    $query_result = db_query($query, $job_id, $date);
}

function _acecrew_get_supplement($supp_id){
    return node_load(array('nid' => $supp_id));
}

function _acecrew_get_job_node_by_id($job_id){
    $job = false;
    $query = "SELECT ctj.nid FROM {content_type_job} ctj WHERE ctj.field_job_number_value = '%d'";
    $query_result =  db_query($query, $job_id);
    $row = db_fetch_object($query_result);
    if (!empty($row)){
        $job = node_load(array('nid' => $row->nid));
    }
    return $job;
}

function acecrew_get_client_by_id($client_id)
{
    $client = false;
    $query = "SELECT ctc.nid FROM {content_type_client} ctc WHERE ctc.field_client_id_value = '%d'";
    $query_result =  db_query($query, $client_id);
    $row = db_fetch_object($query_result);
    if (!empty($row)){
        $client = node_load(array('nid' => $row->nid));
    }
    return $client;
}

function acecrew_get_client_by_name($client_id)
{
    $client = false;
    $query = "SELECT ctc.nid FROM {content_type_client} ctc WHERE ctc.field_client_name_value = '%s'";
    $query_result =  db_query($query, $client_id);
    $row = db_fetch_object($query_result);
    if (!empty($row)){
        $client = node_load(array('nid' => $row->nid));
    }
    return $client;
}

function _acecrew_get_client_emails($client_id)
{
    $client = acecrew_get_client_by_id($client_id);
    $clientEmails = array();
    $clientEmails[] = array('name' => $client->field_client_name[0]['value'], 'email' => $client->field_client_company_email[0]['email']);
    foreach((array)$client->field_client_contact as $contact){
        $clientEmails[] = array('name' => $contact['value']['field_client_contact_name'][0]['value'], 'email' => $contact['value']['field_client_contact_email'][0]['email']);
    }
    return $clientEmails;
}

function acecrew_get_venue_by_id($venue_id)
{
    $venue = false;
    $query = "SELECT ctv.nid FROM {content_type_venue} ctv WHERE ctv.field_venue_id_value = '%d'";
    $query_result =  db_query($query, $venue_id);
    $row = db_fetch_object($query_result);
    if (!empty($row)){
        $venue = node_load(array('nid' => $row->nid));
    }
    return $venue;
}

function acecrew_get_client_by_invoice_id($invoice_id)
{
    $client = false;
    $query = "SELECT ctc.nid FROM drupal_acecrew_job_invoices ji
        JOIN drupal_content_type_job ctj ON ji.job_id = ctj.field_job_number_value
        JOIN drupal_content_type_client ctc ON ctj.field_job_client_name_value = ctc.field_client_id_value
        WHERE ji.invoice_id = '%d'";
    $query_result =  db_query($query, $invoice_id);
    $row = db_fetch_object($query_result);
    if (!empty($row)){
        $job = node_load(array('nid' => $row->nid));
    }
    return $job;
}

function acecrew_get_client_by_quote_id($invoice_id)
{
    $client = false;
    $query = "SELECT ctc.nid FROM drupal_acecrew_job_quotes jq
        JOIN drupal_content_type_job ctj ON jq.job_id = ctj.field_job_number_value
        JOIN drupal_content_type_client ctc ON ctj.field_job_client_name_value = ctc.field_client_id_value
        WHERE jq.quote_id = '%d'";
    $query_result =  db_query($query, $invoice_id);
    $row = db_fetch_object($query_result);
    if (!empty($row)){
        $job = node_load(array('nid' => $row->nid));
    }
    return $job;
}

//??saf modify
/**
* get ADmin-Setting values by field name
* 
* @param mixed $field_name
*/
function acecrew_get_ads($field_name){
 $query = "SELECT n.nid 
        FROM {node} n 
        WHERE n.type = '%s' 
        ORDER BY nid ASC LIMIT 1";
    $nid =  db_result(db_query($query, 'admin_settings'));
    
    $node = node_load($nid);
    $valueArr = explode("\r\n", $node->{$field_name}[0]['value']);
    
    $options = array();
    //generate options
    foreach($valueArr as $v){
      $options["$v"] = "$v/ Hour";
      $c  = 0;
    }
    return $options;
}
//get pay rate form element
//??saf modify
function _acecrew_get_rates_form($payrateid = null) {
    
    //select available variants
    /*$query = "SELECT n.nid, 
            cpr.field_payrate_name_value as payrate_name, 
            cpr.field_payrate_perhour_value as perhour 
        FROM {node} n JOIN {content_type_crew_payrates} cpr ON n.nid = cpr.nid
        WHERE n.type = '%s' 
        ORDER BY perhour ASC";
    $query_result =  db_query($query, 'crew_payrates');
    
    //generate options
    $options = array();
    while ($row = db_fetch_object($query_result)) {
        $options[$row->nid] = $row->perhour  ;
    } */
    $options = acecrew_get_ads('field_admins_crewrates');
    //create select form element
    $form['Crew Skills']['acecrew_payrate_id'] = array(
        '#type' => 'select',
        '#title' => t('Pay Rate'),
        '#options' => $options,
        '#required' => TRUE,
        '#weight' => 99,
    );
    
    //set default(selected) value
    if ($payrateid){
        $form['Crew Skills']['acecrew_payrate_id']['#default_value'] = $payrateid;
    }
    
    return $form;
}

function acecrew_get_payrate($nid, $with_name = false){
    
    //select rate
    //?? saf
    return $nid;
    
    $query = "SELECT n.nid, 
            cpr.field_payrate_name_value as payrate_name, 
            cpr.field_payrate_perhour_value as perhour 
        FROM {node} n JOIN {content_type_crew_payrates} cpr ON n.nid = cpr.nid
        WHERE n.nid = '%d'";
    
    $query_result = db_query($query, $nid);
    $row = db_fetch_object($query_result);
    
    if(empty($row)){
        return '';
    }
    
    if($with_name){
        $string = $row->payrate_name . ' (' . $row->perhour . 'GBP)';
    } else {
        $string = $row->perhour;
    }
    
    return $string;
}

function acecrew_get_user_rate($uid){
    $user = user_load($uid);
    $rate = acecrew_get_payrate($user->profile_payrateid);
    return $rate;
}

function _acecrew_save_session_crews_relate($crews, $session_id){
    $query = "DELETE FROM {acecrew_session_crews} WHERE session_nid = '%d'";
    $query_result =  db_query($query, $session_id);
    foreach($crews as $crew){
        $query = "INSERT INTO {acecrew_session_crews} 
            (session_nid, uid) VALUES
            ('%d', '%d')";       
        $query_result =  db_query($query, $session_id, $crew);
    }
}

function acecrew_get_user_report($startDate, $endDate, $user, $is_html = false){
    $startDate = explode('-', $startDate);
    $endDate = explode('-', $endDate);
    $startDate = date("Y-m-d", mktime(0, 0, 0, $startDate[1], $startDate[2], $startDate[0]));
    $endDate = date("Y-m-d", mktime(0, 0, 0, $endDate[1], $endDate[2], $endDate[0]));    
    $fines = acecrew_get_user_report_fines($startDate, $endDate, $user);
    $clothing = acecrew_get_user_report_clothing($startDate, $endDate, $user);
    $hours = acecrew_get_user_report_hours($startDate, $endDate, $user);
    
    $report = array(
        'hours' => $hours,
        'fines' => $fines,
        'clothing' => $clothing,
    );
    
    if ($is_html){
        $report = theme('acecrew_report_table', $report);
    }
    
    return $report;
}

function acecrew_get_user_report_fines($startDate, $endDate, $user){
    $fines = array();
    $query = "SELECT f.nid AS fine_id, fr.nid AS fine_rate_id FROM {content_type_fines} f
        JOIN {content_type_fines_rates} fr ON f.field_fine_amount_value = fr.nid
        WHERE DATE(f.field_fine_date_value) BETWEEN '%s' AND '%s'
            AND f.field_fine_staff_id_value = %d
        ORDER BY f.field_fine_date_value ASC";
    
    $query_result = db_query($query, $startDate, $endDate, $user);
    while($row = db_fetch_object($query_result)){
        $fine = node_load(array('nid' => $row->fine_id));
        $fine_rate = node_load(array('nid' => $row->fine_rate_id));
        
        $fines[$fine->nid] = array(
            'value' => floatval('-' . $fine_rate->field_fine_rate_amount[0]['value']),
            'name' =>  $fine_rate->field_fine_rate_name[0]['value'] . ' (' . substr($fine->field_fine_date[0]['value'], 0, 10) . ')',
        );
    }
    return $fines;
}

function acecrew_get_user_report_clothing($startDate, $endDate, $user){
    $uniforms = array();
    
    $query = "SELECT up.uniform_nid AS unid FROM {acecrew_uniform_pays} up
        WHERE DATE(up.pay_month) BETWEEN '%s' AND '%s' 
            AND up.uid = %d";
    
    $query_result = db_query($query, $startDate, $endDate, $user);
    while($row = db_fetch_object($query_result)){
        $uniform = node_load(array('nid' => $row->unid));
        
        $value = $uniform->field_unif_amount[0]['value'] / $uniform->field_unif_payment[0]['value'];
        $value = round($value, 2);
        $value = floatval('-' . $value);
        
        $uniforms[$uniform->nid] = array(
            'value' => $uniforms[$uniform->nid]['value'] + $value,
            'name' => $uniform->field_unif_item[0]['value']
        );
    }
    
    return $uniforms;
}

function acecrew_get_user_report_hours($startDate, $endDate, $user){
    
    $query = "SELECT js.nid FROM drupal_content_type_job_session js
       JOIN drupal_acecrew_session_crews sc ON js.nid = sc.session_nid
       WHERE DATE(js.field_job_session_date_time_value) BETWEEN '%s' AND '%s' 
        AND sc.uid = %d";
    
    
    $hours_enrolled = 0;
    $hours_worked = 0;
    $query_result = db_query($query, $startDate, $endDate, $user);
    while($row = db_fetch_object($query_result)){
        $session = node_load(array('nid' => $row->nid));
        foreach($session->field_job_session_crews as $session_crew){
            $crew_hours_worked = $session_crew['value']['field_crew_job_session_hours_wor'][0]['value'];
            $crew_name = $session_crew['value']['field_crew_job_session_name'][0]['value'];
            if(!empty($crew_hours_worked) && $crew_name == $user){
                
                $hours_worked += $crew_hours_worked;
                switch ($crew_hours_worked){
                    case 1:
                        $hours_enrolled += 3;
                        break;
                    case 2:
                        $hours_enrolled += 3;
                        break;
                    case 3:
                        $hours_enrolled += 4;
                        break;
                    default:
                        $hours_enrolled += $hours_worked;
                }
            }
        }
    }
    
    $user_rate = acecrew_get_user_rate($user);
    $hours_worked_cost = $user_rate * $hours_worked;
    $hours_worked_cost = round($hours_worked_cost, 2);
    
    $hours_enrolled_cost = $user_rate * $hours_enrolled;
    $hours_enrolled_cost = round($hours_enrolled_cost, 2);
    
    $crew_hours = array();
    $crew_hours[0] = array(
        'value' => $hours_enrolled_cost,
        'name' => $hours_enrolled . t('h. worked by ') . $user_rate . 'GBP',
    );
    
    return $crew_hours;
}

function _acecrew_get_client_nid_by_job_nid($job_nid){
    $query = "SELECT c.nid FROM drupal_content_type_job j
        JOIN drupal_content_type_client c ON j.field_job_client_name_value = c.field_client_id_value
        WHERE j.nid = '%d'";
    $query_result =  db_query($query, $job_nid);
    $row = db_fetch_object($query_result);
    return $row->nid;
}

function _quote_dompdf_include_lib() {
    require_once(dirname(__FILE__) .'/dompdf/dompdf_config.inc.php');  
}

function _quote_get_html($quote_id, $type='print') {
    $job = NULL;
    $query = "SELECT ctj.*, ajq.* FROM {content_type_job} ctj, {acecrew_job_quotes} ajq WHERE ajq.job_id=ctj.field_job_number_value AND ajq.acecrew_job_quote_id='%s'";
    $query_result = db_query($query, $quote_id);
    while($row = db_fetch_object($query_result)){
        $job = $row;
    }

    $client = _get_client_by_job($job);
    $client_contact = _get_client_contact_by_id($job);
    $venue = _get_venue_by_job($job);
    $callings = _get_callings_by_job($job);
    
    $html = '<!doctype>';
    $html .= '<head>';
    $html .= '<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-15" />';
    $html .= '<html><head>';
    $html .= '</head><body>';

    $html .= theme_acecrew_quote_body($job, $client, $client_contact, $venue, $callings);

    // The eurosign is not supported as applicable character, so replace it with a ascii code

    $html .= '</body></html>';
    return $html;
}

function _get_client_by_job($job) {
    $client = NULL;
    $query = "SELECT * FROM {content_type_job} ctj, {content_type_client} ctc WHERE ctj.field_job_client_name_value = ctc.field_client_id_value and ctj.vid='%d'";
    $query_result = db_query($query, $job->vid);
    while($row = db_fetch_object($query_result)) {
        $client = $row;
    }
    
    return $client;
}

function _get_client_contact_by_id($job) {
    $client_contact = NULL;
    
    $query = "SELECT * FROM {content_type_job} ctj, {content_type_client_contact} ctcc WHERE ctj.field_job_client_contact_value = ctcc.vid and ctj.vid='%d'";
    $query_result = db_query($query, $job->vid);
    while($row = db_fetch_object($query_result)) {
        $client_contact = $row;
    }

    return $client_contact;    
}

function _get_venue_by_job($job) {
    $venue = NULL;
    
    $query = "SELECT * FROM {content_type_job} ctj, {content_type_venue} ctv WHERE ctj.field_job_venue_value = ctv.field_venue_id_value and ctj.vid='%d'";
    $query_result = db_query($query, $job->vid);
    while($row = db_fetch_object($query_result)) {
        $venue = $row;
    }

    return $venue;
}

function _get_callings_by_job($job) {
    $callings = array();
    
    $query = "SELECT * FROM {content_type_job} ctj, {content_type_job_session} ctjs WHERE ctjs.field_session_job_nid_value = ctj.nid and ctj.vid='%d'";
    $query_result = db_query($query, $job->vid);
    while($row = db_fetch_object($query_result)) {
        $callings[] = $row;
    }

    return $callings;
}
    
function theme_acecrew_quote_body($job, $client, $client_contact, $venue, $callings) {
    $node = array(
            'job' => $job, 
            'client' => $client, 
            'client_contact' => $client_contact, 
            'venue' => $venue, 
            'callings' => $callings);
    
    $content = '<div class="quote-wrapper">';

    require_once dirname(__FILE__) .'/templates/acecrewquo.inc';
    drupal_add_css(drupal_get_path('module', 'acecrew') .'/templates/acecrewquo.css', 'module');

    $content_function = '_acecrew_quote_get_template_output';
    $content .= $content_function($node, 'pdf');  

    $content .= '</div>';

    return $content;
}