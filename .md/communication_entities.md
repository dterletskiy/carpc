[Go to previous menu](./carpc_entities_description.md#carpc-entities-description)

# ***Communication entities***

**CARPC** framework contains next  ***Communication entities***:
   1. [Async](#async)
   2. [Service](#service)

----

## ***Async***

**Async** is the **CARPC** entity what is recponsible for asynchronous jobs such as performing some action or processing/transfering data.

There are 3 **Async** type objects:
   - [Event](#event)
   - [Runnable](#runnable)
   - [Callable](#callable)

### ***Event***

**Event** is the **CARPC** entity what is recponsible for communication and transfering data between different entities (**Consumers** and **Senders**) what are represented by **Components** or other objects created by **Components**.

**Consumer** and **Sender** could be running (created) in the same or different (ITC) threads and in the same or different (IPC) processes.

**Event** could be created and sent from any OS context (OS thread), but the destination context must be always a **CARPC Application Thread**. This means that **Sender** entity could be created out of **CARPC** scope and in any OS thread, but **Consumer** must be a part of **CARPC** scope and must be created in context of **CARPC Application Thread**.

Destination context could be set during sending **Event**. If destination context was not set, **Event** would be delivered to the same context where it was sent from. But in this case it **must** be sent from **CARPC Application Thread**

**Event** could contain any data for transfering. The data type is declared during the **Event** definition. The data content is defined during the **Event** creation.


Each **Event** consists of next parts:
   - **Signature**
   - **Data**
   - **Context**
   - **Priority**

#### **Signature**
**Signature** is a part of **Event** to identify concrete event type and subtype in the system. It is used for **Event** subscription and notification. **Signature** is predefined by **CARPC** framework but also could be extended by developer according to corresponding rules. **Signature** contains at least one "field": "type_id" what identifys event type. Also **Signature** could contain additional fields such as "id" and so on. This fields will distinguish events with the same type (type_id).

Each event consumer must subscribe on event by specifying concrete signature additional field. In this case this consumer will receive only such event with mentioned fields. Event consumer could unsibscribe from event with concrete additional fields or from this event in general. In last case this consumer will not receive any event with such event type.

Each event sender must create concrete event with specifying addition field befor sending.

#### **Data**
**Data** is an **Event** payload what has specified type and any value. This value is a data what will be transfered from event sender to event consumer.

#### **Context**
**Context** containes PID and TID what are specify the destination context where event consumer has been created. It could be defined during sending event by event sender. **PID** and **TID** are identifiers of process in the system and thread in the process and assgned by **CARPC runtime**. They are not equivalent to **OS** process and thread IDs.

In case if **Context** was defined during sending by event sender this event will be delivered only to consumers what are created in specified context. All other consumers what are subscribed on this event but have been created in other contexts will be ignored.

By default **Context** (when it is not set) will be set to current **PID** and **Broadcast TID** what means that **Event** will be delivered to all **Consumers** in all **Application Threads** of current application.

#### **Priority**
This part specifies the priority what will be used by **Application Thread** in comparison to other events from **Event Queue**. **Event** with higher **Priority** will be process before **Events** with lower **Priority** event if it was added to **Event Queue** later.

By default all **Events** have equal **Priority** and it could be changed during **Event** creation.

----

## ***Service***

**Service** is a client-server architecture pattern what uses (implements) user predefined interfaces. Interfaces could be defined by **idl** file what is used for required base code generation for clients and server implementations. Each interface could provide the set of:
   - simple request
   - request with connected response
   - attribute and notification
Each interfaces instance implementation has its unique **service name** what consists of **interface name** (permanent for each interface definition) and **role name** (what must be defined in each specific implementation).

For each interface could be implemented any number of servers but each server must have unique **role name**. Each server registers itself in the **CARPC runtime**. Since this time any client with the same **role name** could be connected to this server and use interface functionality what it implements. There could be any amount of clients for each interface with the same **role name**.

----
