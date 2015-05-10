#ifndef __DIALOGS_H__
#define __DIALOGS_H__

#include "wx/wx.h"
#include "mainframe.h"
#include <wx/spinctrl.h>



// boite de dialogue pour l'affichage de la version
class VersionDialog: public wxDialog
{
	public:
		wxBoxSizer *item0;
		wxStaticText *item1; 
		wxButton *item2;
		VersionDialog(wxWindow *parent, wxWindowID id, const wxString &title);
	private:
		DECLARE_EVENT_TABLE()
};


// boite de dialogue pour le choix de l'epaisseur de trait
class EpaisseurDialog: public wxDialog
{
	public:
		wxBoxSizer *item0;
		wxStaticText *item1;
		wxSlider *item2;
		wxButton *item3;
		EpaisseurDialog(wxWindow *parent, wxWindowID id, const wxString &title);
		void OnSlider(wxScrollEvent& event);
	private:
		DECLARE_EVENT_TABLE()
};


// boite de dialogue pour le choix de la couleur
class CouleurDialog: public wxDialog
{
	public:
		wxBoxSizer *item0;
		wxStaticText *item1;
		wxRadioBox *item2;
		wxButton *item3;
		CouleurDialog(wxWindow *parent, wxWindowID id, const wxString &title);
		void OnRadio(wxCommandEvent& event);
	private:
		DECLARE_EVENT_TABLE()
};


// boite de dialogue pour la gestion des triangles
class TrianglesDialog: public wxDialog
{
	public:
		wxBoxSizer *item0;
		wxBoxSizer *item00;
		wxStaticText *item01;
		wxListBox *item02;
		wxBoxSizer *item000;
		wxButton *item001;
		wxButton *item002;
		wxButton *item003;
		TrianglesDialog(wxWindow *parent, wxWindowID id, const wxString &title);
		void OnList(wxCommandEvent& event);
		void OnDCList(wxCommandEvent& event);
		void OnProp(wxCommandEvent& event);
		void OnSuppr(wxCommandEvent& event);
	private:
		DECLARE_EVENT_TABLE()
};


// boite de dialogue pour les proprietes
class PropDialog: public wxDialog
{
	public:
		wxBoxSizer *item0;
		wxButton *item1;
		wxBoxSizer *item00;
		wxRadioBox *item01;
		wxBoxSizer *item000;
		wxStaticText *item001;
		wxTextCtrl *item002;
		wxStaticText *item003;
		wxSpinCtrl *item004;
		PropDialog(wxWindow *parent, wxWindowID id, const wxString &title);
		void OnText(wxCommandEvent& event);
		void OnTextMax(wxCommandEvent& event);
		void OnSpin(wxSpinEvent& event);
		void OnRadio(wxCommandEvent& event);
	private:
		DECLARE_EVENT_TABLE()
};


enum
{ID_TEXTV = 10000, ID_TEXTE = 20000, SLIDER = 30000, ID_TEXTC = 40000, RADIO = 50000, ID_TEXTT = 60000, PROPRIETES = 70000, SUPPRIMER = 80000, LISTBOX = 90000, ID_TEXT1P = 100000, ID_TEXT2P = 110000, RADIOBOX2 = 120000, TEXTCTRL = 130000, SPINCTRL = 140000};



#endif



