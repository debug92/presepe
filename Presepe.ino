#include <FastLED.h>

#define LED_PIN   4

#define NUM_LEDS          30  // Numero totale dei led.
#define COLOR_ORDER       GRB
#define CHIPSET           WS2812B
#define BRIGHTNESS        255 // Intensità luminosa dei LED.
#define FRAMES_PER_SECOND 60 // 60 Hz (PAL)

CRGB leds[NUM_LEDS];

void setup() {

  delay(3000); // sanity delay
  
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );  

}

void loop() {
  // Aggiunge entropia per randomizzare il number generator.
  random16_add_entropy( random() );

  setStars();
  setMoon();
  setCave();
  setHouse();

  FastLED.show();
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}

/* Probabilità dello sparking. */
#define STARS_SPARKING 50

/* Stars Function 
  Range Led delle stelle: 11 al 26 
  Led luna: 17
*/
#define STARS_LED_RANGE_START  11
#define STARS_LED_RANGE_FINAL  26
#define MOON_LED               17
void setStars() {
  CRGB StarsPalette[4] = {CRGB(240, 96, 0), CRGB(208, 83, 0), CRGB(255, 114, 19), CRGB(240, 96, 0)};

  for (int i = STARS_LED_RANGE_START; i <= STARS_LED_RANGE_FINAL; i++) {
    if (i != MOON_LED) { // Escludiamo il led della luna perché verrà controllato da setMoon().
      leds[i] = StarsPalette[random8(0, 4)]; 
    }      
  }

  /* Genero uno spark casuale per creare un effetto PULSE delle stelle. */
  if( random8() < STARS_SPARKING ) {
    int y = random8(STARS_LED_RANGE_START, STARS_LED_RANGE_FINAL);
    if (y != MOON_LED) { // Escludiamo il led della luna perché verrà controllato da setMoon().
      leds[y] = CRGB(random8(200, 255), random8(100, 185), random8(0, 140));
    }    
  }
}

void setMoon() {
  CRGB MoonPalette[3] = {CRGB(255, 139, 62), CRGB(255, 146, 72), CRGB(255, 159, 96)};
  leds[MOON_LED] = MoonPalette[random8(0, 3)];  
}

#define SPARKING_CAVE 120
#define CAVE_LED_RANGE_START  27
#define CAVE_LED_RANGE_FINAL  29
/* Genero uno spark casuale per creare un effetto FIRE/TORCIA con i led della caverna. */
void setCave() {
  CRGB CavePalette[2] = {CRGB(255, 117, 26), CRGB(255, 103, 2)};
  
  for (int i = CAVE_LED_RANGE_START; i <= CAVE_LED_RANGE_FINAL; i++) {
    leds[i] = CavePalette[random8(0, 2)];   
  }

  if( random8() < SPARKING_CAVE ) {
    int y = random8(CAVE_LED_RANGE_START, CAVE_LED_RANGE_FINAL);
    leds[y] = CRGB(random8(206, 255), random8(77, 134), random8(0, 83));
  }  
}

#define HOUSE_COUNT 10 // Numero di case
#define HOUSE_LED_RANGE_START  0 // Led iniziale
#define HOUSE_LED_RANGE_FINAL  11 // Led finale

void setHouse() {
  CRGB HousePalette[4] = {CRGB(0, 0, 0), CRGB(255, 117, 26), CRGB(255, 103, 2), CRGB(0, 0, 0)};
  
  /* In un minuto vengono eseguiti circa 3600 loop.
     (281 / HOUSE_COUNT) = 28.1
     (100*28.1)/3600 = 0.78% di probabilità ogni 16.66 ms, quindi circa una probabilità del 60% al minuto per ogni casa.
     Potete variare le probabilità a vostro piacere.
  */
  unsigned int ON_PERC = (281 / HOUSE_COUNT); 
  unsigned int y = random16(0, 3600);

  if (y <= ON_PERC) {    
    leds[random8(HOUSE_LED_RANGE_START, HOUSE_LED_RANGE_FINAL)] = HousePalette[random8(1, 4)];     
  } 
}
