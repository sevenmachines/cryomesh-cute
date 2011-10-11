/*
 * ClusterArchitectTest.h
 *
 *  Created on: 30 Sep 2011
 *      Author: niall
 */

#ifndef CLUSTERARCHITECTTEST_H_
#define CLUSTERARCHITECTTEST_H_
#include "ICuteSuite.h"

namespace cryomesh {
namespace manipulators {

class ClusterArchitectTest {
public:
	ClusterArchitectTest();
	virtual ~ClusterArchitectTest();
	static void runSuite();
	static void testCreateRandomNodes();
	static void testCreateRandomConnections();
	static void testDestroyRandomNodes();
	static void testDestroyRandomConnections();
	static void testGetRandomNodes();
	static void testGetRandomConnections();
};

} /* namespace manipulators */
} /* namespace cryomesh */
#endif /* CLUSTERARCHITECTTEST_H_ */
