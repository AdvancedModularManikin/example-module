
#include <thread>

/// For logging purposes.
#include <iostream>

/// In order to use the AMM Library, thsi header must be included.
#include <amm_std.h>


/// Tutorial 4 -- Initializing DDS Manager inside an object and subscribing to Assessment data.


/// This tutorial will explain how to define DDS Manager inside another object.


/// Initializing DDS Manager inside a class is no different from putting in global space
/// declaring it on the stack. The only difference comes if the user wants to use a global
/// function or a class method for subscriber callbacks.

namespace T4 {

/// Define a class that contains an instance of DDS Manager.
class Foo {
private:

   /// For this tutorial, we're going to use one of Foo's methods for the callback on Assessment data.
   /// So DDSManager must be declared to use Foo in it's template.
   AMM::DDSManager<Foo>* m_mgr;

public:

   Foo () {
      /// Initialize DDS Manager inside Foo's contructor.
      m_mgr = new AMM::DDSManager<Foo>("Config/Config.xml");

      /// Initialize Assessment.
      m_mgr->InitializeAssessment();

      /// Create a subscriber for Assessment.
      m_mgr->CreateAssessmentSubscriber(this, &Foo::OnAssessmentEvent);

      /// Since DDS manager is being created in this class, and wants to use a method from this class
      /// for subscriber callbacks, we us 'this' to pass as the first argument, and then the desired method
      /// as the second argument.
   }

   ~Foo () {
      m_mgr->Shutdown();
      delete m_mgr;
   }

   /// Assessment subscription callback.
   void OnAssessmentEvent(AMM::Assessment& assessment, eprosima::fastrtps::SampleInfo_t* info) {
      std::cout << "Assessment received!" << std::endl;
   }

};


void Tutorial_4 () {

   /// Create an instance of Foo.
   Foo* foo = new Foo();


   /// Now run the Command Line Module and enter Assessment Topic to publish assessment data to see
   /// it trigger the event handler defined here while running.

   /// END TUTORIAL 4

   /// Keep thread alive to listen for subscribed events for tutorial purposes.
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

   delete foo;
}

} // namespace T4
