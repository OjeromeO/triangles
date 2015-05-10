#include "triangle.h"



bool Triangle::IsPointInTriangle(int x, int y)		// renvoie vrai si le point est dans le triangle, faux sinon
{
	int a=(x-p1.x)*(p2.y-p1.y) - (y - p1.y)*(p2.x-p1.x);
	int b=(x-p2.x)*(p3.y-p2.y) - (y - p2.y)*(p3.x-p2.x);
	int c=(x-p3.x)*(p1.y-p3.y) - (y - p3.y)*(p1.x-p3.x);

	if( (a>0&&b>0&&c>0) || (a<0&&b<0&&c<0))
		return (TRUE);
	else
		return (FALSE);
}



