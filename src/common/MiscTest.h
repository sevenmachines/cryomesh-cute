#ifndef TEST_MISCTEST
#define TEST_MISCTEST


#include <boost/shared_ptr.hpp>

#include "common/Misc.h"
#include "ICuteSuite.h"

namespace cryomesh{
namespace common{
class MiscTest : public ICuteSuite{
public:
	static void runSuite();
	static void testparseArgs() ;
	static void testtoLower() ;
	static void testremoveChars();


};

}
}

#endif /* TEST_MISCTEST */
