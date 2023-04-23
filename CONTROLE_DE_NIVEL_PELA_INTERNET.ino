#include <Ultrasonic.h>

#define BOMBAENTRADA 5 //Define BOMBA DE ENTRADA como 5
#define BOMBASAIDA 6 //Define BOMBA DE SAIDA como 6

#include <SPI.h> //Inclui a biblioteca SPI.h
#include <Ethernet.h> //Inclui a biblioteca Ethernet.h
#define echoPin 9 //Pino 13 recebe o pulso do echo
#define trigPin 8 //Pino 12 envia o pulso para gerar o echo
//iniciando a função e passando os pinos
Ultrasonic ultrasonic(8,9);
// Configurações para o Ethernet Shield
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x83, 0xEA }; // Entre com o valor do MAC

IPAddress ip(192,168,1,2); // Configure um IP válido 
byte gateway[] = { 192 , 168, 1, 1 }; //Entre com o IP do Computador onde a Câmera esta instalada
byte subnet[] = { 255, 255, 255, 0 }; //Entre com a Máskara de Subrede
EthernetServer server(80); //Inicializa a biblioteca EthernetServer com os valores de IP acima citados e configura a porta de acesso(80)

//======================================================================================

byte COD = B10000;
int niveldesejado = 30; // variavel que vai oegar o nivel desejado
float P=0,I=0,D=0; // variaveis do PID
float KP=1.0,KI=1.0,KD=1.0; // Constantes de intensidade do PID
float erro=0; // variavel do erro
float PID=0; // Variavel do PID
float nivel=0; // Variavel do nivel
long lastprocess=0; // Variavel para pegar o tempo do ultimo processo
float PIDN = 0; // variavel para inverter a variavel PID quando ela for negativa, antes de mandar par ao pwm
float distancia=0;
float niveldigitado=0; //variavel para armazenar nivel digitado
float nivelmaximo=100; //seta o nivel maximo
float nivelminimo=30; //seta o nivel minimo
float alturareservatorio=30; //seta a altura do reservatorio em cm
float distsensor=11; //seta a distancia do sensor do ponto maximo do reservatorio em cm

void setup()
{

  pinMode(BOMBASAIDA,OUTPUT); //Define o pino 5 como saída
  pinMode(BOMBAENTRADA,OUTPUT); //Define o pino 6 como saída
  Ethernet.begin(mac, ip);// Inicializa o Server com o IP e Mac atribuido acima
  Serial.begin(9600); //inicia a porta serial
   pinMode(echoPin, INPUT); // define o pino 13 como entrada (recebe)
   pinMode(trigPin, OUTPUT); // define o pino 12 como saida (envia)
}
void medida(){
  //seta o pino 12 com um pulso baixo "LOW" ou desligado ou ainda 0
    digitalWrite(trigPin, LOW);
  // delay de 2 microssegundos
    delayMicroseconds(2);
  //seta o pino 12 com pulso alto "HIGH" ou ligado ou ainda 1
    digitalWrite(trigPin, HIGH);
  //delay de 10 microssegundos
    delayMicroseconds(10);
  //seta o pino 12 com pulso baixo novamente
    digitalWrite(trigPin, LOW);
  // função Ranging, faz a conversão do tempo de
  //resposta do echo em centimetros, e armazena
  //na variavel distancia
  distancia = (ultrasonic.Ranging(CM));
}
void loop()
{
  medida();
    float lastnivel = nivel; //ultimo nivel medido
    float niveltotal = alturareservatorio - distsensor;
    if (distancia>=distsensor){
      nivel=((niveltotal-(distancia-distsensor))/niveltotal)*100;
    }
    else
   {
    analogWrite(BOMBASAIDA,0);//liga a bomba de SAIDA
    digitalWrite(BOMBAENTRADA,HIGH);//desliga a bomba de ENTRADA //
    nivel=((niveltotal-(distancia-distsensor))/niveltotal)*100;
   }
    float nivelmulti=niveldigitado*10;
    
  EthernetClient client = server.available();// Verifica se tem alguém conectado

  if (client)
  {

    boolean currentLineIsBlank = true; // A requisição HTTP termina com uma linha em branco Indica o fim da linha
    String valPag;

    while (client.connected())
    {

      if (client.available())
      {

        char c = client.read(); //Variável para armazenar os caracteres que forem recebidos
        valPag.concat(c); // Pega os valor após o IP do navegador ex: 192.168.1.2/0001        

        //Compara o que foi recebido
        if(valPag.endsWith("0001")) //Se o que for pego após o IP for igual a 0001
        {
          if (niveldigitado<10){
         if (niveldigitado==0){
          niveldigitado=1;
         }
         else {
          niveldigitado=nivelmulti+1;
         }
          }
        }
        else if(valPag.endsWith("0010")) //Senão se o que for pego após o IP for igual a 0010
        {
          if (niveldigitado<10){
         if (niveldigitado==0){
          niveldigitado=2;
         }
         else {
          niveldigitado=nivelmulti+2;
         }
          }
        }
        else if(valPag.endsWith("0011")) //Senão se o que for pego após o IP for igual a 0011
        {
          if (niveldigitado<10){
         if (niveldigitado==0){
          niveldigitado=3;
         }
         else {
          niveldigitado=nivelmulti+3;
         }
          }
        }
        else if(valPag.endsWith("0100")) //Senão se o que for pego após o IP for igual a 0100
        {
          if (niveldigitado<10){
         if (niveldigitado==0){
          niveldigitado=4;
         }
         else {
          niveldigitado=nivelmulti+4;
         }
          }
        }
        else if(valPag.endsWith("0101")) //Senão se o que for pego após o IP for igual a 0101
        {
         if (niveldigitado<10){
         if (niveldigitado==0){
          niveldigitado=5;
         }
         else {
          niveldigitado=nivelmulti+5;
         }
         }
        }
        else if(valPag.endsWith("0110")) //Senão se o que for pego após o IP for igual a 0110
        {
          if (niveldigitado<10){
         if (niveldigitado==0){
          niveldigitado=6;
         }
         else {
          niveldigitado=nivelmulti+6;
         }
          }
        }
        else if(valPag.endsWith("0111")) //Senão se o que for pego após o IP for igual a 0111
        {
          if (niveldigitado<10){
         if (niveldigitado==0){
          niveldigitado=7;
         }
         else {
          niveldigitado=nivelmulti+7;
         }
          }
        }
        else if(valPag.endsWith("1000")) //Senão se o que for pego após o IP for igual a 1000
        {
          if (niveldigitado<10){
         if (niveldigitado==0){
          niveldigitado=8;
         }
         else {
          niveldigitado=nivelmulti+8;
         }
          }
        }
        else if(valPag.endsWith("1001")) //Senão se o que for pego após o IP for igual a 1001
        {
         if (niveldigitado<10){
         if (niveldigitado==0){
          niveldigitado=9;
         }
         else {
          niveldigitado=nivelmulti+9;
         }
         }
        }
        else if(valPag.endsWith("1010")) //Senão se o que for pego após o IP for igual a 1010
        {
           if (niveldigitado<=10){
         if (niveldigitado==0){
          niveldigitado=0;
         }
         else {
          niveldigitado=nivelmulti;
         }
           }
        }
       else if(valPag.endsWith("1011")) //Senão se o que for pego após o IP for igual a 1011
        {
          niveldigitado=0;
        }
        else if(valPag.endsWith("1100")) //Senão se o que for pego após o IP for igual a 1100
        {
         if (niveldigitado>=nivelminimo){
         if (niveldigitado<=nivelmaximo){
          niveldesejado=niveldigitado;
          niveldigitado=0;
         }
         else {
          niveldigitado=niveldigitado;
         }
        }
                else {
          niveldigitado=niveldigitado;
        }
        }
        //=========================================================================================================================


        if (c == '\n' && currentLineIsBlank)
        {
          //Inicia página HTML
          client.println("HTTP/1.1 200 OK\n");
          client.println("<!DOCTYPE HTML>");
          client.println("<html lang=\"pt-br\">");
          client.println("<head>");
          client.println("<meta charset=\"utf-8\"/>");
          client.println("<meta content=\"width=device-width, initial-scale=1, maximum-scale=1\" name=\"viewport\">");
          client.println("<meta http-equiv=\"Content-Type\" content=\"text/html\" />");
          client.print("</head>");
          client.println(); 
          client.println("<body>");
          client.print(F(
           "<title>Projeto de Controle</title>\n"
          "<table width=\"100%\" align=\"center\" id=\"teste\"><font size=\"3%\"><center>Controle de Nivel</center></font> \n"
          "<font size=\"5%\"><center>Nivel atual: <font color=\"#ff0000\">\n"
          ));
          //=========================================================================================================================  
          //Display do nivel atual
            client.print(nivel);
            client.print(F(
              "%</center></font></font>"
              "<font size=\"5%\"><center>Manter em: <font color=\"#ffa200\">\n"
              ));
          //=========================================================================================================================
           //Display do nivel desejado
            client.print(niveldesejado);
           client.print(F(
            "%</center></font></font>\n"
            "<b><font size=\"5%\"><center><mark>Digite o nivel: <font color=\"#159f06\">\n"
            ));
          //Display do nivel digitado
            client.print(niveldigitado);
          //=========================================================================================================================
                client.print(F(
                  "%</b></mark></center></font></font>\n"
"<body><p align=\"center\"><button onclick=\"window.location.href='http://192.168.1.2/0001'\"><FONT size=\"7%\">1</FONT></button>\n"
"<button onclick=\"window.location.href='http://192.168.1.2/0010'\"><FONT size=\"7%\">2</FONT></button>\n"
"<button onclick=\"window.location.href='http://192.168.1.2/0011'\"><FONT size=\"7%\">3</FONT></button>\n"
  "<button onclick=\"window.location.href='http://192.168.1.2/0100'\"><FONT size=\"7%\">4</FONT></button>\n"
  "<button onclick=\"window.location.href='http://192.168.1.2/0101'\"><FONT size=\"7%\">5</FONT></button></p>\n"
"<p align=\"center\"><button onclick=\"window.location.href='http://192.168.1.2/0110'\"><FONT size=\"7%\">6</FONT></button>\n"
  "<button onclick=\"window.location.href='http://192.168.1.2/0111'\"><FONT size=\"7%\">7</FONT></button>\n"
  "<button onclick=\"window.location.href='http://192.168.1.2/1000'\"><FONT size=\"7%\">8</FONT></button>\n"
  "<button onclick=\"window.location.href='http://192.168.1.2/1001'\"><FONT size=\"7%\">9</FONT></button>\n"
  "<button onclick=\"window.location.href='http://192.168.1.2/1010'\"><FONT size=\"7%\">0</FONT></button>\n</p>"
"<p align=\"center\"><button onclick=\"window.location.href='http://192.168.1.2/1011'\"><FONT size=\"7%\">CLR</FONT></button>\n"
  "<button onclick=\"window.location.href='http://192.168.1.2/1100'\"><FONT size=\"7%\">SEND</FONT></button></p>\n"
             "</td></tr></table>\n"
  ));
               client.println("</body>");
               client.print(" <meta http-equiv=\"refresh\" content=\"7; url=http://192.168.1.2/\"> ");
               client.println("</html>");
          erro = niveldesejado - nivel; //implementação do PID

          //P
          P = erro*KP;
          lastprocess = millis();
          float deltatime = millis() - lastprocess;
          //I
          I += (erro*KI)*(deltatime/1000);
          //D
          D = (lastnivel-nivel)*KD;
          //soma tudo
          PID=P+I+D;
          PIDN = -PID;//inverte o valor do PID
          
           if(PID>0)
  {

   if(PID<255){
    analogWrite(BOMBAENTRADA,map(PID,0,255,255,0)); //liga a bomba de ENTRADA se o PID for positivo
    digitalWrite(BOMBASAIDA,HIGH); //desliga a bomba de SAIDA se o PID for positivo
   }
   else{
    analogWrite(BOMBAENTRADA,0); //liga a bomba de ENTRADA se o PID for positivo
    digitalWrite(BOMBASAIDA,HIGH); //desliga a bomba de SAIDA se o PID for positivo
   }
                                                                           
  }
  else if (PID<0){
    analogWrite(BOMBASAIDA,map(PIDN,0,255,255,0)); //liga a bomba de SAIDA se o PID for negativo
    digitalWrite(BOMBAENTRADA,HIGH);//desliga a bomba de ENTRADA se o PID for negativo
    
    }
    else if (PID==0){
    analogWrite(BOMBASAIDA,255);//desligaliga a bomba de SAIDA se o PID for negativo
    digitalWrite(BOMBAENTRADA,HIGH);//desliga a bomba de ENTRADA se o PID for negativo
    }
   
          break;

        } //Fecha if (c == '\n' && currentLineIsBlank)
        
      } //Fecha if (client.available())
      
    } //Fecha While (client.connected())
    delay(1);// Espera um tempo para o navegador receber os dados
    client.stop(); // Fecha a conexão
    
  } //Fecha if(client)
     Serial.print("Nivel: ");
    Serial.println(nivel);
    Serial.print("Distancia ");
    Serial.println(distancia);
} //Fecha loop

