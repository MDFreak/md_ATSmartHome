
#ifndef AT_Display_h
    #define AT_Display_h
    /* @file ATDISPLAYPAGE.h
     * @version 0.5
     * @author Gerald Lechner
     * @contact lechge@gmail.com
     *
     * @description
     *   This library defines functions to show lists and forms on a TFT display
     *   It also interacts with Touch-Screen events
     * #
     *
     * @license
     *   This library is free software; you can redistribute it and/or
     *   modify it under the terms of the GNU Lesser General Public
     *   License as published by the Free Software Foundation; version
     *   2.1 of the License.
     *
     *   This library is distributed in the hope that it will be useful,
     *   but WITHOUT ANY WARRANTY; without even the implied warranty of
     *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
     *   Lesser General Public License for more details.
     *
     *   You should have received a copy of the GNU Lesser General Public
     *   License along with this library; if not, write to the Free Software
     *   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
     * #
     *
     */

    #include "AT_LanguageGE.h"
    #include <Arduino.h>
    #include <AT_Database.h>
    #include <Adafruit_GFX.h>
    #include <Adafruit_ILI9341.h>
    #include <XPT2046_Touchscreen.h>
    #include <md_TouchEvent.h>
    //#include <AT_Layout.h>

    // colors
      #define ATblack         0
      #define ATwhite         0xffff
      #define ATyellow        0xffe0
      #define ATred           0xf800
      #define ATblue          0x001f
      #define ATlime          0x07e0
      #define ATlightgray     0xc618
      #define ATdarkgray      0x8410
      #define ATmagenta       0xf81f
      #define ATcyan          0x07ff
      #define ATviolet        0x801f
      #define ATgreen         0x0660
      #define ATdarkgreen     0x0400
      #define ATdarkblue      0x0010
      #define ATorange        0xfc00
      #define ATdarkred       0xc9a8
    // BAR
      #define ATBARTITLE      0
      #define ATBARTITLECLOCK 1
      #define ATBARTITLEPAGE  2
      #define ATBARDEVICE     3
      #define ATBARCHANNEL    4
      #define ATBARBACK       5
      #define ATBARSAVECANCEL 6
      #define ATBARSTATUS     7
      #define ATBARSCDX       8
      #define ATBARWIDGET     9
      #define ATBARSYSTEM     10
    // CONTENT
      #define ATCONTRESULTS   0
      #define ATCONTLIST      1
      #define ATCONTFORM      2
    // FORM
      #define ATFRMLABEL      0
      #define ATFRMTEXT       1
      #define ATFRMINT        2
      #define ATFRMFLOAT      3
      #define ATFRMCHECK      4
      #define ATFRMCOLOR      5
      #define ATFRMSELECT     6
      #define ATFRMSSID       7
    // EDIT
      #define ATEDTOFF        0
      #define ATEDTTEXT       1
      #define ATEDTINT        2
      #define ATEDTFLOAT      3
      #define ATEDTCOLOR      4
      #define ATEDTSELECT     5
      #define ATEDTCONFIRM    6
      #define ATEDTSSID       7
    // ALIGN
      #define ATALIGNCENTER   0
      #define ATALIGNLEFT     1
      #define ATALIGNRIGHT    2
    typedef struct // ATPAGETYPE struct for page definition
      {
        char title[50];
        uint16_t subpages;
        uint8_t content;
        uint8_t topbarType;
        uint8_t topbarStyle;
        uint8_t botbarType;
        uint8_t botbarStyle;
        char botbarText[30];
        uint8_t previousPage;
      } ATPAGETYPE;

    typedef struct // ATSTYLE
      {
        uint16_t fill;
        uint16_t border;
        uint16_t color;
        uint8_t  alignment;
        const GFXfont *font;
      } ATSTYLE;

    typedef struct // ATFORMELEMENT
      {
        uint8_t type;
        uint8_t size;
        uint8_t row;
        uint8_t col;
        uint8_t style;
        uint8_t optcnt;
        String  optlist[16];
      } ATFORMELEMENT;

    typedef struct // ATFORM
      {
        uint8_t elementCnt;
        ATFORMELEMENT elements[24];
      } ATFORM;

    class AT_Display
      {
        public:
          AT_Display(Adafruit_ILI9341 *tft, AT_Database *database,  uint8_t led, uint8_t arduitouchVersion = 0);
          // *** start & run
            //start display handling set update interval in seconds
            void begin      (uint16_t update, uint8_t rotation = 0);
            void display    (boolean on);//switch background led on or off
            void setStatus  (String msg);//set a new status message
            void updateDisplay();
          // *** display elements
            void showChannelList  (); //show a list of channels for a device
            void showDeviceList   (); //show a list of devices
            void showCurrentPage  (); //show the current page
            void showStatusBar    (); //show a bar with status information
            void showResults      (); //show a list of results with custom widgets
            void showList         (); //show a list out of database
            void showForm         (); //show an edit form
            void showContent      (uint8_t  type);  //show the content part of the current page
            void clearContent     (uint16_t color); //fill the contentarea with color
            void showBackBar      (uint16_t y, uint8_t style); //show a bar with a back button
            void showSaveCancelBar(uint16_t y, uint8_t style); //show a bar with a save and cancel button
            void showDeviceBar    (uint16_t y, uint8_t style); //show a bar with the name of the device to edit
            void showWidgetBar    (uint16_t y, uint8_t style); //show a bar with the name of the widget to edit
            void showSCDXBar      (uint16_t y, uint8_t style, String extra); //show a bar with a save , cancel, delete and detail button
            void showTitleClockBar(uint16_t y, uint8_t style,  String title);//show a bar with title and current time
            void showTitleBar     (uint16_t y, uint8_t style,  String title); //show a bar with title
            void showTitlePageBar (uint16_t y, uint8_t style,  String title); //show a bar with title and number of subpage
            void showSimpleWidget (uint16_t x, uint16_t y,   ATDISPLAYWIDGET wdg);//show a simple widget
            void showBox          (uint16_t x, uint16_t y,   uint16_t w, uint16_t h, String text, ATSTYLE  style);//show a box with centered text with given style
            void showRoundedBox   (uint16_t x, uint16_t y,   uint16_t w, uint16_t h, String text, ATSTYLE style);//show a rounded box with centered text with given style
            void showBoxLine      (uint16_t x, uint16_t y,   uint16_t w, uint16_t h, String text, ATSTYLE  style);//show a box with bottom line and centered text with given style
            void showBar          (uint16_t y, uint8_t type, uint8_t style, String title);//show one of the bars on top or bottom
            void showCheckbox     (uint8_t row,uint8_t col,  uint8_t style, String value);//show a checkbox formelement
            void showSimpleElement(uint8_t size, uint8_t row, uint8_t col, uint8_t style, String value);//show a simple formelement
          // *** unused
            #if (AT_USE_EDIT > OFF)
                //show edit form for a device
                void editDevice(uint8_t device, uint16_t backPage);
                //save device after editiong
                void saveDevice();
                //delete a device
                void deleteDevice();
                //show edit form for a widget
                void editWidget(uint16_t widget, uint16_t backPage);
                //save widget after editiong
                void saveWidget();
                //delete a widget
                void deleteWidget();
              #endif
            #if (AT_USE_DYNSETUP > OFF)
                //show edit form for system setup
                void editSystem();
                //save system after editing
                void saveSystem();
              #endif
          // *** divers
            uint8_t getColorIndex(uint16_t color);//return the color index in palette
            void    registerNewDevice();//reguister a new device in the database and open the configuration page
            void    alignText(uint16_t x, uint16_t y, uint16_t w, uint16_t h,  uint8_t alignment, String text);//center text a height of 0 means center horizontal only
            String  encodeUnicode(String text);//encode extra character from unicode for display
            String  fromKeyboard(String text);//convert keyboard entered text into UTF8
            void    print(String text);//output a string to tft
          // *** callbacks
            void  registerOnSystemChanged(void (*callback)());//this callback will be called if system setup has changed
            void  registerOnResultChange(void (*callback)(uint16_t index));//register a callback function result change event
            void  onClick(TS_Point p);
            void  onLongClick(TS_Point p);
            void  onSwipe(uint8_t direction);
          // *** form editors
            void  editorOff();//close editor and update result
            void  edKbdOn(uint8_t element);//switch keyboard on
            void  edKbdShow();//draw the keyboard area
            void  edKbdUpdate();//redraw the input area
            void  edColorOn(uint8_t element);//switch color editor on
            void  edColorUpdate();//redraw the input area
            void  edColorShow();//show a selector to select a color out of 16
            void  edNumPadOn(uint8_t element, uint8_t editType);//switch numeric editor on
            void  edNumPadUpdate();//redraw the input area
            void  edNumPadShow();//show a num pad
            void  edSelectorOn(uint8_t element);//switch selector list editor on
            void  edSelectorShow();//show a num pad
            void  edSSIDOn(uint8_t element);//switch selector list editor on
            void  edSSIDShow();//show a num pad
        private:
          void    switchPage        (uint8_t  page);
          void    switchSubPage     (uint8_t  subpage);
          int16_t findWidget        (uint8_t  line, uint8_t column);
          int16_t findWidgetSource  (uint8_t  line, uint8_t column);
          int16_t findFormElement   (uint8_t  line, uint8_t column);
          void    clickBar          (boolean  bottom, TS_Point p);
          void    clickSaveCancelBar(boolean  left);
          void    clickSCDXBar      (boolean  left, boolean top);
          void    clickResults      (uint8_t  line, uint8_t column);
          void    clickList         (uint8_t  line);
          void    clickForm         (uint8_t  line, uint8_t column, uint16_t xPos);
          void    clickContent      (TS_Point p);
          void    editorClick       (TS_Point p);
          void    edKbdClick        (TS_Point p);
          void    edColorClick      (TS_Point p);
          void    numPadClick       (TS_Point p, boolean isFloat);
          void    edSelectClick     (TS_Point p);
          void    edSSIDClick       (TS_Point p);

          Adafruit_ILI9341 *_tft;             //pointer to the display handler
          AT_Database      *_database;        //pointer to the database
          uint8_t           _led;             //pin number for background led
          uint16_t          _update;          //update intervall
          uint8_t           _rotation    = 0; //orientation of Display 0: USB up, 1: USB left ..
          uint32_t          _nextDisplay = 0; //timestamp for next update
          uint16_t          _curPage     = 0; //index to the current page
          uint16_t          _subpage     = 0; //index to the current subpages
          uint16_t          _backPage    = 0; //page to return after editing
          uint8_t           _device;          //number of current device if required
          uint8_t           _deviceChannel;   //current channel in a device if required
          uint8_t           _curWidgetPg;     //current widgets page if required
          uint8_t           _curWidgetSl;     //current widgets slot if required
          uint8_t           _statusstyle = 20;//actual status style
          uint16_t          _statusline  = 300;//
          String            _status = "Display gestartet";//status or error message
          String            _idle1  = "Idle links";       //idle text left
          String            _idle2  = "Idle rechts";      //idle text right
          void(*_onResultChange)(uint16_t index) = NULL;
          void(*_onSystemChange)()  = NULL;
          String            _editBuffer[24];  //hold information while editing
          uint8_t           _curElement;      //index to element in form
          const ATFORM     *_curForm;         //pointer to current form
          uint8_t           _edType;          //editor type
          uint8_t           _kbdlevel;        //current keyboard level
          String            _edvaltxt;        //current editor input
          int32_t           _edvalint;        //current edit integer
          float             _edvalflt;        //current edit float
          String            _edOptions[16];   //option list
          uint8_t           _edOptCnt;        //belegte Einträge
          uint8_t           _arduitouchVersion; //version of arduitouch Hardware
      };
  #endif
