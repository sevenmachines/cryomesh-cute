/*
 * ClusterArchitectTest.h
 *
 *  Created on: 30 Sep 2011
 *      Author: niall
 */

#ifndef CLUSTERARCHITECTTEST_H_
#define CLUSTERARCHITECTTEST_H_
#include "ICuteSuite.h"
#include <boost/shared_ptr.hpp>
#include "structures/Cluster.h"
#include "manipulators/ClusterArchitect.h"

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
	static void testAddHistoryEntry();
	static void testRunAnalysis();
	static boost::shared_ptr<ClusterArchitect> createTestClusterArchitect(structures::Cluster & clus);

};

} /* namespace manipulators */
} /* namespace cryomesh */
#endif /* CLUSTERARCHITECTTEST_H_ */
