
#include <thread>

/// For logging purposes.
#include <iostream>

/// In order to use the AMM Library, thsi header must be included.
#include <amm_std.h>

namespace T2 {

/// See main body tutorial first.
void OnAssessmentEvent (AMM::Assessment& assessment, eprosima::fastrtps::SampleInfo_t* info) {
   std::cout << "Assessment received!" << std::endl;
}




/// Tutorial 2 -- Initializaing <void> DDS Manager and subscribing to Assessment data.
void Tutorial_2 () {

   /// Create a void template DDS Manager object.
   AMM::DDSManager<void>* mgr = new AMM::DDSManager<void>("Config/Config.xml");

   /// The reason why <void> is being used here is due to template specialization for DDS Manager.
   /// DDS Manager is designed to accept either a <void> template (meaning no type is required),
   /// or a template of a single user defined type.
   ///
   /// More on the user defined type in Tutorial 3.

   /// Initialize Assessment to start using it.
   mgr->InitializeAssessment();


   /// See Tutorial 1 for all the usable AMM types, or the AMM Specification Docs for a complete list.
   /// https://github.com/AdvancedModularManikin/specification


   /// Once Assessment is initialize, create the subscriber for it.
   ///
   /// Here, pass in a reference to the function the user wants to use for callbacks to receive
   /// Assessment data when it is published on the AMM DDS network.
   mgr->CreateAssessmentSubscriber(&OnAssessmentEvent);

   /// This is where template specialization matters. Since <void> was used for DDS Manager, it's
   /// expecting references to void functions in global scope.
   /// These user-defined functions used for subscription event callbacks must retain a specific signature
   /// to be accepted as a valid funtion reference to be passed in.
   ///
   /// NOTE:
   /// Required function signature for Assessment.
   /// (AMM::Assessment assessment, eprosima::fastrtps::SampleInfo_t* info)
   ///
   /// The first argument can be any of the 17 AMM data types depending on the type that has been initialized.
   /// Passing a function ref where is first argument si defined as AMM::EventFragment, when the method is
   /// aimed for Assessment will result is compilation error.
   ///
   /// The second argument is always SampleInfo_t*.
   ///
   /// ATTENTION:
   /// Please see AMM User Manaual for a list of correct parameter names for argument 1 of this method.
   /// Parameter names set in the user-defined function MUST MATCH how the receiving arugments are defined
   /// in the AMM Library. Failure to match correct parameter names will result in either compilation error
   /// or silent runtime error where the callback receives no data.


   /// Now run the Command Line Module and enter Assessment Topic to publish assessment data to see
   /// it trigger the event handler defined here while running.

   /// END TUTORIAL 2

   /// Keep thread alive to listen for subscribed events for this tutorial.

   bool isRunning = true;
   auto loop = [&]() {
      for (;;) {
         if (!isRunning) break;
      }
   };
   std::thread t(loop);
   std::cout << "Listening for Assessment Data... Press return to exit." << std::endl;
   std::cin.get();
   isRunning = false;
   t.join();

   mgr->Shutdown();
   delete mgr;
}

} // namespace T2
