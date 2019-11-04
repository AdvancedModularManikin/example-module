
/// For simulating delay.
#include <chrono>

/// For error handling.
#include <string>

/// In order to use the AMM Library, thsi header must be included.
#include <amm_std.h>

namespace T5 {

/// See main body tutorial first.
void OnAssessmentEvent (AMM::Assessment& assessment, eprosima::fastrtps::SampleInfo_t* info) {
   std::cout << "Assessment received!" << std::endl;
}


/// Tutorial 5 -- DDS Manager error handling.


void Tutorial_5 () {
   /// This tutorial will explain how to use error handling on all of DDS Manager's methods.

   /// Create an instance of DDS Manager.
   AMM::DDSManager<void>* mgr = new AMM::DDSManager<void>("Config/Config.xml");

   /// Now initialize Assessment catching for any errors it might throw.
   int err = mgr->InitializeAssessment();

   /// Every method in DDS Manager will return an error code.
   /// It's up to the end user if they want to acknowledge what these methods return.

   /// NOTE:
   /// There are no special error codes. It's either 0 for success or 1 for failure.

   /// Handle error.
   if (err != 0) {
      std::cout << "Initialize Assessment returned a non-zero code." << std::endl;
   }



   /// Every method in DDS Manager also has an overload for receiving error messages.
   /// The first argument in each method is a valid overload for error messages.

   /// Declare container for message.
   std::string errmsg = "";

   /// Provide container as first argument.
   err = mgr->InitializeAssessment(errmsg);

   /// Handle error.
   if (err != 0) {
      /// Output the error message using your favorite logging API.
      std::cout << errmsg << std::endl;
   }

   /// This implementation takes a reference to an string type and writes a message to it.

   /// Not every message written is descripting an error.
   /// An error message will be written if the method returns 1, meaning something went wrong and
   /// the routine couldn't be successfully completed.
   ///
   /// In some instances when methods return 0, they'll also write an attention message based on how
   /// the routine returned 0. This means that the whole routine doesn't need to execute again, and
   /// therefore exits early with 0 and a message describing why.
   ///
   /// For example: calling InitializeAssessment twice in a row.
   /// The first invocation will return 0 and write no message, however a second invocation immediately
   /// after the first will return 0 with a message saying that it's already initialzied and doesn't
   /// need to be initialized again -- hence the return of 0 with a message.


   /// Output errmsg if it contains anything.
   if (errmsg.length() > 0) {

      /// NOTE:
      /// This will say that Assessment is already initialized since we tried to initialize
      /// it twice in this tutorial.
      std::cout << errmsg << std::endl;
   }



   /// Going through the rest of establishing Assessment.

   errmsg = "";

   /// Inline invocation in a single statement to catch error.
   if (mgr->CreateAssessmentPublisher(errmsg) != 0) {

      /// Output error message.
      std::cout << errmsg << std::endl;
   }

   /// NOTE:
   /// DDS Manager only supports non-member lambda experssions in it's <void> template.

   /// Same for creating the subscriber
   if (mgr->CreateAssessmentSubscriber(errmsg, &OnAssessmentEvent) != 0) {

      /// Output error message.
      std::cout << errmsg << std::endl;
   }

   /// NOTE:
   /// If DDS Manager were declared with a type and Create Subscriber needed a reference, to
   /// a receiver object, this would be how the errmsg overload is expressed:
   ///
   ///   mgr->CreateAssessmentSubscriber(errmsg, &foo, &Foo:OnEvent);
   ///
   /// As mentioned, the errmsg overload always takes the first argument, and everything else
   /// second and/or third as the otherwise normally would.

   /// Generate Assessment data.
   AMM::Assessment a;

   /// Send Assessment data to the Assessment writer.
   if (mgr->WriteAssessment(errmsg, a) != 0) {

      /// Output error message.
      std::cout << errmsg << std::endl;
   }

   /// Thread delay to show subscriber receiving published Assessment data.
   std::this_thread::sleep_for(std::chrono::milliseconds(500));


   /// END TUTORIAL 5

   mgr->Shutdown();
   delete mgr;
}

} // namespace T5
