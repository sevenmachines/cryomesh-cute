/*
 * Cycle.cpp
 *
 *  Created on: 21 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "CycleTests.h"
#include "structures/Bundle.h"
#include "components/Node.h"
#include "components/Connection.h"
#include "manager/CryoManager.h"

using namespace cryomesh::components;
using namespace cryomesh::structures;

namespace cryomesh {

namespace usecases {

void CycleTests::runSuite() {
	cute::suite s;
	s.push_back(CUTE(CycleTests::testFullCycle));
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "Cycle");
}

	void CycleTests::testFullCycle() {
	const int FIBRE_WIDTH = 10;
	Bundle bundle;
	boost::shared_ptr<Cluster> cluster1 = bundle.createCluster(10, 10);
	boost::shared_ptr<Cluster> cluster2 = bundle.createCluster(10, 10);
	boost::shared_ptr<Cluster> cluster3 = bundle.createCluster(10, 10);
	ASSERT_EQUAL(3, bundle.getClusters().getSize());

	boost::shared_ptr<Fibre> fibre1in = bundle.connectPrimaryInputCluster(cluster1->getUUID(), FIBRE_WIDTH);
	boost::shared_ptr<Fibre> fibre12 = bundle.connectCluster(cluster1->getUUID(), cluster2->getUUID(), FIBRE_WIDTH);
	boost::shared_ptr<Fibre> fibre23 = bundle.connectCluster(cluster2->getUUID(), cluster3->getUUID(), FIBRE_WIDTH);
	boost::shared_ptr<Fibre> fibre13 = bundle.connectCluster(cluster1->getUUID(), cluster3->getUUID(), FIBRE_WIDTH);
	boost::shared_ptr<Fibre> fibre3out = bundle.connectPrimaryOutputCluster(cluster3->getUUID(), FIBRE_WIDTH);
	ASSERT_EQUAL(3, bundle.getFibres().getSize());
	ASSERT_EQUAL(1, bundle.getInputFibres().getSize());
	ASSERT_EQUAL(1, bundle.getOutputFibres().getSize());

	ASSERT_EQUAL(Fibre::OutputCluster, fibre1in->isConnected(cluster1) );
	ASSERT_EQUAL(Fibre::InputCluster, fibre12->isConnected(cluster1) );
	ASSERT_EQUAL(Fibre::OutputCluster, fibre12->isConnected(cluster2) );
	ASSERT_EQUAL(Fibre::InputCluster, fibre23->isConnected(cluster2) );
	ASSERT_EQUAL(Fibre::OutputCluster, fibre23->isConnected(cluster3) );
	ASSERT_EQUAL(Fibre::InputCluster, fibre13->isConnected(cluster1) );
	ASSERT_EQUAL(Fibre::OutputCluster, fibre13->isConnected(cluster3) );
	ASSERT_EQUAL(Fibre::InputCluster, fibre3out->isConnected(cluster3) );

	// Structure summary
	// pin->cluster1-> cluster2->cluster3->pout
	//						 --------------->
	const unsigned int TOTAL_UPDATES = 100;
	for (int i = 0; i < TOTAL_UPDATES; i++) {
		//	std::cout<<"UseCasesTest::testFullCycle: "<<i<<std::endl;
		//	std::cout<<"###################################################"<<std::endl;
		//std::cout<<bundle<<std::endl;
		//	std::cout<<"###################################################"<<std::endl;
		fibre1in->trigger(0.8);
		bundle.update();
	}

	ASSERTM("TODO", false);
}
}

}
