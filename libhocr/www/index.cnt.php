
<h2>העלאת  תמונות</h2>

<form action="procces.php" method="post" enctype="multipart/form-data">

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
  <tr>
     <td></td>
     <td>
עיבוד התמונה לוקח זמן רב, העזרו בסבלנות.
    </td>
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
    <td>נסה לחבר אותיות שבורות</td>
    <td><input type="checkbox" name="fix_broken" value="1" /></td>
  </tr>
  <tr>
    <td>נסה לשבור אותיות מחוברות</td>
    <td><input type="checkbox" name="fix_ligated" value="1" /></td>
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
