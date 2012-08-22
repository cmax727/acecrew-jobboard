<?php
// $Id: acecrewquo.inc,v 1.1.2.2 2009/01/02 21:24:23 pvogelaar Exp $

/**
 * @file
 * Invoice module
 * 
 * This module was developed by Platina Designs, http://www.platinadesigns.nl
 *
 * @author Pieter Vogelaar <ps.vogelaar@platinadesigns.nl>
 */

/**
 * Function to create the invoice layout
 * 
 * Documentation available at:
 * - http://www.digitaljunkies.ca/dompdf/faq.php
 * - http://www.digitaljunkies.ca/dompdf/usage.php
 */
function _acecrew_quote_get_template_output($quote, $type=NULL) {
  ob_start();
  
  $job = $quote['job'];
  $client = $quote['client'];
  $client_contact = $quote['client_contact'];
  $venue = $quote['venue'];
  $callings = $quote['callings'];

/*
  // Create alias
  $template =& $invoice->invoice['template'];
  
  // Set full url path to template
  $path_to_template = 'http://'. $_SERVER['HTTP_HOST'] . base_path() . path_to_theme('module', 'invoice') .'/templates';
  
  if ($type != 'pdf') {
    $s_pagenum = '1/1';
  }
  else {
    $s_pagenum = '';
  }
  
  //fix encoding problem when convert GBP character to HTML entites 
  foreach ($invoice->invoice as $key => $value){
    if(!is_array($value)){
      $invoice->invoice[$key] = str_replace('&Acirc;', '', htmlentities($value));
    }
    
  } 
*/
   
  $footer_content_s1 = 'Ace Crew Office (9 - 5): +44 (0)207 924 6569 24/7 Booking: +44(0) 7947 88 66 99';
  $footer_content_s2 = '126 Bolingbroke grove, Clapham Junction, SW11 1DA.';
  $footer_content_s3 = 'admin@acecrew.co.uk     www.acecrew.co.uk';
  $footer_content_s4 = 'Company No: 6128298';
  
  if ($type == 'pdf') {
    $footer = '<script type="text/php">
                if (isset($pdf)) {
                  $footer = $pdf->open_object();

                  $w = $pdf->get_width();
                  $h = $pdf->get_height();
                  
                  $font = Font_Metrics::get_font("arial");
                  
                  $r=128; $g=128; $b=128;
                  
                  $a_line_color = array(0 => $r / 0xff, 1 => $g / 0xff, 2 => $b / 0xff,
                    "r"=>$r / 0xff, "g"=>$g / 0xff, "b"=>$b / 0xff,
                    "hex" => sprintf("#%02X%02X%02X", $r, $g, $b));
                  
                  $a_text_color = array(0 => $r / 0xff, 1 => $g / 0xff, 2 => $b / 0xff,
                    "r"=>$r / 0xff, "g"=>$g / 0xff, "b"=>$b / 0xff,
                    "hex" => sprintf("#%02X%02X%02X", $r, $g, $b));
                    
                  $pdf->line(40, $h-60, $w-40, $h-60, $a_line_color, 1, array()); 
                    
                  $footer_content_font_size = 8;
                  $footer_content_font_space = 1;
                  
                  $footer_content_s1 = \'' . $footer_content_s1 . '\';
                  $footer_content_s2 = \'' . $footer_content_s2 . '\';
                  $footer_content_s3 = \'' . $footer_content_s3 . '\';
                  $footer_content_s4 = \'' . $footer_content_s4 . '\';
                  
                  
                  $footer_content_s1_len  = $pdf->get_text_width($footer_content_s1, $font, $footer_content_font_size);
                  $footer_content_s2_len  = $pdf->get_text_width($footer_content_s2, $font, $footer_content_font_size);
                  $footer_content_s3_len  = $pdf->get_text_width($footer_content_s3, $font, $footer_content_font_size);
                  $footer_content_s4_len  = $pdf->get_text_width($footer_content_s4, $font, $footer_content_font_size);
                  
                  $pdf->text(($w - $footer_content_s1_len) / 2, $h - 60 + $footer_content_font_space, $footer_content_s1, $font, $footer_content_font_size, $a_text_color);
                  $pdf->text(($w - $footer_content_s2_len) / 2, $h - 60 + $footer_content_font_space*2 + $footer_content_font_size, $footer_content_s2, $font, $footer_content_font_size, $a_text_color);
                  $pdf->text(($w - $footer_content_s3_len) / 2, $h - 60 + $footer_content_font_space*3 + $footer_content_font_size*2, $footer_content_s3, $font, $footer_content_font_size, $a_text_color);
                  $pdf->text(($w - $footer_content_s4_len) / 2, $h - 60 + $footer_content_font_space*4 + $footer_content_font_size*3, $footer_content_s4, $font, $footer_content_font_size, $a_text_color);
                  
                  $pdf->page_text($w-57, $h-39, "{PAGE_NUM}/{PAGE_COUNT}", $font, 10, $a_text_color);
                  
                  $pdf->close_object();
                
                  $pdf->add_object($footer, "all");
                }
                </script>';
    echo $footer;
    $footer = '';
  }
  else {
    $footer = $footer_content;    
  }
  
  $image = url(drupal_get_path('module', 'acecrew') . '/theme/acecrewquo.png', array('absolute' => true));
   
  echo '
  <table class="acecrew_top_tbl" width=100%>
  <tr>
    <td><h1>' . t('Quote') . '</h1></td>
    <td class=acecrew_logo><img src=http://optova.co.uk.local/modules/acecrew/theme/acecrewquo.png alt=Acecrew title=Acecrew /></td>
  </tr>
  <tr>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
  </tr>  
  </table>
  <hr/><br/><br/>
  <table width=100% border=1 cellspacing=0 cellpadding=0>
    <tr>
        <td width=30%>' . t('Client') . ':</td> 
        <td width=70%>' . $client->field_client_name_value . '</td>
        <td width=30%> ' . t('Reference') . ':</td>
        <td width=70%> ' . $job->acecrew_job_quote_id . ' </td>
    </tr>
    <tr> 
        <td>' . t('Contact') . ':</td>
        <td>' . $client_contact->field_client_contact_name_value . '</td>
        <td>'. t('Date') .':</td>
        <td>'. date("d F Y, H:i:s", $job->created) .'</td>
    </tr>
  </table><br/><br/>
  <table width=80%>
    <tr>
        <td width=15%>' . t('Venue') . ': </td><td width=80% style="border-bottom:1px solid black;">' . $venue->field_venue_name_value . '</td>
    </tr>
    <tr>
        <td width=15%>' . t('Adress') . ': </td><td width=80% style="border-bottom:1px solid black;">' . $venue->field_venue_address_value . '</td>
    </tr>
    <tr>
        <td width=15%>' . t('Postcode') . ': </td><td width=80% style="border-bottom:1px solid black;">' . $venue->field_venue_post_code_value . '</td>
    </tr>
  </table><br/><br/>';
  
  $vat = NULL;
  $multiple_vat_values = FALSE;  
  foreach ($invoice->invoice_items as $item) {
      
    if (is_null($vat)) {
      $vat = $item['vat'];  
    }
    
    if ($item['vat'] !== $vat) {
      $multiple_vat_values = TRUE;
    }
  }
  
  echo '<table width=100% border=1 cellspacing=0 cellpadding=0>';
  echo '<tr><td colspan=9></td><td>&nbsp;</td><td>Unit Price</td><td>Total</td></tr>';
  $call_index = 1;  
  foreach ($callings as $call) {
      echo '
        <tr>
            <td width=14% rowspan=3 valign=top>Call: ' . ($call_index++) . '</td>
            <td width=5% valign=top>Date: </td>
            <td width=11% valign=top>' . substr($call->field_job_session_date_time_value, 0, 10) . '</td>
            <td width=5% valign=top>Time: </td>
            <td width=11% valign=top>' . substr($call->field_job_session_date_time_value, 11, 20) . '</td>
            <td width=5% valign=top>Crew: </td>
            <td width=11% valign=top>' . $call->field_job_session_crew_value . '</td>
            <td width=5% valign=top>Hours: </td>
            <td width=11% valign=top>' . $call->field_job_session_hours_value . '</td>
            <td width=2% valign=top>=</td>
            <td width=8% valign=top>' . $call->field_job_session_client_rate_value * $call->field_job_session_hours_value * $call->field_job_session_crew_value . '</td>
            <td width=8% valign=top>0.00</td>
        </tr>
        <tr>
            <td colspan=4 valign=top>Crew Chief Supplement:</td>
            <td colspan=4 valign=top>0</td>
            <td valign=top>&#xd7;</td>
            <td valign=top>0.00</td>
            <td valign=top>0.00</td>
        </tr>
        <tr>
            <td colspan=4 valign=top>Travel supplement (M25):0</td>
            <td colspan=4 valign=top>0</td>
            <td valign=top>&#xd7;</td>
            <td valign=top>0.00</td>
            <td valign=top>0.00</td>
        </tr>';
  }
  
  echo '</table>';
  
  $content = ob_get_contents(); ob_end_clean();
  return $content;
}
