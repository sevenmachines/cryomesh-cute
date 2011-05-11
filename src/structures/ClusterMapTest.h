/*
 * ClusterMapTest.h
 *
 *  Created on: 11 May 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef CLUSTERMAPTEST_H_
#define CLUSTERMAPTEST_H_

#include "ICuteSuite.h"

namespace cryomesh {

namespace structures {

class ClusterMapTest {
public:
	ClusterMapTest(){}
	virtual ~ClusterMapTest(){}

	static void runSuite();
	static void testUpdateEnergyFractions();
};

}

}

#endif /* CLUSTERMAPTEST_H_ */
