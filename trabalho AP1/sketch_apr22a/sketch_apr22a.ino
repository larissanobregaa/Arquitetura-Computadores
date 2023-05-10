/*COMPONENTES:
Carolina Cruz dos Santos- 202201038781 - TA
Fernanda Moysés- 202201038101 - TA
Juliana Hall- 202201037776 - TA
Larissa Nobrega- 202202640514 - TA
Maria Mello- 202201038462 - TA
Victor da Conceição Coutinho- 202211231061 - TA
*/

#include <LiquidCrystal.h>

const int rs = 10, en = 8, d4 = 2, d5 = 3, d6 = 4, d7 = 5; 
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); 

#include <math.h>
#define PALAVRA 11  // é a quantidade de bits utilizados para representar um número binário de 8 bits

const int MODO=0;   // determina o modo de operação 

String entrada;  // define as constantes evariáveis 
String X;   // são strings utilizadas para armazenar dados temporariamente durante as operações aritméticas
String Y;  
int sinal;  // é uma variável que armazena o sinal da operação, caso seja 0, o sinal é positivo e para 1 o sinal é negativo

String adicao(String dado){    // recebe uma string com dois números binários de 8 bits, converte-os para decimal, realiza a adição e retorna o resultado em forma de string
  int num1,num2, soma;
  String resultado;

  num1 = (dado[3] - 48) * 8 + (dado[4] - 48) * 4 + (dado[5] - 48) * 2 + (dado[6] - 48);
  num2 = (dado[7] - 48) * 8 + (dado[8] - 48) * 4 + (dado[9] - 48) * 2 + (dado[10] - 48); 

  soma = num1 + num2;
  if(soma > 255)
  {
    lcd.print("Deu overflow!");
    return X;
  }

  resultado = converteDecimalString(soma);  //utiliza as funções converteDecimalString() para converter os números binários para decimal e geraSaida() para exibir o resultado no display

  sinal = 0;
  Serial.println(resultado);
  return resultado; 
}

String adicaoImediata(String dado)   // recebe uma string com um número binário de 8 bits e uma string com um número binário armazenado em X, converte-os para decimal, realiza a adição e retorna o resultado em forma de string
{
  int num, numx, soma;
  String resultado;

  num = (dado[3] - 48) * 128 + (dado[4] - 48) * 64 + (dado[5] - 48) * 32 + (dado[6] - 48) * 16 + 
  (dado[7] - 48) * 8 + (dado[8] - 48) * 4 + (dado[9] - 48) * 2 + (dado[10] - 48);

  numx = (X[0] - 48) * 128 + (X[1] - 48) * 64 + (X[2] - 48) * 32 + (X[3] - 48) * 16 + 
  (X[4] - 48) * 8 + (X[5] - 48) * 4 + (X[6] - 48) * 2 + (X[7] - 48);

  soma = num + numx;
  if(soma > 255)
  {
    lcd.print("Deu overflow!");
    return X;
  }

  sinal = 0;
  resultado = converteDecimalString(soma);   //utiliza também a função converteDecimalString() e geraSaida() para exibir o resultado no display
  Serial.println(resultado);
  return resultado;
  
}

String converteDecimalString(int dado){   // recebe um número decimal e converte-o para uma string binária de 8 bits
  String resultado = "";
  int j=pow(2,PALAVRA-4);
  for (int i=0;i<PALAVRA-3;i++){
    if (dado<j){
      resultado = resultado + "0";
    }
    else {
      resultado = resultado + "1";
      dado = dado - j;
    }
    j = j/2;  
  }
  
  return resultado;
}

void geraSaida(String dado){   // recebe uma string com o resultado de uma operação aritmética e exibe-o no display LCD 
  Serial.println(dado);
  lcd.setCursor(0, 1);
  lcd.print(dado);
  
}

String complemento1(String dado){    // recebe uma string com um número binário de 8 bits e retorna o seu complemento de 1 e depois irá utilizar a função converteStringDecimal() para converter a string binária para decimal e a função converteDecimalString() para converter o resultado de volta para uma string binária
  String resultado = "";
  for (int i=3;i<PALAVRA;i++){
    if (dado[i] == '1')
    {
        resultado = resultado + "0";
    } 
    else resultado = resultado + "1";
  }

  Serial.println(resultado);
  return resultado;
}

String complemento2(String dado){    //  recebe uma string com um número binário de 8 bits e retorna o seu complemento de 2
  String resultado;
  int decimal;
  resultado = complemento1(dado);
  decimal = converteStringDecimal(resultado) + 1;
  resultado = converteDecimalString(decimal);
  
  return resultado;
}

int converteStringDecimal(String dado){    // recebe uma string binária de 8 bits e converte-a para um número decimal, ela é utiliada pelas funções complemento2() e adicaoImediata()
  int decimal = 0;
  int pot = 1;
  for (int i=0;i<PALAVRA - 3;i++){
    decimal += (dado[PALAVRA - 1 - i] - 48) * pot;
    pot = pot * 2;
  }
  return decimal;
}

String subtracao(String dado){    // recebe uma string com dois números binários de 8 bits, converte-os para decimal, realiza a subtração e retorna o resultado em forma de string. Se o resultado for negativo, o modo de operação definido em MODO é utilizado para determinar a representação do número negativo
                               
  int num1,num2;
  String resultado;

  num1 = (dado[3] - 48) * 8 + (dado[4] - 48) * 4 + (dado[5] - 48) * 2 + 
  (dado[6] - 48); 

  num2 = (dado[7] - 48) * 8 + (dado[8] - 48) * 4 + (dado[9] - 48) * 2 + 
  (dado[10] - 48); 
  
  if (num1 > num2) 
    resultado = converteDecimalString(num1 - num2);
  else 
  {
    resultado = converteDecimalString(num2 - num1);
    sinal = 1;
    switch (MODO) 
    {
      case 0:
        resultado[0]='1';
        break;
      case 1:
        resultado = complemento1(resultado);
        break;
      case 2:
        resultado = complemento2(resultado);
        break;  
    }
  }

  Serial.println(resultado);
  return resultado;
}

String subtracaoImediata(String dado)
{
  int num1,numx;
  String resultado;
  num1 = (dado[3] - 48) * 128 + (dado[4] - 48) * 64 + (dado[5] - 48) * 32 + (dado[6] - 48) * 16 + 
  (dado[7] - 48) * 8 + (dado[8] - 48) * 4 + (dado[9] - 48) * 2 + (dado[10] - 48);

  numx = (X[0] - 48) * 128 + (X[1] - 48) * 64 + (X[2] - 48) * 32 + (X[3] - 48) * 16 + 
  (X[4] - 48) * 8 + (X[5] - 48) * 4 + (X[6] - 48) * 2 + (X[7] - 48);
  if (num1 > numx) 
    resultado = converteDecimalString(num1 - numx);
  else 
  {
    sinal = 1;
    resultado = converteDecimalString(numx - num1);
    switch (MODO) 
    {
      case 0:
        resultado[0]='1';
        break;
      case 1:
        resultado = complemento1(resultado);
        break;
      case 2:
        resultado = complemento2(resultado);
        break;  
    }
  }

  Serial.println(resultado);
  return resultado;
}

String ler(String dado)
{
  String valor = X;
  String resposta;

  for(int i=3;i<PALAVRA;i++)
  {
    if(dado[i] == '1')
    {
      valor = Y;
    }
  }

  resposta = sinal + valor;
  Serial.println("resposta: " + resposta);
  return resposta;

}

void Armazenar(String dado)
{
  X = dado.substring(3, 11);
}

void Copiar(String dado)
{
  Y = X;
}


int selecaoOp(String dado){ // Seleciona a opção com base na String E5E4E3E2E1E0
  if (dado[0] == '0' && dado[1] == '0' && dado[2] == '0') // Operação 1
      X = complemento1(entrada);
  else if (dado[0] == '0' && dado[1] == '0' && dado[2] == '1') // Operação 2
    X = adicao(entrada);
  else if (dado[0] == '0' && dado[1] == '1' && dado[2] == '0') // Operação 3
    X = adicaoImediata(entrada);
  else if (dado[0] == '0' && dado[1] == '1' && dado[2] == '1') // Operação 4
    X = subtracao(entrada); 
  else if (dado[0] == '1' && dado[1] == '0' && dado[2] == '0') // Operação 5
    X = subtracaoImediata(entrada);
  else if (dado[0] == '1' && dado[1] == '0' && dado[2] == '1') // Operação 6
    geraSaida(ler(entrada));
  else if (dado[0] == '1' && dado[1] == '1' && dado[2] == '0') // Operação 7
    Armazenar(entrada);
  else if (dado[0] == '1' && dado[1] == '1' && dado[2] == '1') // Operação 8
    Copiar(entrada);

}

void setup() {
  lcd.begin(16, 2); // Inicializa o lcd
  lcd.print("AP1: "); // e disponibiliza a mensagem na linha 1
  Serial.begin(9600); // Inicializa o monitor serial
}

void loop() {
  Serial.println("Entre os dados:");
  while (Serial.available() == 0) {}     //Aguarda por dados disponíveis
  entrada = Serial.readString();  // Leitura do dado no monitor
  selecaoOp(entrada);
}