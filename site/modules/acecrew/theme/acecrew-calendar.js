$(document).ready(function() {

    //$('#edit-date-forward-days').spinner({ min: -100, max: 100 });
    
    $('#acecrew-calendar-form').submit(function(){
        return false;
    });
    
    /*
    $('#edit-date-timer-datepicker-popup-0').change(function(){
        $("#acecrew_calendar_output").html('<img class="loading-gif" src="' + Drupal.settings.basePath + Drupal.settings.acecrew.basepath + '/theme/ajax-loader.gif" />');
        $.get(Drupal.settings.basePath + "acecrew/calendar/" + this.value, function(data){
            var result = Drupal.parseJson(data);
            $("#acecrew_calendar_output").html(result.html);
        });
    })
    */
    
    // added by anton    
    $('#edit-date-submit').click(function() {
        $("#acecrew_calendar_output").html('<img class="loading-gif" src="' + Drupal.settings.basePath + Drupal.settings.acecrew.basepath + '/theme/ajax-loader.gif" />');
        forward = $('#acecrew_calendar_forward_days').val();
        $.get(Drupal.settings.basePath + "acecrew/calendar/" + $('#edit-date-timer-datepicker-popup-0').val() + "/" + forward, function(data){
            var result = Drupal.parseJson(data);
            $("#acecrew_calendar_output").html(result.html);
        });
    });
    
    $('#edit-date-timer-datepicker-popup-0').trigger('change');
    
});
