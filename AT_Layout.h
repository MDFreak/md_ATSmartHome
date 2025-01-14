/* Defines Styles, Pages and Forms for AT Smarthome
  version 0.5
  */
#ifndef _AT_LAYOUT_H_
  #define _AT_LAYOUT_H_

  #include "fonts\AT_Bold12pt7b.h"
  #include "fonts\AT_BoldOblique9pt7b.h"
  #include "fonts\AT_Oblique9pt7b.h"
  #include "fonts\AT_Standard9pt7b.h"

  #define ATSTYLEBLUEYELLOW 0
  #define ATSTYLEDEVLIST    1
  #define ATSTYLEBUTTON     2
  #define ATSTYLEFRMLBL     3
  #define ATSTYLEFRMINPUT   4
  #define ATSTYLESMALLTITLE 5
  #define ATSTYLEKBDKEY     6
  #define ATSTYLEKBDINP     7
  #define ATSTYLECHLLIST    8
  #define ATSTYLECHLLISTACT 9
  #define ATSTYLEOPTIONS    10
  #define ATSTYLESELECTED   11
  #define ATSTYLEFRMLBLCTR  12
  #define ATSTYLEMAINTITLE  13
  #define ATSTYLECLOCK      14
  #define ATSTYLEPAGE       15
  #define ATSTYLESTATIDLEL  16
  #define ATSTYLESTATIDLER  17
  #define ATSTYLESTATOK     18
  #define ATSTYLESTATWARN   19
  #define ATSTYLESTATERR    20
  const ATSTYLE AT_display_styles[] =
    {
      { //ATSTYLEBLUEYELLOW
          .fill = ATblue,
          .border = ATblue,
          .color = ATyellow,
          .alignment = ATALIGNCENTER,
          .font = &AT_Bold12pt7b
        },
      { //ATSTYLEDEVLIST
          .fill = ATyellow,
          .border = ATblack,
          .color = ATblack,
          .alignment = ATALIGNLEFT,
          .font = &AT_Standard9pt7b
        },
      { //ATSTYLEBUTTON
          .fill = ATgreen,
          .border = ATblack,
          .color = ATblack,
          .alignment = ATALIGNCENTER,
          .font = &AT_Standard9pt7b
        },
      { //ATSTYLEFRMLBL
            .fill = ATblack,
            .border = ATblack,
            .color = ATwhite,
            .alignment = ATALIGNRIGHT,
            .font = &AT_Standard9pt7b
        },
      { //ATSTYLEFRMINPUT
          .fill = ATwhite,
          .border = ATblue,
          .color = ATblack,
          .alignment = ATALIGNLEFT,
          .font = &AT_Standard9pt7b
        },
      { //ATSTYLESMALLTITLE
          .fill = ATblue,
          .border = ATblue,
          .color = ATyellow,
          .alignment = ATALIGNCENTER,
          .font = &AT_Standard9pt7b
        },
      { //ATSTYLEKBDKEY
          .fill = ATlightgray,
          .border = ATblue,
          .color = ATblack,
          .alignment = ATALIGNCENTER,
          .font = &AT_Bold12pt7b
        },
      { //ATSTYLEKBDINP
          .fill = ATwhite,
          .border = ATblack,
          .color = ATblack,
          .alignment = ATALIGNLEFT,
          .font = &AT_Standard9pt7b
        },
      { //ATSTYLECHLLIST
          .fill = ATwhite,
          .border = ATdarkgray,
          .color = ATblack,
          .alignment = ATALIGNLEFT,
          .font = &AT_Standard9pt7b
        },
      { //ATSTYLECHLLISTACT
          .fill = ATyellow,
          .border = ATdarkgray,
          .color = ATblack,
          .alignment = ATALIGNLEFT,
          .font = &AT_Standard9pt7b
        },
      { //ATSTYLEOPTIONS
          .fill = ATwhite,
          .border = ATdarkgray,
          .color = ATblack,
          .alignment = ATALIGNCENTER,
          .font = &AT_Standard9pt7b
        },
      { //ATSTYLESELECTED
          .fill = ATblue,
          .border = ATyellow,
          .color = ATwhite,
          .alignment = ATALIGNCENTER,
          .font = &AT_Standard9pt7b
        },
      { //ATSTYLEFRMLBLCTR
          .fill = ATblack,
          .border = ATblack,
          .color = ATwhite,
          .alignment = ATALIGNCENTER,
          .font = &AT_Standard9pt7b
        },
      { //ATSTYLEMAINTITLE
          .fill = ATlime,
          .border = ATlime,
          .color = ATblack,
          .alignment = ATALIGNCENTER,
          //.font = &AT_Standard9pt7b
          //.font = &AT_BoldOblique9pt7b
          .font = &AT_Bold12pt7b
        },
      { //ATSTYLECLOCK
          .fill = ATlime,
          .border = ATlime,
          .color = ATblack,
          .alignment = ATALIGNLEFT,
          .font = &AT_Standard9pt7b
        },
      { //ATSTYLEPAGE
          .fill = ATlime,
          .border = ATlime,
          .color = ATblack,
          .alignment = ATALIGNRIGHT,
          .font = &AT_Standard9pt7b
        },
      { //ATSTYLESTATIDLEL
          .fill = ATlightgray, // ATlime,
          .border = ATlime,
          .color = ATblack,
          .alignment = ATALIGNLEFT,
          .font = &AT_Standard9pt7b
        },
      { //ATSTYLESTATIDLER
          .fill = ATlightgray, // ATlime,
          .border = ATlime,
          .color = ATblack,
          .alignment = ATALIGNRIGHT,
          .font = &AT_Standard9pt7b
        },
      { //ATSTYLESTATOK
          .fill = ATlime,
          .border = ATlime,
          .color = ATblack,
          .alignment = ATALIGNCENTER,
          .font = &AT_Standard9pt7b
        },
      { //ATSTYLESTATWARN
          .fill = ATyellow,
          .border = ATred,
          .color = ATblack,
          .alignment = ATALIGNCENTER,
          .font = &AT_Standard9pt7b
        },
      { //ATSTYLESTATERR
          .fill = ATdarkred,       // ATlime,
          .border = ATred, // lime,
          .color = ATyellow,
          .alignment = ATALIGNCENTER,
          .font = &AT_Standard9pt7b
        }
    };

  #define ATPAGRESULTS 0
  #define ATPAGDEVICE 1
  #define ATPAGCHANNEL 2
  #define ATPAGDEVSTP 3
  #define ATPAGWDGTSTP 4
  #define ATPAGSYSSTP 5
  #define ATPAGREGISTER 6
  #define ATPAGTRIGGER 7
  #define ATPAGTRIGSTP 8
  const ATPAGETYPE AT_display_pages[8] =
    {
      { //ATPAGRESULTS
          { .title = ATTXTSMARTHOME },
          .subpages = 32,
          .content = ATCONTRESULTS,
          //.topbarType = ATBARTITLECLOCK,
          .topbarType = ATBARTITLE,
          .topbarStyle = ATSTYLEMAINTITLE,
          .botbarType = ATBARSTATUS,
          .botbarStyle = ATSTYLESTATOK,   //ATSTYLEBLUEYELLOW,
          { .botbarText = "" },
          .previousPage = ATPAGRESULTS
        },
      { //ATPAGDEVICE
          { .title = ATTXTDEVICES },
          .subpages = 3,
          .content = ATCONTLIST,
          .topbarType = ATBARTITLEPAGE,
          .topbarStyle = ATSTYLEBLUEYELLOW,
          .botbarType = ATBARBACK,
          .botbarStyle = ATSTYLEBUTTON,
          { .botbarText = "" },
          .previousPage = ATPAGRESULTS
        },
      { //ATPAGCHANNEL
          { .title = ATTXTCHANNELS },
          .subpages = 1,
          .content = ATCONTLIST,
          .topbarType = ATBARTITLEPAGE,
          .topbarStyle = ATSTYLEBLUEYELLOW,
          .botbarType = ATBARBACK,
          .botbarStyle = ATSTYLEBUTTON,
          { .botbarText = "" },
          .previousPage = ATPAGDEVICE
        },
      { //ATPAGDEVSTP
          { .title = ATTXTDEVSTP },
          .subpages = 1,
          .content = ATCONTFORM,
          .topbarType = ATBARDEVICE,
          .topbarStyle = ATSTYLESMALLTITLE,
          .botbarType = ATBARSCDX,
          .botbarStyle = ATSTYLEBUTTON,
          { .botbarText = ATTXTDETAILS },
          .previousPage = ATPAGDEVICE
        },
      { //ATPAGWDGTSTP
          { .title = ATTXTWDGSTP },
          .subpages = 1,
          .content = ATCONTFORM,
          .topbarType = ATBARWIDGET,
          .topbarStyle = ATSTYLESMALLTITLE,
          .botbarType = ATBARSCDX,
          .botbarStyle = ATSTYLEBUTTON,
          { .botbarText = ATTXTEXTRA },
          .previousPage = ATPAGCHANNEL
        },
      { //ATPAGSYSTSTP
          { .title = ATTXTSYSSTP },
          .subpages = 1,
          .content = ATCONTFORM,
          .topbarType = ATBARSYSTEM,
          .topbarStyle = ATSTYLESMALLTITLE,
          .botbarType = ATBARSAVECANCEL,
          .botbarStyle = ATSTYLEBUTTON,
          { .botbarText = "" },
          .previousPage = ATPAGRESULTS
        },
    };
  const ATFORM AT_devfrm =
    {
      .elementCnt = 2,
      .elements =
        {
          { // 1 0 ATFRMLABEL - ATSTYLEFRMLBLCTR
              .type = ATFRMLABEL,
              .size = 2,
              .row = 1,
              .col = 0,
              .style = ATSTYLEFRMLBLCTR,
              .optcnt = 0,
              .optlist = {}
            },
          { // 2 0 ATFRMTEXT - ATSTYLEFRMINPUT
              .type = ATFRMTEXT,
              .size = 2,
              .row = 2,
              .col = 0,
              .style = ATSTYLEFRMINPUT,
              .optcnt = 0,
              .optlist = {}
            },

        }
    };

  const ATFORM AT_wdgfrm =
    {
      .elementCnt = 16,
      .elements =
        {
          { // 0 0 ATFRMLABEL - ATSTYLEFRMLBLCTR
              .type = ATFRMLABEL,
              .size = 2,
              .row = 0,
              .col = 0,
              .style = ATSTYLEFRMLBLCTR,
              .optcnt = 0,
              .optlist = {}
            },
          { // 1 0 ATFRMTEXT - ATSTYLEFRMINPUT
              .type = ATFRMTEXT,
              .size = 2,
              .row = 1,
              .col = 0,
              .style = ATSTYLEFRMINPUT,
              .optcnt = 0,
              .optlist = {}
            },
          { // 2 0 ATFRMLABEL - ATSTYLEFRMLBL
              .type = ATFRMLABEL,
              .size = 1,
              .row = 2,
              .col = 0,
              .style = ATSTYLEFRMLBL,
              .optcnt = 0,
              .optlist = {}
            },
          { // 2 1 ATFRMSELECT - ATSTYLEFRMINPUT
              .type = ATFRMSELECT,
              .size = 1,
              .row = 2,
              .col = 1,
              .style = ATSTYLEFRMINPUT,
              .optcnt = 4,
              .optlist = {ATTXTWDSMALL,ATTXTWDLEFT,ATTXTWDRIGHT,ATTXTWDBIG}
            },
          { // 3 0 ATFRMLABEL - ATSTYLEFRMLBL
              .type = ATFRMLABEL,
              .size = 1,
              .row = 3,
              .col = 0,
              .style = ATSTYLEFRMLBL,
              .optcnt = 0,
              .optlist = {}
            },
          { // 3 1 ATFRMCOLOR
              .type = ATFRMCOLOR,
              .size = 1,
              .row = 3,
              .col = 1,
              .style = 0,
              .optcnt = 0,
              .optlist = {}
            },
          { // 4 0 ATFRMLABEL - ATSTYLEFRMLBL
              .type = ATFRMLABEL,
              .size = 1,
              .row = 4,
              .col = 0,
              .style = ATSTYLEFRMLBL,
              .optcnt = 0,
              .optlist = {}
            },
          { // 4 1 ATFRMCOLOR
              .type = ATFRMCOLOR,
              .size = 1,
              .row = 4,
              .col = 1,
              .style = 0,
              .optcnt = 0,
              .optlist = {}
            },
          { // 5 0 ATFRMLABEL - ATSTYLEFRMLBL
              .type = ATFRMLABEL,
              .size = 1,
              .row = 5,
              .col = 0,
              .style = ATSTYLEFRMLBL,
              .optcnt = 0,
              .optlist = {}
            },
          { // 5 1 ATFRMCOLOR
              .type = ATFRMCOLOR,
              .size = 1,
              .row = 5,
              .col = 1,
              .style = 0,
              .optcnt = 0,
              .optlist = {}
            },
          { // 6 0 ATFRMLABEL - ATSTYLEFRMLBL
              .type = ATFRMLABEL,
              .size = 1,
              .row = 6,
              .col = 0,
              .style = ATSTYLEFRMLBL,
              .optcnt = 0,
              .optlist = {}
            },
          { // 6 1 ATFRMINT - ATSTYLEFRMINPUT
              .type = ATFRMINT,
              .size = 1,
              .row = 6,
              .col = 1,
              .style = ATSTYLEFRMINPUT,
              .optcnt = 0,
              .optlist = {}
            },
          { // 7 0 ATFRMLABEL - ATFRMLABEL
              .type = ATFRMLABEL,
              .size = 1,
              .row = 7,
              .col = 0,
              .style = ATSTYLEFRMLBL,
              .optcnt = 0,
              .optlist = {}
            },
          { // 7 1 ATFRMINT - ATSTYLEFRMINPUT
              .type = ATFRMINT,
              .size = 1,
              .row = 7,
              .col = 1,
              .style = ATSTYLEFRMINPUT,
              .optcnt = 0,
              .optlist = {}
            },
          { // 8 0 ATFRMLABEL - ATSTYLEFRMLBL
              .type = ATFRMLABEL,
              .size = 1,
              .row = 8,
              .col = 0,
              .style = ATSTYLEFRMLBL,
              .optcnt = 0,
              .optlist = {}
            },
          { // 8 1 ATFRMINT - ATSTYLEFRMINPUT
              .type = ATFRMINT,
              .size = 1,
              .row = 8,
              .col = 1,
              .style = ATSTYLEFRMINPUT,
              .optcnt = 0,
              .optlist = {}
            },
        }
    };
  const ATFORM AT_sysfrm =
    {
      .elementCnt = 11,
      .elements =
        {
          { // 0 0 ATFRMLABEL - ATSTYLEFRMLBLCTR
              .type = ATFRMLABEL,
              .size = 2,
              .row = 0,
              .col = 0,
              .style = ATSTYLEFRMLBLCTR,
              .optcnt = 0,
              .optlist = {}
            },
          { // 1 0 ATFRMLABEL - ATSTYLEFRMLBL
              .type = ATFRMLABEL,
              .size = 1,
              .row = 1,
              .col = 0,
              .style = ATSTYLEFRMLBL,
              .optcnt = 0,
              .optlist = {}
            },
          { // 1 1 ATFRMCHECK - ATSTYLEFRMINPUT
              .type = ATFRMCHECK,
              .size = 1,
              .row = 1,
              .col = 1,
              .style = ATSTYLEFRMINPUT,
              .optcnt = 0,
              .optlist = {}
            },
          { // 2 0 ATFRMLABEL - ATSTYLEFRMLBL
              .type = ATFRMLABEL,
              .size = 1,
              .row = 2,
              .col = 0,
              .style = ATSTYLEFRMLBL,
              .optcnt = 0,
              .optlist = {}
            },
          { // 2 1 ATFRMSSID  - ATSTYLEFRMINPUT
              .type = ATFRMSSID,
              .size = 1,
              .row = 2,
              .col = 1,
              .style = ATSTYLEFRMINPUT,
              .optcnt = 0,
              .optlist = {}
            },
          { // 3 0 ATFRMLABEL - ATSTYLEFRMLBL
              .type = ATFRMLABEL,
              .size = 1,
              .row = 3,
              .col = 0,
              .style = ATSTYLEFRMLBL,
              .optcnt = 0,
              .optlist = {}
            },
          { // 3 1 ATFRMTEXT  - ATSTYLEFRMINPUT
              .type = ATFRMTEXT,
              .size = 1,
              .row = 3,
              .col = 1,
              .style = ATSTYLEFRMINPUT,
              .optcnt = 0,
              .optlist = {}
            },
          { // 4 0 ATFRMLABEL - ATSTYLEFRMLBL
              .type = ATFRMLABEL,
              .size = 1,
              .row = 4,
              .col = 0,
              .style = ATSTYLEFRMLBL,
              .optcnt = 0,
              .optlist = {}
            },
          { // 4 1 ATFRMTEXT  - ATSTYLEFRMINPUT
              .type = ATFRMTEXT,
              .size = 1,
              .row = 4,
              .col = 1,
              .style = ATSTYLEFRMINPUT,
              .optcnt = 0,
              .optlist = {}
            },
          { // 5 0 ATFRMLABEL - ATSTYLEFRMLBL
              .type = ATFRMLABEL,
              .size = 1,
              .row = 5,
              .col = 0,
              .style = ATSTYLEFRMLBL,
              .optcnt = 0,
              .optlist = {}
            },
          { // 5 1 ATFRMINT   - ATSTYLEFRMINPUT
              .type = ATFRMINT,
              .size = 1,
              .row = 5,
              .col = 1,
              .style = ATSTYLEFRMINPUT,
              .optcnt = 0,
              .optlist = {}
            },
        }
    };
  const String AT_wdgt_size[4]
    {
      ATTXTWDSMALL,ATTXTWDLEFT,ATTXTWDRIGHT,ATTXTWDBIG
    };
  const uint16_t AT_palette[64] =
    {
      0x0000,0x4820,0xa800,0xf800,0x02c0,0x52a0,0xaaa0,0xfa80,
      0x0560,0x5541,0xb520,0xfd40,0x0fc0,0x5fe0,0xa7e0,0xffe1,
      0x000a,0x500a,0xa80a,0xf80a,0x02ea,0x52aa,0xaaac,0xfa8b,
      0x058a,0x554c,0xad4b,0xfd2c,0x07ec,0x57eb,0xb7e9,0xffcc,
      0x0016,0x5015,0xb014,0xf815,0x02d6,0x52b5,0xb295,0xfa94,
      0x0574,0x4d75,0xad54,0xfd73,0x07d5,0x5ff6,0xaff4,0xfff5,
      0x001f,0x501f,0xa81f,0xf81f,0x02be,0x5abf,0xaabf,0xfabf,
      0x053f,0x557f,0xad7f,0xfd9f,0x07df,0x5fdf,0xafff,0xffff
    };
#endif