/*
 * ClusterArchitectTest.cpp
 *
 *  Created on: 30 Sep 2011
 *      Author: niall
 */

#include "ClusterArchitectTest.h"

namespace cryomesh {
namespace manipulators {

void ClusterArchitectTest::runSuite(){
	cute::suite s;
	//s.push_back(CUTE(thisIsATest));
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "ClusterArchitectTest");
}



} /* namespace manipulators */
} /* namespace cryomesh */
