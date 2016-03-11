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

void ffd::deform(cgp::Point & pnt)
{
    // stub, needs completing
    //TODO
    //convert into stu format

    cgp::Vector S(diagonal.i,0.0,0.0);
    cgp::Vector T(0.0,diagonal.j,0.0);
    cgp::Vector U(0.0,0.0,diagonal.k);

    float s = 0, t = 0, u = 0;

    cgp::Vector TxU;
    TxU.cross(T,U);
    cgp::Vector SxU;
    SxU.cross(S,U);
    cgp::Vector SxT;
    SxT.cross(S,T);

    cgp::Vector xXo( pnt.x - origin.x, pnt.y - origin.y, pnt.z - origin.z);

    float s1=0,s2=0,t1=0,t2=0,u1=0,u2=0;

    cgp::Vector topS;
    s1 = topS.dot(TxU,xXo);
    cgp::Vector botS;
    s2 = botU.dot(TxU,S);

    cgp::Vector topT;
    t1 = topS.dot(SxU,xXo);
    cgp::Vector botT;
    t2 = botU.dot(SxU,T);

    cgp::Vector topU;
    u1 = topS.dot(SxT,xXo);
    cgp::Vector botU;
    u2 = botU.dot(SxT,U);


    s = s1/s2;
    t = t1/t2;
    u = u1/u2;


    //get xffd
    float ansS, ansT, ansU;
    float l = dmix-1, m = dimy-1, n = dimz-1;
    //s
    for (int i = 0; i < l; i++){
        ansS += (1-s);
    }
    //t
    for (int j = 0; j < m; j++){

    }
    //u
    for (int k = 0; k < n; k++){

    }


}

    unsigned nChoosek( unsigned n, unsigned k )
{
    if (k > n) return 0;
    if (k * 2 > n) k = n-k;
    if (k == 0) return 1;

    int result = n;
    for( int i = 2; i <= k; ++i ) {
        result *= (n-i+1);
        result /= i;
    }
    return result;
}
