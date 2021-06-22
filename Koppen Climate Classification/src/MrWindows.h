#pragma once
#include <vector>
#include <cassert>
#include <numeric>
#include <algorithm>
#include <cmath>

using std::vector;

#define XYtoLPARAM(x,y) (LPARAM)((x) | (y) << 16)
#define toWPARAM(i) (WPARAM)(i)

#define MR_ARGB(a, r, g, b)  ((COLORREF)(((BYTE)(b) | ((WORD)((BYTE)(g)) << 8)) | (((DWORD)(BYTE)(r)) << 16) | (((DWORD)(BYTE)(a)) << 24)))
#define RGB_TO_ARGB(a, rgb)	((COLORREF)((COLORREF)(rgb) | (((DWORD)(BYTE)(a)) << 24)))

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
using Gdiplus::GraphicsPath;
using Gdiplus::Region;

namespace MrWindows {
	namespace Functions	{}
	namespace Classes	{}
}

// 각종 유틸리티 함수 선언
namespace MrWindows::Functions {
	CRect getWindowRect(CWnd* wnd);
	CRect getClientRect(CWnd* wnd);
	CString getWindowText(CWnd* wnd);

	int getWindowCX(CWnd* wnd);
	int getWindowCY(CWnd* wnd);

	void MrFormatMessage(const LPCTSTR lpszText, ...);

	// x,y means the center point
	void DrawCircle(Graphics& graphics, const Pen* pen, PointF pointf, SizeF sizef);
	void DrawCircle(Graphics& graphics, const Pen* pen, Point point, Size size);
	void DrawCircle(Graphics& graphics, const Pen* pen, int x, int y, int cx, int cy);
	void DrawCircle(Graphics& graphics, const Pen* pen, REAL x, REAL y, REAL cx, REAL cy);

	// x,y means the center point
	void FillCircle(Graphics& graphics, const Brush* pen, PointF pointf, SizeF sizef);
	void FillCircle(Graphics& graphics, const Brush* pen, Point point, Size size);
	void FillCircle(Graphics& graphics, const Brush* pen, int x, int y, int cx, int cy);
	void FillCircle(Graphics& graphics, const Brush* pen, REAL x, REAL y, REAL cx, REAL cy);

	void setFontSize(CFont& font, int height);
	void setFontName(CFont& font, LPCTSTR str);

	CRect GetDesktopRect();
	void GetDesktopRect(LPRECT rect);
}

// CMarginHelper 선언
namespace MrWindows::Classes {
	enum class MrCenter { Horizontal, Vertical };

	class CMarginHelper {
	public:
		CMarginHelper();
		CMarginHelper(const CMarginHelper& helper);
		CMarginHelper(CMarginHelper&& helper) noexcept;

		virtual ~CMarginHelper();
		CMarginHelper& operator=(const CMarginHelper& helper);
		CMarginHelper& operator=(CMarginHelper&& helper) noexcept;

	private:
		CRect rect;
		int margin;

	public:
		CMarginHelper&	Initiate();
		CMarginHelper&	SetRect(const CRect& rect);
		CMarginHelper&	SetMargin(int margin);
		CMarginHelper&	SetWidth(int width);
		CMarginHelper&	SetHeight(int height);

		CMarginHelper&	OnLeft(int leftside, int margin);
		CMarginHelper&	OnUp(int upside, int margin);
		CMarginHelper&	OnRight(int rightside, int margin);
		CMarginHelper&	OnDown(int downside, int margin);

		CMarginHelper&	OnLeft(int leftside);
		CMarginHelper&	OnUp(int upside);
		CMarginHelper&	OnRight(int rightside);
		CMarginHelper&	OnDown(int downside);

		CMarginHelper&	AttachLeft(int leftside, int margin);
		CMarginHelper&	AttachUp(int upside, int margin);
		CMarginHelper&	AttachRight(int rightside, int margin);
		CMarginHelper&	AttachDown(int downside, int margin);

		CMarginHelper&	AttachLeft(int leftside);
		CMarginHelper&	AttachUp(int upside);
		CMarginHelper&	AttachRight(int rightside);
		CMarginHelper&	AttachDown(int downside);

		CMarginHelper&	OnCenterLine(int center, MrCenter direction);
		CMarginHelper&	OnCenterLine(int upleft, int rightbottom, MrCenter direction);
		CMarginHelper&	OnCenterLine(const CPoint& point, MrCenter direction);
		CMarginHelper&	OnCenterLine(const CPoint& point);

		CMarginHelper&	MoveLeft(int distance);
		CMarginHelper&	MoveUp(int distance);
		CMarginHelper&	MoveRight(int distance);
		CMarginHelper&	MoveDown(int distance);

		CRect			toRect();

		int				GetMargin();
		CRect			GetRect();

		#ifdef _GDIPLUSTYPES_H

		Gdiplus::Rect	toGdiRect();
		Gdiplus::RectF	toGdiRectF();

		#endif
	};

	extern CMarginHelper MarginHelper;
}

// CPointer 선언
namespace MrWindows::Classes {
	class CPointer {
	public:
		explicit CPointer(CWnd* pWnd = nullptr);
		virtual ~CPointer() = default;

	private:
		CWnd* m_pWnd;
		bool m_bDragging;
		bool m_bCaptured;

	public:
		void ClickUp();
		void ClickDown();

		bool isDragging();

		void SetCapture();
		void ReleaseCapture();

		bool isCapturing();
	};
}

// CGraph 선언
namespace MrWindows::Classes {
	enum class MrGraphMode { Lines,  Bars };
	enum class MrDirection { Left, Up, Right, Down };
	enum class MrInOut : int { Inner = 1, Outer = -1 };

	class CGraph 
	{
	public:
		CGraph(int size = 0, MrGraphMode graphmode = MrGraphMode::Lines);
		virtual ~CGraph();

	private:
		vector<double> m_dataVector;

		MrGraphMode m_GraphMode;
		float m_font_height; // Dynamic Sizing Ratio -> (height + width)/2 * m_font_height;
		float m_pen_width; // Dynamic Sizing Ratio -> height * m_pen_width;
		float m_init_point;
		float m_end_point;
		bool m_bShowing;

		float m_bar_width;
		float m_ball_radius;

		Color			m_Color;
		Gdiplus::Font*	m_Font;
		CRect			m_Rect;

	public:
		void setGraphMode(MrGraphMode graphMode);
		MrGraphMode getGraphMode();

		void setFontHeight(float height);
		float getFontHeight();
		void setPenWidth(float width);
		float getPenWidth();
		void setBarWidth(float width);
		float getBarWidth();
		void setBallRadius(float radius);
		float getBallRadius();

		void setGraphLimits(float init, float end);
		tuple<float, float> getGraphLimits();

		vector<double>* getVector();

		void			show();
		void			hide();
		bool			isShowing();

		void			setColor(Color color);
		void			setFont(const Gdiplus::Font* font);
		void			setRect(const CRect* rect);
		void			setRect(const Rect& rect);
		void			setRect(const RectF& rect);

		Color			getColor();
		Gdiplus::Font*	getFont();
		CRect			getRect();
		Rect			getGdiRect();
		RectF			getGdiRectF();

	public:
		CGraph& setData(double data, int index);
		CGraph& setData(std::initializer_list<double> init_list);

		tuple<double, int> getHighest();
		tuple<double, int> getLowest();
		double getTotal();
		double getAverage();

		void drawGraph(Graphics& graphics);

		tuple<int, CPoint> isAnyIndexClicked(CPoint point, CSize size);
		float convertData(int client_point_y);

		CRect getUpdateRect(int index);
		CRect getUpdateRect(int start_index, int end_index);

	private:
		void drawLineGraph(Graphics& graphics);
		void drawBarGraph(Graphics& graphics);

		PointF toForge(int index);
		float toForgeX(int index);
		float toForgeY(int index);

		PointF toForge(int index, const CRect& rect);
		float toForgeX(int index, const CRect& rect);
		float toForgeY(int index, const CRect& rect);
	};

	class CGraphMarker {
	public:
		CGraphMarker();
		virtual ~CGraphMarker();

	private:
		vector<tuple<MrInOut, MrDirection, double, CString>> m_marking_vector;

		// Dynamic Sizing Ratio -> (height + width)/2 * m_font_height;
		float			m_font_height; 
		Gdiplus::Font*	m_Font;

		Brush*			m_Brush;
		Pen*			m_Pen;
		CRect			m_Rect;

	public:
		void setFontHeight(float height);
		float getFontHeight();

		void			setBrush(const SolidBrush* brush);
		void			setPen(const Pen* pen);
		void			setFont(const Gdiplus::Font* font);
		void			setRect(const CRect* rect);
		void			setRect(const Rect& rect);
		void			setRect(const RectF& rect);

		Brush*			GetBrush();
		Pen*			getPen();
		Gdiplus::Font*	getFont();
		CRect			getRect();
		Rect			getGdiRect();
		RectF			getGdiRectF();

		auto getVector() -> vector<tuple<MrInOut, MrDirection, double, CString>>* ;

	public:
		CGraphMarker& addMark(double d, const CString& string, MrDirection diection, MrInOut InOut);
		CGraphMarker& addMark(const tuple<double, CString, MrDirection, MrInOut>& tuple);
		CGraphMarker& addSuccessiveMark(int init, int end, int num, MrDirection direction, MrInOut inOut);


		CGraphMarker& removeMark(MrDirection direction, MrInOut inOut);
		CGraphMarker& removeMark(const CString& str, MrDirection direction, MrInOut inOut);
		CGraphMarker& removeMark(MrDirection direciton, MrInOut inOut, const std::initializer_list<CString>& ilist);

		CString arrange();

		void drawMarkings(Graphics& graphics);
		/*void drawMarkings(Graphics& graphics, MrDirection direction);
		void drawMarkings(Graphics& graphics, MrInOut inOut);
		void drawMarkings(Graphics& graphics, MrDirection direction, MrInOut inOut);*/

	private:
		void drawMarking(Graphics& graphics, const tuple<MrInOut, MrDirection, double, CString>& tuple);
	};
}