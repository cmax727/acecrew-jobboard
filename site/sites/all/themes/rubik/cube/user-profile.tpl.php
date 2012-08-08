<?php
// $Id: user-profile.tpl.php,v 1.2.2.2 2009/10/06 11:50:06 goba Exp $

/**
 * @file user-profile.tpl.php
 * Default theme implementation to present all user profile data.
 *
 * This template is used when viewing a registered member's profile page,
 * e.g., example.com/user/123. 123 being the users ID.
 *
 * By default, all user profile data is printed out with the $user_profile
 * variable. If there is a need to break it up you can use $profile instead.
 * It is keyed to the name of each category or other data attached to the
 * account. If it is a category it will contain all the profile items. By
 * default $profile['summary'] is provided which contains data on the user's
 * history. Other data can be included by modules. $profile['user_picture'] is
 * available by default showing the account picture.
 *
 * Also keep in mind that profile items and their categories can be defined by
 * site administrators. They are also available within $profile. For example,
 * if a site is configured with a category of "contact" with
 * fields for of addresses, phone numbers and other related info, then doing a
 * straight print of $profile['contact'] will output everything in the
 * category. This is useful for altering source order and adding custom
 * markup for the group.
 *
 * To check for all available data within $profile, use the code below.
 * @code
 *   print '<pre>'. check_plain(print_r($profile, 1)) .'</pre>';
 * @endcode
 *
 * Available variables:
 *   - $user_profile: All user profile data. Ready for print.
 *   - $profile: Keyed array of profile categories and their items or other data
 *     provided by modules.
 *
 * @see user-profile-category.tpl.php
 *   Where the html is handled for the group.
 * @see user-profile-item.tpl.php
 *   Where the html is handled for each item in the group.
 * @see template_preprocess_user_profile()
 */
?>
<?php
$user_view=$_GET;
$user_view=explode('/',$user_view['q']);
//echo '<pre>'.print_r($user_view,1);

//echo '<pre>'. print_r($account,1);
//echo '<pre>';print_r(unserialize($account->data));

$skills=array();
if($account->profile_crew_skill_new)
$skills[]='New Crew';
if($account->profile_crew_skills_chief)
$skills[]='Crew Chief';
if($account->profile_crew_skills_driver)
$skills[]='Driver';
if($account->profile_crew_skills_cherry)
$skills[]='Cherry Picker';
if($account->profile_crew_skills_forklift)
$skills[]='Forklift Driver';
?>

<script type="text/javascript">
$(document).ready(function(){
    $('.profile_item').hide();
    $('.profile_item').filter(':first').show();

    $('.profile_title').click(function(){
        $('.profile_item').hide('slow');
        var id=$(this).attr('title');
        $('.'+id).show('slow');
    });
});
</script>

<div id='tabs'>
<div class="page-tabs limiter clear-block" style="width:100%">
<ul class="links secondary-tabs">
<li <?php if(!isset($user_view[3]))echo 'class="active"';?>><a href="/user/<?=$account->uid?>/view">Account</a></li>
<li <?php if(isset($user_view[3]) && $user_view[3]=='profile-information')echo 'class="active"';?>><a href="/user/<?=$account->uid?>/view/profile-information">Profile Information</a></li>
<li <?php if(isset($user_view[3]) && $user_view[3]=='current-activity')echo 'class="active"';?>><a href="/user/<?=$account->uid?>/view/current-activity">Current Activity</a></li>
<li <?php if(isset($user_view[3]) && $user_view[3]=='booked-off')echo 'class="active"';?>><a href="/user/<?=$account->uid?>/view/booked-off">Booked Off</a></li>
</ul>
</div>
</div>
<br />

<div class="profile">
 <?php if(!isset($user_view[3])){?>
  <h2 class="profile_title" title="account_information">Account Information:</h2>
  <div class="account_information profile_item">
  <div>Username: <?=$account->name?></div>
  <div>Email: <?=$account->mail?></div>
  <div>Status: <?=$account->status?'Active':'Blocked'?></div>
  <div>Roles: <?php $i=0; foreach($account->roles as $item){ echo $item; if($i<count($account->roles)-1) echo ', ';$i++;}?></div>
  </div>
  <?php }?>    
      
  <?php if(isset($user_view[3]) && $user_view[3]=='profile-information'){?>
  <h2 class="profile_title" title="personal_information">Personal Information:</h2>
  <div class="personal_information profile_item">
  <div>Full Name: <?php echo ($account->profile_title?$account->profile_title.' ':'').$account->profile_crew_fullname?></div>
  <div>Nationality: <?=$account->profile_nationality?></div>
  <div>Date Of Birth: <?=$account->profile_crew_dob['day'].'-'.$account->profile_crew_dob['month'].'-'.$account->profile_crew_dob['year'];?></div>
  <div>Address: <?=$account->profile_crew_address;?></div>
  <div>Post Code: <?=$account->profile_crew_postcode;?></div>
  <div>Mother's Maiden Name: <?=$account->profile_crew_mothers_name;?></div>
  <div>NI Number: <?=$account->profile_crew_ni_number;?></div>
  <div>Emergency Details: <?=$account->profile_crew_nok;?></div>
  <div>Emergency Telephone: <?=$account->profile_crew_nok_telephone;?></div>
  </div>
  
  <br />    
  <h2 class="profile_title" title="contact_information">Contact Information:</h2>
  <div class="contact_information profile_item">  
  <div>Telephone: <?=$account->profile_crew_telephone;?></div>
  <div>Mobile Number: <?=$account->profile_crew_mobile;?></div>
  <div>Nearest Station/Tube: <?=$account->profile_crew_mobile;?></div>
  </div>

  <br />    
  <h2 class="profile_title" title="crew_skills">Crew Skills:</h2>
  <div class="crew_skills profile_item">
  <div><?php $i=0; foreach($skills as $item){ echo $item; if($i<count($skills)-1) echo ', ';$i++;}?></div>
  <!--<div>Pay Rate: <?=$account->profile_crew_mobile;?></div>-->
  </div>

  <br />    
  <h2 class="profile_title" title="employment_type">Employment Type:</h2>
  <div class="employment_type profile_item">
  <div><?=$account->profile_employment_type;?></div>
  </div>
  
  <br />    
  <h2 class="profile_title" title="miscellaneous">Miscellaneous:</h2>
  <div class="miscellaneous profile_item">
  <div>Notes: <?=$account->profile_notes;?></div>
  <div>Strength: <?=$account->profile_strength;?></div>
  </div>

  <br />    
  <h2 class="profile_title" title="passport" >Passport & Licence:</h2>
  <div class="passport profile_item">
  <div>Passport Number: <?=$account->profile_passport_number;?></div>
  <div>Licence Number: <?=$account->profile_licence;?></div>
  </div>

  <br />    
  <h2 class="profile_title" title="visa_details">Visa Details:</h2>
  <div class="visa_details profile_item">
  <div>Visa Type: <?=$account->profile_visa_type;?></div>
  <div>Visa Start Date: <?=$account->profile_visa_start['day'].'-'.$account->profile_visa_start['month'].'-'.$account->profile_visa_start['year'];?></div>
  <div>Visa End Date: <?=$account->profile_visa_end_date['day'].'-'.$account->profile_visa_end_date['month'].'-'.$account->profile_visa_end_date['year'];?></div>
  </div>


  <br />    
  <h2 class="profile_title" title="crew_sizes">Crew Sizes:</h2>
  <div class="crew_sizes profile_item">
  <div>Shirt Size: <?=$account->profile_crew_shirt;?></div>
  <div>Trouser Size: <?=$account->profile_crew_trouser;?></div>
  <div>Length Of Leg: <?=$account->profile_length_leg;?></div>
  <div>Height: <?=$account->profile_crew_height;?></div>
  </div>
  <?php }?>
      
  <?php if(isset($user_view[3]) && $user_view[3]=='Current Activity'){?>
  <h2 class="profile_title" title="current_activity">Current Activity:</h2>
  <div class="current_activity profile_item">
  <div><?=$account->profile_activity;?></div>
  </div>
  <?php }?>
      
  <?php if(isset($user_view[3]) && $user_view[3]=='Booked Off'){?>    
  <h2 class="profile_title" title="booked_off">Booked Off:</h2>
  <div class="booked_off profile_item">
  <div><?=$account->profile_booked;?></div>
  </div>
  <?php }?>
  
</div>




