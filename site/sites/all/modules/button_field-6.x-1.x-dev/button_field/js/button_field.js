// $Id: button_field.js,v 1.1.2.4 2010/02/07 20:32:38 bassistjimmyjam Exp $
Drupal.behaviors.buttonFieldBehavior = function(context) {
  $('.button_field').bind('click', function() {
    $.ajax({
      url: Drupal.settings.basePath+'button_field/callback',
      dataType: 'json',
      data: {id: $(this).attr('id')},
      success: function(data) {
        if (data.redirect) {
          document.location.href = data.redirect;
        }
      }
    }); // end $.ajax()
    
    return false;
  }); // end function $.bind()
}; // end function buttonFieldBehavior()
