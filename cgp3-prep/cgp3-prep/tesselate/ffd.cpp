//
// ffd
//

#include "ffd.h"
#include <stdio.h>

using namespace std;

GLfloat defaultLatCol[] = {0.2f, 0.2f, 0.2f, 1.0f};
GLfloat highlightLatCol[] = {1.0f, 0.176f, 0.176f, 1.0f};
int maxbezorder = 4;

void ffd::alloc()
{
    // allocate memory for a 3D array of control points and highlighting switches
    if(dimx > 1 && dimy > 1 && dimz > 1 && dimx <= maxbezorder && dimy <= maxbezorder && dimz <= maxbezorder)
    {
        cp = new cgp::Point **[dimx];
        highlight = new bool **[dimx];
        for (int i = 0; i < dimx; i++)
        {
            cp[i] = new cgp::Point *[dimy];
            highlight[i] = new bool *[dimy];

            for (int j = 0; j < dimy; j++)
            {
                cp[i][j] = new cgp::Point[dimz];
                highlight[i][j] = new bool[dimz];
            }
        }
        deactivateAllCP();
    }
}

void ffd::dealloc()
{
    // deallocate 3D array of control points and boolean highlighting switches
    for (int i = 0; i < dimx; i++)
    {
        for (int j = 0; j < dimy; j++)
        {
            delete [] cp[i][j];
            delete [] highlight[i][j];
        }

        delete [] cp[i];
        delete [] highlight[i];
    }
    delete [] cp;
    delete [] highlight;
    cp = NULL;
}

bool ffd::inCPBounds(int i, int j, int k)
{
    return (i >= 0 && j >= 0 && k >= 0 && i < dimx && j < dimy && k < dimz);
}

ffd::ffd()
{
    dimx = dimy = dimz = 0;
    setFrame(cgp::Point(0.0f, 0.0f, 0.0f), cgp::Vector(0.0f, 0.0f, 0.0f));
    cp = NULL;
    highlight = NULL;
}

ffd::ffd(int xnum, int ynum, int znum, cgp::Point corner, cgp::Vector diag)
{
    dimx = xnum;
    dimy = ynum;
    dimz = znum;

    setFrame(corner, diag);
    alloc();
}

void ffd::reset()
{
    //we get 3x3x3
    //loop over the volume and create the control points
    for (int x = 0; x < dimx; x++){
        for (int y = 0; y < dimy; y++){
            for (int z = 0; z < dimz; z++){
                //get the distances and seperate the points with these distances (diagonal/dimension)
                float distX = origin.x + x * diagonal.i/(dimx-1);
                float distY = origin.y + y * diagonal.j/(dimy-1);
                float distZ = origin.z + z * diagonal.k/(dimz-1);
                //temp point
                cgp::Point point(distX,distY,distZ);
                cout << "point: " << point.x << " " << point.y << " " << point.z << endl;

                //create the control point
                setCP(x,y,z,point);
            }
        }
    }

}

void ffd::getDim(int &numx, int &numy, int &numz)
{
    numx = dimx; numy = dimy; numz = dimz;
}

void ffd::setDim(int numx, int numy, int numz)
{
    dimx = numx; dimy = numy; dimz = numz;
    alloc();
    reset();
}

void ffd::getFrame(cgp::Point &corner, cgp::Vector &diag)
{
    corner = origin;
    diag = diagonal;
}

void ffd::setFrame(cgp::Point corner, cgp::Vector diag)
{
    origin = corner;
    diagonal = diag;
    reset();
}

void ffd::activateCP(int i, int j, int k)
{
    if(inCPBounds(i,j,k))
        highlight[i][j][k] = true;
}

void ffd::deactivateCP(int i, int j, int k)
{
    if(inCPBounds(i,j,k))
        highlight[i][j][k] = false;
}

void ffd::deactivateAllCP()
{
    for(int i = 0; i < dimx; i++)
        for(int j = 0; j < dimy; j++)
            for(int k = 0; k < dimz; k++)
                highlight[i][j][k] = false;
}

bool ffd::bindGeometry(View * view, ShapeDrawData &sdd, bool active)
{
    int i, j, k;
    glm::mat4 tfm, idt;
    glm::vec3 trs;
    cgp::Point pnt;
    bool draw;

    if(active)
    {
        activegeom.clear();
        activegeom.setColour(highlightLatCol);
    }
    else
    {
        geom.clear();
        geom.setColour(defaultLatCol);
    }

    idt = glm::mat4(1.0f); // identity matrix

    // place a sphere at non-active control point positions with appropriate colour
    for(i = 0; i < dimx; i++)
        for(j = 0; j < dimy; j++)
            for(k = 0; k < dimz; k++)
            {
                if(active) // only draw those control points that match active flag
                    draw = highlight[i][j][k];
                else
                    draw = !highlight[i][j][k];

                if(draw)
                {
                    pnt = cp[i][j][k];
                    trs = glm::vec3(pnt.x, pnt.y, pnt.z);
                    tfm = glm::translate(idt, trs);
                    if(active)
                        activegeom.genSphere(0.4, 10, 10, tfm);
                    else
                        geom.genSphere(0.4, 10, 10, tfm);
                }
            }

    // bind geometry to buffers and return drawing parameters, if possible
    if(active)
    {
        if(activegeom.bindBuffers(view))
        {
            sdd = activegeom.getDrawParameters();
            return true;
        }
        else
            return false;
    }
    else
    {
        if(geom.bindBuffers(view))
        {
            sdd = geom.getDrawParameters();
            return true;
        }
        else
            return false;
    }
}

cgp::Point ffd::getCP(int i, int j, int k)
{
    if(inCPBounds(i,j,k))
    {
        return cp[i][j][k];
    }
    else
    {
        cerr << "Error ffd::getCP: out of bounds access to lattice" << endl;
        return cgp::Point(0.0f, 0.0f, 0.0f);
    }
}

void ffd::setCP(int i, int j, int k, cgp::Point pnt)
{
    if(inCPBounds(i,j,k))
        cp[i][j][k] = pnt;
}


std::vector<float> ffd::calcSTU(cgp::Point & pnt){
    //create S,T,U vectors from diagonal
    cgp::Vector S(diagonal.i,0.0,0.0);
    cgp::Vector T(0.0,diagonal.j,0.0);
    cgp::Vector U(0.0,0.0,diagonal.k);
    //init
    float s = 0, t = 0, u = 0;

    //create cross product vectors for parts of the s t u calcualtions
    cgp::Vector TxU;
    TxU.cross(T,U);
    cgp::Vector SxU;
    SxU.cross(S,U);
    cgp::Vector SxT;
    SxT.cross(S,T);

    //create difference of point and origin
    cgp::Vector xXo( pnt.x - origin.x, pnt.y - origin.y, pnt.z - origin.z);

    //temps
    float s1=0,s2=0,t1=0,t2=0,u1=0,u2=0;

    //dot product for s top
    s1 = TxU.dot(xXo);
    //dot product for s bottom
    s2 = TxU.dot(S);

    //dot product for t top
    t1 = SxU.dot(xXo);
    //dot product for t bottom
    t2 = SxU.dot(T);

    //dot product for u top
    u1 = SxT.dot(xXo);
    //dot product for u bottom
    u2 = SxT.dot(U);

    //calculate s,t,u
    s = (float)s1/(float)s2;
    t = (float)t1/(float)t2;
    u = (float)u1/(float)u2;

    std::vector<float> temp;
    temp.push_back(s);
    temp.push_back(t);
    temp.push_back(u);

    return temp;

}


void ffd::deform(cgp::Point & pnt)
{

    std::vector<float> stu;

    stu = calcSTU(pnt);

    float s = stu[0];
    float t = stu[1];
    float u = stu[2];
    //calculate xffd elements s,t,u
    //create l,m & n
    float l = dimx-1, m = dimy-1, n = dimz-1;

    //create points to hold the sums for each xyz
    cgp::Point sumX, sumY, sumZ;
    //loop over all control points
    for (int i = 0; i < dimx; i++){
        sumY.reset();
        for (int j = 0; j < dimy; j++){
            sumZ.reset();
            for (int k = 0; k < dimz; k++){
                //get control point
                cgp::Point cp = getCP(i,j,k);
                //calculate the scalar part & then multiply it by the control point counter parts
                float valueK = (float)nChoosek(n,k) * (float)pow((1-u),n-k) * (float)pow(u,k);
                sumZ.x += valueK * cp.x;
                sumZ.y += valueK * cp.y;
                sumZ.z += valueK * cp.z;
            }
            //same operation just using the previous sum and the new scalar part
            float valueJ = (float)nChoosek(m,j) * (float)pow((1-t),m-j) * (float)pow(t,j);
            sumY.x += valueJ * sumZ.x;
            sumY.y += valueJ * sumZ.y;
            sumY.z += valueJ * sumZ.z;
        }
        //same operation just using the previous sum and the new scalar part
        float valueI = (float)nChoosek(l,i) * (float)pow((1-s),l-i) * (float)pow(s,i);
        sumX.x += valueI * sumY.x;
        sumX.y += valueI * sumY.y;
        sumX.z += valueI * sumY.z;
    }
    //assign new point
    pnt = sumX;
}


//method to calulate the combination
//method taken from below and changed to use floats:
//http://stackoverflow.com/questions/9330915/number-of-combinations-n-choose-r-in-c
float ffd::nChoosek( float n, float k )
{
    if (k > n) return 0;
    if (k * 2 > n) k = n-k;
    if (k == 0) return 1;

    float result = n;
    for( int i = 2; i <= k; ++i ) {
        result *= (n-i+1);
        result /= i;
    }
    return result;
}
