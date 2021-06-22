// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"

#include <vector>
#include <set>
#include <unordered_map>
#include <string>
#include <string_view>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <iterator>
#include <tuple>
#include <functional>

using std::vector;
using std::set;
using std::unordered_map;
using std::string;
using std::wstring;
using std::stringstream;
using std::wstringstream;
using std::string_view;
using std::wstring_view;
using std::ifstream;
using std::wifstream;
using std::ofstream;
using std::wofstream;
using std::tuple;
using std::endl;
using std::make_tuple;

#include <gdiplus.h>
#include <gdiplusimaging.h>
#include <atlimage.h>
#pragma comment(lib, "gdiplus")

using Gdiplus::Graphics;
using Gdiplus::Brush;
using Gdiplus::SolidBrush;
using Gdiplus::Pen;
using Gdiplus::Color;
using Gdiplus::Rect;
using Gdiplus::RectF;
using Gdiplus::Point;
using Gdiplus::PointF;
using Gdiplus::Size;
using Gdiplus::SizeF;
using Gdiplus::REAL;
using Gdiplus::StringFormat;
using Gdiplus::StringAlignment;
using Gdiplus::Image;

#include "BufferDC.h"
#include "CMrStatic.h"
#include "MrWindows.h"

using namespace MrWindows::Functions;
using namespace MrWindows::Classes;

#define WM_USER_UPDATE_CLIMATE_INFO		WM_USER + 10
#define WM_USER_CLIMATE_RESET			WM_USER + 30

#endif //PCH_H
