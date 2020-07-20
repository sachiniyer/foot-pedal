
#include <LiquidCrystal.h> 

LiquidCrystal lcd1(13,12,7,6,5,4); 
LiquidCrystal lcd2(11,10,7,6,5,4);
LiquidCrystal lcd3(9,8,7,6,5,4);

void setup()
{
   lcd1.begin(16,2);
   lcd2.begin(16,2);
   lcd3.begin(16,2);
}

void loop()
{   
    lcd1.setCursor(0,0); 
    lcd1.print("  3 LCD 16X2    ");
    delay(100);

    lcd2.setCursor(0,0);
    lcd2.print("  USING ARDUINO ");
    delay(100);

    lcd3.setCursor(0,0);
    lcd3.print("SHUBHAM PAKHALE ");
    delay(100);

    lcd1.setCursor(0,1);
    lcd1.print(" SAME DATA LINE ");
    delay(100);

    lcd2.setCursor(0,1);
    lcd2.print("DESIGNED BY --> ");
    delay(100);

    lcd3.setCursor(0,1);
    lcd3.print(" WWW.EFYMAG.COM ");
    delay(100);
}

