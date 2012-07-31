var acecrew_current_edit_item = null;

function acecrew_session_edit(ses_id)
{
    acecrew_current_edit_item = 'acecrew_session_form_' + ses_id;
    $('#acecrew_session_content_' + ses_id).hide('fast');
    $('#acecrew_session_buttons_' + ses_id).hide('fast');
    $('.acecrew_session_add_block').hide('fast');
    $('#acecrew_session_form_' + ses_id).show('fast');
}

function acecrew_session_del(ses_id)
{
    $.get(Drupal.settings.basePath + "acecrew/ajax/delsession/" + ses_id, function(data){
        window.location.reload();
    });
}



$(document).ready(function() {
    $('#acecrew_session_add_button').click(function(){
        acecrew_current_edit_item = 'acecrew_session_add_form';
        $('#acecrew_session_add_form').show('fast');
        $('#acecrew_session_add_buttons').hide('fast');
        $('.acecrew_session_buttons').hide('fast');
    });
    
    

    
    
    $('body').ajaxComplete(function(){
        //set default values for edit forms.  session crews generation

        var def_val = $('#edit-field-job-session-hours-value').val();
        if(def_val != ''){
            for(i = 0; i <= 100; i = i + 1){
                var curr_element = $('#edit-field-job-session-crews-' + i + '-value-field-crew-job-session-hours-quo-0-value');
                var curr_val = curr_element.val();
                if(curr_val == ''){
                    curr_element.val(def_val);
                }
            }
        }
        
    });
    
    
    Drupal.Ajax.plugins.acecrew = function(hook, args){
        if (hook == 'complete'){
            window.location.reload();
        }
        if (hook == 'scrollFind'){            
            document.getElementById(acecrew_current_edit_item).scrollIntoView(true);
            return false;
        }
    }
});