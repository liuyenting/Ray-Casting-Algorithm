/**

Procedure:
1. Read input
2. Check if the polygon exist (not complex, no duplicate, same line)
3. Check the dot through angle sum

Concept:
If the dot is inside the polygon, sum of the surrounding angle is 360deg.
If the dot is on the line of the polygon, sum is 180deg.
If the dot is outside the polygon, sum is less than 180deg.

Subroutine:
1. Convert dot set into vector
2. Dot product of the vectors
3. Calculate the length of the vectors
4. Acquire arc-cosine value

*/
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

#define PI 3.14159265

using namespace std;

typedef struct
{
    float x;
    float y;
} COORD;

/**

Name: dotProduct
Input: COORD, COORD
Output: float

Function: Calculate dot product of the vectors.

*/
float dotProduct(COORD vec1, COORD vec2)
{
    return vec1.x*vec2.x+vec1.y*vec2.y;
}

/**

Name: vecLength
Input: COORD
Output: float

Function: Calculate length of the vector.

*/
float vecLength(COORD vec)
{
    return sqrt(vec.x*vec.x + vec.y*vec.y);
}

/**

Name: vecAngle
Input: COORD, COORD, COORD
Output: float

Function: Calculate the angle between two vectors.

*/
float vecAngle(COORD pa, COORD pb, COORD cen)
{
    COORD vector1, vector2;
    vector1.x=pa.x-cen.x;
    vector1.y=pa.y-cen.y;
    vector2.x=pb.x-cen.x;
    vector2.y=pb.y-cen.y;
    /*
        cout << vector1.x << " " <<vector1.y<<endl;
        cout <<vector2.x << " " <<vector2.y << endl;

        cout << vecLength(vector1) << " " << vecLength(vector2) << " " << dotProduct(vector1, vector2) << endl;
    */
    float length1=vecLength(vector1);
    float length2=vecLength(vector2);
    float cost=dotProduct(vector1, vector2)/(length1*length2);
    return acos(cost)*180.0/PI;
}

/**

Name: filter
Input: COORD, vector<COORD>
Output: bool

Function: Check if the target dot is on the polygon's edge.

*/
bool filter(COORD target, vector<COORD> polygon)
{
    for(int i=0; i<polygon.size(); i++)
    {
        COORD dot1, dot2;
        if(i!=polygon.size()-1)
        {
            dot1=polygon[i];
            dot2=polygon[i+1];
        }
        else
        {
            dot1=polygon[0];
            dot2=polygon[polygon.size()-1];
        }

        float slope=(dot1.y-dot2.y)/(dot1.x-dot2.x);
        float cons=dot1.y-slope*dot1.x;

        if(slope*target.x+cons==target.y)
            return true;
    }

    return false;
}

/**

Name: identify
Input: COORD, vector<COORD>
Output: int

Function: Check the target dot status.

Return 1 when the dot is on the polygon's edge.
Return 2 when the dot is inside the polygon.
Otherwise, return -1;

*/
int identify(COORD target, vector<COORD> polygon)
{
    if(filter(target, polygon))
        return 1;

    float angle=0;
    for(int i=0; i<polygon.size(); i++)
    {
        if(i!=polygon.size()-1)
            angle+=vecAngle(polygon[i], polygon[i+1], target);
        else
            angle+=vecAngle(polygon[0], polygon[polygon.size()-1], target);
    }

    if(angle==360)
        return 2;

    return -1;
}

/**

Name: segmentIntersection
Input: COORD
Output: bool

Function: Check if intersection exist.

*/
bool segmentIntersection(COORD p1, COORD p2, COORD p3, COORD p4)
{

    COORD a, b, c;

    a.x=p2.x-p1.x;
    a.y=p2.y-p1.y;

    b.x=p3.x-p4.x;
    b.y=p3.y-p4.y;

    c.x=p1.x-p3.x;
    c.y=p1.y-p3.y;

    float alphaNumerator = b.y*c.x - b.x*c.y;
    float alphaDenominator = a.y*b.x - a.x*b.y;
    float betaNumerator  = a.x*c.y - a.y*c.x;
    float betaDenominator  = a.y*b.x - a.x*b.y;

    bool doIntersect = true;

    if (alphaDenominator == 0 || betaDenominator == 0)
    {
        doIntersect = false;
    }
    else
    {

        if (alphaDenominator > 0)
        {
            if (alphaNumerator < 0 || alphaNumerator > alphaDenominator)
            {
                doIntersect = false;

            }
        }
        else if (alphaNumerator > 0 || alphaNumerator < alphaDenominator)
        {
            doIntersect = false;
        }

        if (doIntersect && betaDenominator > 0)
        {
            if (betaNumerator < 0 || betaNumerator > betaDenominator)
            {
                doIntersect = false;
            }
        }
        else if (betaNumerator > 0 || betaNumerator < betaDenominator)
        {
            doIntersect = false;
        }
    }

    return doIntersect;
}

/**

Name: isComplexPolygon
Input: vector<COORD>
Output: bool

Function: Check if the polygon is complex (has intersection).

*/
bool isComplexPolygon(vector<COORD> polygon)
{
    for(int i=0; i<polygon.size()-2 ; i++)
    {
        int n=i+polygon.size()-2;

        if (i>=2)
            n-=i-1;

        for(int j=i+2; j<=n; j++)
        {
            if (segmentIntersection(polygon[i],
                                    polygon[i+1],
                                    polygon[(j)%polygon.size()],
                                    polygon[(j+1)%polygon.size()]))
                return true;

        }
    }

    return false;
}

/**

Name: isConcave
Input: vector<COORD>
Output: bool

Function: Check if the polygon is concave.

*/
bool isConcave(vector<COORD> p)
{
   int n=p.size();
   int i,j,k;
   int flag = 0;
   double z;

   if (n < 3)
      return(0);

   for (i=0;i<n;i++) {
      j = (i + 1) % n;
      k = (i + 2) % n;
      z  = (p[j].x - p[i].x) * (p[k].y - p[j].y);
      z -= (p[j].y - p[i].y) * (p[k].x - p[j].x);
      if (z < 0)
         flag |= 1;
      else if (z > 0)
         flag |= 2;
      if (flag == 3)
         return true;
   }
   if (flag != 0)
      return false;
   else
      return(0);
}

/**

Name: failsafe
Input: vector<COORD>
Output: int

Function: Fail safe for the input.

Return 1 when duplicate dots detected.
Return 2 when the polygon is a complex polygon.
Return 3 when dots are on the same line.
Otherwise, return 0.

*/
int failsafe(vector<COORD> polygon)
{
    // check duplicate dot
    for(int i=0; i<polygon.size()-1; i++)
    {
        for(int j=i+1; j<polygon.size(); j++)
            if(polygon[i].x==polygon[j].x && polygon[i].y==polygon[j].y) return 1;

    }

    // complex polygon
    if(isComplexPolygon(polygon)) return 2;
    if(isConcave(polygon)) return 2;

    COORD a, b, c;
    for(int i=0; i<polygon.size(); i++)
    {
        if(i==polygon.size()-2)
        {
            a=polygon[i];
            b=polygon[i+1];
            c=polygon[0];
        }
        else if(i==polygon.size()-1)
        {
            a=polygon[i];
            b=polygon[0];
            c=polygon[1];
        }
        else
        {
            a=polygon[i];
            b=polygon[i+1];
            c=polygon[i+2];
        }

        if(vecAngle(a, c, b)>180.0) return 2;
    }

    // straight line
    for(int i=0; i<polygon.size(); i++)
    {
        if(i==polygon.size()-2)
        {
            a=polygon[i];
            b=polygon[i+1];
            c=polygon[0];
        }
        else if(i==polygon.size()-1)
        {
            a=polygon[i];
            b=polygon[0];
            c=polygon[1];
        }
        else
        {
            a=polygon[i];
            b=polygon[i+1];
            c=polygon[i+2];
        }
        float slope1=(a.y-b.y)/(a.x-b.x);
        float slope2=(a.y-c.y)/(a.x-c.x);

        if(slope1==slope2) return 3;
    }

    return 0;
}

int main()
{
RESTART:
    int sides;
    for (;;)
    {
        cout << "How many sides this polygon has? S=";
        if (cin >> sides)
        {
            if(sides>=0 && sides<2)
                cout << "Sides count less than 3 is not valid." << endl << endl;
            else if(sides<0)
                cout << "Negative nubmer for sides is not acceptable." << endl << endl;
            else
                break;
        }
        else
        {
            cout << "Please enter a valid integer." << endl << endl;
            cin.clear();
            cin.ignore(100, '\n');
        }
    }

    vector<COORD> polygon;
    COORD temp;
    for(int i=0; i<sides; i++)
    {
        for(;;)
        {
            cout << "Dot" << i+1 << ", (x y)= ";
            if (cin >> temp.x >> temp.y)
                break;
            else
            {
                cout << "Please enter valid number pair." << endl << endl;
                cin.clear();
                cin.ignore(100, '\n');
            }
        }
        polygon.push_back(temp);
    }

    int check=failsafe(polygon);
    if(check==3)
        cout << "Some dots are on the same line." << endl;
    else if(check==2)
        cout << "Only convex polygon is acceptable." << endl;
    else if(check==1)
        cout << "Duplicate dots detected." << endl;

    if(check!=0)
    {
        cout << endl;
        goto RESTART;
    }

    COORD target;
    for (;;)
    {
        cout << "Target dot, (x y)= ";
        if (cin >> target.x >> target.y)
            break;
        else
        {
            cout << "Please enter a valid number pair." << endl << endl;
            cin.clear();
            cin.ignore(100, '\n');
        }
    }

    cout << endl;
    int result=identify(target, polygon);
    if(result==2)
        cout << "INside the polygon";
    else if(result==1)
        cout << "ON the polygon";
    else
        cout << "OUTside the polygon";
    cout << endl;

    return 0;
}
