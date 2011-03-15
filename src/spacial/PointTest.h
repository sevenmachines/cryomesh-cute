#ifndef TEST_POINTTEST
#define TEST_POINTTEST

#include "spacial/Point.h"
#include "spacial/SphericalPoint.h"
#include "ICuteSuite.h"

namespace cryomesh{
namespace spacial{

class PointTest :ICuteSuite{
public:
	PointTest(){}
	virtual ~PointTest(){}

	static void runSuite();
	static void testPoint_SphericalPoint();
	static void  testPoint_CC();
	static void  testgetDistance();
	static void  testoperators() ;
	static void  testgetNormalisedVector() ;
};

} //NAMESPACE
}//NAMESPACE
#endif /* POINTTEST */
