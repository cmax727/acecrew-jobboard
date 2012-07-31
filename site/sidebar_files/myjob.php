<?php
// $Id: index.php,v 1.94 2007/12/26 08:46:48 dries Exp $

/**
 * @file
 * The PHP page that serves all page requests on a Drupal installation.
 *
 * The routines here dispatch control to the appropriate handler, which then
 * prints the appropriate page.
 *
 * All Drupal code is released under the GNU General Public License.
 * See COPYRIGHT.txt and LICENSE.txt.
 */

 ini_set('display_errors', true);
 error_reporting(E_ALL);

require_once './includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
$nid=$_REQUEST['id'];
$job = _acecrew_get_job_node_by_id($nid);
echo $job->nid;


   /* $query = "SELECT nid, field_session_job_nid_value AS jsnid,  TIME(field_job_session_date_time_value) AS jstime
        FROM {content_type_job_session} js, {content_type_job} ctj 
        WHERE ctj.nid=js.nid and ctj.field_job_number_value = '%d'
        ORDER BY jstime ASC";
		*/
		$query = "SELECT ctj.nid as jsnid FROM {content_type_job} ctj WHERE ctj.field_job_number_value = '%d'";
    $query_result =  db_query($query, $nid);
    $scheduler = array();
    $job_statuses = array('1' => 'Pencil', '2' => 'Confirmed', '3' => 'Cancel');
    while ($row = db_fetch_object($query_result)) {
        
        $job = node_load(array('nid' => $row->jsnid));
        $client = acecrew_get_client_by_id($job->field_job_client_name[0]['value']);
        $client_name = $client->field_client_name[0]['value'];
        $venue = acecrew_get_venue_by_id($job->field_job_venue[0]['value']);
        $venue_name = $venue->field_venue_name[0]['value'];
		$crew_suppl = $session_node->field_call_supplements[0]['value'];
        
        
        $session_node = node_load(array('nid' => $row->nid));
        $ses_count_crews = $session_node->field_job_session_crew[0]['value'];
        $ses_count_hours = $session_node->field_job_session_hours[0]['value'];
		
		
        $node_id = '<span>The session node is:' . $session_node->nid . '</span>';
		
        $crews = '';
        foreach($session_node->field_job_session_crews as $ses_crew){
            $crew_name = $ses_crew['value']['field_crew_job_session_name'][0]['value'];
            $crew_status = $ses_crew['value']['field_crew_job_session_status'][0]['value'];
            $user = user_load(array('uid' => $crew_name));
            $crew_name = $user->name;
            $crews .= '<li class="session_crew_statused_name_' . $crew_status . '">' . $crew_name . '</li>';
        }
		
		$supp_name = '';
		foreach($session_node->field_call_supplements as $item) {
		$supp_node = node_load($item['nid']);
		$supp_title = $supp_node->title;
 		$supp_name .= '<span>' . $supp_title . '</span>';
		}
		
		 
		
        $supps =  $supp_name;
		
        $crews = '<ul class="crew_statuses_ul">' . $crews . '</ul>';
        
        $job_status_code = $job->field_job_status[0]['value'];
        $job_status = $job_statuses[$job_status_code];
        
        $scheduler[] = array(
            'time' => $row->jstime,
            'venue' => $venue_name,
            'client' => $client_name,
            'job_id' => $job->field_job_number[0]['value'],
            'job_ref' => $job->path,
            'job_status_code' => $job_status_code,
            'job_status' => $job_status,
            'ses_count_crews' => $ses_count_crews,
            'ses_count_hours' => $ses_count_hours,
            'crews' => $crews,
			'supplement' => $supps,
			'nodeid' => $node_id
        );
        
    }
	/*
	echo "<pre>";
print_r( $scheduler);
echo "</pre>"; */
?>
<div><?php echo $nid;?> / Start / Crew / Hours</div>
<div>
Venue: <?php echo $scheduler[0]['venue'];?><br />
Client:  <?php echo $scheduler[0]['client'];?><br />
Call Status:  <?php echo $scheduler[0]['job_status'];?><br />
</div>
<div class="call-third-row">
    <span>Cherry Picker</span>
    <span>Crew Chief</span>
    <span>Derigger</span>
</div>
