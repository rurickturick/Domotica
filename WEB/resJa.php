<?php
include 'domotica.php';
$accion = $_GET['accion'];
$domotica = new domotica();
switch($accion) {
 case 0: 
   $lista = $domotica->get_temperatura_habitacion();
   break;
 case 1:
   $lista = $domotica->get_humedad_habitacion();
   break;
 case 2:
   $lista = $domotica->get_temperatura_salon();
   break;
 case 3:
   $lista = $domotica->get_humedad_salon();
   break; 
 case 4:
   $lista = $domotica->get_pantalla_habitacion();
   break; 
 case 5:
   $lista = $domotica->get_pantalla_salon();
   break;
}

if($accion < 4){
 while ($row=mysqli_fetch_row($lista)){
  $fechat=date_create_from_format('Y-m-d',$row[0]);
  $fecha_formatt=date_format($fechat,"d/m/Y");
  $response=$fecha_formatt . "," . $row[1] . "," . $row[2]  . ",";
 }
}
else{
 while ($row=mysqli_fetch_row($lista)){
  $response=$row[0] . ","; 
 }
}
$domotica->_domotica();
header('Content-Type: text/html; charset=UTF-8');
echo"BODY:,$response";
?>
