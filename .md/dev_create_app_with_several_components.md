[Go to previous menu](./development.md#development)

# ***Creating application with several components***

   - [Implementation](#implementation)
   - [Building](#building)
   - [Verification](#verification)
   - [Source code](#source-code)



## ***Implementation***

### ***Example folder***

Let's use previous example code for base implementation, but moved to another folder ***ex_03***.

### ***Building rules***

Afterward we should change project name in {CARPC_EXAMPLES_SOURCE_DIR}/examples/ex_03/CMakeLists.txt to ***EXAMPLE_03***

As in the previous example let's extend this application by a new ***Application Component*** and ***Application Thread***. This could be done in two steps:
   - [aidl implementation](#adding-component-and-thread-description-in-aidl-file)
   - [source implementation](#source-code-implementation-for-application-component)

### ***Adding Component and Thread description in .aidl file***

#### ***Creating Component description section:***

Similar to the previous example first of all we must add new ***Component*** description section.
Let's call new component as **Driver**. It will be placed in our filesystem in similar to **OnOff** component location only corresponding folder will be renamed from **OnOff** to **Driver**. Also C++ namespace will be changed from **onoff** to **driver**. Final C++ class name we will use the same. This will require only some simple copy/paste changes with minimal adaptations.

So, component description section for new Driver component will be look like:

<pre>
component: Component_Driver
{
   include: "imp/app/components/Driver/Component.hpp";
   creator: application::components::driver::Component::creator;
};
</pre>

#### ***Creating/Extending Application Thread description section:***

On the next step of application configuration we could follow two ways.

First way is extending existing Application Thread (**Thread_OnOff**) with a newly created Driver component. For this we just need to add this component name **Component_Driver** to **Thread_OnOff** section to **components** parameter additionally to **Component_OnOff** with separated comma.

<pre>
thread: Thread_OnOff
{
   components: Component_OnOff, Component_Driver;
   watchdog: 5;
};
</pre>

In this case both components will be created by ***CARPC runtime*** in context of one **Thread_OnOff** ***Application Thread***. This means all code of these components will be executed synchronously in this thread. Also all communication events for these components will go to the current thread's event loop and will be processed one by one synchronously.

The second way is creating new Application Thread description section and mention new Driver component in it.

<pre>
thread: Thread_Driver
{
   components: Component_Driver;
   watchdog: 5;
};
</pre>

In this case ***CARPC runtime*** will create second Application Thread and Driver component will be created in context of last one. This means all code of these two components (OnOff and Driver) will be executed asynchronously in these two different threads. Also all communication events for these components will go to two different thread's event loops and will be processed asynchronously.

To understand how runtime works we wiil follow second way but for this Application Process description section also must be extended by adding newly created Application Thread.

#### ***Extending Application Process description section:***

<pre>
application: example_02
{
   threads: Thread_OnOff, Thread_Driver;
};
</pre>

Only in this case ***CARPC runtime*** will create both thread described in corresponding description sections.

### ***Source code implementation for Application Component***

On this step we should add implementation for **Driver** component according to ***CARPC framework*** rules and place its header according to the path mentioned in *.aidl file corresponding component description section.

Since our application already has one root component **Driver** must be inherited from ***carpc::application::Component***. In this case there is no any obligatory function implementation.

All code implemented in **Driver** component will be executed only by some event from root **OnOff** component. But this will be done later and right now current component will containe nothing exept 7of constructor and destructor functions with some debug traces.

## ***Building***

To build current application ***build.sh*** script should be called with next parameters:

<pre>
./build.sh --carpc=${CARPC_DELIVERY_DIR} --arch=${TARGET_ARCH} --os=${TARGET_OS} --action=clean_build
</pre>

As the result there will be ***${CARPC_EXAMPLES_DELIVERY_DIR}/bin/ex_03*** binary file.

## ***Verification***

To verify current application it is just enough to run it and have a look to produced traces:

<pre>
./build.sh --carpc=${CARPC_DELIVERY_DIR} --arch=${TARGET_ARCH} --os=${TARGET_OS} --action=start --target=ex_03
</pre>

Application will be started and stopped immidiatly because of callind sutdown methos from root component logic inside.

There will be next traces in console:

<pre>
[runtime] creating...
[runtime] created
[runtime] starting...
[runtime] starting 'OnOff_Thread' thread
Created: OnOff
Created: Driver
[runtime] all application threads started
[runtime] started
[runtime] running...
booting application
shutting down application
Destroyed: Driver
Destroyed: OnOff
[runtime] stopped
[runtime] destroyed
</pre>

## ***Source code***

Source code for current example could be fould here:

<pre>
https://github.com/dterletskiy/carpc-tutorial/examples/ex_03/
</pre>

