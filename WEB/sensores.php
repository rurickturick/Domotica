<?php
//Inicio del procesamiento
require_once 'domotica.php';

	$fecha = $_GET['fecha']; 
	$hora = $_GET['hora'];
        $info = $_GET['info'];
        $id_sensor = $_GET['id_sensor'];

	$domotica= new domotica();
        if ( $id_sensor == '1' || $id_sensor == '3' )
        {
           echo "Sensor Temperatura\n";
	   $domotica->add_temperatura($fecha,$hora,$info,$id_sensor);
        }
        else {
           echo  "Sensor humedad\n";
           $domotica->add_humedad($fecha,$hora,$info,$id_sensor);
          }
	$domotica->_domotica();
?>
