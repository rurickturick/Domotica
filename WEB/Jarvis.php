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

       .off
       {
        width:32px;
        height:32px;
        padding:1px 2px 3px 3px;	
        font-size:12px;
        background:lightgray;
        text-align:center;	
        color:gray;
        }
      .onoff
       {
        width:32px;
        height:32px;
        padding:1px 2px 3px 3px;
        font-size:12px;
        background:lightgray;
        text-align:center;
        }

       .on
       {
        width:32px;
        height:32px;
        padding:1px 2px 3px 3px;
        font-size:12px;
        background:lightgray;
        text-align:center;
        color:orange;
        }


       .onoff div
       {
        width:18px;
        height:18px;
        min-height:18px;	
        background:lightgray;
        overflow:hidden;
        border-top:1px solid gray;
        border-right:1px solid white;
        border-bottom:1px solid white;
        border-left:1px solid gray;			
        margin:0 auto;
        color:gray;
        }

       .off div
       {
        width:18px;
        height:18px;
        min-height:18px;
        background:lightgray;
        overflow:hidden;
        border-top:1px solid gray;
        border-right:1px solid white;
        border-bottom:1px solid white;
        border-left:1px solid gray;
        margin:0 auto;
        color:gray;
        }

       .on div
       {
        width:18px;
        height:18px;
        min-height:18px;
        background:green;
        overflow:hidden;
        border-top:1px solid gray;
        border-right:1px solid white;
        border-bottom:1px solid white;
        border-left:1px solid gray;
        margin:0 auto;
        color:lightgray;
        }

   -->
   </style>
</HEAD>

<BODY bgcolor="#C0C0C0" link="green" vlink="green" alink="green">
<BASEFONT face="arial, helvetica">
<script type="text/javascript">
   var buttonstate_sal=<?php
     include 'domotica.php';
     $domotica= new domotica();
        $lista=$domotica->get_pantalla_salon();
          while ($row=mysqli_fetch_row($lista)){
           $pantalla=$row[0];
          };
         echo $pantalla;
         $domotica->_domotica();
          ?>;
   var buttonstate_hab=<?php 
     $domotica= new domotica();
        $lista=$domotica->get_pantalla_habitacion();
          while ($row=mysqli_fetch_row($lista)){
           $pantalla=$row[0];
          };
         echo $pantalla;  
         $domotica->_domotica();
          ?>;      

  function onoff_hab(element)
   {
     buttonstate_hab= 1 - buttonstate_hab;
     var blabel, bstyle, bcolor;
     if(buttonstate_hab)
     {
       blabel="on";
       bstyle="green";
       bcolor="lightgreen";
     }
     else
     {
       blabel="off";
       bstyle="lightgray";
       bcolor="gray";
     }
     var child=element.firstChild;
     child.style.background=bstyle;
     child.style.color=bcolor;
     child.innerHTML=blabel;
   }

 function onoff_sal(element)
   {
     buttonstate_sal= 1 - buttonstate_sal;
     var blabel, bstyle, bcolor;
     if(buttonstate_sal)
     {
       blabel="on";
       bstyle="green";
       bcolor="lightgreen";
     }
     else
     {
       blabel="off";
       bstyle="lightgray";
       bcolor="gray";
     }
     var child=element.firstChild;
     child.style.background=bstyle;
     child.style.color=bcolor;
     child.innerHTML=blabel;
   }


   function switch_hab()
   {
    const xhttp = new XMLHttpRequest();
   xhttp.open("GET", "switch_hab.php");
   xhttp.send();
   }

   function switch_sal()
   {
    const xhttp = new XMLHttpRequest();
   xhttp.open("GET", "switch_sal.php");
   xhttp.send();
   }

</script>

<TABLE border="0" align="center" cellspacing="3" cellpadding="3" width="1000">
<TR><TH colspan="2" width="100%" bgcolor="green"><FONT size="6" color="white">Sensores</FONT></TH>
</TR></TABLE><P>
<?php

	  $domotica= new domotica();
	  $lista=$domotica->get_temperatura_habitacion(); 
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
          $lista=$domotica->get_humedad_habitacion();
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
          <h5>Temperatura</h5>
          <table border='0' width='350'>
          <tr> <td bgcolor='green' width ='80'align='center'><font color='FFFFFF' bgcolor='green'> Fecha  </font></td>
          <td bgcolor='green' width='50'align='center'><font color='FFFFFF' bgcolor='green' >  Hora </font> </td>
          <td bgcolor='green' width='50'align='center'><font color='FFFFFF' bgcolor='green'> Temperatura</font> </td></tr>";
          $lista=$domotica->get_temperatura_salon(); 
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
          $lista=$domotica->get_humedad_salon();
          while ($row=mysqli_fetch_row($lista)){
           $fechah=date_create_from_format('Y-m-d',$row[0]);
           $fecha_formath=date_format($fechah,"d/m/Y");
           echo"
           <tr> <td align='center'> <b>$fecha_formath</b>  </td><td align='center'> <b> $row[1]</b> </td><td align='center'><b>$row[2] %</b> </td></tr>";
          };
          echo"</table>          
           "; 
          echo" </div>
          ";
          $lista=$domotica->get_pantalla_habitacion();
          while ($row=mysqli_fetch_row($lista)){
           $pantalla_hab=$row[0];
          };
          if($pantalla_hab == 0){
            $pinta_boton_hab='off';
           } 
          else{
            $pinta_boton_hab='on';
           }; 
          $lista=$domotica->get_pantalla_salon();
          while ($row=mysqli_fetch_row($lista)){
           $pantalla_sal=$row[0];
          };
          $domotica->_domotica();
          if($pantalla_sal == 0){
            $pinta_boton_sal='off';
           }
          else{
            $pinta_boton_sal='on';
           } 
          echo"<div>
          <h5>Pantalla Habitación</h5>
            <button class='$pinta_boton_hab' onclick='onoff_hab(this); switch_hab()'><div>$pinta_boton_hab</div></button>
          </div>";
           
          echo"<div>
          <h5>Pantalla Salón</h5>
            <button class='$pinta_boton_sal' onclick='onoff_sal(this); switch_sal()'><div>$pinta_boton_sal</div></button>
          </div>";
?>
</BODY>
</HTML>
