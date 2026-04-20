#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Configuración de pines y sensores
const int sensorPin = 2;  // Pin del sensor de temperatura
const int outputPin = 3;  // Pin de salida digital para la señal

OneWire ourWire(sensorPin);
DallasTemperature sensors(&ourWire);

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Dirección I2C de la pantalla LCD y tamaño (16x2)

// Modo de operación
int Modo = 2;  // 1: Modo manual (lazo abierto), 2: Modo automático (lazo cerrado)

// Parámetros para análisis a lazo abierto
float Potencia_1 = 0;     // Valor inicial del cambio escalón (min = 0)
float Potencia_2 = 100;   // Valor final del cambio escalón (max = 100)

// Parámetros para análisis a lazo cerrado
float Setpoint = 38.5;       // Celsius

// Rango de control
const float MinTemp = 37.5;
const float MaxTemp = 39.5;

// Constantes de PID
float Kp = 5.5;   // constante proporcional
float Ki = 0.6;  // constante integral
//Kd = 0, constante derivativa en cero porque los cambios son muy lentos 

// Variables de tiempo
int Tiempo0 = 0;     // Retardo (en milisegundos) para ejecutar cambio escalón cuando se encuentra 

float Potencia = 0;  // Potencia inicial 
int valor = 0;
int detectado = 0;
int last_CH1_state = 0;

// Variables de tiempo y lectura
unsigned long Tiempo_previo = 0;
unsigned long Tiempo_actual = 0;
int Read_Delay = 1000;     // Periodo de muestreo en milisegundos
float Temperatura = 0;     // Celsius
float sp = 0;    

// Variables para PID
float PID_error = 0;
float previous_error = 0;
float PID_value = 0;
float Error_INT = 0;

void setup() {
  Serial.begin(9600);
  sensors.begin();
  pinMode(outputPin, OUTPUT);
  pinMode(pin_cruce_cero, INPUT);
  PCICR |= (1 << PCIE0);
  PCMSK0 |= (1 << PCINT0);

  lcd.init();
  lcd.backlight();
  lcd.print("Iniciando...");
  delay(2000);  // Esperar un poco para que el mensaje se pueda leer
}

void loop() {
  Tiempo_actual = millis(); // Tiempo Actual    

  valor = map(Potencia, 0, 100, 7600, 10);
  if (detectado) {
    delayMicroseconds(valor);
    digitalWrite(outputPin, HIGH);
    delayMicroseconds(100);
    digitalWrite(outputPin, LOW);
    detectado = 0;
  }

  if (Tiempo_actual - Tiempo_previo >= Read_Delay) {
    Tiempo_previo += Read_Delay;

    sensors.requestTemperatures();
    Temperatura = sensors.getTempCByIndex(0);

    if (Modo == 1) {
      // Modo manual (lazo abierto)
      if (Tiempo_actual <= Tiempo0) {
        Potencia = Potencia_1;
      } else if (Tiempo_actual >= Tiempo0) {
        Potencia = Potencia_2;
      }
    } else if (Modo == 2) {
      // Modo automático (lazo cerrado)
      if (Tiempo_actual >= Tiempo0) {
        PID_error = Setpoint - Temperatura; // Calculo del error    
        Error_INT = Error_INT + PID_error * (1000.0 / Read_Delay); // Calculo de la integral del error
        PID_value = Kp * (PID_error + (1.0 / Ki) * Error_INT); // Calculo de la salida del controlador PI

        // Limite de salida del controlador
        if (PID_value < 0) {
          PID_value = 0;
        }
        if (PID_value > 100) {
          PID_value = 100;
        }

        Potencia = PID_value; // Asignación a la entrada de la planta.
      }
    }

    // Control ON-OFF del calefactor
    if (Potencia > 50) {
      digitalWrite(outputPin, LOW); // Encender calefactor
    } else {
      digitalWrite(outputPin, HIGH);  // Apagar calefactor
    }

    // Asegurar que la temperatura esté dentro del rango deseado
    if (Temperatura < MinTemp) {
      Potencia = 100; // Calefacción máxima
    } else if (Temperatura > MaxTemp) {
      Potencia = 0; // Apagar calefacción
    }

    /*Serial.print("Potencia: ");
    Serial.print(Potencia);
    Serial.print(" %Temp: ");*/
    Serial.println(Temperatura);/*
    Serial.print(" C, Setpoint: ");
    Serial.println(Setpoint);*/

    // Actualizar la pantalla LCD con la temperatura y la potencia
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(Temperatura);
    lcd.print(" C");

    lcd.setCursor(0, 1);
    lcd.print("Pot: ");
    lcd.print(Potencia);
    lcd.print(" %");
  }
}

// Rutina de interrupción
ISR(PCINT0_vect) {
  if (PINB & B00000001) { 
    if (last_CH1_state == 0) { // Si el último estado fue 0, entonces tenemos un cambio de estado...
      detectado = 1; // Hemos detectado un cambio de estado
      last_CH1_state = 1;
    }
  } else if (last_CH1_state == 1) { 
    detectado = 1; // Hemos detectado un cambio de estado
    last_CH1_state = 0; // Almacenar el estado actual en el último estado para el siguiente ciclo
  }
}
