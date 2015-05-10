#ifndef __MAINFRAME_H__
#define __MAINFRAME_H__

#include "wx/wx.h"
#include "triangle.h"
#include "openglcanvas.h"
#include <vector>
#include <wx/html/helpctrl.h>

#define MON_CANEVAS 1111



class CMainFrame: public wxFrame
{
	public:
		int epaisseurtraitcourante;
		wxColour couleurcourante;
		bool is_drawing;
		int num_tri;
		std::vector<Triangle> tab_tri;
		OpenGLCanvas * canvas;
		wxHtmlHelpController * help;
		CMainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
		void CreateMyToolbar();
		wxToolBar * getToolbar();
		void OnHelp(wxCommandEvent& event);
	private:
		wxToolBar *m_toolbar;
		void OnNew(wxCommandEvent& event);
		void OnOpen(wxCommandEvent& event);
		void OnSave(wxCommandEvent& event);
		void OnQuit(wxCommandEvent& event);
		void OnQuit2(wxCloseEvent& event);
		void OnToolBar(wxCommandEvent& event);
		void OnThickness(wxCommandEvent& event);
		void OnColor(wxCommandEvent& event);
		void OnTriM(wxCommandEvent& event);
		void OnVersion(wxCommandEvent& event);
		void OnDraw(wxCommandEvent& event);
		DECLARE_EVENT_TABLE();
};


enum
{NOUVEAU,OUVRIR,SAUVEGARDER,QUITTER,BARRE_OUTILS,EPAISSEUR,COULEUR,TRIGESTION,AIDE,VERSION,TOOLBAR_TOOLS,DRAW,POPFICH,POPGEST,POPCOUL};



#endif


