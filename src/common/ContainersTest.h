/**
 * ContainersTest.h
 *
 *  Created on: 31 Jan 2011
 *      Author: SevenMachines<SevenMachines@yahoo.co.uk>
 */

#ifndef CONTAINERSTEST_H_
#define CONTAINERSTEST_H_

#include "ICuteSuite.h"
/**
 *  Tests for Container objects
 */
namespace cryomesh {

namespace common {

class ContainersTest {
public:

	virtual ~ContainersTest(){}
	static void runSuite();
	static void testCompare() ;

	static void testAdd();
protected:
	ContainersTest(){}
};

}

}

#endif /* CONTAINERSTEST_H_ */
