#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <test/testutil.h>
#include "test_ffd.h"
#include <stdio.h>
#include <cstdint>
#include <sstream>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/extensions/HelperMacros.h>

void TestFFD::setUp()
{
    ffdobj = new ffd();
}

void TestFFD::tearDown()
{
    delete ffdobj;
}



void TestFFD::testNChooseK(){
    cout << "\t...TESTING N CHOOSE K METHOD..." << endl;

    //testing basic case
    float an1 = 1;
    float an2 = 4;
    float an3 = 6;
    CPPUNIT_ASSERT(ffdobj->nChoosek(4,4) == an1);
    CPPUNIT_ASSERT(ffdobj->nChoosek(4,3) == an2);
    CPPUNIT_ASSERT(ffdobj->nChoosek(4,2) == an3);
    CPPUNIT_ASSERT(ffdobj->nChoosek(4,1) == an2);
    CPPUNIT_ASSERT(ffdobj->nChoosek(4,0) == an1);

    //answers calculated on calculator
    float ans1 = 56;
    float ans2 = 45;
    float ans3 = 13;
    //testing bigger numbers
    CPPUNIT_ASSERT(ffdobj->nChoosek(8,3) == ans1);
    CPPUNIT_ASSERT(ffdobj->nChoosek(10,2) == ans2);
    CPPUNIT_ASSERT(ffdobj->nChoosek(13,12) == ans3);

    cout << "\t...TESTED N CHOOSE K METHOD..." << endl;
}



void TestFFD::resetTest(){
    cout << "\t...TESTING RESET METHOD..." << endl;
    cgp::Vector diag(2.0f, 2.0f, 2.0f);
    cgp::Point corner(-1.0f, -1.0f, -1.0f);
    ffdobj->setDim(2,2,2);

    //CHECK THAT all are 0

    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 2; j++){
            for (int k = 0; k < 2; k++){
                CPPUNIT_ASSERT(ffdobj->getCP(i,j,k).x == 0);
                CPPUNIT_ASSERT(ffdobj->getCP(i,j,k).y == 0);
                CPPUNIT_ASSERT(ffdobj->getCP(i,j,k).z == 0);
            }
        }
    }


    ffdobj->setFrame(corner,diag);


    std::vector<cgp::Point> answers;
    answers.push_back(cgp::Point(-1,-1,-1));
    answers.push_back(cgp::Point(-1,-1,1));
    answers.push_back(cgp::Point(-1,1,-1));
    answers.push_back(cgp::Point(-1,1,1));
    answers.push_back(cgp::Point(1,-1,-1));
    answers.push_back(cgp::Point(1,-1,1));
    answers.push_back(cgp::Point(1,1,-1));
    answers.push_back(cgp::Point(1,1,1));

    int index = 0;
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 2; j++){
            for (int k = 0; k < 2; k++ ){
                CPPUNIT_ASSERT(answers[index].x == ffdobj->getCP(i,j,k).x);
                CPPUNIT_ASSERT(answers[index].y == ffdobj->getCP(i,j,k).y);
                CPPUNIT_ASSERT(answers[index].z == ffdobj->getCP(i,j,k).z);
                index++;
            }
        }
    }

    cout << "\t...TESTED RESET METHOD..." << endl;
}


void TestFFD::testSTU(){
    cout << "\t...TESTING STU METHOD..." << endl;
    CPPUNIT_ASSERT(STUSetUp());
    cout << "\t...TESTED STU METHOD..." << endl;
}

bool TestFFD::STUSetUp(){
    //create lattice
    cgp::Vector diag(2.0f, 2.0f, 2.0f);
    cgp::Point corner(-1.0f, -1.0f, -1.0f);
    ffdobj->setDim(2,2,2);
    ffdobj->setFrame(corner,diag);

    //calculating stu for point 0,0,0
    cgp::Point point(0,0,0);
    std::vector<float> stu = ffdobj->calcSTU(point);
    //check values, these have been calculated by hand to get right answers
    if (stu[0] != 0.5 || stu[1] != 0.5 || stu[2] != 0.5){
        return false;
    }
    return true;
}



void TestFFD::testDeform(){
    cout << "\t...TESTING DEFORM METHOD..." << endl;
    CPPUNIT_ASSERT(setUpDeform());
    cout << "\t...TESTED DEFORM METHOD..." << endl;
}
bool TestFFD::setUpDeform(){
    //create lattice
    cgp::Vector diag(2.0f, 2.0f, 2.0f);
    cgp::Point corner(-1.0f, -1.0f, -1.0f);
    ffdobj->setDim(2,2,2);
    ffdobj->setFrame(corner,diag);
    //stu values checked in the previous unit test
    //checking if points do not move if we have not changed any vertices
    cgp::Point point(0,0,0);
    ffdobj->deform(point);
    if (point.x != 0 || point.y != 0 || point.z != 0){
        return false;
    }

    cgp::Point point1(1,1,1);
    ffdobj->deform(point1);
    if (point1.x != 1 || point1.y != 1 || point1.z != 1){
        return false;
    }

    cgp::Point point2(2,-1,3);
    ffdobj->deform(point2);
    if (point2.x != 2 || point2.y != -1 || point2.z != 3){
        return false;
    }


    //checking deformed points by changing control points

    cgp::Point pnt(0,0,0);
    //move control point, 1 left, 1 up, 1 backwards -> which should mean that the point 0,0,0 should move in those directions
    ffdobj->setCP(0,0,0,cgp::Point(-2,0,-2));
    ffdobj->deform(pnt);
    if (pnt.x >= 0 || pnt.y <= 0 || pnt.z >= 0){
        return false;
    }


    cgp::Point pnt2(0,1,1);
    //move control point, 2 left, 0 up, 3 backwards -> which should mean that the point 0,0,0 should move in those directions
    ffdobj->setCP(1,1,1,cgp::Point(-3,-1,2));
    ffdobj->deform(pnt2);
    if (pnt2.x >= 0 || pnt2.y == 1 || pnt2.z <= 1){
        return false;
    }


    cgp::Point pnt3(0,0,0);
    //move control point1, 1 left, 0 up, 0 backwards -> which should mean that the point 0,0,0 should move in those directions
    //move control point1, 0 left, 1 up, 0 backwards -> which should mean that the point 0,0,0 should move in those directions
    //move control point1, 0 left, 0 up, 1 backwards -> which should mean that the point 0,0,0 should move in those directions
    ffdobj->setCP(1,0,0,cgp::Point(-4,-1,-1));
    ffdobj->setCP(0,1,0,cgp::Point(-1,3,-1));
    ffdobj->setCP(0,0,1,cgp::Point(-1,-1,-4));
    ffdobj->deform(pnt3);
    //we check if the x got less, y got more, z got less
    if (pnt3.x >= 0 || pnt3.y <= 0 || pnt3.z >= 0){
        return false;
    }


    return true;



}

//#if 0 /* Disabled since it crashes the whole test suite */
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestFFD, TestSet::perBuild());
//#endif
