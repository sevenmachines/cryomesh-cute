#ifndef TEST_SPHERICALPOINTTEST
#define TEST_SPHERICALPOINTTEST

#include "ICuteSuite.h"
namespace cryomesh{
namespace spacial{

class SphericalPointTest : public ICuteSuite {
public:
	SphericalPointTest(){}
	virtual ~SphericalPointTest(){}
	static void runSuite();
	static void testSphericalPoint_Point();
	static void testPoint_CC();
	static void testgetArcLength();
	static void testoperators() ;
	static void testgetOpposite();
};

}//NAMESPACE
}//NAMESPACE

#endif // TEST_SPHERICALPOINTTEST
