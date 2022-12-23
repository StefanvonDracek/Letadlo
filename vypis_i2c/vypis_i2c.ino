// Arduino I2C skener
// navody.dratek.cz

// připojení potřebné knihovny
#include <Wire.h>
// nastavení čísel propojovacích pinů
#define I2C_SDA 21
#define I2C_SCL 22



void setup() {
  // zahájení komunikace po I2C sběrnici,
  // lze použít buď výchozí piny s variantou ()
  // nebo specifikovat piny např. u ESP desek (SDA, SCL)
  Wire.begin();
  //Wire.begin(I2C_SDA, I2C_SCL);

  // zahájení komunikace po sériové lince
  Serial.begin(9600);
  Serial.println("Start I2C skeneru...");
}

void loop() {
  // vytvoření pomocných proměnných
  byte chyba, adresa;
  int pocetZarizeni = 0;
  Serial.println("Zacatek hledani...");
  // smyčka pro hledání zařízení
  // I2C zařízení mohou mít adresu 1-126
  for (adresa = 1; adresa < 127; adresa++ ) {
    // zahájení komunikace a jeho následné ukončení,
    // při ukončení probíhá detekce zařízení pomocí
    // kontroly návratové hodnoty
    Wire.beginTransmission(adresa);
    chyba = Wire.endTransmission();
    // pokud nám zařízení odpoví '0',
    // tak funguje správně
    if (chyba == 0) {
      // vytištění informací o zařízení včetně
      // jeho adresy v hexa formátu
      Serial.print("I2C zarizeni nalezeno na adrese 0x");
      Serial.print(adresa, HEX);
      Serial.println("  !");
      // inkrementace počítadla zařízení
      pocetZarizeni++;
    }
    // při chybě v komunikaci je chybný výstup 4
    else if (chyba == 4) {
      // vytištění informací o zařízení včetně
      // jeho adresy v hexa formátu
      Serial.print("Neznama chyba na adrese 0x");
      Serial.println(adresa, HEX);
    }
  }
  // kontrola počítadla zařízení
  if (pocetZarizeni == 0) {
    // nebylo nalezeno žádné zařízení, protože neproběhla inkrementace
    Serial.println("Zadne I2C zarizeni nebylo nalezeno.");
  }
  else {
    // oznámení o konci hledání
    Serial.print("Konec hledani, nalezeno ");
    Serial.print(pocetZarizeni);
    Serial.println(" zarizeni.");
  }
  // pauza před novým během smyčky
  delay(5000);
}
