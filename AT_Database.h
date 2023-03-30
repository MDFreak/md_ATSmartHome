/*
  ||
  || @file AT_Database.h
  || @version 0.5
  || @author Gerald Lechner
  || @contact lechge@gmail.com
  ||
  || @description
  || |This library defines structures an functions to manage devices, results
  || |forms and configurations in a smart home control center
  || #
  ||
  || @license
  || | This library is free software; you can redistribute it and/or
  || | modify it under the terms of the GNU Lesser General Public
  || | License as published by the Free Software Foundation; version
  || | 2.1 of the License.
  || |
  || | This library is distributed in the hope that it will be useful,
  || | but WITHOUT ANY WARRANTY; without even the implied warranty of
  || | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  || | Lesser General Public License for more details.
  || |
  || | You should have received a copy of the GNU Lesser General Public
  || | License along with this library; if not, write to the Free Software
  || | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
  || #
  ||
 */

#include <Arduino.h>
#include <md_defines.h>
#include <md_wifi.h>
#include <AT_MessageBuffer.h>

#ifndef AT_Database_h
    #define AT_Database_h

    // config defines
      #define ATMAXCHANNELS 64 //max number of channels
      #define ATMAXDEVICE   8 //max number of devices
      #define ATMAXDEVCHAN  8 //max number channels per device
      #define ATMAXPAGES    8 //max number of result pages

      #define ATWIDGETSPERPAGE  8 //number of widgets to be placed on a result page
      #define AT_USE_DYNSETUP   OFF  // ON not working
      #define AT_USE_EDIT       OFF  // ON not working
    // widget defines
      #define ATWIDGET_UNUSED   0
      #define ATWIDGET_USED     1
      #define ATWIDGET_PLACE    2
      #define ATWIDGET_HIDDEN   3

      #define ATWIDGET_SMALL    0
      #define ATWIDGET_LEFT     1
      #define ATWIDGET_RIGHT    2
      #define ATWIDGET_BIG      3
      #define ATWIDGET_BIG1     4

      #define ATWIDGET_SIMPLE       0

    typedef struct  //ATCURVALUES structure to hold current values
      {
        uint8_t valid;  //the value is valid
        uint8_t step;   //processing step 1 means updated
        uint8_t type;   //type of the data see AT_MessageBuffer.h
        uint8_t unit;   //unit of the data see AT_MessageBuffer.h
        uint8_t value[4]; //four data bytes to hold a float or a long integer
      }  ATCURVALUES;
    typedef struct //ATDEVICE structure to  hold a device definition
      {
        uint8_t activ = 0;        //the device is in use
        uint8_t service = 0;      //0=ESP-Now
        uint8_t id[6] = {0,0,0,0};//id of the device
        uint16_t devicebits = 0;  //capabilities for the device
        String name = "";         //name of the device
        String last = "";         //timestamp of last receiving data
      } ATDEVICE;
    typedef struct //ATDISPLAYWIDGET structure to hold a widget setup
      {
        uint16_t source;      //index into result list
        uint8_t status;     //status 0=not used, 1=used, 2=placeholder, 3=hidden
        uint8_t size;        //size 0=240x30,1=240x60 left 2=120x60 right 3=120x60
        uint8_t type;        //type 0=simple
        uint16_t bgcolor;    //fill color normal
        uint16_t bgcolorOn;  //fillcolor for buttons on
        uint16_t fontcolor;  //font color
        uint8_t image;       //index to an image
        uint8_t precision;   //precision to show values
        String label = "";   //label
        float val1; //threshold value1
        uint16_t color1; //color associated with value 1
        float val2; //threshold value2
        uint16_t color2; //color associated with value 2
        float val3; //threshold value3
        uint16_t color3; //color associated with value 3
      } ATDISPLAYWIDGET;
    typedef struct //ATDISPLAYPAGE struct to hold widgets per page
      {
        ATDISPLAYWIDGET widgets[ATWIDGETSPERPAGE];
      } ATDISPLAYPAGE;
    typedef struct //ATSETUP struct setup
      {
        boolean  useWlan = OFF;
        String   SSID;
        String   password;
        String   NTPserver = "ntp1.rwth-aachen.de" ;
        uint16_t refresh   = 15;
      } ATSETUP;
    // global functions
      String AT_GetId(uint8_t id[6]);//convers id to a string with format xx:xx:xx:xx:xx:xx
      String AT_GetLocalTime();//returns the current date and time as a string (not working yet)
    class AT_Database
      {
        public:
          AT_Database(String deviceFile = "/devices.txt", String confFile = "/config.txt", String setupFile = "/setup.txt");
          // widgets
            boolean         readConfig(String fileName);//read the widget configuration from SPIFFS
            boolean         readConfig();
            boolean         writeConfig(String fileName);//write the widget configuration into a SPIFFS file
            boolean         writeConfig();
            int16_t         getFreeSlot(uint8_t page, uint8_t size); // get a free widget slot for a defined size return -1 if no free slot
            int16_t         getFreeSlot(uint8_t size); //find a free widget slot on all pages for a defined size return -1 if no free slot
            ATDISPLAYWIDGET getWidget(uint8_t page, uint8_t slot); //getWidget returns the widget in slot on page
            ATDISPLAYWIDGET*getWidgetAdr(uint8_t page, uint8_t slot); //getWidget returns the address of a widget in slot on page to allow editing
              //return the index = pag* ATWIDGETSPERPAGE + slot for a widget of given source
                // return -1 if no widget exists
            int16_t         findWidgetBySource(uint16_t source);
            void            deleteWidgetsForDevice(uint8_t device);//delete all widgets for a device
            void            setWidgetSource(uint16_t widget, uint16_t source);//set the source for a widget
          // devices
            boolean         readDevices(String fileName);//read the device list from SPIFFS file
            boolean         readDevices();
            boolean         writeDevices(String fileName);//write the device list into a SPIFFS file
            boolean         writeDevices();
            int16_t         findDevice(uint8_t id[6]);//return the index for a device with a certain id or -1 if not Foundation
              //  register a device with the device id as string with format xx:xx:xx:xx:xx:xx
                // devicebits are the device capabilities channels is the number of channels
                //return the new device number or -1 if not possible
            int8_t          registerDev(String deviceId, AT_MessageBuffer msg);
            int8_t          registerDev();//use information saved with readNewMessage
            boolean         hasNewDevice();//return true if new device waits for registration
            String          getDeviceId(uint8_t device); //get the device id
            String          getDeviceName(uint8_t device);//return the device name or id or - if inaktiv
            ATDEVICE       *getDevice(uint8_t device);//return pointer to a device
            boolean         clearDevices();//delete all device
            boolean         deleteDevice(uint8_t device);//delete one device
              //set result from a message buffer data packets
                //index = device index x channels per device + channel number
          // set
            void            setResult(uint8_t index, ATDATAPACKET data);
              //set the step value for a result
                //index = device index x channels per device + channel number
            void            setStep(uint8_t step, uint16_t index);
            void            toggleResult(uint16_t index);//toggle switch result
          // get & read
            //get the pointer on a result data structure
              //index = device index x channels per device + channel number
            ATCURVALUES     getResult(uint16_t index);
            /*check if a device has data to be returned parameters are
              / the number of the device, a pointer to the buffer and a pointer
              / to the maximum size it returns the number of packets found or -1 if buffer overflow
              / after return the size parameter contains the used buffer size
             */
            int8_t          getResponse(int16_t device, uint8_t * buffer, uint8_t * size);
            ATDISPLAYPAGE   getPage(uint8_t page); //getPage returns the specified display page
            String          getValueString(uint16_t index, uint8_t precision, boolean useunit );//return a value as formatted string with or without units
            uint8_t         getBooleanValue(uint16_t index); //return value for switches
            void            readNewMessage(String newDevice, const uint8_t * buffer);//read message data into _newMsg
          // check data
            boolean         isValueOutput(uint16_t index); //check if a result is a output to device
            boolean         isSwitchOut(uint16_t index);   //check if a result is a output switch
            boolean         isValueZero(uint16_t index);   //check if a value is 0
            boolean         isValid(uint16_t index);       //check if a result is valid
          // wifi
            //define wifi setup (filenames included)
            #if !(AT_USE_DYNSETUP > OFF)   // will be saved in flash
                boolean       setupWiFi(ATSETUP* psetup, md_wifi* pwifi);
              #else
                //read the device list from SPIFFS file
                  boolean         readSetup(String fileName);
                  boolean         readSetup();
                //write the device list into a SPIFFS file
                  boolean         writeSetup(String fileName);
                  boolean         writeSetup();
                //get pointer to setup structure
                  ATSETUP        *getSetup();
              #endif
        private:
          ATCURVALUES       _results[ATMAXCHANNELS];//array to hold results
          ATDEVICE          _devices[ATMAXDEVICE];  //array to hold devices
          ATDISPLAYPAGE     _pages  [ATMAXPAGES];   //array to hold widget configurations
          ATSETUP*          _psetup    = NULL;
          md_wifi*          _pwifi     = NULL;
          String            _deviceFile;            //file to save devices
          String            _confFile;              //file to save devices
          #if (AT_USE_DYNSETUP > OFF)
              String        _setupFile;             //file to save setup
            #endif
          String            _setupFile;             //file to save setup
          String            _newDevice = "";        //id of a newly detected device
          AT_MessageBuffer  _newMsg;                //messageblock received from this device
      };

#endif
