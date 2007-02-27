
<h2>חיפוש</h2>

<form action="read.php" method="get">
	<table class="send">
	  <tr>
		<td>כותרת</td>
		<td><input type="text" name="title" value=<?php print '"'.remove_quats ($_GET["title"]).'"'; ?> /></td>
	  </tr>
	  <tr>
		<td>מחבר</td>
		<td><input type="text" name="author" value=
		  <?php print '"'.remove_quats ($_GET["author"]).'"'; ?>
		  /></td>
	  </tr>
	  <tr>
		<td>מספר דף</td>
		<td><input type="text" name="page_number" value=
			<?php print '"'.remove_quats ($_GET["page_number"]).'"'; ?>
			/></td>
	  </tr>
	<tr><td colspan="2"><input type="submit" value="חפש" /></td></tr>    
	</table>
</form>

<?php
   // look for page
  $page_info = get_page (remove_quats ($_GET["author"]), remove_quats ($_GET["title"]), remove_quats ($_GET["page_number"]));
  $page = $page_info['page'];
  $writing = $page_info['writing'];
  // if no .txt for some reasone create one 
  if ($page)
  {
    touch($page);
  }
  
  // print page
  if (is_file($page))
  {
     print '<h2>'.remove_quats ($_GET['title']).'</h2>'."\n";
     print '<h3>'.remove_quats ($_GET['author']).'</h3>'."\n";
     print 'דף מס. '.remove_quats ($_GET['page_number']).'<br />'."\n";
    
    print '<p>'; 
    $lines = file($page);
    
    foreach ($lines as $line) 
	{
       print stripcslashes($line)."<br />\n";
    }
?>
    </p>
	<h2></h2>
	<p><a href="<?php print $writing; ?>">להגהה של טקסט זה לחצו כאן</a>
    </p>
<?php
  }  
  // can't find page
  else
  {
  
?>
<h2>הדף לא קיים</h2>
<p>הדף המבוקש לא נמצא</p>
<?php
  }
?>
