(function ($) {

  Drupal.behaviors.externanaliFrame = {
    attach: function(context, settings) {
      // Set iframe height
      function resizeframe() {
        var buffer = $('#springboard_admin-toolbar-container').height();
        var newHeight = $(window).height();
        var newFrameHeight = newHeight - buffer;
        $('#springboard_admin-site-container').css('height', newFrameHeight);
      }
      $(window).resize(function() {
        resizeframe();
      });
      $(window).load(function() {
        resizeframe();
      });
    }
  };

})(jQuery);
