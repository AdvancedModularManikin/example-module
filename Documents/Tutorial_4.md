


# Tutorial 4
## Initializing DDS Manager inside an object and subscribing to Assessment data.

---

This tutorial will explain how to define DDS Manager inside another object.

In order to use the AMM Library, this header must be included.
```
#include <amm_std.h>
```


Initializing DDS Manager inside a class is no different from putting in global space declaring it on the stack. The only difference comes if the user wants to use a global function or a class method for subscriber callbacks.

Define a class that contains an instance of DDS Manager.
```
class Foo {
private:
   AMM::DDSManager<Foo>* m_mgr;

public:
   Foo () {
      // Initialize DDS Manager inside Foo's constructor.
      m_mgr = new AMM::DDSManager<Foo>("Config/Config.xml");

      // Initialize Assessment.
      m_mgr->InitializeAssessment();

      // Create a subscriber for Assessment.
      m_mgr->CreateAssessmentSubscriber(this, &Foo::OnAssessmentEvent);
   }

   ~Foo () {
      m_mgr->Shutdown();
      delete m_mgr;
   }

   // Assessment subscription callback.
   void OnAssessmentEvent(AMM::Assessment& assessment, eprosima::fastrtps::SampleInfo_t* info) {
      std::cout << "Assessment received!" << std::endl;
   }

};
```
For this tutorial, we're going to use one of Foo's methods for the callback on Assessment data. So DDS Manager must be declared to use Foo in it's template. Since DDS Manager is being created in this class, and wants to use a method from this class for subscriber callbacks, `this` is used to pass as the first argument, and then the desired method as the second argument.


Create an instance of Foo.
```
Foo* foo = new Foo();
```


Now run the source code for yourself!\
https://github.com/AdvancedModularManikin/example-module/blob/master/Source/Tutorial_4.cpp

### END TUTORIAL 4

Move on to Tutorial 5.\
https://github.com/AdvancedModularManikin/example-module/blob/master/Documents/Tutorial_5.md
