$(document).ready(function() {
    //prevent submit
    $('#acecrew-calendar-form').submit(function(){
        return false;
    });
    
    $('#edit-date-timer-datepicker-popup-0').change(function(){
        $("#acecrew_calendar_output").html('<img class="loading-gif" src="' + Drupal.settings.basePath + Drupal.settings.acecrew.basepath + '/theme/ajax-loader.gif" />');
        
        $.get(Drupal.settings.basePath + "acecrew/calendar/" + this.value, function(data){
            var result = Drupal.parseJson(data);
            $("#acecrew_calendar_output").html(result.html);
        });
    })
    
    $('#edit-date-timer-datepicker-popup-0').trigger('change');
});
