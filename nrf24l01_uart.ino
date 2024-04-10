#include <SPI.h>
#include <RF24.h>

#define CE_PIN   9
#define CSN_PIN 10
#define LED_PIN 2
double timer1=0;

RF24 radio(CE_PIN, CSN_PIN);

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(0xABCDABCD02LL); // Адрес трубы для отправки
  radio.openReadingPipe(1, 0xABCDABCD01LL); // Адрес трубы для приема
  timer1=millis();
  Serial.print("Start");
}
void loop() {
  if (Serial.available()) {
  //if (0) {
    // Если есть данные в последовательном порту, отправляем их
    String message = Serial.readString();
    if (!radio.isChipConnected()) {
      // Если нет соединения с модулем, не отправляем данные
      Serial.println("No connection with radio module.");
    } else if (radio.isAckPayloadAvailable()) {
      // Если доступен ответ от удаленного устройства, не отправляем данные
      Serial.println("Remote device is busy.");
    } else {
      // Отправка данных
      radio.stopListening();
      radio.write(message.c_str(), message.length() + 1);
      radio.startListening();
    }
  }
  
  if (radio.available()) {
  //if (0) {
    // Если есть данные для чтения
    char receivedMessage[32] = "";
    radio.read(&receivedMessage, sizeof(receivedMessage));
    // Обработка принятых данных
    Serial.println("Received message: " + String(receivedMessage));
    blink(500); // Моргаем светодиодом раз в полсекунды при обмене информацией
  } else {
    // Моргаем светодиодом раз в секунду при наличии соединения
    blink(1000); 
  }
}

void blink(int duration){
  if(millis() - timer1 >= duration){
    digitalWrite(LED_PIN,!digitalRead(LED_PIN));
    timer1=millis();
  }
}