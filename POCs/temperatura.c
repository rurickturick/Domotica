//#########################################
//# Programa que lee la temperatura y la
//# humedad mediante un sensor DHT11 en
//# una Raspberry
//#
//#Autor: Ángel Luis Ortiz Folgado
//#      10/12/2021
//########################################

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#define MAXTIMINGS  85
#define DHTPIN      5
int dht11_dat[5] = { 0, 0, 0, 0, 0 };

int read_dht11_dat()
{
    uint8_t laststate   = HIGH;
    uint8_t counter     = 0;
    uint8_t j       = 0, i;
    float   f; 

    dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

    pinMode( DHTPIN, OUTPUT );
    digitalWrite( DHTPIN, LOW );
    delay( 18 );
    digitalWrite( DHTPIN, HIGH );
    delayMicroseconds( 40 );
    pinMode( DHTPIN, INPUT );

    for ( i = 0; i < MAXTIMINGS; i++ )
    {
        counter = 0;
        while ( digitalRead( DHTPIN ) == laststate )
        {
            counter++;
            delayMicroseconds( 1 );
            if ( counter == 255 )
            {
                break;
            }
        }
        laststate = digitalRead( DHTPIN );

        if ( counter == 255 )
            break;

        if ( (i >= 4) && (i % 2 == 0) )
        {
            dht11_dat[j / 8] <<= 1;
            if ( counter > 16 )
                dht11_dat[j / 8] |= 1;
            j++;
        }
    }

    if ( (j >= 40) &&
         (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) )
    {
        printf( "Humidity = %d.%d %% Temperature = %d.%d C \n",
            dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3] );
        return 0;
    }else  {
        printf( "Data not good, skip\n" );
        return 1;
    }
}

int main( void )
{
    printf( "Raspberry Pi wiringPi DHT11 Temperature test program\n" );

    if ( wiringPiSetup() == -1 )
        exit( 1 );
     time_t now =time(NULL);
     struct tm *tm;
     char fechayhora[100];
     
    while ( 1 )
    {   
        now = time(NULL);
        tm=localtime(&now);
         strftime(fechayhora,100,"%d/%m/%Y %X ",tm);
         printf("%s",fechayhora);
        if( read_dht11_dat() == 0) 
           delay( 5000 );
        else delay(1000); 
    }

    return(0);
}
