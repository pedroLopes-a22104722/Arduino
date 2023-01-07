#include <LiquidCrystal.h>
#include <IRremote.h>



// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

IRrecv irrecv (10);
decode_results results;
                                                    
#define REDLED 12
#define GREENLED 11
#define TSPIN 9


//variables related to the security door
int locked = 1;
int lockedNbrTries = 1;
int lockedNbrPresses = 0;
int lockingCode = 8888;
int secretCode = 1512;
int unlockedNbrPresses = 0;
int code = 2525;
int finalButton = 0;
int codePrinted=0;
//variables related to the security door


//actions
int lookAnimation(){
  Serial.println("animacao de bloqueio");
  digitalWrite(GREENLED, HIGH);
  delay(1000);
  digitalWrite(GREENLED, LOW);
  digitalWrite(REDLED, HIGH);
  lcd.clear();
  lcd.print("goodbye :(");
  delay(2000);
  digitalWrite(REDLED, LOW);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("code:");
  return 1;
}

void cooldownAnimacion(){
  digitalWrite(REDLED, HIGH);            
  for(int i = 0; i < 5; i++){
    lcd.print("cooldown");
    delay(500);
    lcd.print(".");
    delay(500);
    lcd.print(".");
    delay(500);
    lcd.print(".");
    delay(500);
    lcd.clear();
    lcd.setCursor(0,0);    
  }
  digitalWrite(REDLED, LOW);
}

void welcomeAnimation(){
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Welcome, enter ");
  lcd.setCursor(0,1);
  lcd.print("the code");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("code:");
  digitalWrite(GREENLED, LOW);  
}


//function to make condePrinted
void makeCodePrinted(int value)
{
  lcd.print(value);
  
  if(lockedNbrPresses==0)
  {
    codePrinted += value;
  }
  if(lockedNbrPresses==1)
  {
    codePrinted = (codePrinted*10)+value;
  }
  if(lockedNbrPresses==2)
  {
    codePrinted = (codePrinted*10)+value;
  }
  if(lockedNbrPresses==3)
  {
    codePrinted = (codePrinted*10)+value;
  }
  lockedNbrPresses++;
}
//function to make condePrinted



void setup()
{
  Serial.begin(9600);

  pinMode(REDLED, OUTPUT);
  pinMode(GREENLED, OUTPUT);
  pinMode(TSPIN, INPUT);
  digitalWrite(GREENLED, HIGH);

  welcomeAnimation();cvvvvvvvvvvv

  irrecv.enableIRIn();  
  
}

void loop()
{
  if(locked==1){
    while(1)
    {

      if(finalButton == 1)
      {
        finalButton = 0;
        
        if(codePrinted == code)
        {
          digitalWrite(GREENLED, HIGH);
          lcd.print("Unlocked");
          delay(2000);
          lcd.clear();
          digitalWrite(GREENLED, LOW);

          lockedNbrTries = 0;
          locked=0;
          codePrinted=0;
          break;
        }
        else
        {
          if(lockedNbrTries > 2)
          {
         
            cooldownAnimacion();
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("code:");
            
            lockedNbrTries=0;
            codePrinted=0;
          }
          else
          {
            digitalWrite(REDLED, HIGH);
            lcd.print("Wrong code");
            delay(2000);
            lcd.clear();
            digitalWrite(REDLED, LOW);
            lcd.setCursor(0,0);
            lcd.print("code:");
    
            
            
            lockedNbrTries++;
            codePrinted=0;
            
            //led vermelho
          }
        }
      }else if(digitalRead(TSPIN) == HIGH){
          lcd.clear();
          digitalWrite(GREENLED, HIGH);
          lcd.print("Unlocked");
          delay(2000);
          lcd.clear();
          digitalWrite(GREENLED, LOW);

          lockedNbrTries = 0;
          locked=0;
          codePrinted=0;
          break;
      }
      else
      {
        
        if(irrecv.decode(&results))
        {
          

          unsigned int value = results.value;
          Serial.println(value);   
          
          switch(value)
          {
            
            case 12495://1
            makeCodePrinted(1);
           
            break;
            
            case 6375: //2
            makeCodePrinted(2);   
          
            break;
            
            case 31365: //3
            makeCodePrinted(3);            
            break;
            
            case 4335: //4
            makeCodePrinted(4);   
            break;
            
            case 14535: //5
            makeCodePrinted(5);           
            break;
            
            case 23205: //6
            makeCodePrinted(6);
            break;
            
            case 17085: //7
            makeCodePrinted(7);
            break;
            
            case 19125: //8
            makeCodePrinted(8);
            break;
            
            case 21165: //9
            makeCodePrinted(9);
            break;
            
            case 43095: //FINAL
            lcd.clear();
            delay(1000);
            if(lockedNbrPresses == 4)
            {
              finalButton=1;
              lockedNbrPresses = 0;
            }
            else
            {
              lcd.print("Invalid");
              delay(2000);
          	  lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("code:");
              lockedNbrPresses = 0;
              codePrinted=0;
            }
            break;
          }
          irrecv.resume();
        
        }
      }
    }
  }  
  else{
    Serial.println("desbloqueado");
    if(irrecv.decode(&results)){
      unsigned int value = results.value;
      if(value == 41565){
        locked = lookAnimation();
      }
    }else if(digitalRead(TSPIN) == HIGH){
        locked = lookAnimation();
    }
  }
}