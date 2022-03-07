<?php

//Inicio del procesamiento
session_start();

?>

<?php header('Content-type: text/html; charset=utf-8'); ?>
<HTML>
<HEAD><TITLE>Jarvis</TITLE>
   <STYLE  TYPE="text/css">
   <!--
	input
	{
	  font-family : Arial, Helvetica;
	  font-size : 14;
	  color : #000033;
	  font-weight : normal;
	  border-color : #999999;
	  border-width : 1;
	  background-color : #FFFFFF;
        }
       
       .center {
        margin-left: auto;
        margin-right: auto; 
       }
      
       #col1 {
       float: left;
       width: 500;
       margin-right: 50px;
       }

       #col2{
       float: left;
       width: 500; 
       }

   -->
   </style>
</HEAD>

<BODY bgcolor="#C0C0C0" link="green" vlink="green" alink="green">
<BASEFONT face="arial, helvetica">

<TABLE border="0" align="center" cellspacing="3" cellpadding="3" width="1000">
<TR><TH colspan="2" width="100%" bgcolor="green"><FONT size="6" color="white">Sensores</FONT></TH>
</TR></TABLE><P>
<?php
include 'domotica.php';

	  $domotica= new domotica();
	  $lista=$domotica->get_temperatura(); 
          echo"
	  <div id='col1'>
          <h2>Habitación</h2>
          <h5>Temperatura</h5> 
          <table border='0' width='350'>
	  <tr> <td bgcolor='green' width ='80'align='center'><font color='FFFFFF' bgcolor='green'> Fecha  </font></td>
	  <td bgcolor='green' width='50'align='center'><font color='FFFFFF' bgcolor='green' >  Hora </font> </td>
	  <td bgcolor='green' width='50'align='center'><font color='FFFFFF' bgcolor='green'> Temperatura</font> </td></tr>";
          while ($row=mysqli_fetch_row($lista)){
           $fechat=date_create_from_format('Y-m-d',$row[0]);
           $fecha_formatt=date_format($fechat,"d/m/Y");  
           echo"
	   <tr> <td align='center' > <b>$fecha_formatt</b>  </td><td align='center'> <b> $row[1]</b> </td><td align='center'><b>$row[2] ºC</b> </td></tr>";
	  };
	  echo"</table>
          <h5>Humedad</h5>
          <table border='0' width='350'>
          <tr> <td bgcolor='green' width ='50'align='center'><font color='FFFFFF' bgcolor='green'> Fecha  </font></td>
          <td bgcolor='green' width='50'align='center'><font color='FFFFFF' bgcolor='green' >  Hora </font> </td>
          <td bgcolor='green' width='50'align='center'><font color='FFFFFF' bgcolor='green'>Humedad </font> </td></tr>";
          $lista=$domotica->get_humedad();
          while ($row=mysqli_fetch_row($lista)){
           $fechah=date_create_from_format('Y-m-d',$row[0]);  
           $fecha_formath=date_format($fechah,"d/m/Y"); 
           echo"
           <tr> <td align='center'> <b>$fecha_formath</b>  </td><td align='center'> <b> $row[1]</b> </td><td align='center'><b>$row[2] %</b> </td></tr>";
          };
          echo"</table> 
          </div>
	  "; 
          echo"<div id='col2'>
          <h2>Salón</h2>
          "; 
          echo" </div>
          ";
          $domotica->_domotica();
?>
</BODY>
</HTML>
