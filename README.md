# LightAlarm
An alarm clock that provides plenty of functionality and uses light to provide optimal wake-up experience. All powered by Arduino.

The project consists of two main components:  

1.) An alarm box. It provides the UI, produces sound and communicates with the external lightbox. The alarm box represents the main control unit of the project and could work independently.

The main code for the alarm box can be found in "LightAlarm/Main alarm box/MainAlarm/".  
The Fritzing hardware schematic can be found in "LightAlarm/Main alarm box/MainAlarm_SchematicFull.fzz".


2.) A lightbox. It controls its LEDs based on the commands received from the alarm box and contains the software and hardware required to achieve dynamically controlled PWL dimming. This component cannot work independently and can be disabled using the alarm box. 

The main code for the light box can be found in "LightAlarm/External Light/slave_device/".  
The Fritzing hardware schematic can be found in "LightAlarm/External Light/ExternalLight_SchematicFull.fzz".
