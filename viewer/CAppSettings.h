#ifndef _CAPPSETTINGS_H_
#define _CAPPSETTINGS_H_
class CAppSettings
{
public:
	static COLORREF WindowBackgroundTopColor;			// window background color gradient (top color)
	static COLORREF DefaultWindowBackgroundTopColor;	// default value
	static CString WindowBackgroundTopColorEntry;		// registry location

	static COLORREF WindowBackgroundBottomColor;		// window background color gradient (bottom color)
	static COLORREF DefaultWindowBackgroundBottomColor;	// default value
	static CString WindowBackgroundBottomColorEntry;	// registry location 
protected:
private:
};

#endif