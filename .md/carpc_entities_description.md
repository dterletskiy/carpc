[Go to contents](../README.md#table-of_contents)

# ***CARPC entities description***

**CARPC** framework consists of next core entities:
   1. [Process](#process)
   2. [Thread](#thread)
   3. [Event](#event)
   4. [Component](#component)
   5. [Service](#service)

----

## ***Process***

**Process** is a singletone object what is created in generated main function, contains other lower level by hierarchy objects (such as **Threads**, **Service Registry** and **Application configuration data**) and is responcible for creation, initialization, deinitialization and deletion of this objects during booting and shutting down application.

----

## ***Thread***

**Thread** is the **CARPC** object what encapsulates creationd of **OS** thread and manages its lifetime. In context of this thread are created predefined **Components**. In this case **CARPC Thread** is a container of these **Components** and also manages their lifetimes.

Each **Thread** also contains event queue and consumers map what are created and registered in the context of current thread. Each **Event** what delivered in mentioned queue will be processed in current thread contex, corresponding consumer will be found and consumer's event processor function will be called with corresponding **Event**.This circumstance guarantees that **Event** will be delivered to its consumer and process by last one **always** in context where this consumer has been created.

----

## ***Event***

**Event** is the **CARPC** entity what is recponsible for communication and transfering data between different entities (components or other objects created by components) what are running in the same or different (ITC) threads and in the same or different (IPC) processes.
Each **Event** consists of next parts:
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

### **Priority**

----

## ***Component***

----

## ***Service***

----
