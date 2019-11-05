

# Tutorial 6
## Cleaning up and shutting down DDS Manager

---

In order to use the AMM Library, this header must be included.
```
#include <amm_std.h>
```


This tutorial will explain the correct proceedures to cleanup after initialized types that aren't being used anymore.


Create DDS Manager
```
AMM::DDSManager<void>* mgr = new AMM::DDSManager<void>("Config/Config.xml");
```


For every **Initialize** method DDS Manager has for a type, there is a **Decommission** method to complement. Decommission should be used to clean up after a type is no longer being used. This allows all publishers and subscribers associated with a type to be removed so memory
can be properly freed by the FastRTPS framework.
\
\
There are also individual methods to remove the Publisher or Subscriber with a type if the user needs to retain one of them in an active state for whatever reason.


Create the Publiser.
```
mgr->CreateAssessmentPublisher();
```

Remove the Publisher.
```
mgr->RemoveAssessmentPublisher();
```

Create the Subscriber.
```
mgr->CreateAssessmentSubscriber([](AMM::Assessment& assessment, eprosima::fastrtps::SampleInfo_t* info){});
```

> **NOTE:**\
DDS Manager supports non-member lambda experssions in it's `<void>` template only.

Remove the Subscriber.
```
mgr->RemoveAssessmentSubscriber();
```

Decommision calls Remove for both Publisher and Subscriber, and unregisters Assessment.\
Even though the Publisher and Subscriber can be removed manually in the above manner, it is not recommended. Decommission needs to be used, always, when a type is done so DDS Manager is told to unregister the type from it's involvement on the DDS network.
```
mgr->DecommissionAssessment();
```

When a module is ready to be shutdown, the method Shutdown needs to be called on DDS Manager. This needs to be done, always, before a program exits so that DDS Manager has a chance to remove it's DDS Participant from the DDS network. This ensures that the FastRTPS framework can free memory for unused Participants.
\
\
Shutdown also ensures that Decommission is called for every currently initialized type.
```
mgr->Shutdown();
```


Shutdown doesn't return any error code. It's a void return. However it still outputs an error message if anything happened during it's shutdown routine. If an error occurs, Shutdown will still continue to properly close down everything else.
```
std::string errmsg;

mgr->Shutdown(errmsg);

if (errmsg.length() > 0) {
   std::cout << errmsg << std::endl;
}
```

> **NOTE:**\
There should be no output from this tutorial unless there is an error.
Running Shutdown sequentially generates no error or message.
DDS Manager will attempt a cleanup, if it needs it, every time this is called.


> **ATTENTION:**\
Shutdown does not free memory for the instance of DDS Manager itself. Freeing memory for a heap allocated DDS Manager is the sole responsibility of the user.
\
\
If the deconstructor for DDS Manager is used before Shutdown, there is a failsafe "Force Clean" that will attempt a rapid cleanup of ALL initialized types and closes the Participant.
\
\
If Shutdown is used on an instance of DDS Manager and the program doesn't exit, the constructor on DDS Manager must be called again for DDS Manager to work properly after Shutdown. This is because Shutdown closes the Participant, and the constructor creates a Participant.

```
delete mgr;
```

Now run the source code for yourself!\
https://github.com/AdvancedModularManikin/example-module/blob/master/Source/Tutorial_6.cpp

### END TUTORIAL 6

Move on to Tutorial 7.\
https://github.com/AdvancedModularManikin/example-module/blob/master/Documents/Tutorial_7.md
