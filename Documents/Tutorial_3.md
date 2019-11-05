


# Tutorial 3
## Initializing <TYPE> DDS Manager and subscribing to Assessment data

---

In order to use the AMM Library, this header must be included.
```
#include <amm_std.h>
```


This tutorial will explain how to use user-defined types with DDS Manager.\
Take note of the class definition below.
```
class Foo {
public:
   void OnAssessmentEvent(AMM::Assessment& assessment, eprosima::fastrtps::SampleInfo_t* info) {
      std::cout << "Assessment received!" << std::endl;
   }
};
```
> **NOTE:**\
Class methods designed for subscriber callbacks must be public. User-defined methods for subscriber callbacks can be named whatever. The signature has to match as described in Tutorial 2.

Class Foo contains a method with the same signature as OnAssessmentEvent in Tutorial 2.
The only difference is ths function is associated with a user defined type.

When invoking a Create Subscriber method on DDS Manager for a type, the user is asked for a function reference to pass in as a callback. When creating DDS Manager with a `<void>` template, the user is able to pass in globally defined void functions. The user can also use a class method as the designated callback too, but given how a reference to a global function is very different from a method associated with a type, this requires DDS Manager to be structured slightly different to support the user-defined type -- hence why DDS Manager has been specialized into two template versions.

> **NOTE:**\
The two template versions only alter how subscriptions to data and events are handled and nothing else.
Initializing, Publishling, and Writing are unaffected.


Create a Foo template DDS Manager object.
```
AMM::DDSManager<Foo>* mgr = new AMM::DDSManager<Foo>("Config/Config.xml");
```

Initialize Assessment.
```
mgr->InitializeAssessment();
```


We now have an instance of DDS Manager defined to support Foo as a type.\
Before creating a subcriber for Assessment, we need an instance of Foo.
```
Foo foo;
```

Now we can create a subscriber for Assessment using this instance of Foo as the receiver object, and it's method as the callback.
```
mgr->CreateAssessmentSubscriber(&foo, &Foo::OnAssessmentEvent);
```

With this invocation of Create Assessment Subscriber, we're now telling DDS Manager to store both a reference to the class method desired for callbacks, and the object that the method must be called on.

> **NOTE:**\
Only non-static class members are supported at this time.


Now run the source code for yourself!\
https://github.com/AdvancedModularManikin/example-module/blob/master/Source/Tutorial_3.cpp

### END TUTORIAL 3

Move on to Tutorial 4.\
https://github.com/AdvancedModularManikin/example-module/blob/master/Documents/Tutorial_4.md
