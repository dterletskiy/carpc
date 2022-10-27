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

Now let's extend this application by new ***Application Component*** and ***Application Thread***. This could be done in two steps:
   - [aidl implementation](#adding-component-and-thread-description-in-aidl-file)
   - [source implementation](#source-code-implementation-for-application-component)

### ***Adding Component and Thread description in .aidl file***

This could be done by adding sections for component and thread description in *.adl file:

<pre>
component: OnOff
{
   include: "imp/app/components/OnOff/Component.hpp";
   creator: application::components::onoff::Component::creator;
};

thread: OnOff_Thread
{
   components: OnOff;
   watchdog: 5;
};
</pre>

There are three mandatory parameter what must be defined in component description:
   - Component name after keyword ***component*** (***OnOff*** in our case)
   - Component implementation header location after keyword ***include*** (***"imp/app/components/OnOff/Component.hpp"***)
   - Component static creator function name with namespace after keyword ***creator*** (***application::components::onoff::Component::creator***)

There are two mandatory parameter what must be defined in thread description:
   - Component names list (separated by comma) what will be created during runtime in context of current thread. This list must be defined after keyword ***components*** (***OnOff*** only in our case).
   - Watchdog timout in seconds for current thread after keyword ***watchdog***.

### ***Source code implementation for Application Component***

On this step we should add implementation for ***OnOff*** component according to ***CARPC framework*** rules and place its header according to the path mentioned in *.aidl file corresponding component description section.

First of all each component must be inherited from ***carpc::application::Component*** or ***carpc::application::RootComponent***

## ***Building***

To build current application ***build.sh*** script should be called with next parameters:

<pre>
./build.sh --sfw=${SHELL_FW_DIR} --pfw=${PYTHON_FW_DIR} --carpc=${CARPC_DELIVERY_DIR} --arch=x86_64 --os=linux --action=clean_build
</pre>

As the result there will be ***${CARPC_EXAMPLES_DELIVERY_DIR}/bin/ex_02*** binary file.

## ***Verification***

To verify current application it is just enough to run it and have a look to produced traces:

<pre>
./build.sh --sfw=${SHELL_FW_DIR} --pfw=${PYTHON_FW_DIR} --carpc=${CARPC_DELIVERY_DIR} --arch=x86_64 --os=linux --action=start --target=ex_01
</pre>

Application will be started and stopped immidiatly because of missing and logic inside.

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

