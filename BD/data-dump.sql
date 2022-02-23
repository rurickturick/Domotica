-- MariaDB dump 10.19  Distrib 10.5.12-MariaDB, for debian-linux-gnueabihf (armv7l)
--
-- Host: localhost    Database: domotica
-- ------------------------------------------------------
-- Server version	10.5.12-MariaDB-0+deb11u1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `humedad`
--

DROP TABLE IF EXISTS `humedad`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `humedad` (
  `fecha` date NOT NULL,
  `hora` time NOT NULL,
  `humedad` decimal(3,1) NOT NULL,
  `id_sensor` tinyint(3) NOT NULL,
  PRIMARY KEY (`fecha`,`hora`,`id_sensor`),
  KEY `FK_humedad_id_sensor` (`id_sensor`),
  CONSTRAINT `FK_humedad_id_sensor` FOREIGN KEY (`id_sensor`) REFERENCES `sensor` (`id_sensor`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;



--
-- Table structure for table `localizacion`
--

DROP TABLE IF EXISTS `localizacion`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `localizacion` (
  `id_localizacion` tinyint(3) NOT NULL,
  `desc_localizacion` char(50) DEFAULT NULL,
  PRIMARY KEY (`id_localizacion`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `localizacion`
--



--
-- Table structure for table `sensor`
--

DROP TABLE IF EXISTS `sensor`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sensor` (
  `id_sensor` tinyint(3) NOT NULL,
  `id_localizacion` tinyint(3) DEFAULT NULL,
  `id_tipo_sensor` tinyint(3) DEFAULT NULL,
  PRIMARY KEY (`id_sensor`),
  KEY `FK_sensores_tipo_sensor` (`id_tipo_sensor`),
  KEY `FK_sensor_localizacion` (`id_localizacion`),
  CONSTRAINT `FK_sensor_localizacion` FOREIGN KEY (`id_localizacion`) REFERENCES `localizacion` (`id_localizacion`) ON DELETE CASCADE,
  CONSTRAINT `FK_sensores_tipo_sensor` FOREIGN KEY (`id_tipo_sensor`) REFERENCES `tipo_sensor` (`id_tipo_sensor`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `sensor`
--


--
-- Table structure for table `temperatura`
--

DROP TABLE IF EXISTS `temperatura`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `temperatura` (
  `fecha` date NOT NULL,
  `hora` time NOT NULL,
  `temperatura` decimal(3,1) NOT NULL,
  `id_sensor` tinyint(3) NOT NULL,
  PRIMARY KEY (`fecha`,`hora`,`id_sensor`),
  KEY `FK_temperatura_id_sensor` (`id_sensor`),
  CONSTRAINT `FK_temperatura_id_sensor` FOREIGN KEY (`id_sensor`) REFERENCES `sensor` (`id_sensor`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;



--
-- Table structure for table `tipo_sensor`
--

DROP TABLE IF EXISTS `tipo_sensor`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tipo_sensor` (
  `id_tipo_sensor` tinyint(3) NOT NULL,
  `desc_tipo_sensor` char(50) DEFAULT NULL,
  PRIMARY KEY (`id_tipo_sensor`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tipo_sensor`
--

L
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-02-23 19:46:44
