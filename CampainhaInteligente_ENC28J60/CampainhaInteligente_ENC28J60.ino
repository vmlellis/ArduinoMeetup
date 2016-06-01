#define BLYNK_PRINT Serial
#include <UIPEthernet.h>
#include <BlynkSimpleUIPEthernet.h>

// Define o pino da campainha (D2)
#define CAMPAINHA_PIN 2

// Tempo de estabilizacao
#define DEBOUNCE_DELAY 50

// Registro de um minuto para apagar o LCD
const unsigned long oneMinute = 60 * 1000;

// Leitura da campainha
int campainhaReading;

// Leitura atual do pino da campainha
int campainhaState;

// Leitura anterior do pino da campainha
int lastCampainhaState = HIGH;

// Ultima vez que o pino da campainha foi modificado
long lastDebounceCampainhaTime = 0;

// Você deve usar o "Auth Token" de seu Blynk.app aqui
// Vá em "Project Settings" (ícone porca de parafuso).
// char auth[] = "seu token entre aspas abaixo"
char auth[] = "seu token de autenticação do blynk";

// Configure seu e-mail abaixo entre as aspas
char emailDestinatario[] = "seu_email com arroba e tudo mais";
// Configure o assunto do e-mail que você quer receber
char emailAssunto[] = "Subject: Campainha de casa";
// Configure o conteúdo da mensagem do e-mail
char emailConteudo[] = "Alguém acaba de tocar a campainha!";

// LCD do Blynk (Setar no aplicativo como V1)
WidgetLCD lcd(V1);

// Controle para apagar o LCD do Blynk
int clearLCD = 0;

void setup()
{
  // Habilita o resistor de PULL UP interno (nao eh necessario colocar resistor no botao)
  pinMode(CAMPAINHA_PIN, INPUT_PULLUP);

  Serial.begin(9600);
  Blynk.begin(auth);

  while (!Blynk.connect())
}

void notifyLCD() {
  // Apaga o LCD
  lcd.clear();

  // use: (position X: 0-15, position Y: 0-1, "Message you want to print")
  lcd.print(4, 0, F("Atenda"));
  lcd.print(4, 1, F("a porta."));
}

void notifyPushNotification() {
  Blynk.notify(F("Alguém acaba de tocar a camapinha de CASA!"));
}

void notifyEmail() {
  Blynk.email(emailDestinatario , emailAssunto , emailConteudo);
}

void apagarLCD() {
  // Apaga o LCD
  lcd.clear();

  // Seta a FLAG informando que o LCD jah foi apagado
  clearLCD = 0;
}

void notify() {
  Serial.println("Button is pressed.");

  // Recebe a notificação do LCD do Blynk (Comente para não receber)
  notifyLCD();

  // Receber uma notificação no celular (Comente para não receber)
  notifyPushNotification();

  // Receber um email (Comente para não receber)
  notifyEmail();

  // Informa que pode apagar o LCD
  clearLCD = 1;
}


void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();

  campainhaReading = digitalRead(CAMPAINHA_PIN);

  // Se o estado do botão mudou seja por pressionamento ou ruido
  if (campainhaReading != lastButtonState) {
    // Reseta o temporizador de debounce
    lastDebounceCampainhaTime = millis();
  }

  // Quando pressionado por mais tempoo que o tempo de debounce
  if ((millis() - lastDebounceCampainhaTime) > DEBOUNCE_DELAY) {
    if (campainhaReading != campainhaState) {
      campainhaState = campainhaReading;

      // Realiza a notificação
      notify();
    }
  }

  if ((millis() - lastDebounceCampainhaTime) > oneMinute) {
    if (clearLCD) {
      apagarLCD();
    }
  }

  // Salva a leitura do ultimo estado do botao da campainha
  lastCampainhaState = campainhaReading;
}
