//Version 0.5
  // This library is free software; you can redistribute it and/or
  // modify it under the terms of the GNU Lesser General Public
  // License as published by the Free Software Foundation; either
  // version 2.1 of the License, or (at your option) any later version.

#include <AT_Display.h>
#include <AT_Layout.h>
#include <AT_Database.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>
#include <md_TouchEvent.h>
#if defined(ESP32)
    #if (USE_ATWIFI > OFF)
        #include "WiFi.h"
      #endif
#else
  #include "ESP8266WiFi.h"
#endif

//fonts from the ArduiTouch library
#ifndef _AT_BOLD12PT7B_H_
    #include "fonts/AT_Bold12pt7b.h"
    #include "fonts/AT_Standard9pt7b.h"
  #endif
#define ATVERSION "0.5"

const char keys[3][42] =
  {
    {'a','b','c','d','e','f',
     'g','h','i','j','k','l',
     'm','n','o','p','q','r',
     's','t','u','v','w','x',
     'y','z',130,131,132,133,
     ',','.','-','_','/','@',
     30,26,' ',' ',31,28
    },
    {'A','B','C','D','E','F',
     'G','H','I','J','K','L',
     'M','N','O','P','Q','R',
     'S','T','U','V','W','X',
     'Y','Z',127,128,129,133,
     ',','.','-','_','/','@',
     30,27,' ',' ',31,28
    },
    {'0','1','2','3','4','5',
     '6','7','8','9',';',':',
     '!','?','#',34,39,92,
     '(','[','{','}',']',')',
     '+','*','=','$','%','@',
     '|',134,'^','&','<','>',
     29,26,' ',' ',31,28
    }
  };
// service functions
  uint16_t convertColor(uint8_t r, uint8_t g, uint8_t b)
  {
    r = r/8;
    g = g/4;
    b = b/8;
    uint16_t tftColor = r*2048+g*32+b;
    return tftColor;
  }
// class AT_Display
  AT_Display::AT_Display(Adafruit_ILI9341 *tft, AT_Database *database, uint8_t led, uint8_t arduitouchVersion)
    {
      _tft = tft;
      _database = database;
      _led = led;
      _arduitouchVersion = arduitouchVersion;
    }
  // *** start & run
    void AT_Display::begin      (uint16_t update, uint8_t rotation)
      {
        _tft->begin();
        _tft->fillScreen(ATblack);
        _tft->setRotation(rotation);
        _update = update;
        _rotation = rotation;
        _curPage = 0;
        _subpage = 0;
        _edType = ATEDTOFF;
      }
    void AT_Display::display    (boolean on)
      {
        if (_arduitouchVersion > 0)
          {
            if (on) { digitalWrite(_led,0); }
            else    { digitalWrite(_led,1); }
          }
        else
          {
            if (on) { digitalWrite(_led,1); }
            else    { digitalWrite(_led,0); }
          }
      }
    void AT_Display::setStatus  (String msg)
      {
        _status = msg;
      }
    void AT_Display::updateDisplay()
      {
        if (millis() > _nextDisplay)
          {
            if (_curPage == ATPAGRESULTS) showCurrentPage();
            _nextDisplay = millis()+(_update * 1000);
          }
      }
  // *** display elements
    void AT_Display::showChannelList  ()
      { //show a list of channels for a device
        String name;
        for (uint8_t i = 0; i < 8; i++)
          {
            name = ATTXTCHANNEL;
            name+= " "+String(i);
            if (_database->isValid(_device * 8 + i))
              {
                showBoxLine(0,40+i*20,240,20,name,AT_display_styles[ATSTYLECHLLISTACT]);
              }
            else
              {
                showBoxLine(0,40+i*20,240,20,name,AT_display_styles[ATSTYLECHLLIST]);
              }
          }
      }
    void AT_Display::showDeviceList   ()
      { //show a list of devices
        String name;
        for (uint8_t i = 0; i < 12; i++)
          {
            name = _database->getDeviceName(_subpage * 12 + i);
            showBoxLine(0,40+i*20,240,20,name,AT_display_styles[ATSTYLEDEVLIST]);
          }
      }
    void AT_Display::showCurrentPage  ()
      {
        showBar(0,  AT_display_pages[_curPage].topbarType,
                    AT_display_pages[_curPage].topbarStyle,
                    String(AT_display_pages[_curPage].title));
        showContent(AT_display_pages[_curPage].content);
        showBar(280,AT_display_pages[_curPage].botbarType,
                    AT_display_pages[_curPage].botbarStyle,
                    String(AT_display_pages[_curPage].botbarText));
      }
    void AT_Display::showStatusBar    ()
      { //show a bar with status information
        switch (_statusstyle)
          {
            case ATSTYLESTATIDLEL:
            case ATSTYLESTATIDLER:
                showBox(0,_statusline,119,20,_idle1,
                        AT_display_styles[ATSTYLESTATIDLEL]);
                showBox(120,_statusline,119,20,_idle2,
                        AT_display_styles[ATSTYLESTATIDLER]);
              break;
            default:
                showBox(0,_statusline,240,20,_status,
                        AT_display_styles[_statusstyle]);
              break;
          }
      }
    void AT_Display::showResults      ()
      { //show a list of results with custom widgets
        String msg;
        uint16_t x,y;
        x = 0; y = 0;
        ATDISPLAYPAGE pg;
        ATDISPLAYWIDGET wdg;
        pg = _database->getPage(_subpage);
        for (uint8_t i = 0;i<ATWIDGETSPERPAGE;i++)
          {
            wdg = pg.widgets[i];
            if (wdg.status == ATWIDGET_USED)
              {
                switch (wdg.size)
                  {
                    case ATWIDGET_SMALL:
                    case ATWIDGET_BIG: x=0;y=i*30+40;
                      break;
                    case ATWIDGET_LEFT: x=0; y=i*30+40;
                      break;
                    case ATWIDGET_RIGHT: x=120; y= (i-1)*30 + 40;
                      break;
                  }
                switch (wdg.type)
                  {
                    case ATWIDGET_SIMPLE: showSimpleWidget(x, y, wdg); break;
                  }
              }
          }
      }
    void AT_Display::showList         ()
      { //show a list out of database
        switch (_curPage)
          {
            case ATPAGDEVICE : showDeviceList(); break;
            case ATPAGCHANNEL : showChannelList(); break;
          }
      }
    void AT_Display::showForm         ()
      { //show an edit form
        uint8_t sz,rw,cl,sy;
        switch (_curPage)
          {
            case ATPAGDEVSTP: _curForm = &AT_devfrm; break;
            case ATPAGWDGTSTP: _curForm = &AT_wdgfrm; break;
            case ATPAGSYSSTP: _curForm = &AT_sysfrm; break;
            default: _curForm = 0;
          }
        if (_curForm == 0) return;
        uint8_t max = _curForm->elementCnt;
        for (uint8_t i = 0; i<max; i++)
          {
            sz = _curForm->elements[i].size;
            rw = _curForm->elements[i].row;
            cl = _curForm->elements[i].col;
            sy = _curForm->elements[i].style;
            switch (_curForm->elements[i].type)
              {
                case ATFRMLABEL:
                case ATFRMTEXT:
                case ATFRMFLOAT:
                case ATFRMSSID:
                case ATFRMINT: showSimpleElement(sz, rw, cl, sy, _editBuffer[i]); break;
                case ATFRMCHECK:   _tft->fillRect(cl*120,rw*20+40,sz*120,20,ATblack);
                  showCheckbox(rw,cl,sy,_editBuffer[i]); break;
                case ATFRMCOLOR: _tft->fillRect(cl*120,rw*20+40,sz*120,20,_editBuffer[i].toInt());
                  _tft->drawRect(cl*120,rw*20+40,sz*120,20,ATwhite);
                  break;
                case ATFRMSELECT: showBox(cl*120,rw*20+40,sz*120,20,_curForm->elements[i].optlist[_editBuffer[i].toInt()],AT_display_styles[ATSTYLEOPTIONS]);
                  break;
              }
          }
      }
    void AT_Display::showContent      (uint8_t type)
      {
        switch (type)
          {
            case ATCONTRESULTS: showResults(); break;
            case ATCONTLIST: showList(); break;
            case ATCONTFORM: showForm(); break;
          }
      }
    void AT_Display::clearContent     (uint16_t color)
      { //fill the contentarea with color
        _tft->fillRect(0,40,240,240,color);
      }
    void AT_Display::showBackBar      (uint16_t y, uint8_t style)
      { //show a bar with a back button
        showRoundedBox(0,y,240,40,ATTXTBACK,AT_display_styles[style]);
      }
    void AT_Display::showSaveCancelBar(uint16_t y, uint8_t style)
      { //show a bar with a save and cancel button
        showRoundedBox(0,y,120,40,ATTXTSAVE,AT_display_styles[style]);
        showRoundedBox(120,y,120,40,ATTXTCANCEL,AT_display_styles[style]);
      }
    void AT_Display::showDeviceBar    (uint16_t y, uint8_t style)
      { //show a bar with the name of the device to edit
        String msg = ATTXTDEVICE;
        msg+= " "+_database->getDeviceName(_device);
        showBox(0,y,240,40,msg,AT_display_styles[style]);
      }
    void AT_Display::showWidgetBar    (uint16_t y, uint8_t style)
      { //show a bar with the name of the widget to edit
        String msg = ATTXTWIDGET;
        ATDISPLAYWIDGET wdg = _database->getWidget(_curWidgetPg,_curWidgetSl);
        msg+= " "+wdg.label;
        showBox(0,y,240,40,msg,AT_display_styles[style]);
      }
    void AT_Display::showSCDXBar      (uint16_t y, uint8_t style, String extra)
      { //show a bar with a save , cancel, delete and detail button
        showRoundedBox(0,y,120,20,extra,AT_display_styles[style]);
        showRoundedBox(120,y,120,20,ATTXTCANCEL,AT_display_styles[style]);
        showRoundedBox(0,y+20,120,20,ATTXTSAVE,AT_display_styles[style]);
        showRoundedBox(120,y+20,120,20,ATTXTDELETE,AT_display_styles[style]);
      }
    void AT_Display::showTitleClockBar(uint16_t y, uint8_t style, String title)
      { //show a bar with title and current time
        showBox(0,y,240,20,title+" V "+ATVERSION,AT_display_styles[style]);
        showBox(0,y+20,175,20,AT_GetLocalTime(),AT_display_styles[ATSTYLECLOCK]);
        showBox(175,y+20,65,20,String(ATTXTPAGE)+" "+String(_subpage+1),AT_display_styles[ATSTYLEPAGE]);
      }
    void AT_Display::showTitleBar     (uint16_t y, uint8_t style, String title)
      { //show a bar withtitle
        showBox(0,y,240,20,title,AT_display_styles[style]);
      }
    void AT_Display::showTitlePageBar (uint16_t y, uint8_t style, String title)
      { //show a bar with title and number of subpage
        String msg = title+" "+String(_subpage+1);
        showBox(0,y,240,40,msg,AT_display_styles[style]);
      }
    void AT_Display::showSimpleWidget (uint16_t x, uint16_t y, ATDISPLAYWIDGET wdg)
      { //show a simple widget
        ATSTYLE style;
        String val;
        uint16_t fill;
        style.font = (wdg.size == ATWIDGET_BIG)?&AT_Bold12pt7b:&AT_Standard9pt7b;
        style.fill = wdg.bgcolor;
        style.border = wdg.bgcolor;
        style.color = wdg.fontcolor;
        style.alignment = ATALIGNCENTER;
        fill = style.fill;
        if (_database->isSwitchOut(wdg.source))
          {
            val = ATTXTOFF;
            if (_database->getBooleanValue(wdg.source)==0)
              {
                fill = wdg.bgcolorOn;
                val = ATTXTON;
              }
          }
        else
          {
            val = _database->getValueString(wdg.source,wdg.precision,true);
          }
        String msg = wdg.label+" "+val;
        switch(wdg.size)
          {
            case ATWIDGET_SMALL : showBox(x,y,120,30,wdg.label,style);
              style.fill = fill;
              showBox(x+120,y,120,30,val,style);
              break;
            case ATWIDGET_LEFT:
            case ATWIDGET_RIGHT: showBox(x,y,120,30,wdg.label,style);
              style.fill = fill;
              showBox(x,y+30,120,30,val,style);
              break;
            case ATWIDGET_BIG: showBox(x,y,240,30,wdg.label,style);
              style.fill = fill;
              showBox(x,y+30,240,30,val,style);
              break;
          }
      }
    void AT_Display::showBox          (uint16_t x, uint16_t y, uint16_t w, uint16_t h, String text, ATSTYLE style)
      { //show a box with centered text with given style
        _tft->setFont(style.font);
        _tft->setTextColor(style.color,style.fill);
        _tft->fillRect(x,y,w,h,style.fill);
        _tft->drawRect(x,y,w,h,style.border);
        _tft->drawRect(x+1,y+1,w-2,h-2,style.border);
        alignText(x,y,w,h,style.alignment,text);
      }
    void AT_Display::showRoundedBox   (uint16_t x, uint16_t y, uint16_t w, uint16_t h, String text, ATSTYLE style)
      { //show a rounded box with centered text with given style
        _tft->setFont(style.font);
        _tft->setTextColor(style.color,style.fill);
        _tft->fillRoundRect(x,y,w,h,6,style.fill);
        _tft->drawRoundRect(x,y,w,h,6,style.border);
        _tft->drawRoundRect(x+1,y+1,w-2,h-2,5,style.border);
        alignText(x,y,w,h,style.alignment,text);
      }
    void AT_Display::showBoxLine      (uint16_t x, uint16_t y, uint16_t w, uint16_t h, String text, ATSTYLE  style)
      { //show a box with bottom line and centered text with given style
        _tft->setFont(style.font);
        _tft->setTextColor(style.color,style.fill);
        _tft->fillRect(x,y,w,h,style.fill);
        _tft->drawLine(x,y+h-1,x+w,y+h-1,style.border);
        alignText(x,y,w,h,style.alignment,text);
      }
    void AT_Display::showBar          (uint16_t y,    uint8_t type,uint8_t style, String title)
      {
        switch (type)
          {
            case ATBARSTATUS: showStatusBar(); break;
            case ATBARTITLECLOCK: showTitleClockBar(y,style,title); break;
            case ATBARTITLE: showTitleBar(y,style,title); break;
            case ATBARTITLEPAGE: showTitlePageBar(y,style,title); break;
            case ATBARBACK: showBackBar(y,style); break;
            case ATBARSAVECANCEL: showSaveCancelBar(y,style); break;
            case ATBARDEVICE: showDeviceBar(y,style); break;
            case ATBARSCDX: showSCDXBar(y,style,title); break;
            case ATBARWIDGET: showWidgetBar(y,style); break;
            case ATBARSYSTEM: showTitlePageBar(y,style,"System"); break;
          }
      }
    void AT_Display::showCheckbox     (uint8_t  row,  uint8_t col, uint8_t style, String value)
      { //show a checkbox formelement
        uint16_t x = col*120;
        uint16_t y = row*20+40;
        ATSTYLE stl=AT_display_styles[style];

        _tft->fillRect(x,y,20,20,stl.fill);
        _tft->drawRect(x,y,20,20,stl.border);
        _tft->drawRect(x+1,y+1,18,18,stl.border);
        if (value != "0")
          {
            _tft->drawLine(x,y,x+20,y+20,stl.border);
            _tft->drawLine(x+1,y,x+20,y+19,stl.border);
            _tft->drawLine(x,y+1,x+19,y+20,stl.border);
            _tft->drawLine(x+20,y,x,y+20,stl.border);
            _tft->drawLine(x+19,y,x,y+19,stl.border);
            _tft->drawLine(x+20,y+1,x+1,y+20,stl.border);
          }
      }
    void AT_Display::showSimpleElement(uint8_t  size, uint8_t row, uint8_t col,   uint8_t style, String value)
      { //show a simple formelement
        showBox(col*120,row*20+40,size * 120,20,value,AT_display_styles[style]);
      }
  // *** unused
    #if (AT_USE_EDIT > OFF)
        void AT_Display::editDevice(uint8_t device, uint16_t backPage)
          { //show edit form for a device
            ATDEVICE * dev = _database->getDevice(device);
            _editBuffer[0] = ATTXTNAME;
            _editBuffer[1] = dev->name;
            _device = device;
            _backPage = backPage;
            switchPage(ATPAGDEVSTP);
          }
        void AT_Display::saveDevice()
          { //save device after editiong
            ATDEVICE * dev = _database->getDevice(_device);
            dev->name = _editBuffer[1];
            _database->writeDevices();
            _database->writeConfig();
            switchPage(_backPage);
          }
        void AT_Display::deleteDevice()
          { //delete a device
            _database->deleteDevice(_device);
            _database->writeDevices();
            _database->writeConfig();
            switchPage(_backPage);
          }
        void AT_Display::editWidget(uint16_t widget, uint16_t backPage)
          { //show edit form for a widget
            ATDISPLAYWIDGET wdg;
            _curWidgetPg = widget/ATWIDGETSPERPAGE;
            _curWidgetSl = widget%ATWIDGETSPERPAGE;
            wdg = _database->getWidget(_curWidgetPg,_curWidgetSl);
            _editBuffer[0]=ATTXTLABEL;
            _editBuffer[1]=wdg.label;
            _editBuffer[2]=ATTXTSIZE;
            _editBuffer[3]=String(wdg.size);
            _editBuffer[4]=ATTXTBACKGROUND;
            _editBuffer[5]=String(wdg.bgcolor);
            _editBuffer[6]=ATTXTBGON;
            _editBuffer[7]=String(wdg.bgcolorOn);
            _editBuffer[8]=ATTXTFONT;
            _editBuffer[9]=String(wdg.fontcolor);
            _editBuffer[10]=ATTXTDECIMALS,
            _editBuffer[11]=String(wdg.precision);
            _editBuffer[12]=ATTXTPAGE,
            _editBuffer[13]=String(_curWidgetPg);
            _editBuffer[14]=ATTXTPOSAITION,
            _editBuffer[15]=String(_curWidgetSl);
            _backPage = backPage;
            switchPage(ATPAGWDGTSTP);
          }
        void AT_Display::saveWidget()
          { //save widget after editiong
            ATDISPLAYWIDGET * wdg = _database->getWidgetAdr(_curWidgetPg,_curWidgetSl);
            uint8_t newPage = _editBuffer[13].toInt();
            if (newPage > ATMAXPAGES) newPage=ATMAXPAGES;
            uint8_t newPos = _editBuffer[15].toInt();
            if (newPos > ATWIDGETSPERPAGE) newPos=ATWIDGETSPERPAGE;
            if ((newPage != _curWidgetPg) || (newPos != _curWidgetSl))
              {
                //we have a new position first check if available
                ATDISPLAYWIDGET * newwdg = _database->getWidgetAdr(newPage,newPos);
                if (newwdg->status == ATWIDGET_UNUSED)
                  {
                    newwdg->status = ATWIDGET_USED;
                    wdg->status = ATWIDGET_UNUSED;
                    newwdg->type = wdg->type;
                    newwdg->source = wdg->source;
                    newwdg->val1 = wdg->val1;
                    newwdg->color1 = wdg->color1;
                    newwdg->val2 = wdg->val2;
                    newwdg->color2 = wdg->color2;
                    newwdg->val3 = wdg->val3;
                    newwdg->color3 = wdg->color3;
                    wdg = newwdg; //to save remaining data
                  }
              }
            wdg->status = ATWIDGET_USED;
            wdg->label = _editBuffer[1];
            wdg->size = _editBuffer[3].toInt();
            wdg->bgcolor = _editBuffer[5].toInt();
            wdg->bgcolorOn = _editBuffer[7].toInt();
            wdg->fontcolor = _editBuffer[9].toInt();
            wdg->precision = _editBuffer[11].toInt();
            _database->writeConfig();
            switchPage(_backPage);
          }
        void AT_Display::deleteWidget()
          { //delete a widget
            ATDISPLAYWIDGET * wdg = _database->getWidgetAdr(_curWidgetPg,_curWidgetSl);
            wdg->status = ATWIDGET_UNUSED;
            _database->writeConfig();
            switchPage(_backPage);
          }
      #endif
    #if (AT_USE_DYNSETUP > OFF)
        //show edit form for system setup
        void AT_Display::editSystem()
          {
            ATSETUP * stp = _database->getSetup();
             _editBuffer[0]=ATTXTWLAN;
             _editBuffer[1]=ATTXTACTIVE;
             _editBuffer[2]= (stp->useWlan)?"1":"0";
             _editBuffer[3]=ATTXTNETWORK;
             _editBuffer[4]=stp->SSID;
             _editBuffer[5]=ATTXTPASSWORD;
             _editBuffer[6]=stp->password;
             _editBuffer[7]=ATTXTNTPSERVER;
             _editBuffer[8]=stp->NTPserver;
             _editBuffer[9]=ATTXTINTERVAL;
             _editBuffer[10]=stp->refresh;
             switchPage(ATPAGSYSSTP);
          }
        //save system after editing
        void AT_Display::saveSystem()
          {
            ATSETUP * stp = _database->getSetup();
            stp->useWlan = _editBuffer[2] == "1";
            stp->SSID = _editBuffer[4];
            stp->password = _editBuffer[6];
            stp->NTPserver = _editBuffer[8];
            stp->refresh = _editBuffer[10].toInt();
            _database->writeSetup();
            if (_onSystemChange) _onSystemChange();
            switchPage(ATPAGRESULTS);
          }
      #endif
  // *** divers
    uint8_t AT_Display::getColorIndex   (uint16_t color)
      { //return the color index in palette
        uint8_t index = 0;
        while ((index < 16) && (AT_palette[index] != color)) index++;
        if (index >= 16) index = 0;
        return index;
      }
    void    AT_Display::registerNewDevice()
      { // register new device -> database, open conf page
        int8_t dev = _database->registerDev();
        if (dev >= 0)
          {
            _status = "";
            #if (AT_USE_DYNSETUP > OFF)
                editDevice(dev,ATPAGRESULTS);
              #endif
          }
      }
    void    AT_Display::alignText       (uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t alignment, String text)
      { //center text a height of 0 means center horizontal only
        uint16_t w1,h1,xx;
        int16_t x1,y1,hx;
        _tft->getTextBounds(encodeUnicode(text),x,y,&x1,&y1,&w1,&h1);
        hx = y-y1;
        xx = 0;
        switch (alignment)
          {
            case ATALIGNCENTER : xx = x+(w-w1)/2; break;
            case ATALIGNLEFT : xx = x+4; break;
            case ATALIGNRIGHT : xx = x + w - w1-4;
          }
        if (h > 0)
          {
            _tft->setCursor(xx,y+(h-hx)/2+hx);
          }
        else
          {
            _tft->setCursor(xx/2,y);
          }
        print(text);
      }
    String  AT_Display::encodeUnicode   (String text)
      { //encode extra character from unicode for display
        //while in keyboard mode we dont need encoding
        if (_edType == ATEDTTEXT) return text;
        String res = "";
        uint8_t i = 0;
        char c;
        while (i<text.length())
          {
            c=text[i];
            if (c==195)
              { //UTF8 characters German Umlaute
                i++;
                switch (text[i])
                  {
                    case 164: c=130; break; //ä
                    case 182: c=131; break; //ö
                    case 188: c=132; break; //ü
                    case 159: c=133; break; //ß
                    case 132: c=127; break; //Ä
                    case 150: c=128; break; //Ö
                    case 156: c=129; break; //Ü
                    default: c=0;
                  }
              }
            else if (c == 194)
              { //UTF8 char for Degrees symbol
                i++;
                if (text[i] == 176)
                  c=134;
                else
                  c=0;
              }
            else if (c > 128)
              { //normal characters unchanged
                c=0;
              }
            if (c>0) res.concat(c);
            i++;
          }
        return res;
      }
    String  AT_Display::fromKeyboard    (String text)
      { //convert keyboard entered text into UTF8
        String res = "";
        char c;
        for (uint8_t i=0; i<text.length(); i++)
          {
            c = text[i];
            if (c > 126)
              { //it is a special character
                if (c==134)
                  {
                    res.concat("°");
                  }
                else
                  {
                    switch (c)
                      {
                        case 130: res.concat("ä"); break;
                        case 131: res.concat("ö"); break;
                        case 132: res.concat("ü"); break;
                        case 133: res.concat("ß"); break;
                        case 127: res.concat("Ä"); break;
                        case 128: res.concat("Ö"); break;
                        case 129: res.concat("Ü"); break;
                      }
                  }
              }
            else
              { //normales Zeichen
                res.concat(c);
              }
          }
        return res;
      }
    void    AT_Display::print           (String text)
      { //output a string to tft
        _tft->print(encodeUnicode(text));
      }
  // *** callbacks
    void AT_Display::registerOnSystemChanged(void (*callback)())
      { //this callback will be called if system setup has changed
        _onSystemChange = callback;
      }
    void AT_Display::registerOnResultChange(void (*callback)(uint16_t index))
      { //register a callback function result change event
        _onResultChange = callback;
      }
    void AT_Display::onClick(TS_Point p)
      { //callback for click events
        char tmp[30];
        sprintf(tmp," x=%i y=%1",p.x,p.y);
        SVAL(" onClick ",tmp);
        setStatus(tmp);
        if (_edType > 0)
          {
            editorClick(p);
          }
        else
          {
            if((p.y <= 20) || (p.y >= 295))
              { clickBar(p.y >= 280,p); }
            else
              {
                clickContent(p);
              }
          }
      }
    void AT_Display::onLongClick(TS_Point p)
      { //callback for longclick events
        switch (_curPage)
          {
            case ATPAGRESULTS:
              if ((p.y > 280) && _database->hasNewDevice()) registerNewDevice();
              if ((p.y > 40) && (p.y < 280))
                {
                  int16_t index = findWidget((p.y-40)/30,p.x/120);
                  //Serial.printf("Pos x=%i y=%i widget %i subpage %i \n",p.x, p.y, index, _subpage);
                  #if (AT_USE_DYNSETUP > OFF)
                      if ((index >= 0) && (index < ATWIDGETSPERPAGE))
                         { editWidget(_subpage*ATWIDGETSPERPAGE+index,ATPAGRESULTS); }
                    #endif
                }
              break;
          }
      }
    void AT_Display::onSwipe(uint8_t direction)
      { //callback for click events
        uint8_t sub = _subpage;
        if ((direction %2) == 0)
          { sub++; }
        else
          { if (sub > 0)
              { sub--; } }
        if (sub < AT_display_pages[_curPage].subpages)
          { switchSubPage(sub); }
      }
  // *** form editors
    void AT_Display::editorOff()
      { //close editor and update editbuffer
        switch (_edType)
          {
            case ATEDTTEXT: _editBuffer[_curElement]=fromKeyboard(_edvaltxt); break;
            case ATEDTINT: _editBuffer[_curElement]=_edvaltxt; break;
            case ATEDTFLOAT: _editBuffer[_curElement]=_edvaltxt; break;
            case ATEDTCOLOR: _editBuffer[_curElement]=String(_edvalint); break;
            case ATEDTSELECT: _editBuffer[_curElement]=String(_edvalint); break;
            case ATEDTSSID: _editBuffer[_curElement]=_edvaltxt; break;
          }
        _edType = 0;
        _tft->fillScreen(ATblack);
        showCurrentPage();
      }
    void AT_Display::edKbdOn(uint8_t element)
      { //switch keyboard on
        _curElement = element;
        _kbdlevel = 0;
        _edvaltxt = encodeUnicode(_editBuffer[element]);
        _edType = ATEDTTEXT;
        _tft->fillScreen(ATblack);
        edKbdUpdate();
        edKbdShow();
      }
    void AT_Display::edKbdShow()
      { //draw the keyboard area
        for (uint8_t i = 1; i<8; i++)
          {
            for (uint8_t j = 0; j<6; j++)
              {
                if ((i==7) && ((j==2) || (j == 3)))
                  {
                    if (j==2) showRoundedBox(j*40+1,i*40+1,78,38," ",AT_display_styles[ATSTYLEKBDKEY]);
                  }
                else
                  {
                    showRoundedBox(j*40+1,i*40+1,38,38,String(keys[_kbdlevel][(i-1)*6+j]),AT_display_styles[ATSTYLEKBDKEY]);
                  }
              }
          }
      }
    void AT_Display::edKbdUpdate()
      { //redraw the input area
        showBox(0,0,240,40,_edvaltxt,AT_display_styles[ATSTYLEKBDINP]);
      }
    void AT_Display::edColorOn(uint8_t element)
      { //switch color editor on
        _curElement = element;
        _edvalint = _editBuffer[element].toInt();
        _edType = ATEDTCOLOR;
        _tft->fillScreen(ATblack);
        edColorUpdate();
        edColorShow();
      }
    void AT_Display::edColorUpdate()
      { //redraw the input area
        _tft->fillRect(70,40,100,30,_edvalint);
        _tft->drawRect(70,40,100,30,ATwhite);
      }
    void AT_Display::edColorShow()
      { //show a selector to select a color out of 16
        for (uint8_t y =0; y<8; y++)
          {
            for (uint8_t x = 0; x<8; x++)
              {
                _tft->fillRect(x*20+40,y*20+80,20,20,AT_palette[y*8+x]);
                _tft->drawRect(x*20+40,y*20+80,20,20,ATwhite);
              }
          }
        showRoundedBox(70,280,100,30,ATTXTDONE,AT_display_styles[ATSTYLEBUTTON]);
      }
    void AT_Display::edNumPadOn(uint8_t element, uint8_t editType)
      { //switch numeric editor on
        _curElement = element;
        _edvaltxt = _editBuffer[element];
        _edType = editType;
        _tft->fillScreen(ATblack);
        edNumPadUpdate();
        edNumPadShow();
      }
    void AT_Display::edNumPadUpdate()
      { //redraw the input area
        showBox(40,20,160,40,_edvaltxt,AT_display_styles[ATSTYLEKBDINP]);
      }
    void AT_Display::edNumPadShow()
      { //show a num pad
        for (uint8_t i = 0; i<3; i++)
          {
            for (uint8_t j = 0; j<3; j++)
              {
                showRoundedBox(40+i*40+1,80+j*40+1,38,38,String(char((2-j)*3+i+49)),AT_display_styles[ATSTYLEKBDKEY]);
              }
          }
        showRoundedBox(41,201,38,38,".",AT_display_styles[ATSTYLEKBDKEY]);
        showRoundedBox(81,201,38,38,"0",AT_display_styles[ATSTYLEKBDKEY]);
        showRoundedBox(121,201,38,38,"-",AT_display_styles[ATSTYLEKBDKEY]);
        showRoundedBox(161,81,38,78,String(char(31)),AT_display_styles[ATSTYLEKBDKEY]);
        showRoundedBox(161,161,38,78,String(char(28)),AT_display_styles[ATSTYLEKBDKEY]);
      }
    void AT_Display::edSelectorOn(uint8_t element)
      { //switch selector list editor on
        _curElement = element;
        _edvalint = _editBuffer[element].toInt();
        _edType = ATEDTSELECT;
        _tft->fillScreen(ATblack);
        edSelectorShow();
      }
    void AT_Display::edSelectorShow()
      { //show selector list
        uint8_t sl;
        for (uint8_t i = 0; i < _curForm->elements[_curElement].optcnt; i++)
          {
            sl = (i == _edvalint)?ATSTYLESELECTED:ATSTYLEOPTIONS;
            showBox(0,i*20,240,20,_curForm->elements[_curElement].optlist[i],AT_display_styles[sl]);
          }
      }
    void AT_Display::edSSIDOn(uint8_t element)
      { //switch selector list editor on
        _curElement = element;
        _edvaltxt = _editBuffer[element];
        _edType = ATEDTSSID;
        _tft->fillScreen(ATblack);
        #if (USE_ATWIFI > OFF)
            WiFi.disconnect();
            int n = WiFi.scanNetworks();
            Serial.println("scan done");
            if (n == 0)
              {
                _edOptCnt=0;
                showBox(0,40,240,20,ATTXTNOWLAN,AT_display_styles[ATSTYLESELECTED]);
              }
            else
              {
                _edOptCnt = n;
                edSSIDShow();
              }
          #endif
      }
    void AT_Display::edSSIDShow()
      { //show a num pad
        #if (USE_ATWIFI > OFF)
            uint8_t sl;
            for (int i = 0; i < _edOptCnt; ++i)
              {
                _edOptions[i] = WiFi.SSID(i);
                sl=(_edvaltxt == _edOptions[i])?ATSTYLESELECTED:ATSTYLEOPTIONS;
                showBox(0,i*20,240,20,_edOptions[i],AT_display_styles[sl]);
                delay(10);
              }
          #endif
      }
  // *** private functions for touch events
    void    AT_Display::switchPage        (uint8_t  page)
      {
        clearContent(ATblack);
        _curPage = page;
        _subpage = 0;
        showCurrentPage();
      }
    void    AT_Display::switchSubPage     (uint8_t  subpage)
      {
        if (subpage >= AT_display_pages[_curPage].subpages) return;
        clearContent(ATblack);
        _subpage = subpage;
        showCurrentPage();
      }
    int16_t AT_Display::findWidgetSource  (uint8_t  line,  uint8_t  column)
      {
        int16_t index = findWidget(line,column);
        ATDISPLAYPAGE pg = _database->getPage(_subpage);
        return pg.widgets[index].source;
      }
    int16_t AT_Display::findWidget        (uint8_t  line,  uint8_t  column)
      {
        int16_t result = -1;
        ATDISPLAYPAGE pg;
        ATDISPLAYWIDGET wdg;
        pg = _database->getPage(_subpage);
        wdg = pg.widgets[line];
        switch (wdg.size)
          {
            case ATWIDGET_SMALL: result = line; break;
            case ATWIDGET_LEFT:
              if (column == 0)
                {
                  result = line;
                }
              else
                {
                  result = line-1;
                }
              break;
            case ATWIDGET_RIGHT:
              if (column == 0)
                { result = line+1; }
              else
                { result = line; }
              break;
            case ATWIDGET_BIG: result = line; break;
            case ATWIDGET_BIG1: result = line-1; break;
          }
        return result;
      }
    int16_t AT_Display::findFormElement   ( uint8_t line,  uint8_t  column)
      {
        int16_t i = _curForm->elementCnt - 1;
        while ((i>=0) && !(   (    _curForm->elements[i].row  == line)
                           && (   (_curForm->elements[i].size == 2)
                               || (_curForm->elements[i].col  == column)
                              )
                          )
              )
          { i--; }
        return i;
      }
    void    AT_Display::clickSaveCancelBar(boolean  left)
      {
        if (left)
          {
            #if (AT_USE_DYNSETUP > OFF)
                switch (_curPage)
                  {
                    case ATPAGDEVSTP:
                      saveDevice();
                      break;
                    case ATPAGSYSSTP:
                      saveSystem();
                      break;
                  }
              #endif
          }
        else
          {
            switchPage(AT_display_pages[_curPage].previousPage);
          }
      }
    void    AT_Display::clickSCDXBar      (boolean  left,  boolean  top)
      {
        if (top)
          {
            if (left)
              {
                switch (_curPage)
                  {
                    case ATPAGDEVSTP:
                      switchPage(ATPAGCHANNEL);
                  }
              }
            else
              { switchPage(_backPage); }
          }
        else
          {
            if (left)
              {
                #if (AT_USE_DYNSETUP > OFF)
                    switch (_curPage)
                      {
                        case ATPAGDEVSTP:
                          saveDevice();
                          break;
                        case ATPAGWDGTSTP:
                          saveWidget();
                          break;
                      }
                  #endif
              }
            else
              {
                #if (AT_USE_DYNSETUP > OFF)
                    switch (_curPage)
                      {
                        case ATPAGDEVSTP:
                          deleteDevice();
                          break;
                        case ATPAGWDGTSTP:
                          deleteWidget();
                          break;
                      }
                  #endif
                switchPage(_backPage);
              }
          }
      }
    void    AT_Display::clickBar          (boolean  bottom,TS_Point p)
      {
        uint16_t y = (bottom)?p.y-280:p.y;
        uint8_t typ = (bottom)?AT_display_pages[_curPage].botbarType:AT_display_pages[_curPage].topbarType;
        switch (typ)
          {
            case ATBARBACK: switchPage(AT_display_pages[_curPage].previousPage); break;
            case ATBARSTATUS: switchPage(ATPAGDEVICE); break;
            case ATBARSAVECANCEL: clickSaveCancelBar(p.x < 120); break;
            case ATBARSCDX: clickSCDXBar(p.x < 120, y < 20); break;
            #if (AT_USE_DYNSETUP > OFF)
                case ATBARTITLECLOCK: editSystem(); break;
              #endif
          }
      }
    void    AT_Display::clickResults      (uint8_t  line,  uint8_t  column)
      {
        int16_t index = findWidgetSource(line,column);
        if (index >= 0)
          {
            ATCURVALUES res = _database->getResult(index);
            if (res.type == ATTYPE_SWITCHOUT)
              {
                _database->toggleResult(index);
                if (_onResultChange) _onResultChange(index);
              }
          }
      }
    void    AT_Display::clickList         (uint8_t  line)
      {
        switch (_curPage)
          {
            case ATPAGDEVICE:
              #if (AT_USE_DYNSETUP > OFF)
                  editDevice(_subpage*8+line, AT_display_pages[_curPage].previousPage);
                #endif
              break;
            case ATPAGCHANNEL:
              int16_t wdg = _database->findWidgetBySource(_device*ATMAXDEVCHAN+line);
              if (wdg < 0)
                { wdg = _database->getFreeSlot(ATWIDGET_SMALL); }
              if (wdg >= 0)
                {
                  _database->setWidgetSource(wdg,_device*ATMAXDEVCHAN+line);
                  #if (AT_USE_DYNSETUP > OFF)
                      editWidget(wdg, AT_display_pages[_curPage].previousPage);
                    #endif
                  break;
                }
          }
      }
    void    AT_Display::clickForm         (uint8_t  line,  uint8_t  column, uint16_t xPos)
      {
        int16_t element = findFormElement(line,column);
        if (element < 0) return;
        switch (_curForm->elements[element].type) {
          case ATFRMTEXT: edKbdOn(element); break;
          case ATFRMCOLOR: edColorOn(element); break;
          case ATFRMFLOAT: edNumPadOn(element,ATEDTFLOAT); break;
          case ATFRMINT: edNumPadOn(element, ATEDTINT); break;
          case ATFRMSELECT: edSelectorOn(element); break;
          case ATFRMSSID: edSSIDOn(element); break;
          case ATFRMCHECK: _editBuffer[element] = (_editBuffer[element]=="0")?"1":"0";
            showCheckbox(_curForm->elements[element].row,_curForm->elements[element].col,_curForm->elements[element].style,_editBuffer[element]);
            break;
        }
      }
    void    AT_Display::clickContent      (TS_Point p)
      {
        uint8_t content = AT_display_pages[_curPage].content;
        uint8_t line, column;
        column = (p.x >= 120)?1:0;
        line = (content == ATCONTRESULTS)?(p.y-40)/30:(p.y-40)/20;
        Serial.printf("x = %i y= %i line = %i column = %i \n",p.x,p.y,line,column);
        switch (content)
          {
            case ATCONTRESULTS: clickResults(line,column); break;
            case ATCONTLIST   : clickList(line);break;
            case ATCONTFORM   : clickForm(line,column,p.x);break;
          }
      }
    void    AT_Display::editorClick       (TS_Point p)
      {
        switch (_edType)
          {
            case ATEDTTEXT: edKbdClick(p); break;
            case ATEDTINT: numPadClick(p,false); break;
            case ATEDTFLOAT: numPadClick(p,true); break;
            case ATEDTCOLOR: edColorClick(p); break;
            case ATEDTSELECT: edSelectClick(p); break;
            case ATEDTSSID: edSSIDClick(p); break;
          }
      }
    void    AT_Display::edKbdClick        (TS_Point p)
      {
        uint8_t r,c,index;
        char ch;
        if (p.y>40) //below input area
          {
            //wir ermitteln die Zeile und Spalte der Taste
            r = (p.y-40) / 40;
            c = (p.x) / 40;
            //index into keys array
            index = r*6+c;
            //Wert der berührten Taste
            ch = keys[_kbdlevel][index];
            if (ch > 31)
              {
                // above 31 are valid characters to add to input text
                _edvaltxt += ch; //und geben den neuen Text aus
                edKbdUpdate();
              }
            else
              {
                //we have a control key
                switch (ch)
                  {
                    case 26: _kbdlevel = 1; //switch to capitals
                      edKbdShow();
                      break;
                    case 27: _kbdlevel = 0; //switch to normal
                      edKbdShow();
                      break;
                    case 29: _kbdlevel = 0; //switch to normal
                      edKbdShow();
                      break;
                    case 30: _kbdlevel = 2; //switch to signs and numbers
                      edKbdShow();
                      break;
                    case 31: _edvaltxt.remove(_edvaltxt.length()-1); //delete rightmost character
                      edKbdUpdate();
                      break;
                    case 28: editorOff(); //switch keyboard off
                      break;
                  }
              }
          }
      }
    void    AT_Display::numPadClick       (TS_Point p,     boolean  isFloat)
      {
        uint8_t r,c;
        int8_t num;
        boolean update = true;
        if ((p.y>80) && (p.y <240) && (p.x > 40) && (p.x < 200)) //num pad area
          {
            r = (p.y-80) / 40;
            c = (p.x-40) / 40;
            if ((c < 3) && (r < 3))
              {
                num = (2-r) * 3 + 1 + c;
                _edvaltxt += char(48+num);
              }
            else if ((r == 3) && (c<3))
              {
                if (c == 0)
                  {
                    if (isFloat && (_edvaltxt.indexOf('.') < 0))
                      { _edvaltxt += "."; }
                  }
                else if (c == 1)
                  {
                    _edvaltxt += "0";
                  }
                else
                  {
                    if (_edvaltxt.length()==0) _edvaltxt="-";
                  }
              }
            else
              {
                if (r<2)
                  {
                    _edvaltxt.remove(_edvaltxt.length()-1);
                  }
                else
                  {
                    editorOff();
                    update = false;
                  }
              }
            if (update) edNumPadUpdate();
          }
      }
    void    AT_Display::edColorClick      (TS_Point p)
      {
        Serial.println("COLOR");
        if ((p.x>40) && (p.x<200) && (p.y > 80) && (p.y<240))
          {
            uint16_t ix=((p.y-80)/20) * 8 + (p.x-40)/20;
            _edvalint = AT_palette[ix];
            edColorUpdate();
          }
        if ((p.x>70) && (p.x<170) && (p.y > 280) && (p.y<310))
          { editorOff(); }
      }
    void    AT_Display::edSelectClick     (TS_Point p)
      {
        uint8_t index = p.y / 20;
        if (index < _curForm->elements[_curElement].optcnt)
          {
            _edvalint = index;
            editorOff();
          }
      }
    void    AT_Display::edSSIDClick       (TS_Point p)
      {
        uint8_t index = p.y / 20;
        if (index < _edOptCnt)
          {
            _edvaltxt = _edOptions[index];
            editorOff();
          }
      }
