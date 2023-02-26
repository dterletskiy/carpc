[Go to previous menu](./development.md#development)

# ***Creating application component***

   - [Implementation](#implementation)
   - [Building](#building)
   - [Verification](#verification)
   - [Source code](#source-code)



## ***Implementation***

### ***Example folder***

Let's use previous example code for base implementation, but moved to another folder ***ex_02***.

### ***Building rules***

Afterward we should change project name in {CARPC_EXAMPLES_SOURCE_DIR}/examples/ex_02/CMakeLists.txt to ***EXAMPLE_02***

Now let's extend this application by a new ***Application Component*** and ***Application Thread***. This could be done in two steps:
   - [aidl implementation](#adding-component-and-thread-description-in-aidl-file)
   - [source implementation](#source-code-implementation-for-application-component)

### ***Adding Component and Thread description in .aidl file***

To tell ***CARPC runtime*** to create an instance of our ***Component*** we should extent our *.adl configuration file by adding ***Component*** description section.

Additionally to this we also should extend application description file by ***Application Thread*** description section because each ***Component*** will be created by runtime in the context of corresponding ***Application Thread***.

And finally we must extend existing ***Application Process*** section by a newly added ***Application Thread***.

#### ***Creating Component description section:***

<pre>
component: Component_OnOff
{
   include: "imp/app/components/OnOff/Component.hpp";
   creator: application::components::onoff::Component::creator;
};
</pre>

There are three mandatory parameter what must be defined in component description:
   - Component name after keyword ***component*** (***OnOff*** in our case)
   - Component implementation header location after keyword ***include*** (***"imp/app/components/OnOff/Component.hpp"***)
   - Component static creator function name with namespace after keyword ***creator*** (***application::components::onoff::Component::creator***)

#### ***Creating Application Thread description section:***

<pre>
thread: Thread_OnOff
{
   components: Component_OnOff;
   watchdog: 5;
};
</pre>

There are two mandatory parameter what must be defined in thread description:
   - Component names list (separated by comma) what will be created during runtime in context of current thread. This list must be defined after keyword ***components*** (***Component_OnOff*** only in our case).
   - Watchdog timout in seconds for current thread after keyword ***watchdog***.

***Application Thread***  will be created by ***CARPC runtime*** automatically and there is no need to do some source code implementation for it.

#### ***Extending Application Process description section:***

<pre>
application: example_02
{
   threads: Thread_OnOff;
};
</pre>

There is only one mandatory parameter what must be defined in process description:
   - Threads names list (separated by comma) what will be created during runtime in context of current process. This list must be defined after keyword ***threads*** (***Thread_OnOff*** only in our case).

### ***Source code implementation for Application Component***

On this step we should add implementation for ***OnOff*** component according to ***CARPC framework*** rules and place its header according to the path mentioned in *.aidl file corresponding component description section.

First of all each component must be inherited from ***carpc::application::Component*** or ***carpc::application::RootComponent***.

One application must have only one Root Component. All other components must be usual. For final developer there is only one difference between these two component types. Root component must implement two methods: **process_boot** and **process_shutdown**. First one is an entry point for developer's code and will be called when runtime will have finished all initializetion and creation actions. Second one we will discuss later.

Since our component is the only one in the system it must be the root component. As was mentioned above **process_boot** is an entry point for final developer's code so here should be places all our business logic based on the fact that this **our** code will be executed **first**.

Each root component has protected implementation of **shutdown** method what initiates of shutdown process for whole system. To simplify our example we will just call this method from **process_boot**. In this case our application will be started and shutdown immidiatly.

## ***Building***

To build current application ***build.sh*** script should be called with next parameters:

<pre>
./build.sh --sfw=${SHELL_FW_DIR} --pfw=${PYTHON_FW_DIR} --carpc=${CARPC_DELIVERY_DIR} --arch=${TARGET_ARCH} --os=${TARGET_OS} --action=clean_build
</pre>

As the result there will be ***${CARPC_EXAMPLES_DELIVERY_DIR}/bin/ex_02*** binary file.

## ***Verification***

To verify current application it is just enough to run it and have a look to produced traces:

<pre>
./build.sh --sfw=${SHELL_FW_DIR} --pfw=${PYTHON_FW_DIR} --carpc=${CARPC_DELIVERY_DIR} --arch=${TARGET_ARCH} --os=${TARGET_OS} --action=start --target=ex_02
</pre>

Application will be started and stopped immidiatly because of callind sutdown methos from root component logic inside.

There will be next traces in console:

<pre>
[runtime] creating...
[runtime] created
[runtime] starting...
[runtime] starting 'OnOff_Thread' thread
Created: OnOff
[runtime] all application threads started
[runtime] started
[runtime] running...
booting application
shutting down application
Destroyed: OnOff
[runtime] stopped
[runtime] destroyed
</pre>

## ***Source code***

Source code for current example could be fould here:

<pre>
https://github.com/dterletskiy/carpc-tutorial/examples/ex_02/
</pre>

