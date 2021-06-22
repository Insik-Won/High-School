// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"
#include "BufferDC.h"
#include "MrDoubleFormat.h"
using namespace MrDoubleFormat;

#include <time.h>
#include <vector>
#include <iterator>
using namespace std;

#include <gdiplus.h>
#pragma comment(lib, "gdiplus")
using namespace Gdiplus;

#define MR_ARGB(a, r, g, b)  ((COLORREF)(((BYTE)(b) | ((WORD)((BYTE)(g)) << 8)) | (((DWORD)(BYTE)(r)) << 16) | (((DWORD)(BYTE)(a)) << 24)))
enum MrColor { Ambient_Blue = MR_ARGB(255, 0, 76, 255), Purple_Blue = MR_ARGB(255, 70, 0, 255) , Laser_Red = MR_ARGB(255, 255, 0, 20)};


#define WM_SIMUL_SPREAD		WM_USER + 10
#define WM_SIMUL_MUTATE		WM_USER + 20

#define TIMER_SM_SPREAD		10
#define TIMER_SM_MOVING		20
#define TIMER_SM_LIVING		30

#define TIMER_GP_RECORD		110
// 각종 커스텀 메시지

#endif //PCH_H
