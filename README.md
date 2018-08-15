# Personal-Assistent
Embedded Systems Project
Requirements document
1. Overview
  1.1. Objectives: Why are we doing this project? What is the purpose?
The objectives of this project are to design, build and test a personal assistant. Educationally, students are learning how to interface an alarm clock, wifi, and speaker components to create a coherent unit. 
 
  1.2. Process: How will the project be developed?
The project will be developed using the TM4C123 board. There will be three to five switches that the operator will use to control the personal assistant. The system will be built on a printed PCB and run on a +5 V rechargeable portable battery. The system will use off-board switches soldered to the PCB. A hardware/software interface will be designed that allows software to control the assistant. There will be no limit to the number of  hardware/software module.  The process will be to design and test each module independently from the other modules. After each module is tested, the system will be built and tested. 
 
  1.3. Roles and Responsibilities: Who will do what?  Who are the clients?
EE445L students are the engineers and the TA is the client. Students are expected to make minor modifications to this document in order to clarify exactly what they plan to build. Students are allowed to divide responsibilities of the project however they wish, but, at the time of demonstration, both students are expected to understand all aspects of the design. Everything will be done together in person. The hardware will be put together by both students. The music playing functionality was done together in person so that each person understood the code.

  1.4. Interactions with Existing Systems: How will it fit in?
            	The system will use the TM4C123 board, a printed PCB designed by the students, and the 32-Ohm speaker provided by Dr. Valvano. It will be interfaced using the DAC and AMP from Lab 5. It will be powered using a +5V portable battery. In addition, a wifi module will be installed so that the system can pull data from the internet using the CC3100 Booster Pack which will sit on top of the TM4C123.
 
  1.5. Terminology: Define terms used in the document.
SSI - is a widely used serial interface standard for industrial applications between a master (e.g. controller) and a slave (e.g. sensor)
Linearity - the property of a mathematical relationship or function which means that it can be graphically represented as a straight line. Examples are the relationship of voltage and current across a resistor (Ohm's law), or the mass and weight of an object
Frequency response - the dependence on signal frequency of the output–input ratio of an amplifier or other device.
Loudness - That attribute of auditory sensation in terms of which sounds can be ordered on a scale extending from quiet to loud
Pitch - the quality of a sound governed by the rate of vibrations producing it; the degree of highness or lowness of a tone
Instrument - a tool or implement, especially one for delicate or scientific work.
Tempo - the speed at which a passage of music is or should be played.
Envelope - a covering or containing structure or layer
Melody - a sequence of single notes that is musically satisfying
Harmony - the combination of simultaneously sounded musical notes to produce chords and chord progressions having a pleasing effect
Power budget: Maximum power that the power supply can drive.  This limits the number of components that you can connect to a supply
Device driver: Interface between a device and a software module.  An example would be the functions defined in ST7735.c
Critical section: Multiple threads access shared memory locations/variables.  A higher priority thread has to interrupt another thread and write to a variable that the lower priority thread was reading.
Latency: The time between a service request and the process actually being serviced
Time jitter: The difference in the maximum latency and the minimum latency that occurs.  Some systems have a strict minimum time jitter requirement.
Modular programming: Break up your software into modules with distinct tasks.  Each of the modules should be kept separate so that you can easily debug or change specific features of your system.
 
  1.6. Security: How will intellectual property be managed?
The system may include software from StellarisWare and from the book. No software written for this project may be transmitted, viewed, or communicated with any other EE445L student past, present, or future (other than the lab partner of course). It is the responsibility of the team to keep its EE445L lab solutions secure.
 
2. Function Description
  2.1. Functionality: What will the system do precisely?
It will display hours and minutes in both graphical and numeric forms on the LCD. The graphical output will include the 12 numbers around a circle, the hour hand, and the minute hand. The numerical output will be easy to read.
It will allow the operator to set the current time using switches or a keypad. 
It will allow the operator to set the alarm time including enabling/disabling alarms. 
It will make a sound at the alarm time. 
It will allow the operator to stop the sound. An LED heartbeat will show when the system is running.
Support a secondary alarm
Supports a snooze alarm
Change clock backgrounds
Supports a stopwatch mode
Has a minute hand
Pause/play for the music
Rewind Button
Change instruments
Enveloping of sound waves
Wifi Module
Pulling weather from wifi
Pulling time from wifi
Proximity sensor for snooze (or to wake up system) 
Calendar/ reminder
Loading screen
Error screen
Slide Pot that controls speaker volume
Pulling music from Wifi that will be fetched from the web server using predefined format (not encoded)

  2.2. Scope: List the phases and what will be delivered in each phase.
The lab is divided up into 3 parts: preparation, demo, and lab report.  In the preparation, we are expected to have software written for each module as well as a main program to test the alarm.  For the demo, we will show the TA our system with the alarm-selector as an extra feature.  We will then answer any questions that the TA may have for our team.  For the lab report, we will include this requirements document, a software diagram, hardware specifications, and answers to the questions shown in the lab manual.
 
  2.3. Prototypes: How will intermediate progress be demonstrated?
A prototype system running on the TM4C123 board and solderless breadboard will be demonstrated. Progress will be judged by the preparation, demonstration and lab report.
 
  2.4. Performance: Define the measures and describe how they will be determined.
The system will be judged by six qualitative measures. First, the software modules must be easy to understand and well-organized. Second, the system must employ an abstract data structures to hold the sound and the music. There should be a clear and obvious translation from sheet music to the data structure. Four,  the clock display should be beautiful and effective in telling time. Five, the operation of setting the time and alarm should be simple and intuitive. Six, the wifi module should be effective and timely in delivering data. 
 
  2.5. Usability: Describe the interfaces. Be quantitative if possible.
There will be three switches to give switch inputs. The DAC will be interfaced to a 32-ohm speaker. In the main menu, the switches can be navigate the screen and access various menu options. The user should be able to set the time (hours, minutes) and be able to set the alarm (hour, minute). After some amount of inactivity the system reverts to the main menu. The user should be about to control some aspects of the display configuring the look and feel of the device. The switches will be debounced, so only one action occurs when the operator touches a switch once. 
The LCD display shows the time using graphical display typical of a standard on a digital clock. It will be large and shown in landscape so the user can see the time easily. The alarm sound will be some sort of predefined music that the user will be able to set. The sound amplitude will be just loud enough for the TA to hear when within 3 feet. In addition, there will be a wifi module that updates the time, and allows for songs to be downloaded from the internet.
After a certain amount of time, the system will enter sleep mode where most of the functionality is disabled to preserve power. The proximity sensor will be used to detect if the system will need to be awakened. Furthermore, when an alarm is sounded, the proximity sensor will be used to detect if the system should be snoozed, disabling the alarm will be a manual action.
 
  2.6. Safety: Explain any safety requirements and how they will be measured.
    	The alarm sound will be VERY quiet in order to respect other people in the room during testing. Connecting or disconnecting wires on the protoboard while power is applied may damage the board.
 
3. Deliverables
  3.1. Reports: How will the system be described?
A lab report described below is due by the due date listed in the syllabus. This report includes the final requirements document.
 
  3.2. Audits: How will the clients evaluate progress?
The preparation is due at the beginning of the lab period on the date listed in the syllabus.
 
  3.3. Outcomes: What are the deliverables? How do we know when it is done?
There are three deliverables: preparation, demonstration, and report. 


