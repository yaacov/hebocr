
<?php
 
$path = $output_path;

if ($_SERVER['REQUEST_METHOD'] == "POST") 
{
  $filename = $_FILES['imgfile']['name'];
  $size = $_FILES['imgfile']['size'];
  $filemame_array = explode('.',$filename);
  $ext = $filemame_array[count(filemame_array)];
  $name = $filemame_array[0].'-'.date('YmdHis');
  $filename= $name.'.'.$ext;
  $client_ip = $_SERVER['REMOTE_ADDRESS'];
  $domain = $_SERVER['REMOTE_HOST']; 
  $client = $_SERVER['HTTP_USER_AGENT'];
  $command1 = 'convert '.$path.$filename.' '.$path.$name.'.pgm';
  $command2 = 'hocr -Nd -i '.$path.$name.'.pgm -O '.$path.$name;	
  $command4 = 'echo ...';
  $command5 = 'echo ...';
  $command6 = 'rm '.$path.'*.pgm';
  $meta_data1 = '[page]';
  $meta_data2 = 'author = "'.remove_quats ($_POST['author']).'"';
  $meta_data3= 'title = "'.remove_quats ($_POST['title']).'"';
  $meta_data4 = 'page_number = "'.remove_quats ($_POST['n_page']).'"';
  $meta_data5 = 'image = "'.$filename.'"';
  $meta_data6 = 'layout = ';
  $meta_data7 = 'bw = ';
  $meta_data8 = 'text = ';
  
  $command2 = $command2.' -c '.$_POST['blocks_setup'];  
  $command2 = $command2.' -s '.$_POST['scale'].' -a '.$_POST['threshold_type'];
  $command2 = $command2.' -t '.$_POST['threshold'].' -T '.$_POST['a_threshold'];
  if ($_POST['image'])
    {$command2 = $command2.' -R';}
  if ($_POST['halftone'])
    {$command2 = $command2.' -r';}
  if ($_POST['fix_ligated'])
    {$command2 = $command2.' -E';}
  if ($_POST['fix_broken'])
    {$command2 = $command2.' -e';}
  if ($_POST['bw'])
    {$command2 = $command2.' -b';
    $command4 = 'convert '.$path.$name.'-image-bw.pgm '.$path.$name.'-I.jpeg';
    $meta_data7 = 'bw = "'.$name.'-I.jpeg'.'"';}
  if ($_POST['layout'])
    {$command2 = $command2.' -l';
    $command5 = 'convert '.$path.$name.'-image-segments.pgm '.$path.$name.'-L.jpeg';
    $meta_data6 = 'layout = "'.$name.'-L.jpeg'.'"';}
  if ($_POST['text'])
    {$command2 = $command2.' -o '.$path.$name.'.txt';
    $meta_data8 = 'text = "'.$name.'.txt'.'"';}
  
  print '<h2>מעבד קובץ</h2>'."\n";
  print '<p>מעבד את הקובץ: '.$filename.'<br />'."\n";
  
  print 'מחבר: '.remove_quats ($_POST['author']).'<br />'."\n";
  print 'כותרת: '.remove_quats ($_POST['title']).'<br />'."\n";
  print 'מספר דף: '.remove_quats ($_POST['n_page']).'<br />'."\n";
	
  print 'גודל הקובץ: '.$size.' בתים<br />'."\n";
  print 'נשלח מכתובת IP: '.$client_ip.' ('.$domain.') </p>'."\n";
  print '<p>הדף הזה ימשיך להיכתב עד לסיום פעולת מזהה האותיות.'."\n";
  print ' במידה ולא יתגלו שגיאות, קבצי הפלט ירשמו לספריית הפלט.</p>'."\n";
  print '<p class="top"><a href="#hocr_web">ראשית</a></p>'."\n";

  if (get_page (remove_quats ($_POST['author']), remove_quats ($_POST['title']), remove_quats ($_POST['n_page'])))
  {
	log_str ('error - '.date('YmdHis'));
	print '<h2>שגיאה</h2>'."\n";
	print '<p>במערכת כבר רשום דף בעל מחבר, כותרת ומספר דף זהים.'."\n";
	print ' ניתן להוסיף אות למספר הדף, לדוגמה, אם דף מספר 34 כבר קיים במערכת, עדיין אפשר לשלוח את דף 34ב</p>'."\n";	
	log_str ('  Duplicate name title and page number');
  }
  else
  {
	  if (move_uploaded_file ($_FILES['imgfile'] ['tmp_name'],$path.$name.'.'.$ext))
	  {
		log_str ('start procces of '.$name.'- '.$client_ip.' - '.$client);
		log_str ($command2);
		print '<h2>מעבד...</h2>';
		print '<h3>אין לסגור דף זה עד לסיום פעולת מזהה האותיות, סגירת הדף תפסיק את פעולת מזהה האותיות</h3>'."\n";
		print '<p>משנה את קידוד התמונה ...</p>'."\n";
		system($command1);
		print '<p>מעבד את התמונה ...</p><p><pre><div dir=\'ltr\'>'."\n";
		system($command2);
		print '</div></pre></p>'."\n";
		print '<p>מבצע ניקיונות ...</p>'."\n";
		system($command4);
		system($command5);
		system($command6);
		chmod( $path.$name.'.'.$ext, 0666);
		 if ($_POST['bw'] and is_file($path.$name.'-I.jpeg'))
		  {chmod( $path.$name.'-I.jpeg', 0666);}
		 if ($_POST['layout'] and is_file($path.$name.'-L.jpeg'))
		  {chmod( $path.$name.'-L.jpeg', 0666);}
		 if ($_POST['text'] and is_file($path.$name.'.txt'))
		  {chmod( $path.$name.'.txt', 0666);}
		print '<p>כותב קובץ מידע ...</p>';
		file_put_contents ($path.$name.'.meta', 
		$meta_data1."\n".$meta_data2."\n".
		$meta_data3."\n".$meta_data4."\n".
		$meta_data5."\n".$meta_data6."\n".
		$meta_data7."\n".$meta_data8."\n");
		chmod( $path.$name.'.meta', 0666);
		log_str('  end procces of '.$name);
	  }
	  else
	  {
		// log this
		log_str ('error - '.date('YmdHis'));
		print '<h2>שגיאה</h2>'."\n";
		switch ($_FILES['imgfile'] ['error'])
		{
		  case 1:
			print '<p> הקובץ גדול מידי עבור שרת הPHP</p>'."\n";
			log_str ('  The file is bigger than this PHP installation allows');
			break;
		  case 2:
			print '<p> הקובץ גדול מידי עבור הטופס</p>'."\n";
			log_str ('  The file is bigger than this form allows');
			break;
		  case 3:
			print '<p> העלאת הקובץ לא הושלמה</p>'."\n";
			log_str ('  Only part of the file was uploaded');
			break;
		  case 4:
			print '<p> לא הועלה קובץ</p>'."\n";
			log_str (' No file was uploaded');
			break;
		}
	}
  }
}
else
{
  print '<p class="top"><a href="#hocr_web">ראשית</a></p>'."\n"; 
  print '<h2>מעבד קובץ</h2>'."\n";
  print '<p>לא הועלה קובץ</p>'."\n";
}
?>
<p class="top"><a href="#hocr_web">ראשית</a></p> 

<h2>סוף הדף</h2>
<p>פעולת העלאת התמונה הסתיימה.</p>
<p>אם פעולת הזיהוי הושלמה בהצלחה, קבצי הפלט ימצאו <a href="directory.php">בספריה</a>.</p>
<p class="top"><a href="#hocr_web">ראשית</a></p>
