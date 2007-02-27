
<h2>העלאת  תמונות</h2>
<p>התמונות המקוריות, תמונות לאחר עיבוד, וקבצי טקסט ימצאו <a href="directory.php">בספריה</a>.</p>

<form action="procces.php" method="post" enctype="multipart/form-data">
	
<table class="send">
  
  <tr>
    <td colspan="2" class="title">
	<input type="hidden" name="MAX_FILE_SIZE" value="2500000" />
	<strong>העלאת התמונה</strong></td>
  </tr>
  <tr>
     <td colspan="2">
העלה תמונה: 
<input type="file" name="imgfile" /> <input type="submit" value="שלח תמונה לזיהוי" />
     </td>
  </tr> 
  <tr>
     <td></td>
     <td>
עדיף לשלוח תמונות צבעוניות, עומק צבע של 32 ביט, סרוקות ב 300 נקודות לאינץ. 
קידודי תמונה נתמכים: jpeg, tiff, pnm, gif. גודל תמונה מקסימאלי 1.5 מגה ביט.
 
    </td>
  </tr>
</table> 
<p></p> 
<table class="info">
  <tr>
    <td colspan="2"  class="title"><strong>הגדרות תוכן</strong></td>
  </tr>
  <tr>
    <td>מחבר</td>
    <td><input type="text" name="author" value="לא ידוע" /></td>
  </tr>
  <tr>
    <td>כותרת</td>
    <td><input type="text" name="title" value="לא ידוע" /></td>
  </tr>
  <tr>
    <td>מספר דף</td>
    <td><input type="text" name="n_page" value="1" /></td>
  </tr>
</table> 
<p></p> 
<table class="form">
  <tr>
    <td colspan="2"><strong>הגדרות עיבוד תמונה</strong></td>
  </tr>
  <tr>
    <td>הגדלה</td>
    <td>
      <select name="scale" size="1">	
        <option value="0">אוטומאטית</option>
        <option value="1">ללא הגדלה</option>
        <option value="2">פי שתיים</option>
        <option value="4">פי ארבע</option>
      </select>
    </td>
  </tr>
  <tr>
  <td>שיטת זיהוי סף</td>
    <td>
      <select name="threshold_type" size="1">	
        <option value="0">אוטומאטית</option>
        <option value="1">סף קבוע</option>
        <option value="2">סף עדין</option>
      </select>
    </td>
  </tr>
  <tr>
    <td>רמת סף </td>
    <td><input type="text" name="threshold" value="0" /></td>
  </tr>
  <tr>
    <td> רמת סף עדין</td>
    <td><input type="text" name="a_threshold" value="0" /></td>
  </tr>
  <tr>
    <td></td>
    <td>0 - רמת סף אוטומאטית </td>
  </tr>
  <tr>
    <td></td>
    <td>סרגל רמות הסף מתחיל מ 1 (הכל לבן) וממשיך עד 100</td>
  </tr>
  <tr>
    <td>נסה להסיר תמונות</td>
    <td><input type="checkbox" name="image" value="1" /></td>
  </tr>
  <tr>
    <td>נסה להסיר נקודות</td>
    <td><input type="checkbox" name="halftone" value="1" /></td>
  </tr>
  <tr>
    <td colspan="2"><strong>הגדרות ניתוח עימוד</strong></td>
  </tr>
  <tr>
    <td>שיטת זיהוי עימוד</td>
    <td>
      <select name="blocks_setup" size="1">	
        <option value="0">עמודות</option>
        <option value="1">סידור חופשי</option>
        <option value="2">שתי עמודות</option>
        <option value="3">שלוש עמודות</option>
        <option value="4">ארבע עמודות</option>
      </select>
    </td>
  </tr>
  <tr>
    <td colspan="2"><strong>הגדרות פלט</strong></td>
  </tr>
  <tr>
    <td>תמונה שחור לבן</td>
    <td><input type="checkbox" name="bw" value="1" checked="checked" /></td>
  </tr>
  <tr>
    <td>תמונת ניתוח העימוד</td>
    <td><input type="checkbox" name="layout" value="1" checked="checked" /></td>
  </tr>
  <tr>
    <td></td>
    <td>תמונת ניתוח העימוד מיועדת להקל על הגהת הטקסט</td>
  </tr>
  <tr>
    <td>טקטסט</td>
    <td><input type="checkbox" name="text" value="1" checked="checked" /></td>
  </tr>
</table>

</form>

<p class="top"><a href="#hocr_web">ראשית</a></p> 
<h2><a name="sever">אודות השרת</a></h2>
<p><strong>מנוע השרת</strong></p>
<p>שרת רשת זה מפעיל את תכנת HOCR, תכנה לזיהוי אותיות אופטי.</p>
<ul>
  <li>אתר הבית של התכנה : <a href="http://hocr.berlios.de">http://hocr.berlios.de</a></li> 
  <li>כתובת להערות והארות אודות התכנה: <a href="mailto:kzamir_walla.co.il">kzamir_walla.co.il</a></li>
</ul>
<p><strong>מאפיינים טכניים</strong></p>
<ul>
  <li>מנח טקסט מותר: אופקי</li>
  <li>מזהה את חלקי הטקסט הבאים:
    <ul>
      <li>דף</li>
      <li>בלוק טקסט</li>
      <li>שורה</li>
      <li>מילה</li>
      <li>אות</li>
    </ul>
  </li>
  <li>סוגי אותיות: דפוס</li>
  <li>גודל תמונה מקסימאלי בפיקסלים:	5000*5000</li>
  <li>קידודי תמונה נתמכים: png, jpeg, tiff, gif</li>
  <li>קידודי טקסט נתמכים: utf-8 (עברית בלבד)</li>
</ul>
<p><strong>תכנות נוספות בהן נעשה שימוש</strong></p>
<ul>
  <li>ImageMagic: להמרת תסדירי הקבצים הגרפיים <a href="http://www.imagemagick.org">(אתר)</a></li>
  <li>hspell: בדיקת איות בשפה העברית <a href="http://ivrix.org.il/projects/spell-checker/">(אתר)</a></li>
  <li>php: שפה לצד השרת <a href="http://php.net">(אתר)</a></li>
</ul>
<p class="top"><a href="#hocr_web">ראשית</a></p> 
<h2><a name="usage">שימוש</a></h2>
<p>
מלאו את שדה הקובץ בטופס שבתחילת העמוד, לאחר מכן לחצו על כפתור 'שלח תמונה לזיהוי' והמתינו. 
לאחר שהתמונה תשלח, יופיע עמוד חדש, ובו הודעה לגבי הצלחה או כשלון של פעולת העלאת הקובץ. 
אם העלאת הקובץ הצליחה, תופעל תכנת HOCR על הקובץ שנשלח. הדף יתמלה בהודעות המערכת של התכנה. 
מילוי הדף יסתיים כאשר התכנה תסיים את פעולתה. קבצי הטקסט והתמונות המעובדות יופיעו בספריית הפלט.
</p>
<p>
כאשר התכנה תסיים לעבד את התמונה, יופיעו הקבצים הבאים בספריית הפלט:
</p>
<ul>
  <li>התמונה המקורית</li>
  <li>תמונה המכילה פרטים על ניתוח העימוד והאותיות שזוהו, התמונה מיועדת לעזור להגהת הטקסט שזוהה, שמורה כ "שם הקובץ"-L עם סיומת jpeg</li>
  <li>הטקסט שזוהה, שמור כ "שם הקובץ" עם סיומת txt</li>
</ul>
<p><b>עיבוד התמונה אורך זמן רב, היו סבלניים. אין לסגור את דף הדפדפן במהלך תהליך הזיהוי.</b></p>
<p class="top"><a href="#hocr_web">ראשית</a></p> 
<h2><a name="files">שימוש נאות</a></h2>
<p>
עיבוד תמונה צורך משאבי מערכת רבים.
בדוק את התרשומת בסוף העמוד כדי לוודא שהמערכת לא עובדת כרגע על תמונה אחרת. אם המערכת תנסה לזהות מספר תמונות במקביל היא עלולה להפסיק לפעול ללא אזהרה, עקב מחסור בזיכרון.
</p>
<p class="top"><a href="#hocr_web">ראשית</a></p> 
<h2><a name="log">תרשומת</a></h2>

<div class="code">
  <p><object data="<?php print $log_path; ?>" width="100%"></object></p>
</div>

<p class="top"><a href="#hocr_web">ראשית</a></p> 
