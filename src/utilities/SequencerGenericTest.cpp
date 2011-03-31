#include "SequencerGenericTest.h"


#include "cute.h"
#include "ide_listener.h"
#include "cute_runner.h"

namespace cryomesh {
namespace utilities {

void SequencerGenericTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(SequencerGenericTest::testSequencerGeneric));

	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "SequencerGenericTest");
}

void SequencerGenericTest::testSequencerGeneric(void){
	const std::string TEST_FILE_IN = "TestData/sequences_3x2x1.xml";
	SequencerGeneric sg(TEST_FILE_IN);
	//std::cout<<"testSequencerGeneric: "<<" "<<std::endl;
	//std::cout<<sg<<std::endl;
	ASSERT(false);
}
}
}
