#include <stdio.h>
#include <stdlib.h>
#include <wx/wx.h>
#include <wx/accel.h>
#include <wx/filedlg.h>
#include <fstream>
#include <vector>

#include "dialogs.h"
#include "mainframe.h"
#include "triangle.h"
#include "openglcanvas.h"
#include <wx/html/helpctrl.h>



BEGIN_EVENT_TABLE(CMainFrame, wxFrame)
EVT_MENU(NOUVEAU, CMainFrame::OnNew)
EVT_MENU(OUVRIR, CMainFrame::OnOpen)
EVT_MENU(SAUVEGARDER, CMainFrame::OnSave)
EVT_MENU(QUITTER, CMainFrame::OnQuit)
EVT_CLOSE(CMainFrame::OnQuit2)
EVT_MENU(BARRE_OUTILS, CMainFrame::OnToolBar)
EVT_MENU(EPAISSEUR, CMainFrame::OnThickness)
EVT_MENU(COULEUR, CMainFrame::OnColor)
EVT_MENU(TRIGESTION, CMainFrame::OnTriM)
EVT_MENU(VERSION, CMainFrame::OnVersion)
EVT_MENU(DRAW, CMainFrame::OnDraw)
EVT_MENU(AIDE, CMainFrame::OnHelp)
END_EVENT_TABLE()



// constructeur
CMainFrame::CMainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame((wxFrame *)NULL, -1, title, pos, size) 
{
	epaisseurtraitcourante = 3;
	couleurcourante = *wxBLUE;
	is_drawing = false;
	num_tri = 0;
	tab_tri.clear();
	canvas = new OpenGLCanvas(this, MON_CANEVAS, wxDefaultPosition, wxDefaultSize, 0, wxT("GLCanvas"));
}


// creation de la barre d'outils
void CMainFrame::CreateMyToolbar()
{
	m_toolbar=CreateToolBar(wxNO_BORDER | wxTB_HORIZONTAL, TOOLBAR_TOOLS);
	
	wxBitmap toolBarBitmaps[4];
	toolBarBitmaps[0] = wxBitmap(wxT("new.bmp"),wxBITMAP_TYPE_BMP);
	toolBarBitmaps[1] = wxBitmap(wxT("open.bmp"),wxBITMAP_TYPE_BMP);
	toolBarBitmaps[2] = wxBitmap(wxT("save.bmp"),wxBITMAP_TYPE_BMP);
	toolBarBitmaps[3] = wxBitmap(wxT("draw.bmp"),wxBITMAP_TYPE_BMP);
	
	m_toolbar->SetToolBitmapSize(wxSize(toolBarBitmaps[0].GetWidth(),toolBarBitmaps[0].GetHeight()));
	
	m_toolbar->AddTool(NOUVEAU, wxT("Nouveau"), toolBarBitmaps[0]);
	m_toolbar->AddTool(OUVRIR, wxT("Ouvrir"), toolBarBitmaps[1]);
	m_toolbar->AddTool(SAUVEGARDER, wxT("Sauvegarder"), toolBarBitmaps[2]);
	m_toolbar->AddSeparator();
	m_toolbar->AddCheckTool(DRAW, wxT("Dessiner"), toolBarBitmaps[3]);
	
	m_toolbar->Realize();
	
	SetToolBar(m_toolbar);
}



// *** fonctions associees aux evenements *** //


void CMainFrame::OnNew(wxCommandEvent& event)
{
	if(num_tri != 0)		// si on avait dessine des triangles
	{
		wxMessageDialog msg(this, wxT("Voulez-vous enregistrer vos modifications ?"), wxT("Triangulum "), wxYES_NO|wxYES_DEFAULT|wxCENTRE|wxICON_INFORMATION);
		
		if(msg.ShowModal() == wxID_YES)
		{
			wxCommandEvent e;
			OnSave(e);
		}
	}
	
	num_tri = 0;
	tab_tri.clear();
	(this->GetMenuBar())->Enable(TRIGESTION, false);
	wxPaintEvent e;
	canvas->OnPaint(e);
	canvas->pa.x = -10000;
	canvas->pa.y = -10000;
	canvas->pb.x = -10000;
	canvas->pb.y = -10000;
	canvas->pc.x = -10000;
	canvas->pc.y = -10000;
}


void CMainFrame::OnOpen(wxCommandEvent& event)
{
	(this->GetMenuBar())->Enable(TRIGESTION, false);
	wxFileDialog fdlg(this, wxT("Ouvrir un fichier"), wxT(""), wxT(""), wxT("*.tri"), wxOPEN|wxFILE_MUST_EXIST|wxCHANGE_DIR);
	
	if(fdlg.ShowModal() == wxID_OK)
	{
		std::ifstream fo(fdlg.GetPath().fn_str(), std::ios::in);
		
		if(!fo)		// si on n'arrive pas a ouvrir le fichier, on affiche un message d'erreur et on sort
		{
			wxString errormsg;
			errormsg.Printf(wxT("Impossible d'ouvrir le fichier."));
			errormsg.Append(fdlg.GetPath());
			wxMessageDialog msg(this, errormsg, wxT("Erreur"), wxOK|wxCENTRE|wxICON_ERROR);
			msg.ShowModal();
			return;
		}
		
		num_tri = 0;		// on attend d'etre sur que l'utilisateur puisse ouvrir un fichier avant de tout effacer
		tab_tri.clear();
		
		fo >> num_tri;
		
		if(num_tri == 0)		// si on a 0 en premiere valeur, alors pas de triangles, on affiche un message d'information et on sort
		{
			wxMessageDialog msg(this, wxT("Le fichier choisi ne contient aucun triangle a afficher."), wxT("Erreur"), wxOK|wxCENTRE|wxICON_ERROR);
			msg.ShowModal();
			return;
		}
		
		int i;
		int count = 1;				// variable pour compter le nombre de valeurs dans le fichier
		for(i=0;i<num_tri;i++)		// extraction des donnees du fichier
		{
			Triangle * t = new Triangle();
			int r = -1;
			int g = -1;
			int b = -1;
			
			fo >> t->p1.x;		// apres chaque extraction de valeur...
			if(!fo.eof())		// ...on teste si on est arrive a la fin du fichier...
				count++;		// ...et si non alors on incremente le compteur des valeurs lues
			fo >> t->p1.y;
			if(!fo.eof())
				count++;
			fo >> t->p2.x;
			if(!fo.eof())
				count++;
			fo >> t->p2.y;
			if(!fo.eof())
				count++;
			fo >> t->p3.x;
			if(!fo.eof())
				count++;
			fo >> t->p3.y;
			if(!fo.eof())
				count++;
			
			fo >> r;
			if(!fo.eof())
				count++;
			fo >> g;
			if(!fo.eof())
				count++;
			fo >> b;
			if(!fo.eof())
				count++;
			
			wxColour clr(r, g, b);
			t->colour = clr;
			
			fo >> t->thickness;
			if(!fo.eof())
				count++;
			
			
			//on teste s'il y a des valeurs erronees (la valeur thickness sera a 0 si on est arrive a la fin du fichier et elle sera donc erronee) avant d'ajouter ce triangle au tableau
			if( (((r==255)&&(g==0)&&(b==0)) || ((r==0)&&(g==255)&&(b==0)) || ((r==0)&&(g==0)&&(b==255))) && ((t->thickness >= 1)&&(t->thickness <= 10)) )
				tab_tri.push_back(*t);
		}
		
		num_tri = tab_tri.size();	// on ajuste le nombre reel de triangles a afficher
		
		if(count < 11)				// si on n'a meme pas un bloc de donnees (donc un triangle) alors on informe l'utilisateur et on sort
		{
			wxMessageDialog msg(this, wxT("Le fichier choisi est incomplet."), wxT("Erreur"), wxOK|wxCENTRE|wxICON_ERROR);
			msg.ShowModal();
			num_tri = 0;
			tab_tri.clear();
			return;
		}
		
		if(count%10 != 1)			// si on n'a pas un nombre de valeurs normal (la premiere valeur + les 10 valeurs d'un bloc de donnees) alors on informe l'utilisateur qu'on va tenter d'utiliser les donnees lues
		{
			wxMessageDialog msg(this, wxT("Le fichier choisi est incomplet : ce programme va tenter de lire les donnees valides."), wxT("Information"), wxOK|wxCENTRE|wxICON_INFORMATION);
			msg.ShowModal();
		}
		
		if(tab_tri.size() == 0)		// si les valeurs extraites sont impossibles a utiliser, alors on informe l'utilisateur qu'on ne peut rien faire avec le fichier choisi
		{
			wxMessageDialog msg(this, wxT("Le fichier choisi contient trop de donnees erronees."), wxT("Erreur"), wxOK|wxCENTRE|wxICON_ERROR);
			msg.ShowModal();
			num_tri = 0;
			tab_tri.clear();
			return;
		}
		else						// si on a des triangles utilisables, alors on active le bouton de gestion des triangles
			(this->GetMenuBar())->Enable(TRIGESTION, true);
	}
	
	wxPaintEvent e;
	canvas->OnPaint(e);
}


void CMainFrame::OnSave(wxCommandEvent& event)
{
	wxFileDialog fdlg(this, wxT("Enregistrer un fichier"), wxT(""), wxT("Sans_titre.tri"), wxT("*.tri"), wxSAVE|wxOVERWRITE_PROMPT|wxCHANGE_DIR);
	if(fdlg.ShowModal() == wxID_OK)
	{
		std::ofstream fs(fdlg.GetPath().fn_str(), std::ios::out);
		
		if(!fs)		// si on n'arrive pas a creer le fichier, on affiche un message d'erreur et on sort
		{
			wxString errormsg;
			errormsg.Printf(wxT("Impossible de creer le fichier."));
			errormsg.Append(fdlg.GetPath());
			wxMessageDialog msg(this, errormsg, wxT("Erreur"), wxOK|wxCENTRE|wxICON_ERROR);
			msg.ShowModal();
			return;
		}
		
		fs << tab_tri.size() << std::endl << std::endl;
		int i;
		for(i=0;i<tab_tri.size();i++)
		{
			fs << (tab_tri[i]).p1.x << " ";
			fs << (tab_tri[i]).p1.y << " ";
			fs << (tab_tri[i]).p2.x << " ";
			fs << (tab_tri[i]).p2.y << " ";
			fs << (tab_tri[i]).p3.x << " ";
			fs << (tab_tri[i]).p3.y;
			
			fs << std::endl;
			
			int r, g, b;
			r = static_cast<int>((tab_tri[i]).colour.Red());
			g = static_cast<int>((tab_tri[i]).colour.Green());
			b = static_cast<int>((tab_tri[i]).colour.Blue());
			fs << r << " ";
			fs << g << " ";
			fs << b << " ";
			
			fs << std::endl;
			
			fs << (tab_tri[i]).thickness;
			
			fs << std::endl << std::endl;
		}
	}
	
	wxPaintEvent e;
	canvas->OnPaint(e);
}


void CMainFrame::OnQuit(wxCommandEvent& event)		// fonction appelee quand on utilise le menu pour quitter l'application
{
	if(num_tri == 0)
	{
		wxMessageDialog msg(this, wxT("Voulez-vous vraiment quitter ?"), wxT("Triangulum "), wxYES_NO|wxNO_DEFAULT|wxCENTRE|wxICON_INFORMATION);
		
		if(msg.ShowModal() == wxID_YES)
			Destroy();
		
		return;
	}
	
	wxMessageDialog msg(this, wxT("Voulez-vous enregistrer vos modifications avant de quitter ?"), wxT("Triangulum "), wxYES_NO|wxNO_DEFAULT|wxCANCEL|wxCENTRE|wxICON_INFORMATION);
	int tmp = msg.ShowModal();
	
	if(tmp == wxID_NO)
		Destroy();
	
	if(tmp == wxID_YES)
	{
		wxCommandEvent e;
		OnSave(e);
		Destroy();
	}
}


void CMainFrame::OnQuit2(wxCloseEvent& event)		// fonction appelee quand on utilise la croix en haut a droite pour quitter l'application
{
	if(num_tri == 0)
	{
		wxMessageDialog msg(this, wxT("Voulez-vous vraiment quitter ?"), wxT("Triangulum "), wxYES_NO|wxNO_DEFAULT|wxCENTRE|wxICON_INFORMATION);
		
		if(msg.ShowModal() == wxID_YES)
			Destroy();
		
		return;
	}
	
	wxMessageDialog msg(this, wxT("Voulez-vous enregistrer vos modifications avant de quitter ?"), wxT("Triangulum "), wxYES_NO|wxNO_DEFAULT|wxCANCEL|wxCENTRE|wxICON_INFORMATION);
	int tmp = msg.ShowModal();
	
	if(tmp == wxID_NO)
		Destroy();
	
	if(tmp == wxID_YES)
	{
		wxCommandEvent e;
		OnSave(e);
		Destroy();
	}
}


void CMainFrame::OnToolBar(wxCommandEvent& event)
{
	if(m_toolbar->IsShown())
		m_toolbar->Show(FALSE);
	else
		m_toolbar->Show(TRUE);
}


void CMainFrame::OnThickness(wxCommandEvent& event)
{
	EpaisseurDialog edlg(this, -1, wxT("Epaisseur"));
	edlg.ShowModal();
}


void CMainFrame::OnColor(wxCommandEvent& event)
{
	CouleurDialog cdlg(this, -1, wxT("Couleur"));
	cdlg.ShowModal();
}


void CMainFrame::OnTriM(wxCommandEvent& event)
{
	TrianglesDialog tdlg(this, -1, wxT("Gestion des triangles"));
	tdlg.ShowModal();
}


void CMainFrame::OnVersion(wxCommandEvent& event)
{
	VersionDialog vdlg(this, -1, wxT("Version"));
	vdlg.ShowModal();
}


void CMainFrame::OnDraw(wxCommandEvent& event)
{
	if(m_toolbar->GetToolState(DRAW))	// si le bouton est enfonce
		is_drawing = true;
	else								// sinon
	{
		is_drawing = false;
		wxPaintEvent e;
		canvas->OnPaint(e);				// on repaint le canvas pour que le triangle en cours de dessin (non fini) soient efface, mais on garde ses coordonnees en memoire
	}
}


wxToolBar * CMainFrame::getToolbar()
{
	return m_toolbar;
}


void CMainFrame::OnHelp(wxCommandEvent& event)
{
	help->DisplayContents();
}




