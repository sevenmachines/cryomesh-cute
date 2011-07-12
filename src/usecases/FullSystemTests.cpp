/*
 * FullSystem.cpp
 *
 *  Created on: 21 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "FullSystemTests.h"
#include "structures/Bundle.h"
#include "components/Node.h"
#include "components/Connection.h"
#include "manager/CryoManager.h"

using namespace cryomesh::components;
using namespace cryomesh::structures;

namespace cryomesh {

namespace usecases {

void FullSystemTests::runSuite() {
	cute::suite s;
	s.push_back(CUTE(FullSystemTests::testBasicFullSystem));
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "FullSystem");
}

void FullSystemTests::testBasicFullSystem() {
	manager::CryoManager manager;
	manager.create("Data/basic-2c.config");
	boost::shared_ptr<structures::Bundle> bundle = manager.getMutableBundle();
	//bundle->setDebug(true);
	//bundle->update();
	// Test structure
	{
		boost::shared_ptr<utilities::Statistician> statistician = manager.getBundle()->getMutableStatistician();
		if (statistician == 0) {
			ASSERTM("Null statistician", false);
		}
		statistician->update();
		/**
		 int cluster_count =statistician->getClusterCount();
		 int fibpatmap_count = manager.getBundle()->getFibrePatternChannelMap().size();
		 int input_fibres_count = statistician->getInputFibresCount();
		 int output_fibres_count = statistician->getOutputFibresCount();
		 int normal_fibres_count = statistician->getNormalFibresCount();
		 int input_channels_map_count = statistician->getInputChannelsCount();
		 int output_channels_map_count = statistician->getOutputChannelsCount();
		 */
		int cluster_count = manager.getBundle()->getClusters().getSize();
		int fibpatmap_count = manager.getBundle()->getRealFibrePatternChannelMap().size();
		int input_fibres_count = statistician->getInputFibresCount();
		int output_fibres_count = statistician->getOutputFibresCount();
		int normal_fibres_count = statistician->getNormalFibresCount();
		int input_channels_map_count = statistician->getInputChannelsCount();
		int output_channels_map_count = statistician->getOutputChannelsCount();
		ASSERT_EQUAL(2, cluster_count);
		ASSERT_EQUAL(6, fibpatmap_count);
		ASSERT_EQUAL(3, input_fibres_count);
		ASSERT_EQUAL(3, output_fibres_count);
		ASSERT_EQUAL(1, normal_fibres_count);
		ASSERT_EQUAL(3, input_channels_map_count);
		ASSERT_EQUAL(3, output_channels_map_count);
	}

	for (int i = 0; i < 100; i++) {
		bundle->update();
#ifdef FULLSYSTEMTESTS_DEBUG
		std::cout << *(bundle) << std::endl;
#endif
	}
	ASSERTM("TODO", false);
}
}

}
