#include <iostream>
#include <vector>

using namespace std;

typedef struct
{
    float x, y;
} COORD;

/**

Name: inPolygon
Input: vector<COORD>, COORD
Output: bool

Function: Check whether the target is in the polygon or not.

Notice: Dots on the edge will give an unpredictable result.

*/
bool inPolygon(vector<COORD> polygon, COORD target)
{
    int sides=polygon.size(), clamp=sides-1;
    bool result=false;


    for (int i=0; i<polygon.size(); i++)
    {
        // clamp through each dot that match the threshold
        if((polygon[i].y<target.y && polygon[clamp].y>=target.y
                || polygon[clamp].y<target.y && polygon[i].y>=target.y)
                // interpolation through lines
                && (polygon[i].x+
                    (target.y-polygon[i].y)/(polygon[clamp].y-polygon[i].y)*(polygon[clamp].x-polygon[i].x)
                    <target.x))
        {
            result=!result; // reverse bool, since odd-even rule
        }

        clamp=i; // lower the clamp
    }

    return result;
}

/**

Name: onEdge
Input: vector<COORD>, COORD
Output: bool

Function: Check whether the point is on the polygon or not.

*/
bool onEdge(vector<COORD> polygon, COORD target)
{
    int sides=polygon.size();
    COORD dot1, dot2;
    float slope, cons;

    for(int i=0; i<sides; i++)
    {
        // find two dot
        dot1=polygon[(i!=sides-1)?i:0];
        dot2=polygon[(i!=sides-1)?i+1:sides-1];

        // calculate coefficients
        slope=(dot1.y-dot2.y)/(dot1.x-dot2.x);
        cons=dot1.y-slope*dot1.x;

        // check if the equation match
        if(slope*target.x+cons==target.y)
            return true;
    }

    return false;
}

/**

Name: failsafe
Input: vector<COORD>
Output: int

Function: Fail safe for the input.

Return 1 when duplicate dots detected.
Return 2 when dots are on the same line.
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

    // check if straight line exist
    COORD a,b,c;
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

        if(slope1==slope2) return 2;
    }

    return 0;
}

int main()
{
    int sides;
    vector<COORD> polygon;
    COORD target, temp;

RESTART:
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

    for(int i=0; i<sides; i++)
    {
        for(;;)
        {
            cout << "Dot" << i+1 << ", (x y)= ";
            if (cin >> temp.x >> temp.y)
                break;
            else
            {
                cout << "Please enter a valid number pair." << endl << endl;
                cin.clear();
                cin.ignore(100, '\n');
            }
        }
        polygon.push_back(temp);
    }

    int check=failsafe(polygon);
    if(check==2)
        cout << "Some dots are on the same line." << endl;
    else if(check==1)
        cout << "Duplicate dots detected." << endl;

    if(check!=0)
    {
        cout << endl;
        goto RESTART;
    }

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
    for(int i=0; i<polygon.size(); i++)
        cout << polygon[i].x << " " << polygon[i].y << endl;
    cout <<endl;
    cout << target.x << " " << target.y << endl;
    cout << endl;

    cout << endl;
    if(onEdge(polygon, target))
        cout << "ON the polygon";
    else if(inPolygon(polygon, target))
        cout << "IN the polygon";
    else
        cout << "OUT the polygon";
    cout << endl;

    return 0;
}
