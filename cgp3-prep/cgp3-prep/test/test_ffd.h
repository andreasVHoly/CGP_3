#ifndef TILER_TEST_FFD_H
#define TILER_TEST_FFD_H


#include <string>
#include <cppunit/extensions/HelperMacros.h>
#include "tesselate/ffd.h"

/// Test code for @ref ffd
class TestFFD : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestFFD);
    CPPUNIT_TEST(resetTest);
    CPPUNIT_TEST(testNChooseK);
    CPPUNIT_TEST(testSTU);
    CPPUNIT_TEST(testDeform);
    CPPUNIT_TEST_SUITE_END();

private:
    ffd * ffdobj;

public:

    /// Initialization before unit tests
    void setUp();

    /// Tidying up after unit tests
    void tearDown();

    void resetTest();

    void testNChooseK();

    bool STUSetUp();
    void testSTU();
    void testDeform();
    bool setUpDeform();


};

#endif /* !TILER_TEST_FFD_H */
