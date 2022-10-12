<?php header('Content-type: text/html; charset=utf-8'); ?>
<?php
class domotica {
  	private $datos;
  	private $id_conexion;

	//Constructor
  	function domotica ()
  	{
  	  //Datos propios de la base de datos	  
	  $DBHost=" ";
	  $DBUser=" ";
	  $DBPass=" ";
	  $DB=" ";

  	  /* Intentamos establecer una conexión persistente con el servidor.*/
  	  $this->id_conexion = mysqli_connect($DBHost, $DBUser, $DBPass, $DB) or
  	  		die("<CENTER><H3>No se ha podido establecer la conexión.
  	  			<P>Compruebe si está activado el servidor de bases de datos MySQL.</H3></CENTER>");

	}

	//Destructor
	function _domotica ()
	{
	  /* Liberamos la conexión persistente con el servidor.*/
	  if (isset($this->datos)) mysqli_free_result($this->datos);
	  if (isset($this->id_conexion)) mysqli_close($this->id_conexion);
	}

	// Añade uns temperatura
	function add_temperatura ($fecha, $hora, $temperatura, $id_sensor)
	{
       //Ejecuta un insert sobre la tabla temperatura
	   $query = "INSERT INTO temperatura (`fecha`, `hora`, `temperatura`, `id_sensor`) VALUES (str_to_date('$fecha','%d/%m/%Y'), str_to_date('$hora','%H:%i:%s'),'$temperatura', '$id_sensor')";
	   $consulta = mysqli_query ($this->id_conexion, $query);
         if($consulta)
                    echo "Exito se ha insertado correctamente Temperatura\n";

         else
                   echo "fail insert Temperatura\n";

          return;
       	}

	// Ejecuta un insert sobre la tabla humedad
	function add_humedad ($fecha, $hora, $humedad, $id_sensor)
	{

		$query = "INSERT INTO humedad (`fecha`, `hora`, `humedad`, `id_sensor`) VALUES (str_to_date('$fecha','%d/%m/%Y'), str_to_date('$hora','%H:%i:%s'), '$humedad', '$id_sensor')";

		$consulta = mysqli_query ($this->id_conexion, $query);
         if($consulta)
                     echo "Exito se ha insertado correctamente Humedad\n";


                else
                        echo "Fail insert Humedad\n";

        return;
       }

 // selecciona el valor más reciente de temperatura
	function get_temperatura_salon ()
	{

		$query = "select t1.fecha,t1.hora,t1.temperatura,t3.desc_localizacion,t4.desc_tipo_sensor from temperatura t1,sensor t2, localizacion t3, tipo_sensor t4 
                         where t1.id_sensor =t2.id_sensor and t2.id_localizacion = t3.id_localizacion and t2.id_tipo_sensor = t4.id_tipo_sensor and t3.desc_localizacion ='Salón' order by 1 desc, 2 desc limit 1;";

		$this-> datos = mysqli_query ($this->id_conexion, $query);
         

        return $this->datos;
       }
       
       function get_temperatura_habitacion ()
        {

                $query = "select t1.fecha,t1.hora,t1.temperatura,t3.desc_localizacion,t4.desc_tipo_sensor from temperatura t1,sensor t2, localizacion t3, tipo_sensor t4
                         where t1.id_sensor =t2.id_sensor and t2.id_localizacion = t3.id_localizacion and t2.id_tipo_sensor = t4.id_tipo_sensor and t3.desc_localizacion ='Habitación' order by 1 desc, 2 desc limit 1;";

                $this-> datos = mysqli_query ($this->id_conexion, $query);


        return $this->datos;
       } 
       // selecciona el valor más reciente de humedad
	function get_humedad_salon ()
	{

		$query = "select t1.fecha,t1.hora,t1.humedad,t3.desc_localizacion,t4.desc_tipo_sensor from humedad t1,sensor t2, localizacion t3, tipo_sensor t4
                           where t1.id_sensor =t2.id_sensor and t2.id_localizacion = t3.id_localizacion and t2.id_tipo_sensor = t4.id_tipo_sensor and t3.desc_localizacion ='Salón'  order by 1 desc, 2 desc limit 1;";

		$this-> datos = mysqli_query ($this->id_conexion, $query);
         

        return $this->datos;
        }
        function get_humedad_habitacion ()
        {

                $query = "select t1.fecha,t1.hora,t1.humedad,t3.desc_localizacion,t4.desc_tipo_sensor from humedad t1,sensor t2, localizacion t3, tipo_sensor t4
                           where t1.id_sensor =t2.id_sensor and t2.id_localizacion = t3.id_localizacion and t2.id_tipo_sensor = t4.id_tipo_sensor and t3.desc_localizacion ='Habitación' order by 1 desc, 2 desc limit 1;";

                $this-> datos = mysqli_query ($this->id_conexion, $query);


        return $this->datos;
       }
      // Enciende pantallas
      function set_on_pantalla_habitacion ()
        {

                $query = "update domotica_parametro set valor_parametro = 1 
                           where cod_parametro='SWITCH_PANTALLA_HAB';";

                $consulta-> datos = mysqli_query ($this->id_conexion, $query);

                if($consulta)
                     echo "Exito se ha encendido la pantalla de la habitacion\n";


                else
                     echo "Fail encendido pantalla habitacion\n";

        return;
       }

        // apaga pantalla
        function set_off_pantalla_habitacion ()
        {

                $query = "update domotica_parametro set valor_parametro = 0 
                           where cod_parametro='SWITCH_PANTALLA_HAB';";

                $consulta-> datos = mysqli_query ($this->id_conexion, $query);

                if($consulta)
                     echo "Exito se ha apagado la pantalla de la habitacion\n";


                else
                     echo "Fail encendido pantalla habitacion\n";

        return;
       }

       //Recupera el estado de la pantalla
        function get_pantalla_habitacion ()
        {

                $query = "select valor_parametro from domotica_parametro
                           where cod_parametro='SWITCH_PANTALLA_HAB';";

                $this-> datos = mysqli_query ($this->id_conexion, $query);


        return $this->datos;
       }
	//Recupera hora de encendido de la pantalla
        function get_inicio_pantalla_habitacion ()
        {

                $query = "select valor_parametro from domotica_parametro
                           where cod_parametro='HORA_MIN_PANT_HAB';";

                $this-> datos = mysqli_query ($this->id_conexion, $query);


        return $this->datos;
       }
         // recupera la hora de apagado de la pantalla
        function get_fin_pantalla_habitacion ()
        {

                $query = "select valor_parametro from domotica_parametro
                           where cod_parametro='HORA_MAX_PANT_HAB';";

                $this-> datos = mysqli_query ($this->id_conexion, $query);


        return $this->datos;
       }
         // Recupera la hora de encendido de la pantalla
	function set_inicio_pantalla_habitacion ($hora)
	{

	   $query = "update domotica_parametro set valor_parametro = $hora 
                           where cod_parametro='HORA_MIN_PANT_HAB';";
	   $consulta = mysqli_query ($this->id_conexion, $query);
         if($consulta)
                    echo "Exito se ha actualizado correctamente hora min pantalla habitacion\n";

         else
                   echo "fail update hora min pantalla habitacion\n";

          return;
       	}
	//Cambia la hora de apagado de la pantalla
	function set_fin_pantalla_habitacion ($hora)
	{
       //Ejecuta un insert sobre la tabla temperatura
	   $query = "update domotica_parametro set valor_parametro = $hora 
                           where cod_parametro='HORA_MAX_PANT_HAB';";
	   $consulta = mysqli_query ($this->id_conexion, $query);
         if($consulta)
                    echo "Exito se ha actualizado correctamente hora max pantalla habitacion\n";

         else
                   echo "fail update hora max pantalla habitacion\n";

          return;
       	}
        // Cambia la hora de encendido de la pantalla
      function set_on_pantalla_salon ()
        {

                $query = "update domotica_parametro set valor_parametro = 1 
                           where cod_parametro='SWITCH_PANTALLA_SAL';";

                $consulta-> datos = mysqli_query ($this->id_conexion, $query);

                if($consulta)
                     echo "Exito se ha encendido la pantalla del salon\n";


                else
                     echo "Fail encendido pantalla salon\n";

        return;
       }

         // apaga pantalla
        function set_off_pantalla_salon ()
        {

                $query = "update domotica_parametro set valor_parametro = 0 
                           where cod_parametro='SWITCH_PANTALLA_SAL';";

                $consulta-> datos = mysqli_query ($this->id_conexion, $query);

                if($consulta)
                     echo "Exito se ha apagado la pantalla del salon \n";


                else
                     echo "Fail encendido pantalla salon\n";

        return;
       }

       //Recupera el estado de la pantalla
        function get_pantalla_salon ()
        {

                $query = "select valor_parametro from domotica_parametro
                           where cod_parametro='SWITCH_PANTALLA_SAL';";

                $this-> datos = mysqli_query ($this->id_conexion, $query);


        return $this->datos;
       }
        //Recupera hora de encendido de la pantalla
        function get_inicio_pantalla_salon ()
        {

                $query = "select valor_parametro from domotica_parametro
                           where cod_parametro='HORA_MIN_PANT_SAL';";

                $this-> datos = mysqli_query ($this->id_conexion, $query);


        return $this->datos;
       }
         // Recupera hora de apagado de la pantalla
        function get_fin_pantalla_salon ()
        {

                $query = "select valor_parametro from domotica_parametro
                           where cod_parametro='HORA_MAX_PANT_SAL';";

                $this-> datos = mysqli_query ($this->id_conexion, $query);


        return $this->datos;
       }
        	//Cambia la hora de encendido de la pantalla
	function set_inicio_pantalla_salon ($hora)
	{
	   $query = "update domotica_parametro set valor_parametro = $hora 
                           where cod_parametro='HORA_MIN_PANT_SAL';";
	   $consulta = mysqli_query ($this->id_conexion, $query);
         if($consulta)
                    echo "Exito se ha actualizado correctamente hora min pantalla salon\n";

         else
                   echo "fail update hora min pantalla salon\n";

          return;
       	}
         //Cambia la hora de apagado de la pantalla
	function set_fin_pantalla_salon ($hora)
	{
	   $query = "update domotica_parametro set valor_parametro = $hora 
                           where cod_parametro='HORA_MAX_PANT_SAL';";
	   $consulta = mysqli_query ($this->id_conexion, $query);
         if($consulta)
                    echo "Exito se ha actualizado correctamente hora max pantalla salon\n";

         else
                   echo "fail update hora max pantalla salon\n";

          return;
       	} 
}
?>
