var calendar_popup_sms_click = function(baseurl,call_nid){
    $url = baseurl + "/acecrew/calendar-popup/sms/" + call_nid;
    options = {
        url : $url,
        width: 300,
        height: 400,
        autoFit: true
    }
    Drupal.modalFrame.open(options);
    return false;
}

