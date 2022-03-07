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
	function get_temperatura ()
	{

		$query = "select t1.fecha,t1.hora,t1.temperatura,t3.desc_localizacion,t4.desc_tipo_sensor from temperatura t1,sensor t2, localizacion t3, tipo_sensor t4 
                         where t1.id_sensor =t2.id_sensor and t2.id_localizacion = t3.id_localizacion and t2.id_tipo_sensor = t4.id_tipo_sensor order by 1 desc, 2 desc limit 1;";

		$this-> datos = mysqli_query ($this->id_conexion, $query);
         

        return $this->datos;
       }
       
       // selecciona el valor más reciente de humedad
	function get_humedad ()
	{

		$query = "select t1.fecha,t1.hora,t1.humedad,t3.desc_localizacion,t4.desc_tipo_sensor from humedad t1,sensor t2, localizacion t3, tipo_sensor t4
                           where t1.id_sensor =t2.id_sensor and t2.id_localizacion = t3.id_localizacion and t2.id_tipo_sensor = t4.id_tipo_sensor order by 1 desc, 2 desc limit 1;";

		$this-> datos = mysqli_query ($this->id_conexion, $query);
         

        return $this->datos;
       }
}
