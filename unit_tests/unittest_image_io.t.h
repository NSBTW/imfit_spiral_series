// Unit tests for code in image_io.cpp

// $CXXTESTGEN --error-printer -o test_runner_imageio.cpp unit_tests/unittest_image_io.t.h 
// $CPP -o test_runner_imageio test_runner_imageio.cpp image_io.cpp -I. -I/usr/local/include -I$CXXTEST -lcfitsio

#include <cxxtest/TestSuite.h>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

#include "utilities_pub.h"
#include "image_io.h"

// small, along with values of lower-left and upper-right pixels
const string  TEST_IMAGE_32x32("tests/testimage_expdisk32.fits");
const double  VALUE_LL = 20.903625;
const double  VALUE_UR = 14.947027;
// nonexistent file (for error-checking)
const string  BAD_IMAGE_NAME("no_image_with_this_name.fits");
// filename for an image file we can't possibly write (permission denied)
const string  IMPOSSIBLE_IMAGE_FILENAME("/arglebargle.fits");

// simple image data
double  tinyImage[9] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
const string  TINY_IMAGE_FILENAME("tinyimage_temp.fits");



class NewTestSuite : public CxxTest::TestSuite 
{
public:

  // Test for GetImageSize() with good file
  void testGetImageSize_good( void )
  {
    int  status;
    int  nCols, nRows;

    status = GetImageSize(TEST_IMAGE_32x32, &nCols, &nRows);    
    TS_ASSERT_EQUALS(nCols, 32);
    TS_ASSERT_EQUALS(nRows, 32);
  }

  // Test for GetImageSize() with nonexistent file
  void testGetImageSize_failure( void )
  {
    int  status;
    int  nCols, nRows;

    status = GetImageSize(BAD_IMAGE_NAME, &nCols, &nRows);
    TS_ASSERT_EQUALS(-1, status);
  }


  // Test to see that ReadImageAsVector() succeeds
  void testReadImageAsVector_good( void )
  {
    int  nCols, nRows;
    double  *pixelData;

	pixelData = ReadImageAsVector(TEST_IMAGE_32x32, &nCols, &nRows);
	// check image size
    TS_ASSERT_EQUALS(nCols, 32);
    TS_ASSERT_EQUALS(nRows, 32);
    // check lower-left and upper-right pixel values
    TS_ASSERT_DELTA(pixelData[0], VALUE_LL, 1.0e-6);
    TS_ASSERT_DELTA(pixelData[32*32 - 1], VALUE_UR, 1.0e-6);
    
    free(pixelData);
  }

  // Test to see that ReadImageAsVector() correctly reports failure
  void testReadImageAsVector_failure( void )
  {
    int  nCols, nRows;
    double  *pixelData;

	pixelData = ReadImageAsVector(BAD_IMAGE_NAME, &nCols, &nRows);
	// check that error value (NULL) was returned
    TS_ASSERT(pixelData == NULL);
  }


  // Test round-trip of image data with SaveVectorAsImage() and ReadImageAsVector()
  void testReadAndWrite_noComments( void )
  {
    int nColsOrig, nRowsOrig, nCols, nRows, status;
    vector<string>  comments;
    double  *pixelData;
    
    nColsOrig = 3;
    nRowsOrig = 3;
    
    status = SaveVectorAsImage(tinyImage, TINY_IMAGE_FILENAME, nColsOrig, nRowsOrig, comments);
    TS_ASSERT_EQUALS(status, 0);

	pixelData = ReadImageAsVector(TINY_IMAGE_FILENAME, &nCols, &nRows);
	// check image size
    TS_ASSERT_EQUALS(nCols, 3);
    TS_ASSERT_EQUALS(nRows, 3);
    // check lower-left and upper-right pixel values
    TS_ASSERT_DELTA(pixelData[0], tinyImage[0], 1.0e-6);
    TS_ASSERT_DELTA(pixelData[8], tinyImage[8], 1.0e-6);
    
    free(pixelData);
  }

  // Same as previous, but now with comments for FITS header as well
  void testReadAndWrite_withComments( void )
  {
    int nColsOrig, nRowsOrig, nCols, nRows, status;
    vector<string>  comments;
    string  aComment;
    double  *pixelData;
    
    nColsOrig = 3;
    nRowsOrig = 3;
    aComment = "Here is a comment";
    comments.push_back(aComment);
    aComment = "this is a second comment";
    comments.push_back(aComment);
    
    status = SaveVectorAsImage(tinyImage, TINY_IMAGE_FILENAME, nColsOrig, nRowsOrig, comments);
    TS_ASSERT_EQUALS(status, 0);

	pixelData = ReadImageAsVector(TINY_IMAGE_FILENAME, &nCols, &nRows);
	// check image size
    TS_ASSERT_EQUALS(nCols, 3);
    TS_ASSERT_EQUALS(nRows, 3);
    // check lower-left and upper-right pixel values
    TS_ASSERT_DELTA(pixelData[0], tinyImage[0], 1.0e-6);
    TS_ASSERT_DELTA(pixelData[8], tinyImage[8], 1.0e-6);
    
    free(pixelData);
  }

  // Test to see that SaveVectorAsImage() correctly reports failure
  void testSaveImageAsVector_failure( void )
  {
    int  nCols, nRows, status;
    vector<string>  comments;
    
    nCols = 3;
    nRows = 3;

    status = SaveVectorAsImage(tinyImage, IMPOSSIBLE_IMAGE_FILENAME, nCols, nRows, comments);
	// check that error value was returned
    TS_ASSERT_EQUALS(status, -1);
  }

};