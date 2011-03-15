/*
 * ClusterTest.h
 *
 *  Created on: 15 Feb 2011
 *      Author: niall
 */

#ifndef CLUSTERTEST_H_
#define CLUSTERTEST_H_

#include "ICuteSuite.h"

namespace cryomesh {

namespace structures {

class ClusterTest {
public:
	ClusterTest(){}
	virtual ~ClusterTest(){}
	static void runSuite();
	static void testCreateNodes();
	static void testCreateConnections();
};

}

}

#endif /* CLUSTERTEST_H_ */
