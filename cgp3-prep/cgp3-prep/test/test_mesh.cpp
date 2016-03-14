#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <test/testutil.h>
#include "test_mesh.h"
#include <stdio.h>
#include <cstdint>
#include <sstream>
#include "tesselate/voxels.h"
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/extensions/HelperMacros.h>

void TestMesh::setUp()
{
    mesh = new Mesh();
}

void TestMesh::tearDown()
{
    delete mesh;
}

void TestMesh::testSmoothing(){
    cout << "\t...TESTING SMOOTHING METHOD..." << endl;
    CPPUNIT_ASSERT(mesh->setUpSmoothTest());
    //CPPUNIT_ASSERT(mesh->setUpSmoothTest2());
    cout << "\t...TESTED SMOOTHING METHOD..." << endl;
}

void TestMesh::testAdjList(){
    cout << "\t...TESTING ADJ METHOD..." << endl;
    CPPUNIT_ASSERT(mesh->checkAdjList());
    cout << "\t...TESTED ADJ METHOD..." << endl;
}

void TestMesh::testMarchCube(){
    cout << "\t...TESTING MARCH CUBE METHOD..." << endl;

    /*VoxelVolume vox;
    int dx, dy, dz;
    cout << "test0" << endl;
    dx = dy = dz = 4;
    vox.setDim(dx, dy, dz);
    vox.setFrame(cgp::Point(0.0f, 0.0f, 0.0f), cgp::Vector(0.5f, 0.5f, 0.5f)); // unit cube
    vox.fill(false);
    vox.set(1,0,0,true);
    vox.set(0,1,0,true);
    vox.set(0,0,1,true);
    vox.set(0.5,1,0.5,true);
    vox.set(0.5,0.5,1,true);
    vox.set(0.5,1,1,true);
    vox.set(1,1,0.5,true);
    vox.set(1,0.5,1,true);
    cout << "test1" << endl;
    mesh->marchingCubes(vox);*/
    //CPPUNIT_ASSERT();
    cout << "\t...TESTED MARCH CUBE METHOD..." << endl;
}


void TestMesh::testMeshing()
{

    mesh->readSTL("../meshes/bunny.stl");
    CPPUNIT_ASSERT(mesh->basicValidity());
    CPPUNIT_ASSERT(!mesh->manifoldValidity()); // bunny has known holes in the bottom
    cerr << "BUNNY TEST PASSED" << endl << endl;

    // test simple valid 2-manifold
    mesh->validTetTest();
    CPPUNIT_ASSERT(mesh->basicValidity());
    CPPUNIT_ASSERT(mesh->manifoldValidity());
    cerr << "SIMPLE VALIDITY TEST PASSED" << endl << endl;

    // test for duplicate vertices, dangling vertices and out of bounds on vertex indices
    mesh->basicBreakTest();
    CPPUNIT_ASSERT(!mesh->basicValidity());
    cerr << "BASIC INVALID MESH DETECTED CORRECTLY" << endl << endl;

    // test for 2-manifold with boundary
    mesh->openTetTest();
    CPPUNIT_ASSERT(mesh->basicValidity());
    CPPUNIT_ASSERT(!mesh->manifoldValidity());
    cerr << "INVALID MESH WITH BOUNDARY DETECTED CORRECTLY" << endl << endl;

    // test for non 2-manifold failure where surfaces touch at a single vertex
    mesh->touchTetsTest();
    CPPUNIT_ASSERT(mesh->basicValidity());
    CPPUNIT_ASSERT(!mesh->manifoldValidity());
    cerr << "INVALID PINCHED SURFACE TEST PASSED" << endl << endl;

    // test for non 2-manifold overlapping triangles
    mesh->overlapTetTest();
    CPPUNIT_ASSERT(mesh->basicValidity());
    CPPUNIT_ASSERT(!mesh->manifoldValidity());
    cerr << "INVALID NON-2-MANIFOLD DETECTED CORRECTLY" << endl << endl;

}

//#if 0 /* Disabled since it crashes the whole test suite */
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestMesh, TestSet::perBuild());
//#endif
