/*
 * ClusterArchitectTest.cpp
 *
 *  Created on: 30 Sep 2011
 *      Author: niall
 */

#include "ClusterArchitectTest.h"
#include "manipulators/ClusterArchitect.h"

namespace cryomesh {
namespace manipulators {

void ClusterArchitectTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(ClusterArchitectTest::testCreateRandomNodes));
	s.push_back(CUTE(ClusterArchitectTest::testCreateRandomConnections));
	s.push_back(CUTE(ClusterArchitectTest::testDestroyRandomNodes));
	s.push_back(CUTE(ClusterArchitectTest::testDestroyRandomConnections));
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "ClusterArchitectTest");
}

void ClusterArchitectTest::testCreateRandomNodes() {
// test self connect strategy

// test evenly distributed strategy
	ASSERTM("TODO", false);
}

void ClusterArchitectTest::testCreateRandomConnections() {
	ASSERTM("TODO", false);
}

void ClusterArchitectTest::testDestroyRandomNodes() {
	ASSERTM("TODO", false);
}

void ClusterArchitectTest::testDestroyRandomConnections() {
	ASSERTM("TODO", false);
}

} /* namespace manipulators */
} /* namespace cryomesh */
