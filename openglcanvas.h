#ifndef __OPENGLCANVAS_H__
#define __OPENGLCANVAS_H__

#include "wx/wx.h"
#include "triangle.h"
#include <wx/glcanvas.h>
#include <wx/menu.h>



class OpenGLCanvas: public wxGLCanvas
{
	public:
		point pa;
		point pb;
		point pc;
		wxMenu * popup;
		wxMenu * fichier;
		wxMenu * gestion;
		wxMenu * valcour;
		int selected_tri;
		long x;
		long y;
		OpenGLCanvas(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name);
		~OpenGLCanvas(void);
		void OnPaint(wxPaintEvent& event);
		int Est_dans_triangle(int x, int y);
	private:
		void OnSize(wxSizeEvent& event);
		void OnEraseBackground(wxEraseEvent& event);
		void OnMouseMove(wxMouseEvent& event);
		void OnLeftDown(wxMouseEvent& event);
		void OnLeftUp(wxMouseEvent& event);
		void Draw();
		void OnRightDown(wxMouseEvent& event);
		void OnContextPptes(wxCommandEvent& event);
		void OnContextSuppr(wxCommandEvent& event);
		DECLARE_EVENT_TABLE();
};


enum
{POPPROP = 111, POPSUPPR = 222};



#endif



