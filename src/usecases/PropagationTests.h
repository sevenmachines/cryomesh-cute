/*
 * Propagation.h
 *
 *  Created on: 21 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef PROPAGATIONTESTS_H_
#define PROPAGATIONTESTS_H_
#include "ICuteSuite.h"
namespace cryomesh {

namespace usecases {

class PropagationTests {
public:
	PropagationTests(){}
	virtual ~PropagationTests(){}
	static void runSuite() ;
	static void testPropagation();

};

}

}

#endif /* PROPAGATIONTESTS_H_ */
