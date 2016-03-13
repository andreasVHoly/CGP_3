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

    //answers calculated on calculator
    float ans1 = 56;
    float ans2 = 45;
    float ans3 = 13;

    CPPUNIT_ASSERT(ffdobj->nChoosek(8,3) == ans1);
    CPPUNIT_ASSERT(ffdobj->nChoosek(10,2) == ans2);
    CPPUNIT_ASSERT(ffdobj->nChoosek(13,12) == ans3);
}



void TestFFD::resetTest(){

    cgp::Vector diag(2.0f, 2.0f, 2.0f);
    cgp::Point corner(-1.0f, -1.0f, -1.0f);
    ffdobj.setDim(2,2,2);

    //CHECK THAT all are 0

    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 2; j++){
            for (int k = 0; k < 2; k++){
                CPPUNIT_ASSERT(getCP(i,j,k).x == 0);
                CPPUNIT_ASSERT(getCP(i,j,k).y == 0);
                CPPUNIT_ASSERT(getCP(i,j,k).z == 0);
            }
        }
    }


    ffdobj.setFrame(corner,diag);


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
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            for (int k = 0; k < 3; k++){
                CPPUNIT_ASSERT(answers[index].x == ffdobj->getCP(i,j,k).x);
                CPPUNIT_ASSERT(answers[index].y == ffdobj->getCP(i,j,k).y);
                CPPUNIT_ASSERT(answers[index].z == ffdobj->getCP(i,j,k)).z;
            }
        }
    }


}

//#if 0 /* Disabled since it crashes the whole test suite */
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestFFD, TestSet::perBuild());
//#endif
