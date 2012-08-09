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

var udpate_status_onclick = function(){
    var types = $(".supplements-type");
    var args = '';
    
    types.each(function() {
        args += $(this).val() + ":" + $(this).attr("id") + ":";    
    });
    
    $.get(Drupal.settings.basePath + "acecrew/calendar/set-status/" + args, function(data){
        alert("congratulation! your work is successed.");
    });
}