# RevMatrix-Forza

ESP32-powered Forza Horizon telemetry dashboard using MAX7219 LED matrices and WS2812B RGB shift lights.

RevMatrix transforms real-time Forza Horizon telemetry into a compact racing-style dashboard featuring speed display, gear indicator, RPM bars, RGB shift lights, and aggressive redline flash effects.

# TOC

- [FEATURES](#features)
- [HARDWARES USED](#Hardware-Used)
- [SCHEMATICS](#SCHEMATICS)
- [HOW TO MAKE](#building-procedure)
- [HOW IT WORKS](#How-It-Works)
- [LAYOUT](#Layout)
- [SNEAK PEAK](#Sneak-Peak)
- [BACKSTORY (JUST HOW I DONE IT, DOESNT MATTER XD](#backstory)
- [POSSIBLE UPDATES](#Possible-Updates)
- [COMPLETE SHOWCASE](#Complete-Showcase)

# Features

- Real-time Forza Horizon telemetry
- Speed display
- Gear indicator
- Matrix RPM bar
- WS2812B RGB shift lights
- Full-system redline blinking effect
- Boot animations
- ESP32 powered
- Compact 4-module MAX7219 layout

---

# Hardware Used

- ESP32-WROOM-32
- MAX7219 8x8 LED Matrix Modules ×4
- WS2812B RGB LED Strip (12 LEDs)
- USB Power Supply
- Jumper Wires

# SCHEMATICS

<img width="1264" height="622" alt="image" src="https://github.com/user-attachments/assets/e4838ec5-f1c6-4971-b318-0f05f432392e" />

### PIN CONNECTIONS
- GPIO12 → MAX7219 DIN
- GPIO13 → MAX7219 CS
- GPIO14 → MAX7219 CLK
- GPIO4  → WS2812B DATA

Make sure both strip and matrix grounds are common

# BUILDING PROCEDURE

- Make the connections as shown in [SCHEMATICS](#SCHEMATICS)
- Connect the Board with pc using a data cable
- Open Arduino IDE and install libraries and boards. [Refer how to install esp32 boards](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html)
- Download the Code and libraries.
- In IDE Make sure Correct board is chosen, Most of the ESP32 Boards should work on "ESP32 DEV MODULE"

   <img width="247" height="133" alt="image" src="https://github.com/user-attachments/assets/1cc906c2-66d0-4f6b-bdb8-387d19a54061" />

- In the Code, Replace "YOUR SSID" With ur WiFi SSID and "YOUR PASSWORD" with your WiFi Password

   <img width="361" height="47" alt="image" src="https://github.com/user-attachments/assets/4f0df9c3-65b1-4a1a-99d1-aa9c9f9d89d3" />
   
- Compile and Upload the Code, The Compiling will take some time, Just wait for it, or restart if too much time is taken.
- Open Serial Monitor and Change Baud Rate to 11500 baud

  <img width="275" height="44" alt="image" src="https://github.com/user-attachments/assets/092a792e-c878-42c5-b9d4-f0188dc0bbcb" />
  
- Unplug and Plug ESP32 if nothing is showing or it showing gibberish.
- If u can see "Connecting to Wifi", Then ur Board is trying to connect to wifi and its a good sign.
- After it succesfully connected to wifi, Serial Monitor will show the IP Address and wait for recieving telematry from the game

   <img width="267" height="100" alt="image" src="https://github.com/user-attachments/assets/39b9eea9-0c4a-4905-8cde-b23fc62d9fb1" />
   
- Congrats, ur Hardware is officially completed.
- Now Open the game and head into settings.
- Go to HUD and Gameplay, Scroll down, U can see these settings there
 
  <img width="1224" height="768" alt="Screenshot 2026-05-07 053237" src="https://github.com/user-attachments/assets/9649d587-70fe-4fc9-95c2-a3773b812b62" />

- Turn on the "DATA OUT"
- Enter the IP Address shown in the Serial Monitor on "DATA OUT IP ADDRESS"
- Enter "8000" on "DATA OUT IP PORT". this is the udp port id we've mentioned in the code
- Save the settings
- Congrats You have made the RevMatrix-Forza



# How It Works

RevMatrix uses the telemetry output system built into Forza Horizon to receive live vehicle data over WiFi.
The ESP32 connects to the same network as the PC running the game and listens for UDP telemetry packets sent by Forza Horizon.
The incoming telemetry data contains information such as:

- Vehicle speed
- Engine RPM
- Current gear
- Maximum RPM

The ESP32 processes this data in real time and updates the dashboard components accordingly.

# Layout

### Speed Display

The first two matrix modules display the current vehicle speed in km/h.

### RPM Bar

The third matrix module acts as a miniature RPM indicator using vertical LED bars that progressively fill as engine RPM increases.

### Gear Indicator

The final matrix module displays the currently selected gear.

### WS2812B Shift Lights

The WS2812B LED strip functions as a racing-style RPM light bar.

#### LED layout:

-First 4 LEDs → Green

-Middle 4 LEDs → Yellow

=Final 4 LEDs → Red

As RPM increases:

More LEDs illuminate progressively
Colors transition from green to yellow to red

At redline:

All LEDs flash red rapidly
Entire MAX7219 display blinks simultaneously

This simulates the aggressive shift-light behavior commonly seen in race cars and professional sim-racing dashboards.

# SNEAK PEAK

<img width="4080" height="2294" alt="PXL_20260507_024334332 RAW-02 ORIGINAL" src="https://github.com/user-attachments/assets/d6449c03-35a5-40ac-a341-1348fe121326" />
<img width="2294" height="4080" alt="PXL_20260507_024404458 RAW-02 ORIGINAL" src="https://github.com/user-attachments/assets/d7c86243-3b08-44e6-9539-592da83fb981" />

# BACKSTORY

So I was playing Forza Horizon 4 and just wanted to explore the settings. There i saw a option to get data out to any IP Address by UDP by Assigning IP and UDP Port. Thats when i was curious about what data can forza telematry can output. I Created a simple ESP8266 Raw Data Debugger and found out there is a lot. As someone who doesnt have anything to do, I've decided to make use of some of these data. I Have a MAX7219 Module (Chained 4 of them) and so many microcontrollers in my inventory and started programming. As someone who have 5 Supplimentary in Programming Papers, I was doomed but as a challenge, I tried to program it myself and ofcourse Dario Amodei Helped me with bullshitless Error Detection. But at the end Claude, was the one fixed most of the errors so i give credits to claude even though i spent 5 hours in coding.

After successfully completing the code for MAX7219, I was like, Hmmm... looks intresting but there is no aura, no aggressiveness. Thats when I realized a Sequential Shift Light System gonna make it looks good, but there was a catch, I had many WS2812B Addressible RGB Strips in my inventory so i decided to use one of those, the 8266 I've used with matrix display only offers 3v output so inorder to use ws2812b, i should add external power of 5v. Thats why i decided to use ESP32-WROOM32 Board which have one single 5V Output. I used 12 LEDs of ws2812b and divided it into 3 Parts. Since RPM Levels are diffrent for each cars, i cant take pure telematry out since it shows pure rpm values, So I Used percentage levels

FIRST 4 LED TURNS GREEN AT 0-33% OF RPM

NEXT 4 LED TURNS YELLOW AT 34-66%

FINAL 4 LEDS TURN RED AT 67%-82%

ABOVE 82%, The whole 12 LEDs will flash into red colour and the whole matrix display will Flash.

The reason I've Used 82% is because rarely cars go upto that rpm based on telematry data and the final 2 red lights in the strip doesnt blink at all and only if they all 4 blinks, the flashing will works, so i made the final 4 slightly lesser RPM.

The 3rd module of matrix display was awfully empty since even at 3 digit speed, it only reaches 2 Modules so I Added another RPM Bar and flashing animation to it. but it was very used ingame since it was so frkn bright and the flashing can be noticed even if we dont look at the dashboard

About the cosmetics, the MAX7219 already had a 3D Printed case that i used in my clock animation but it was connected via ESP8266 CH340 DEV BOARD. Since the length of max7219 is lesser and roughly 8 LEDs can be placed under it, so i decided to extend the LED Part only. I used Cable Channels that was sitting here and glued 12 LEDs into the side of it, and i glued it beneath the matrix display. it doesnt looks that good but yeah it will do the job.


# Possible Updates

- Since i am soo lazy on measurements and everything, I have created a Complete 3D Case or design the module perfectly.
- The pixel display just sits IDLE if Forza Telematry Data is not recieving, We can actually place a Clock in this idle part and Forza Dashboard can be automatically launch whenever telematry data is recieved.

# COMPLETE SHOWCASE



https://github.com/user-attachments/assets/d5dda74a-4b8d-410d-ae94-f711f6d90f3f









# Display Layout

```text
[SPEED][SPEED][RPM][GEAR]
