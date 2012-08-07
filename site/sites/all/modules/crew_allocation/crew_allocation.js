

(function ($) {

Drupal.behaviors.crewAllocation = function() {
  $('.crew-allocation-child:not(.crew-allocation-processed)').addClass('crew-allocation-processed').click(function() {
    var element = this;

    // This is our onSubmit callback that will be called from the child window
    // when it is requested by a call to modalframe_close_dialog() performed
    // from server-side submit handlers.
    function onSubmitCallbackExample(args, statusMessages) {
      // Display status messages generated during submit processing.
      if (statusMessages) {
        // $('.crew-allocation-messages').hide().html(statusMessages).show('slow');
        window.location.reload();
      }
    }

    // Hide the messages are before opening a new dialog.
    $('.crew-allocation-messages').hide('fast');

    // Build modal frame options.
    var modalOptions = {
      url: $(element).attr('href'),
      autoFit: true,
      onSubmit: onSubmitCallbackExample
    };

    // Try to obtain the dialog size from the className of the element.
    var regExp = /^.*crew-allocation-size\[\s*([0-9]*\s*,\s*[0-9]*)\s*\].*$/;
    if (typeof element.className == 'string' && regExp.test(element.className)) {
      var size = element.className.replace(regExp, '$1').split(',');
      modalOptions.width = parseInt(size[0].replace(/ /g, ''));
      modalOptions.height = parseInt(size[1].replace(/ /g, ''));
    }

    // Open the modal frame dialog.
    Drupal.modalFrame.open(modalOptions);

    // Prevent default action of the link click event.
    return false;
  });
};

})(jQuery);
