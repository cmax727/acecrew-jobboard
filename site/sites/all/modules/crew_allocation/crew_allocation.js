function onSubmitCallbackExample()
{
    alert("Ya!!");
        $("#acecrew_calendar_output", window.parent.document).html('<img class="loading-gif" src="/modules/acecrew/theme/ajax-loader.gif" />');
        forward = $('#acecrew_calendar_forward_days', window.parent.document).val();
        $.get(Drupal.settings.basePath + "acecrew/calendar/" + $('#edit-date-timer-datepicker-popup-0', window.parent.document).val() + "/" + forward, function(data){
            var result = Drupal.parseJson(data);
            $("#acecrew_calendar_output", window.parent.document).html(result.html);
            window.close();
        });
}
    

