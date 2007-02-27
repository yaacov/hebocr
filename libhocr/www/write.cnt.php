
<?php
// check for input data
// FIXME: a user can send anything, to any file, is strip_tags enough here ? 
if ($_SERVER['REQUEST_METHOD'] == "POST") 
{
  $name = $HTTP_POST_VARS['file'];
  if (is_file($name))
  {
    rename($name, $name.'.'.date('YmdHis'));
    file_put_contents ($name, strip_tags ($_POST['text']));
	chmod($name, 0666);
	chmod($name.'.'.date('YmdHis'), 0666);
  }
}
?>

<!-- FIXME: how to set caret position after text insert ? -->
 <script  language="javascript"  type="text/javascript">
	function add_nikud(ch)
	{
		// ie
		if (document.selection) 
		{
			document.edit.text.focus();
			sel = document.selection.createRange();
			sel.text = ch;
		}
		// iceweasel
		else if (document.edit.text.selectionStart || document.edit.text.selectionStart == '0') 
		{
			var start = document.edit.text.selectionStart;
			var end = document.edit.text.selectionEnd;

			document.edit.text.value = document.edit.text.value.substring(0, start)
			+ ch
			+ document.edit.text.value.substring(end, document.edit.text.value.length);
		} else 
		{
			document.edit.text.value+=ch;
		}
		document.edit.text.focus();
	}
</script>

<h2>חיפוש</h2>
<form action="write.php" method="get">
	<table class="send">
	  <tr>
		<td>כותרת</td>
		<td><input type="text" name="title" value=<?php print '"'.$_GET["title"].'"'; ?> /></td>
	  </tr>
	  <tr>
		<td>מחבר</td>
		<td><input type="text" name="author" value=
		  <?php print '"'.$_GET["author"].'"'; ?>
		  /></td>
	  </tr>
	  <tr>
		<td >מספר דף</td>
		<td><input type="text" name="page_number" value=
			<?php print '"'.$_GET["page_number"].'"'; ?>
			/></td>
	  </tr>
	<tr><td colspan="2"><input type="submit" value="חפש" /></td></tr>  
	</table>
</form>

<?php
  // look for page
  $page_info = get_page ($_GET["author"], $_GET["title"], $_GET["page_number"]);
  $page = $page_info['page'];
  $image = $page_info['image'];  
  $writing = $page_info['writing'];
  $reading = $page_info['reading'];  
  
  // if no .txt for some reasone create one 
  if ($page)
  {
    touch($page);
  }
  
  // print page
  if (is_file($page))
  {
     print '<h2>'.$_GET['title'].'</h2>'."\n";
     print '<h3>'.$_GET['author'].'</h3>'."\n";
     print 'דף מס. '.$_GET['page_number'].'<br />'."\n";
     print "\n";
     print '<form action="'.$writing.'" method="post" enctype="multipart/form-data" name="edit">'."\n";
     print '<table class="send">'."\n";
	 
     if (is_file($image))
     {
       print '<tr>'."\n";
       print '   <td>'."\n";
	   print '   <!-- FIXME: this is not valid *Strict* xhtml :-( -->'."\n";
       print '   <iframe src="'.$image.'"></iframe>'."\n";
       print '   </td>'."\n";   
       print '</tr>'."\n";
     }
     print '  <tr>'."\n";
     print '    <td>'."\n";
     print '      <input type="hidden" name="file" value="'.$page.'" />'."\n";
     print '      <textarea name="text" rows="10" cols="40">';
     $lines = file($page);
     foreach ($lines as $line) {
       print stripcslashes($line);
     }
	print '</textarea><br />'."\n";
?>
   </td>
 </tr>
<tr> 
 <td>
	<table>
		<tr>
			<td onclick="add_nikud('ְ')" onmouseover="this.className='title';" onmouseout="this.className='';">שְ</td>
			<td onclick="add_nikud('ֳ')" onmouseover="this.className='title';" onmouseout="this.className='';">שֳ</td>
			<td onclick="add_nikud('ֲ')" onmouseover="this.className='title';" onmouseout="this.className='';">שֲ</td>
			<td onclick="add_nikud('ֱ')" onmouseover="this.className='title';" onmouseout="this.className='';">שֱ</td>
			
			<td onclick="add_nikud('ָ')" onmouseover="this.className='title';" onmouseout="this.className='';">אָ</td>
			<td onclick="add_nikud('ַ')" onmouseover="this.className='title';" onmouseout="this.className='';">אַ</td>
			<td onclick="add_nikud('ֵ')" onmouseover="this.className='title';" onmouseout="this.className='';">אֵ</td>
			<td onclick="add_nikud('ֶ')" onmouseover="this.className='title';" onmouseout="this.className='';">אֶ</td>
			<td onclick="add_nikud('ִ')" onmouseover="this.className='title';" onmouseout="this.className='';">אִ</td>
			<td onclick="add_nikud('ֹ')" onmouseover="this.className='title';" onmouseout="this.className='';">וֹ</td>
			<td onclick="add_nikud('ּ')" onmouseover="this.className='title';" onmouseout="this.className='';">וּ</td>
			<td onclick="add_nikud('ֻ')" onmouseover="this.className='title';" onmouseout="this.className='';">אֻ</td>
			
			<td onclick="add_nikud('ּ')" onmouseover="this.className='title';" onmouseout="this.className='';">דּ</td>
			<td onclick="add_nikud('ׁ')" onmouseover="this.className='title';" onmouseout="this.className='';">שׁ</td>
			<td onclick="add_nikud('ׂ')" onmouseover="this.className='title';" onmouseout="this.className='';">שׂ</td>
		</tr>
	</table>
 </td>
 </tr>
 <tr>
   <td>
   כל הגרסאות הקודמות מתועדות. במידה ושגיתם בעריכה, פנו למנהל המערכת כדי לשחזר גרסה קודמת
   </td>
 </tr>
 <tr>
   <td>
     <input type="submit" value="שמור טקסט לקובץ" />
   </td>
  </tr>
</table>
</form>

  <h2></h2>
  <p><a href="<?php print $reading; ?>">לקריאה של טקסט זה לחצו כאן</a>
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
