#include "SequencerChannelsTest.h"
#include "cute.h"
#include "ide_listener.h"
#include "cute_runner.h"

namespace cryomesh {
namespace utilities {

void SequencerChannelsTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(SequencerChannelsTest::testSequencerChannels));

	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "SequencerChannelsTest");
}
void SequencerChannelsTest::testSequencerChannels(void) {
	// 1st test
	{
		const std::string TEST_FILE_IN = "TestData/sequences_3x2x1.xml";
		SequencerChannels sg;
		state::PatternChannelMap  in_chans;
		state::PatternChannelMap out_chans;
		sg.readSequences(TEST_FILE_IN, in_chans, out_chans);
		//std::cout<<"SequencerChannelsTest: "<<" "<<std::endl;
		//std::cout<<sg<<std::endl;
		// simple tests
		{
			int in_sz = in_chans.getSize();
			int out_sz = out_chans.getSize();

			ASSERT_EQUAL(1, in_sz);
			ASSERT_EQUAL(1, out_sz);
		}

		// check input channel and size
		{
			//for all in in_chans
			{
				std::map<boost::uuids::uuid, boost::shared_ptr<PatternChannel> >::const_iterator it_in_chans =
						in_chans.begin();
				const std::map<boost::uuids::uuid, boost::shared_ptr<PatternChannel> >::const_iterator it_in_chans_end =
						in_chans.end();
				while (it_in_chans != it_in_chans_end) {
					int width = it_in_chans->second->getWidth();
					int length = it_in_chans->second->getLength();
					ASSERT_EQUAL(3, width);
					ASSERT_EQUAL(2, length);
					++it_in_chans;
				}
			}
		}
		//check output channel and size
		{
			//for all in out_chans
			{
				std::map<boost::uuids::uuid, boost::shared_ptr<PatternChannel> >::const_iterator it_out_chans =
						out_chans.begin();
				const std::map<boost::uuids::uuid, boost::shared_ptr<PatternChannel> >::const_iterator
						it_out_chans_end = out_chans.end();
				while (it_out_chans != it_out_chans_end) {
					int width = it_out_chans->second->getWidth();
					int length = it_out_chans->second->getLength();
					ASSERT_EQUAL(3, width);
					ASSERT_EQUAL(2, length);
					++it_out_chans;
				}
			}
		}
	}

	// 1st test
	{
		const std::string TEST_FILE_IN = "TestData/sequences_3x2x3-2x3x1.xml";
		SequencerChannels sg;
		state::PatternChannelMap  in_chans;
				state::PatternChannelMap out_chans;
				sg.readSequences(TEST_FILE_IN, in_chans, out_chans);
		//std::cout<<"SequencerChannelsTest: "<<" "<<std::endl;
		//std::cout<<sg<<std::endl;
		// simple tests
		{
			int in_sz = in_chans.getSize();
			int out_sz = out_chans.getSize();

			ASSERT_EQUAL(3, in_sz);
			ASSERT_EQUAL(1, out_sz);
		}
	}
}

}
}
