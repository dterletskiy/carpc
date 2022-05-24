[Go to contents](../README.md#table-of_contents)

# ***CARPC entities description***

**CARPC** framework consists of next core entities:
   1. [Application Process](#application-process)
   2. [Application Thread](#application-thread)
   3. [Application Component](#application-component)
   4. [Event](#event)
   5. [Service](#service)

----

## ***Application Process***

**Application Process** is a singletone object what is created in generated main function, contains other lower level by hierarchy objects (such as **Application Threads**, **Service Registry** and **Application configuration data**) and is responcible for creation, initialization, deinitialization and deletion of this objects during booting and shutting down application.

**Application Process** is not implemented by application developer. It just could be configured and used as a part of framework runtime.

----

## ***Application Thread***

**Application Thread** is the **CARPC** object what encapsulates creationd of **OS thread** and manages its lifetime. In context of this threads are created predefined **Application Components**. In this case **CARPC Application Thread** is a container of these **Application Components** and also manages their lifetimes.

Each **Application Thread** also contains event queue and consumers map what are created and registered in the context of current thread. Each **Event** what delivered to mentioned queue will be processed in current **OS thread contex**, corresponding consumer will be found and consumer's event processor function will be called with corresponding **Event** as a parameter. This circumstance guarantees that **Event** will be delivered to its consumer and process by last one **always** in context where this consumer has been created.

Similar to **Application Process** **Application Thread** is not implemented by application developer. Last one could just configure amount of **Application Threads** in application configure each of them.

----

## ***Application Component***

**Application Component** is the **CARPC** entity that is an entry point and sanbox for implemention application code by developer.

Each application could contain any quantity of **Application Components**.

In contrast with **Application Process** and **Application Thread** each **Application Component** is designed and implemented by application developer. But at the same time all implemented **Components** should be a part of configuration for **Application Process**.

During startup all **Application Components** will be created automatically and in predefined context (**Application Thread**) by framework runtime according to user's defined configuration.

----

## ***Event***

**Event** is the **CARPC** entity what is recponsible for communication and transfering data between different entities (components or other objects created by components) what are running in the same or different (ITC) threads and in the same or different (IPC) processes.

**Event** could be created and sent from any context (OS thread), but the destination context must be always a **CARPC Application Thread**.

Destination context could be set during sending **Event**. If destination context was not set, **Event** would be delivered to the same context where it was sent from. But in this case it **must** be sent from **CARPC Application Thread**

**Event** could contain any data for transfering. The data type is declared during the **Event** definition. The data content is defined during the **Event** creation.


<!-- Each **Event** consists of next parts:
   - **Signature**
   - **Data**
   - **Context**
   - **Priority**

### **Signature**
**Signature** is a part of **Event** to identify concrete event type and subtype in the system. It is used for **Event** subscription and notification. **Signature** is predefined by **CARPC** framework but also could be extended by developer according to corresponding rules. **Signature** contains at least one "field": "type_id" what identifys event type. Also **Signature** could contain additional fields such as "id" and so on. This fields will distinguish events with the same type (type_id).

Each event consumer must subscribe on event by specifying concrete signature additional field. In this case this consumer will receive only such event with mentioned fields. Event consumer could unsibscribe from event with concrete additional fields or from this event in general. In last case this consumer will not receive any event with such event type.

Each event sender must create concrete event with specifying addition field befor sending.

### **Data**
**Data** is an **Event** payload what has specified type and any value. This value is a data what will be transfered from event sender to event consumer.

### **Context**
**Context** containes PID and TID what are specify the destination context where event consumer has been created. It could be defined during sending event by event sender. **PID** and **TID** are identifiers of process in the system and thread in the process but not assigned by **OS** but assgned by **CARPC** framework.

By default

In case if **Context** was defined during sending by event sender this event will be delivered only to consumers what are created in specified context. All other consumers what are subscribed on this event but have been created in other contexts will be ignored.

### **Priority** -->

----

## ***Service***

**Service** is a client-server architecture pattern what uses (implements) user predefined interfaces. Interfaces could be defined by **idl** file what is used for required base code generation for clients and server implementations. Each interface could provide the set of:
   - simple request
   - request with connected response
   - attribute and notification
Each interfaces instance implementation has its unique **service name** what consists of **interface name** (permanent for each interface definition) and **role name** (what must be defined in each specific implementation).

For each interface could be implemented any number of servers but each server must have unique **role name**. Each server registers itself in the **CARPC runtime**. Since this time any client with the same **role name** could be connected to this server and use interface functionality what it implements. There could be any amount of clients for each interface with the same **role name**.

----
