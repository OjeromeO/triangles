#include "wx/wx.h" 
#include "dialogs.h"
#include "mainframe.h"
#include <wx/html/helpctrl.h>



class MyApp: public wxApp 
{
	virtual bool OnInit();
	CMainFrame *m_MainFrame;
};


IMPLEMENT_APP(MyApp)


bool MyApp::OnInit() 
{
	m_MainFrame = new CMainFrame( wxString("Triangulum", wxConvUTF8), wxPoint(50,50), wxSize(450,340) );

	m_MainFrame->help = new wxHtmlHelpController(wxHF_TOOLBAR | wxHF_CONTENTS | wxHF_SEARCH | wxHF_PRINT);

	if ( !m_MainFrame->help->Initialize(wxT("HELP")) )
	{
		wxLogError(wxT("Cannot initialize the help system, aborting."));
		return FALSE;
	}

	wxMenuBar * menu_bar = new wxMenuBar;
    
	wxMenu * fichier = new wxMenu;
	wxMenu * affichage = new wxMenu;
	wxMenu * options = new wxMenu;
	wxMenu * aide = new wxMenu;
	
	menu_bar->Append(fichier,wxT("Fichier"));
	menu_bar->Append(affichage,wxT("Affichage"));
	menu_bar->Append(options,wxT("Options"));
	menu_bar->Append(aide,wxT("Aide"));
	
	fichier->Append(NOUVEAU,wxT("Nouveau\tCtrl-N"));
	fichier->AppendSeparator();
	fichier->Append(OUVRIR,wxT("Ouvrir\tCtrl-O"));
	fichier->Append(SAUVEGARDER,wxT("Sauvegarder\tCtrl-S"));
	fichier->AppendSeparator();
	fichier->Append(QUITTER,wxT("Quitter\tCtrl-X"));
	affichage->AppendCheckItem(BARRE_OUTILS,wxT("Barre d'outils\tCtrl-B"));
	options->Append(EPAISSEUR,wxT("Epaisseur de trait\tCtrl-E"));
	options->Append(COULEUR,wxT("Couleur\tCtrl-C"));
	options->Append(TRIGESTION,wxT("Gestion des triangles"));
	aide->Append(AIDE,wxT("Ouvrir l'aide\tCtrl-H"));
	aide->Append(VERSION,wxT("Version"));
	
	affichage->Check(BARRE_OUTILS,TRUE);
	menu_bar->Enable(TRIGESTION,false);
	
	m_MainFrame->SetMenuBar(menu_bar);
	m_MainFrame->CreateMyToolbar();
	m_MainFrame->Show(TRUE);

	return TRUE;
} 


