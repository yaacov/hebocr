
<h2>מיון</h2>
<form action="directory.php" method="get">
	
<table class="send">
  <tr>
    <td>מיין לפי</td>
    <td>
      <select name="sort_by" size="1">
<?php if ($_GET["sort_by"] == 'title') { ?>
		<option value="title">כותרת</option>
        <option value="author">מחבר</option>
<?php } else { ?>
		<option value="author">מחבר</option>
        <option value="title">כותרת</option>
<?php } ?>
      </select>
    </td>
  </tr>
  <tr>
    <td>כותרת</td>
    <td><input type="text" name="title" value=<?php print '"'.remove_quats ($_GET["title"]).'"'; ?> /></td>
  </tr>
  <tr>
    <td>מחבר</td>
    <td><input type="text" name="author" value=<?php print '"'.remove_quats ($_GET["author"]).'"'; ?> /></td>
  </tr>
  <tr>
    <td>מילה בטקסט</td>
    <td><input type="text" name="word" value=<?php print '"'.remove_quats ($_GET["word"]).'"'; ?> /></td>
  </tr>
<tr><td colspan="2"><input type="submit" value="בצע מיון" /></td></tr>    
</table>
</form>
<h2>תוכן</h2>
	<table class="directory">
	<tr>
		<td class="title">מחבר</td>
		<td class="title">כותרת</td>
		<td class="title">דף</td>
		<td class="title">קישור לקריאה</td>
		<td class="title">קישור להגהה</td>
		<td class="title">קישורים ישירים</td>
	</tr>

<?php
	$index = get_index (remove_quats ($_GET["author"]), remove_quats ($_GET["title"]), remove_quats ($_GET["word"]), $_GET["sort_by"]);
	
	if ($index)
	{
		foreach ($index as $page_info)
		{
?>
	<tr>
		<td class="page"><?php print $page_info['author']; ?>
		</td>
		<td class="page"><?php print $page_info['title']; ?>
		</td>
		<td class="page"><?php print $page_info['page_number']; ?>
		</td>
		<td class="page"><a href="<?php print $page_info['reading']; ?>">קריאה</a>
		</td>
		<td class="page"><a href="<?php print $page_info['writing']; ?>">הגהה</a>
		</td>
		<td class="link">
    <a href="<?php print $page_info['text']; ?>">קובץ טקסט</a><br />
    <a href="<?php print $page_info['image']; ?>">תמונה מקורית</a><br />
    <a href="<?php print $page_info['bw']; ?>">תמונה ש/ל</a><br />
    <a href="<?php print $page_info['layout']; ?>">תמונת עימוד</a>
		</td>
		</tr>
<?php
		}
	}
?>
	</table>

<p><a href="<?php print $output_path; ?>">קישור ישיר  לספריה</a></p>

<p class="top"><a href="#hocr_web">ראשית</a></p> 

