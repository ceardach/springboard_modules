(function($) {
  var callback = {
    exec:function(editor) {
      // get the current location so we can pass in the path
      var url = $(location).attr('href');
      url = url.replace('http://', ''); // strip off http bs
      var parts = url.split('/');
      if (parts[1] == 'node' && parts[3] == 'webform' && parts[4] == 'confirmations') {
        var token = window.showModalDialog("/fundraiser/confirmation-token-list/"+parts[2],null,"dialogWidth:750px;dialogHeight:500px;center:yes; resizable: yes; help: no");  
        if (token != false && token != null) {
          editor.insertHtml(token);    
        }
      }
      else {
        alert('This only works for fundraiser confirmations');
      }  
    }
  },

  /**
   * Adds a CKEditor plugin to insert <pre> tags.
   *
   * This is heavily based on blog posts by:
   *
   * Nikolay Ulyanitsky
   * http://blog.lystor.org.ua/2010/11/ckeditor-plugin-and-toolbar-button-for.html
   *
   * and
   *
   * Peter Petrik
   * http://peterpetrik.com/blog/ckeditor-and-geshi-filter
   */
  buttonName = 'fundraiser-tokens';
  CKEDITOR.plugins.add(buttonName, {
    init: function (editor) {
      // Create a new CKEditor style to add <pre> tags.
      //var buttonName = b;
      

      // Add the command and the button to the editor.
      editor.addCommand(buttonName, callback);
      editor.ui.addButton(buttonName, {
        label: Drupal.t('Fundraiser tokens'),
        icon: this.path + 'fundraiser-tokens.png',
        command: buttonName
      });
    }
  });

})(jQuery);
