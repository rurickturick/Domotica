<?php
//Inicio del procesamiento
require_once 'domotica.php';
	$domotica= new domotica();
        $lista=$domotica->get_pantalla_habitacion ();
          while ($row=mysqli_fetch_row($lista)){
           $pantalla=$row[0];
          };      
          if ( $pantalla == '1'){
            $domotica->set_off_pantalla_habitacion ();
           }
          else{
            $domotica->set_on_pantalla_habitacion ();
           } 
	$domotica->_domotica();
?>
