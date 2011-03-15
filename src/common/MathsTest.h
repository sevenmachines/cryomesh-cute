/**
 * MathsTest.h
 *
 *  Created on: 27 Jan 2011
 *      Author: SevenMachines<SevenMachines@yahoo.co.uk>
 */

#ifndef MATHSTEST_H_
#define MATHSTEST_H_

/**
 *  Description
 */
#include "ICuteSuite.h"

namespace cryomesh {

namespace common {

class MathsTest: public ICuteSuite {
public:
	MathsTest();
	virtual ~MathsTest();
	static void runSuite();
	static void testGetRandomValue();
};

}

}

#endif /* MATHSTEST_H_ */
