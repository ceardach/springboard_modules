<div id="springboard_admin-toolbar-wrapper">
<?php if ($messages): ?>
  <?php print render($messages); ?>
<?php endif; ?>
<div id="springboard_admin-toolbar-container">
  <div id="springboard_admin-toolbar">
    <?php print render($page['content']['system_main']['main']); ?>
  </div>
</div>
<div id="springboard_admin-site-container" height="100%">
  <iframe id="springboard_admin-site" name="springboard_admin-iframe" src="<?php print $url; ?>" scrolling="auto" frameBorder="0" height="100%" />
    <h3>Your Browser Does Not Support iFrames. <a href="<?php print $url; ?>" title="<?php print $url; ?>">Click here to view the page you selected</a></h3>
  </iframe>
</div>
</div>
