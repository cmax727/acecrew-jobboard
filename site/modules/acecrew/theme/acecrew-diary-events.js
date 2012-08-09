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
    var args = $('#job-session-id').val();
    
    types.each(function() {
        args +=  ":" + $(this).attr("id")+ ":" + $(this).val();    
    });
    
    $.get(Drupal.settings.basePath + "acecrew/calendar/set-status/" + args, function(data){
        $("#acecrew_calendar_output").html('<img class="loading-gif" src="' + Drupal.settings.basePath + Drupal.settings.acecrew.basepath + '/theme/ajax-loader.gif" />');
        forward = $('#acecrew_calendar_forward_days').val();
        $("#block-block-2").hide();
        $("#block-block-2").html("<div class=\"block-content clear-block prose\"><span class=\"icon-info\"></span><p>Click on the load call button at the bottom of each call, to load more information</p></div>");
        $("#block-block-2").fadeIn(1000);
        $.get(Drupal.settings.basePath + "acecrew/calendar/" + $('#edit-date-timer-datepicker-popup-0').val() + "/" + forward, function(data){
            var result = Drupal.parseJson(data);
            $("#acecrew_calendar_output").html(result.html);
        });
    });
}