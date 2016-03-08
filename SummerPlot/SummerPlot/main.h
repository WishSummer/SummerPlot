#pragma once

#pragma comment(linker, \
  "\"/manifestdependency:type='Win32' "\
  "name='Microsoft.Windows.Common-Controls' "\
  "version='6.0.0.0' "\
  "processorArchitecture='*' "\
  "publicKeyToken='6595b64144ccf1df' "\
  "language='*'\"")

#pragma comment(lib, "ComCtl32.lib")
#pragma comment(lib, "Propsys.lib")
#pragma comment(lib, "Gdiplus.lib")
#pragma comment(lib, "Uxtheme.lib")

#include "resource.h"
#include "ribbon.h"

#define WM_RIBBON_RESIZE (WM_USER + 0)

#include "main.controls.h"
#include "main.options.h"
#include "main.status.h"
#include "main.util.export.h"
#include "main.util.dialog.h"
#include "main.util.translate.h"
#include "main.util.doublebuffer.h"
#include "main.util.ribbon.h"
#include "main.handler.window.h"
#include "main.handler.canvas.h"
#include "main.handler.ribbon.h"