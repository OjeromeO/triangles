#include <stdio.h>
#include <stdlib.h>
#include <wx/wx.h>
#include <wx/accel.h>
#include <wx/spinctrl.h>

#include "dialogs.h"
#include "mainframe.h"



// boite de dialogue pour l'affichage de la version
BEGIN_EVENT_TABLE(VersionDialog, wxDialog)
END_EVENT_TABLE()


VersionDialog::VersionDialog(wxWindow *parent, wxWindowID id, const wxString &title)
: wxDialog(parent, id, title)
{
	item0 = new wxBoxSizer(wxVERTICAL);
	item1 = new wxStaticText(this, ID_TEXTV, wxT("Version 1.2.5"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
	item2 = new wxButton(this, wxID_OK, wxT("OK"), wxDefaultPosition);

	item0->Add(item1, 0, wxALIGN_CENTRE|wxALL, 5);
	item0->Add(item2, 0, wxALIGN_CENTRE|wxALL, 5);

	this->SetAutoLayout(TRUE);
	this->SetSizer(item0);
	
	item0->Fit(this);
	item0->SetSizeHints(this);
}



// boite de dialogue pour le choix de l'epaisseur de trait
BEGIN_EVENT_TABLE(EpaisseurDialog, wxDialog)
EVT_COMMAND_SCROLL(SLIDER,EpaisseurDialog::OnSlider)
END_EVENT_TABLE()


EpaisseurDialog::EpaisseurDialog(wxWindow *parent, wxWindowID id, const wxString &title)
: wxDialog(parent, id, title)
{
	item0 = new wxBoxSizer(wxVERTICAL);
	item1 = new wxStaticText(this, ID_TEXTE, wxT("Choisir la nouvelle epaisseur de trait"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
	item2 = new wxSlider(this, SLIDER, static_cast<CMainFrame *>(this->GetParent())->epaisseurtraitcourante, 1, 10, wxDefaultPosition, wxSize(150,40), wxSL_HORIZONTAL|wxSL_LABELS);
	item3 = new wxButton(this, wxID_OK, wxT("OK"), wxDefaultPosition);

	item0->Add(item1, 0, wxALIGN_CENTRE|wxALL, 5);
	item0->Add(item2, 0, wxALIGN_CENTRE|wxALL, 5);
	item0->Add(item3, 0, wxALIGN_CENTRE|wxALL, 5);

	this->SetAutoLayout(TRUE);
	this->SetSizer(item0);
	
	item0->Fit(this);
	item0->SetSizeHints(this);
}


void EpaisseurDialog::OnSlider(wxScrollEvent& event)
{
	static_cast<CMainFrame *>(this->GetParent())->epaisseurtraitcourante = item2->GetValue();
}



// boite de dialogue pour le choix de la couleur
BEGIN_EVENT_TABLE(CouleurDialog, wxDialog)
EVT_RADIOBOX(RADIO, CouleurDialog::OnRadio)
END_EVENT_TABLE()


CouleurDialog::CouleurDialog(wxWindow *parent, wxWindowID id, const wxString &title)
: wxDialog(parent, id, title)
{
	item0 = new wxBoxSizer(wxVERTICAL);
	item1 = new wxStaticText(this, ID_TEXTC, wxT("Choisir la nouvelle couleur"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
	
	wxString strs8[] = {wxT("Rouge"), wxT("Vert"), wxT("Bleu")};
	item2 = new wxRadioBox(this, RADIO, wxT("Couleur"), wxDefaultPosition, wxDefaultSize, 3, strs8, 1, wxRA_SPECIFY_COLS);
	if(static_cast<CMainFrame *>(this->GetParent())->couleurcourante == *wxRED)
		item2->SetSelection(0);
	if(static_cast<CMainFrame *>(this->GetParent())->couleurcourante == *wxGREEN)
		item2->SetSelection(1);
	if(static_cast<CMainFrame *>(this->GetParent())->couleurcourante == *wxBLUE)
		item2->SetSelection(2);

	item3 = new wxButton(this, wxID_OK, wxT("OK"), wxDefaultPosition);

	item0->Add(item1, 0, wxALIGN_CENTRE|wxALL, 5);
	item0->Add(item2, 0, wxALIGN_CENTRE|wxALL, 5);
	item0->Add(item3, 0, wxALIGN_CENTRE|wxALL, 5);

	this->SetAutoLayout(TRUE);
	this->SetSizer(item0);
	
	item0->Fit(this);
	item0->SetSizeHints(this);
}


void CouleurDialog::OnRadio(wxCommandEvent& event)
{
	if(item2->GetStringSelection() == wxT("Rouge"))
		static_cast<CMainFrame *>(this->GetParent())->couleurcourante = *wxRED;

	if(item2->GetStringSelection() == wxT("Vert"))
		static_cast<CMainFrame *>(this->GetParent())->couleurcourante = *wxGREEN;

	if(item2->GetStringSelection() == wxT("Bleu"))
		static_cast<CMainFrame *>(this->GetParent())->couleurcourante = *wxBLUE;
}



// boite de dialogue pour la gestion des triangles
BEGIN_EVENT_TABLE(TrianglesDialog, wxDialog)
EVT_LISTBOX(LISTBOX, TrianglesDialog::OnList)
EVT_LISTBOX_DCLICK(LISTBOX, TrianglesDialog::OnDCList)
EVT_BUTTON(PROPRIETES, TrianglesDialog::OnProp)
EVT_BUTTON(SUPPRIMER, TrianglesDialog::OnSuppr)
END_EVENT_TABLE()


TrianglesDialog::TrianglesDialog(wxWindow *parent, wxWindowID id, const wxString &title)
: wxDialog(parent, id, title)
{

	item0 = new wxBoxSizer(wxHORIZONTAL);    
	item00 = new wxBoxSizer(wxVERTICAL);
	item01 = new wxStaticText(this, ID_TEXTT, wxT("Liste des triangles"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
	
	int n = static_cast<CMainFrame *>(this->GetParent())->num_tri;
	wxString str[n];	
	int i;
	for(i=0;i<n;i++)		// on remplit le tableau qui va etre affiche avec les triangles deja enregistres
	{
		wxString tr;
		tr.Printf(wxT("Triangle "));
		tr << (i+1);		// on fait en sorte de ne pas commencer a 0
		str[i] = tr;
	}
	item02 = new wxListBox(this, LISTBOX, wxDefaultPosition, wxSize(110,160), n, str, wxLB_SINGLE|wxLB_HSCROLL|wxLB_NEEDED_SB|wxLB_SORT);
	item02->SetSelection(0);

	item000 = new wxBoxSizer(wxVERTICAL);
	item001 = new wxButton(this,PROPRIETES,wxT("Proprietes"),wxDefaultPosition);
	item002 = new wxButton(this,SUPPRIMER,wxT("Supprimer"),wxDefaultPosition);
	item003 = new wxButton(this,wxID_OK,wxT("OK"),wxDefaultPosition);

	item000->Add(item001, 0, wxALIGN_CENTRE|wxALL, 5);
	item000->Add(item002, 0, wxALIGN_CENTRE|wxALL, 5);
	item000->Add(item003, 0, wxALIGN_CENTRE|wxALL, 5);

	item00->Add(item01, 0, wxALIGN_CENTRE|wxALL, 5);
	item00->Add(item02, 0, wxALIGN_CENTRE|wxALL, 5);

	item0->Add(item00, 0, wxALIGN_CENTRE|wxALL, 5);
	item0->Add(item000, 0, wxALIGN_CENTRE|wxALL, 5);

	this->SetAutoLayout(true);
	this->SetSizer(item0);
	
	item0->Fit(this);
	item0->SetSizeHints(this);
}


void TrianglesDialog::OnList(wxCommandEvent& event)
{
}


void TrianglesDialog::OnDCList(wxCommandEvent& event)
{
}


void TrianglesDialog::OnProp(wxCommandEvent& event)
{
	if(static_cast<CMainFrame *>(this->GetParent())->num_tri == 0)		// si on a deja supprime tous les triangles, pas de proprietes, on sort tout de suite de la fonction
		return;

	Triangle * t = new Triangle();
	*t = static_cast<CMainFrame *>(this->GetParent())->tab_tri[this->item02->GetSelection()];
	
	PropDialog pdlg(this, 1000, wxT("Proprietes"));
	
	pdlg.item002->SetValue(this->item02->GetStringSelection());
	pdlg.item004->SetValue(t->thickness);

	if(t->colour == *wxRED)
		pdlg.item01->SetSelection(0);

	if(t->colour == *wxGREEN)
		pdlg.item01->SetSelection(1);

	if(t->colour == *wxBLUE)
		pdlg.item01->SetSelection(2);
	
	pdlg.ShowModal();
}


void TrianglesDialog::OnSuppr(wxCommandEvent& event)
{
	if(static_cast<CMainFrame *>(this->GetParent())->num_tri == 0)		// si on a deja supprime tous les triangles, rien a supprimer, on sort tout de suite de la fonction
		return;

	if(static_cast<CMainFrame *>(this->GetParent())->num_tri == 1)		// s'il reste un seul triangle, celui qu'on va supprimer, alors on grise la gestion des triangles
		(static_cast<CMainFrame *>(this->GetParent())->GetMenuBar())->Enable(TRIGESTION, false);
	
	static_cast<CMainFrame *>(this->GetParent())->tab_tri.erase(static_cast<CMainFrame *>(this->GetParent())->tab_tri.begin() + this->item02->GetSelection());
	static_cast<CMainFrame *>(this->GetParent())->num_tri--;
	
	this->item02->Delete(this->item02->GetSelection());
	
	if(static_cast<CMainFrame *>(this->GetParent())->num_tri != 0)		// tant q'il reste des triangles, alors on remet la selection au premier element
	{
		this->item02->SetSelection(0);
	}
	
	if(static_cast<CMainFrame *>(this->GetParent())->num_tri == 0)		// s'il n'y a plus de triangles, alors on desactive les boutons "proprietes" et "supprimer"
	{
		this->item001->Enable(false);
		this->item002->Enable(false);
	}
}



// boite de dialogue pour les proprietes
BEGIN_EVENT_TABLE(PropDialog, wxDialog)
EVT_TEXT(TEXTCTRL, PropDialog::OnText)
EVT_TEXT_MAXLEN(TEXTCTRL, PropDialog::OnTextMax)
EVT_SPINCTRL(SPINCTRL, PropDialog::OnSpin)
EVT_RADIOBOX(RADIOBOX2, PropDialog::OnRadio)
END_EVENT_TABLE()


PropDialog::PropDialog(wxWindow *parent, wxWindowID id, const wxString &title)
: wxDialog(parent, id, title)
{
	item0 = new wxBoxSizer(wxVERTICAL);
	item1 = new wxButton(this, wxID_OK, wxT("OK"), wxDefaultPosition);

	item00 = new wxBoxSizer(wxHORIZONTAL);
	
	wxString strs8[] = {wxT("Rouge"), wxT("Vert"), wxT("Bleu")};
	item01 = new wxRadioBox(this, RADIOBOX2, wxT("Couleur"), wxDefaultPosition, wxDefaultSize, 3, strs8, 1, wxRA_SPECIFY_COLS);

	item000 = new wxBoxSizer(wxVERTICAL);
	item001 = new wxStaticText(this, ID_TEXT1P, wxT("Identifiant du triangle"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
	item002 = new wxTextCtrl(this, TEXTCTRL, wxT("triangle 0"), wxDefaultPosition, wxSize(110,20), wxTE_NOHIDESEL|wxHSCROLL|wxTE_LEFT|wxTE_READONLY);
	item003 = new wxStaticText(this, ID_TEXT2P, wxT("Epaisseur trait"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
	item004 = new wxSpinCtrl(this, SPINCTRL, wxT("2"), wxDefaultPosition, wxSize(50,20), wxSP_ARROW_KEYS, 1, 10);

	item000->Add(item001, 0, wxALIGN_CENTRE|wxALL, 5);
	item000->Add(item002, 0, wxALIGN_CENTRE|wxALL, 5);
	item000->Add(item003, 0, wxALIGN_CENTRE|wxALL, 5);
	item000->Add(item004, 0, wxALIGN_CENTRE|wxALL, 5);

	item00->Add(item000, 0, wxALIGN_CENTRE|wxALL, 5);
	item00->Add(item01, 0, wxALIGN_CENTRE|wxALL, 5);

	item0->Add(item00, 0, wxALIGN_CENTRE|wxALL, 5);
	item0->Add(item1, 0, wxALIGN_CENTRE|wxALL, 5);

	this->SetAutoLayout(true);
	this->SetSizer(item0);
	
	item0->Fit(this);
	item0->SetSizeHints(this);
}


void PropDialog::OnText(wxCommandEvent& event)
{
}


void PropDialog::OnTextMax(wxCommandEvent& event)
{
}


void PropDialog::OnSpin(wxSpinEvent& event)
{
	int n = 0;				// numero du triangle selectionne
	
	if(GetId() == 1000)		// si on est passe par la boite de dialogue de gestion des triangles pour aller dans les proprietes
		n = static_cast<TrianglesDialog *>(this->GetParent())->item02->GetSelection();
	else					// sinon on a utilise le menu du canvas
		n = static_cast<OpenGLCanvas *>(this->GetParent())->selected_tri;
	
	static_cast<CMainFrame *>(this->GetParent()->GetParent())->tab_tri[n].thickness = item004->GetValue();
}


void PropDialog::OnRadio(wxCommandEvent& event)
{
	int n = 0;				// numero du triangle selectionne
	
	if(GetId() == 1000)		// si on est passe par la boite de dialogue de gestion des triangles pour aller dans les proprietes
		n = static_cast<TrianglesDialog *>(this->GetParent())->item02->GetSelection();
	else					// sinon on a utilise le menu du canvas
		n = static_cast<OpenGLCanvas *>(this->GetParent())->selected_tri;
	
	if(item01->GetStringSelection() == wxT("Rouge"))
		static_cast<CMainFrame *>(this->GetParent()->GetParent())->tab_tri[n].colour = *wxRED;

	if(item01->GetStringSelection() == wxT("Vert"))
		static_cast<CMainFrame *>(this->GetParent()->GetParent())->tab_tri[n].colour = *wxGREEN;

	if(item01->GetStringSelection() == wxT("Bleu"))
		static_cast<CMainFrame *>(this->GetParent()->GetParent())->tab_tri[n].colour = *wxBLUE;
}



