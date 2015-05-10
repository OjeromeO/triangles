#include <stdio.h>
#include <stdlib.h>
#include <wx/wx.h>

#include "mainframe.h"
#include "openglcanvas.h"
#include "triangle.h"
#include "dialogs.h"



BEGIN_EVENT_TABLE(OpenGLCanvas, wxGLCanvas)
EVT_PAINT(OpenGLCanvas::OnPaint)
EVT_SIZE(OpenGLCanvas::OnSize)
EVT_ERASE_BACKGROUND(OpenGLCanvas::OnEraseBackground)
EVT_MOTION(OpenGLCanvas::OnMouseMove)
EVT_LEFT_DOWN(OpenGLCanvas::OnLeftDown)
EVT_LEFT_UP(OpenGLCanvas::OnLeftUp)
EVT_RIGHT_DOWN(OpenGLCanvas::OnRightDown)
EVT_MENU(POPPROP, OpenGLCanvas::OnContextPptes)
EVT_MENU(POPSUPPR, OpenGLCanvas::OnContextSuppr)
END_EVENT_TABLE()



OpenGLCanvas::OpenGLCanvas(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
: wxGLCanvas(parent, id, pos, size, style, name)
{
	pa.x = -10000;		// on initialise nos points a de très grandes valeurs, impossibles a atteindre pour l'utilisateur
	pa.y = -10000;
	pb.x = -10000;
	pb.y = -10000;
	pc.x = -10000;
	pc.y = -10000;
	selected_tri = -1;
	x = -10000;
	y = -10000;
}


OpenGLCanvas::~OpenGLCanvas(void)
{
}


void OpenGLCanvas::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);		// on fait de notre fenetre principale (sauf la barre d'outils) la zone d'affichage
    SetCurrent();			// on dit a openGL de lier son contexte de rendu au device context implicite ci-dessus
	Draw();
	SwapBuffers();			// rafraichissement
}


void OpenGLCanvas::OnSize(wxSizeEvent& event)
{
	wxGLCanvas::OnSize(event);
	int w, h;
	GetClientSize(&w, &h);
	glViewport(0, 0, (GLint) w, (GLint) h);		// definit la fenetre logique (= sous fenetre) dans laquelle on va dessiner : ici il s'agit de tout notre canvas
}


void OpenGLCanvas::OnEraseBackground(wxEraseEvent& event)
{
}


void OpenGLCanvas::Draw()
{
	glMatrixMode(GL_PROJECTION);						// matrice de projection
	glLoadIdentity();
	int w, h;
	GetClientSize(&w, &h);
	glOrtho(-w/2., w/2., -h/2., h/2., -1, 3);
	
	glMatrixMode(GL_MODELVIEW);							// matrice de modelisation
	glLoadIdentity();
	glClearColor( .3f, .4f, .6f, 1);					// declaration des valeurs a utiliser pour l'initialisation du color buffer (= remplissage) a chaque glClear() sur le color buffer
	glClear(GL_COLOR_BUFFER_BIT);						// effacement du buffer d'affichage (= color buffer)
	
	glEnable(GL_LINE_SMOOTH);							// on active l'antialiasing
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// pour cela on configure le blending
	glEnable(GL_BLEND);									// et on l'active ensuite
	
	int i;
	int n = static_cast<CMainFrame *>(this->GetParent())->num_tri;
	for(i=0;i<n;i++)									// dessin des triangles du tableau
	{
		Triangle * t = new Triangle();
		*t = static_cast<CMainFrame *>(this->GetParent())->tab_tri[i];
		
		if(t->colour == *wxRED)
			glColor3ub(255, 0, 0);
		
		if(t->colour == *wxGREEN)
			glColor3ub(0, 255, 0);
		
		if(t->colour == *wxBLUE)
			glColor3ub(0, 0, 255);
		
		glBegin(GL_TRIANGLES);							// dessin du triangle plein
		glVertex2f(t->p1.x, t->p1.y);
		glVertex2f(t->p2.x, t->p2.y);
		glVertex2f(t->p3.x, t->p3.y);
		glEnd();
		
		glColor3ub(0, 0, 0);
		glLineWidth(t->thickness);
		
		glBegin(GL_LINE_LOOP);							// dessin de la bordure
		glVertex2f(t->p1.x, t->p1.y);
		glVertex2f(t->p2.x, t->p2.y);
		glVertex2f(t->p3.x, t->p3.y);
		glEnd();
	}
	
	if((pa.x == -10000)&&(pa.y == -10000)&&(pb.x == -10000)&&(pb.y == -10000)&&(pc.x == -10000)&&(pc.y == -10000))	// si on a toujours les 3 valeurs par defaut, on ne fait rien de plus et on retourne
		return;
	
	float ep = static_cast<CMainFrame *>(this->GetParent())->epaisseurtraitcourante;
	wxColour c = static_cast<CMainFrame *>(this->GetParent())->couleurcourante;
	
	if((pb.x == -10000)&&(pb.y == -10000)&&(pc.x == -10000)&&(pc.y == -10000))										// si on a 2 valeurs par defaut, on dessine un trait et on retourne
	{
		glLineWidth(ep);
		glColor3ub(0, 0, 0);
		glBegin(GL_LINES);
		glVertex2f(pa.x, pa.y);
		glVertex2f(x-w/2., h/2.-y);
		glEnd();
		return;
	}
	
	if((pc.x == -10000)&&(pc.y == -10000))																			// si on a 1 valeur par defaut, on dessine un triangle et on retourne
	{
		glLineWidth(ep);
		
		if(c == *wxRED)
			glColor3ub(255, 0, 0);
		
		if(c == *wxGREEN)
			glColor3ub(0, 255, 0);
		
		if(c == *wxBLUE)
			glColor3ub(0, 0, 255);
		
		glBegin(GL_TRIANGLES);
		glVertex2f(pa.x, pa.y);
		glVertex2f(pb.x, pb.y);
		glVertex2f(x-w/2., h/2.-y);
		glEnd();
		
		glColor3ub(0, 0, 0);
		glBegin(GL_LINE_LOOP);
		glVertex2f(pa.x, pa.y);
		glVertex2f(pb.x, pb.y);
		glVertex2f(x-w/2., h/2.-y);
		glEnd();
		return;
	}
	
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_BLEND);
}


void OpenGLCanvas::OnMouseMove(wxMouseEvent& event)		// on trace un segment ou un triangle
{
	wxToolBar * tb = static_cast<CMainFrame *>(this->GetParent())->getToolbar();
	
	if(!(tb->GetToolState(DRAW)))			// si le bouton draw n'est pas enfonce, on ne fait rien et on retourne
		return;
	
	if((pa.x == -10000)&&(pa.y == -10000)&&(pb.x == -10000)&&(pb.y == -10000)&&(pc.x == -10000)&&(pc.y == -10000))	// si on a toujours les 3 valeurs par defaut, on ne fait rien et on retourne
		return;
	
	x = event.GetX();
	y = event.GetY();
	
	wxPaintEvent e;
	OnPaint(e);
}


void OpenGLCanvas::OnLeftDown(wxMouseEvent& event)
{
	wxToolBar * tb = static_cast<CMainFrame *>(this->GetParent())->getToolbar();
	if(!(tb->GetToolState(DRAW)))			// si le bouton draw n'est pas enfonce, on ne fait rien et on retourne
		return;
	
	wxColour c = static_cast<CMainFrame *>(this->GetParent())->couleurcourante;
	float ep = static_cast<CMainFrame *>(this->GetParent())->epaisseurtraitcourante;
	
	int w, h;
	GetClientSize(&w, &h);

	if((pa.x == -10000)&&(pa.y == -10000)&&(pb.x == -10000)&&(pb.y == -10000)&&(pc.x == -10000)&&(pc.y == -10000))	// si on a toujours les 3 valeurs par defaut, on enregistre le premier point et on retourne
	{
		pa.x = event.GetX()-w/2.;
		pa.y = h/2.-event.GetY();
		return;
	}
	
	if((pb.x == -10000)&&(pb.y == -10000)&&(pc.x == -10000)&&(pc.y == -10000))										// si on a 2 valeurs par defaut, on enregistre le deuxieme point et on retourne
	{
		pb.x = event.GetX()-w/2.;
		pb.y = h/2.-event.GetY();
		return;
	}
	
	if((pc.x == -10000)&&(pc.y == -10000))																			// si on a 1 seule valeur par defaut, on enregistre le dernier point...
	{
		pc.x = event.GetX()-w/2.;
		pc.y = h/2.-event.GetY();
		Triangle * tr = new Triangle();
		tr->p1 = pa;
		tr->p2 = pb;
		tr->p3 = pc;
		tr->colour = c;
		tr->thickness = ep;
		static_cast<CMainFrame *>(this->GetParent())->tab_tri.push_back(*tr);										// ...puis on ajoute le triangle au tableau...
		static_cast<CMainFrame *>(this->GetParent())->num_tri++;
		
		pa.x = -10000;																								// ... et on reinitialise les valeurs
		pa.y = -10000;
		pb.x = -10000;
		pb.y = -10000;
		pc.x = -10000;
		pc.y = -10000;
		
		if(static_cast<CMainFrame *>(this->GetParent())->num_tri == 1)												// et si c'est le premier, on degrise la gestion des triangles
			static_cast<CMainFrame *>(this->GetParent())->GetMenuBar()->Enable(TRIGESTION, true);
	}
	
	wxPaintEvent e;
	OnPaint(e);
}


void OpenGLCanvas::OnLeftUp(wxMouseEvent& event)
{
}


void OpenGLCanvas::OnRightDown(wxMouseEvent& event)
{
	popup = new wxMenu;

	fichier = new wxMenu;
	gestion = new wxMenu;
	valcour = new wxMenu;

	int w, h;
	GetClientSize(&w, &h);

	int tmp = -1;
	tmp = Est_dans_triangle(event.GetX()-w/2., h/2.-event.GetY());

	if(tmp == -1)	// si on n'a pas clique sur un triangle, menu par defaut
	{
		fichier->Append(OUVRIR,wxT("Ouvrir"));
		fichier->Append(SAUVEGARDER,wxT("Sauvegarder"));
		gestion->Append(TRIGESTION,wxT("Gestion des triangles"));
		valcour->Append(EPAISSEUR,wxT("Epaisseur de trait"));
		valcour->Append(COULEUR,wxT("Couleur"));
	
		popup->Append(POPFICH,wxT("Fichier"),fichier);
		popup->Append(POPGEST,wxT("Gestion"),gestion);
		popup->Append(POPCOUL,wxT("Options"),valcour);
	
		if(static_cast<CMainFrame *>(this->GetParent())->num_tri == 0)
			popup->Enable(TRIGESTION, false);
	}
	else			// sinon, menu special et on enregistre le numero du triangle
	{
		selected_tri = tmp;
		popup->Append(POPPROP,wxT("Proprietes du triangle"));
		popup->Append(POPSUPPR,wxT("Supprimer le triangle"));
	}
	
	PopupMenu(popup, event.GetX(),event.GetY());
}


int OpenGLCanvas::Est_dans_triangle(int x, int y)	// donne le numero du triangle sur lequel on a clique, -1 sinon
{
	int i;
	for(i=(static_cast<CMainFrame *>(this->GetParent())->num_tri - 1);i>=0;i--)		// on parcourt dans l'ordre decroissant, puisque c'est le dernier triangle qui vient d'etre trace qui a la priorite
	{
		if(static_cast<CMainFrame *>(this->GetParent())->tab_tri[i].IsPointInTriangle(x,y))		// si c'est dans un triangle
			return i;
	}
	return -1;	// si ce n'est pas dans un triangle
}


void OpenGLCanvas::OnContextPptes(wxCommandEvent& event)
{
	Triangle * t = new Triangle();
	*t = static_cast<CMainFrame *>(this->GetParent())->tab_tri[selected_tri];
	
	PropDialog pdlg(this, 1001, wxT("Proprietes"));
	
	wxString tr;
	tr.Printf(wxT("Triangle "));
	tr << (selected_tri+1);		// on fait en sorte de ne pas commencer a 0
	
	pdlg.item002->SetValue(tr);
	pdlg.item004->SetValue(t->thickness);
	
	if(t->colour == *wxRED)
		pdlg.item01->SetSelection(0);
	
	if(t->colour == *wxGREEN)
		pdlg.item01->SetSelection(1);
	
	if(t->colour == *wxBLUE)
		pdlg.item01->SetSelection(2);
	
	pdlg.ShowModal();
}


void OpenGLCanvas::OnContextSuppr(wxCommandEvent& event)
{
	if(static_cast<CMainFrame *>(this->GetParent())->num_tri == 1)		// s'il reste un seul triangle, celui qu'on va supprimer, alors on grise la gestion des triangles
		(static_cast<CMainFrame *>(this->GetParent())->GetMenuBar())->Enable(TRIGESTION, false);
	
	static_cast<CMainFrame *>(this->GetParent())->tab_tri.erase(static_cast<CMainFrame *>(this->GetParent())->tab_tri.begin() + selected_tri);
	static_cast<CMainFrame *>(this->GetParent())->num_tri--;
}





