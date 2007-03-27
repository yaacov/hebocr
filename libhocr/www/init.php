<?php

$output_path = './output/';
$log_path = './log.txt';
	
function init()
{
	global $output_path;
	global $log_path;

	if (!file_exists($output_path))
	{
		mkdir($output_path);
		chmod($output_path, 0777);
	}
	if (!file_exists($log_path))
	{
		$handle = fopen($log_path, 'w');
		fwrite($handle, 'create: '.date('YmdHis')."\n");
		fclose($handle);
		chmod($log_path, 0666);
	}
}

function log_str($str)
{
	global $output_path;
	global $log_path;

	if (!$handle = fopen($log_path, 'a')) 
	{
		exit;
	}

	if (fwrite($handle, $str."\n") === FALSE) {
       exit;
	}

	fclose($handle);
}

function get_page ($author, $title, $page_number)
{
	global $output_path;
	global $log_path;

	$files = scandir($output_path);

	foreach ($files as $filename)
	{
		$filemame_array = explode('.',$filename);
		$ext = $filemame_array[count($filemame_array) - 1];
	
		if ($ext == 'meta')
		{
			$page_info = parse_ini_file($output_path.$filename);
				
			if ($author == $page_info['author'] and
			$title == $page_info['title'] and
			$page_number == $page_info['page_number'])
			{
				$page = $output_path.$page_info['text'];
				$image = $output_path.$page_info['image'];
					
				$reading = '?author='.$page_info['author'].'&amp;';
				$reading = $reading.'title='.$page_info['title'].'&amp;';
				$reading = $reading.'page_number='.$page_info['page_number'].'&amp;';
			
		
				return array (
					'page' => $page, 
					'image' => $image,
					'reading' => 'read.php'.$reading,
					'writing' => 'write.php'.$reading);
			}
		} 
	}
}

function remove_nikud ($str)
{
   $arr = str_split($str, 1);

	for ($i = 0; $i < sizeof($arr);)
	{
		$or = ord($arr[$i]);
		if ($or == '215') // Hebrew
		{
			$out[] = $arr[$i];
			$out[] = $arr[$i + 1];
			$i = $i + 2;
		}
		else if ($or == '214') // Nikud
		{
			$i = $i + 2;
		}
		else // just copy
		{
			$out[] = $arr[$i];
			$i = $i + 1;
		}
	}
	
	return implode ('', $out);
}

function remove_quats ($str)
{
   $arr = str_split($str, 1);

	for ($i = 0; $i < sizeof($arr);)
	{
		if ($arr[$i] == '\'' or $arr[$i] == '"' or 
		$arr[$i] == '\\' or $arr[$i] == '/' or
		$arr[$i] == '.' or $arr[$i] == '-' or
		$arr[$i] == '?' or $arr[$i] == '#' or
		$arr[$i] == '&' or $arr[$i] == '<' or
		$arr[$i] == '>')
		{
			$i = $i + 1;
		}
		else // just copy
		{
			$out[] = $arr[$i];
			$i = $i + 1;
		}
	}
	
	return implode ('', $out);
}
	
function file_has_word ($word, $page)
{
	global $output_path;
	global $log_path;

	$nikudles_word = remove_nikud ($word);
	
	if (is_readable ($page))
	{
		$lines = file($page);
		foreach ($lines as $line)
		{
			if (strchr (remove_nikud ($line), $nikudles_word))
			{
				return true;
			}
		}
	}
	
	return false;
}
	
function get_index ($author, $title, $word, $sort_by)
{
	global $output_path;
	global $log_path;

	$index = array();
	
	$files = scandir($output_path);
	foreach ($files as $filename)
	{
		$filemame_array = explode('.',$filename);
		$ext = $filemame_array[count($filemame_array) - 1];
    	if ($ext == 'meta')
		{
			$page_info = parse_ini_file($output_path.$filename);
  
			$page = $output_path.$page_info['text'];
			if (($author == '' or strchr ($page_info['author'], $author)) and
				($title == '' or strchr ($page_info['title'], $title)) and
				($word == '' or file_has_word ($word, $page)))
			{
				$reading = '?author='.$page_info['author'].'&amp;';
				$reading = $reading.'title='.$page_info['title'].'&amp;';
				$reading = $reading.'page_number='.$page_info['page_number'].'&amp;';
			  
				$index[] = array (
					'author' => $page_info['author'], 
					'title' => $page_info['title'],
					'page_number' => $page_info['page_number'],
					'image' => $output_path.$page_info['image'],
					'layout' => $output_path.$page_info['layout'],
					'bw' => $output_path.$page_info['bw'],
					'reading' => 'read.php'.$reading,
					'writing' => 'write.php'.$reading,
						'text' => $output_path.$page_info['text']);

			}
		}  
	}
	
	if ($index)
	{
		foreach ($index as $key => $row) 
		{
			$author_arr[$key]  = $row['author'];
			$title_arr[$key] = $row['title'];
		}
	
		if ($sort_by == 'title')
		{
			array_multisort($title_arr, SORT_ASC, $author_arr, SORT_ASC, $index);
		}
		else
		{
			array_multisort($author_arr, SORT_ASC, $title_arr, SORT_ASC, $index);
		}
	}
	
	return $index;
}

function print_nav ($index)
{
	global $output_path;
	global $log_path;

	print '<ul id="nav">'."\n";
	
	if ($index == 1)
		{print '<li><a href="index.php" class="active">העלאת תמונה</a></li>'."\n";}
	else
		{print '<li><a href="index.php">העלאת תמונה</a></li>'."\n";}
	if ($index == 2)
		{print '<li><a href="directory.php" class="active">ספריה</a></li>'."\n";}
	else
		{print '<li><a href="directory.php">ספריה</a></li>'."\n";}
	if ($index == 3)
		{print '<li><a href="read.php" class="active">קריאה</a></li>'."\n";}
	else
		{print '<li><a href="read.php">קריאה</a></li>'."\n";}
	if ($index == 4)
		{print '<li><a href="write.php" class="active">הגהה</a></li>'."\n";}
  else
		{print '<li><a href="write.php">הגהה</a></li>'."\n";}
  if ($index == 5)
		{print '<li><a href="about.php" class="active">אודות</a></li>'."\n";}
	else
		{print '<li><a href="about.php">אודות</a></li>'."\n";}
	print '</ul>'."\n";
}

?>
