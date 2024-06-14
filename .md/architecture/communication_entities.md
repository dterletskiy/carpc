[Go to previous menu](./entities_description.md#entities-description)

# Communication Entities

The ***CARPC*** framework contains the following `Communication entities`:
- [Communication Entities](#communication-entities)
  - [Async](#async)
    - [Event](#event)
      - [Signature](#signature)
      - [Data](#data)
      - [Context](#context)
      - [Priority](#priority)
    - [Runnable](#runnable)
    - [Callable](#callable)
  - [Service](#service)

----

## Async

`Async` is the ***CARPC*** entity responsible for asynchronous tasks such as performing actions or processing/transferring data.

There are three types of `Async` objects:
1. [Event](#event)
2. [Runnable](#runnable)
3. [Callable](#callable)

### Event

An `Event` is a ***CARPC*** entity responsible for communication and data transfer between different entities (`Consumers` and `Senders`) represented by [**Components**](./application_structure_entities.md#application-component) or other objects created by [**Components**](./application_structure_entities.md#application-component).

`Consumers` and `Senders` can be running in the same or different threads (ITC) and in the same or different processes (IPC).

An `Event` can be created and sent from any OS context (OS thread), but the destination context must always be a ***CARPC*** [**Application Thread**](./application_structure_entities.md#application-thread). This means that the `Sender` entity can be created outside the ***CARPC*** scope and in any OS thread, but the `Consumer` must be part of the ***CARPC*** scope and created in the context of a ***CARPC*** [**Application Thread**](./application_structure_entities.md#application-thread).

The destination context can be set when sending the `Event`. If the destination context is not set, the `Event` will be delivered to the same context from which it was sent. However, in this case, it **must** be sent from a ***CARPC*** [**Application Thread**](./application_structure_entities.md#application-thread).

An `Event` can contain any data to be transferred. The data type is declared during the `Event` definition, and the data content is defined during the `Event` creation.

Each `Event` consists of the following parts:
1. [Signature](#signature)
2. [Data](#data)
3. [Context](#context)
4. [Priority](#priority)

#### Signature
The `Signature` identifies the specific event type and subtype in the system. It is used for `Event` subscription and notification. The `Signature` is predefined by the ***CARPC*** framework but can also be extended by the developer according to specific rules. The `Signature` contains at least one field, "type_id", which identifies the event type. Additional fields such as "id" can also be included to distinguish events of the same type.

Each event consumer must subscribe to an event by specifying a specific signature field. In this case, the consumer will receive only events with the specified fields. A consumer can unsubscribe from an event with specific fields or from the event in general, in which case the consumer will not receive any event of that type.

Each event sender must create a specific event by specifying additional fields before sending.

#### Data
**Data** is the `Event` payload, which has a specified type and value. This value is the data transferred from the event sender to the event consumer.

#### Context
The **Context** contains the PID and TID, specifying the destination context where the event consumer was created. This can be defined when sending the event by the event sender. **PID** and **TID** are identifiers of the process in the system and the thread in the process, assigned by the ***CARPC runtime***. They are not equivalent to OS process and thread IDs.

If the **Context** is defined when sending, the event will be delivered only to consumers created in the specified context. All other consumers subscribed to the event but created in other contexts will be ignored.

By default, if **Context** is not set, it will be set to the current **PID** and **Broadcast TID**, meaning the `Event` will be delivered to all `Consumers` in all **Application Threads** of the current application.

#### Priority
**Priority** specifies the importance of the `Event` compared to other events in the **Event Queue**. An `Event` with a higher **Priority** will be processed before events with a lower **Priority**, even if it was added to the **Event Queue** later.

By default, all **Events** have equal **Priority**, which can be changed during `Event` creation.

### Runnable

`Runnable` is the ***CARPC*** entity responsible for executing a defined operation (function) in the same or another **Service Application Thread** asynchronously. Users can define an operation (a function with the signature `void(void)`), attach it to a created `Runnable` object, and send it for asynchronous execution to a service thread.

A `Runnable` object can be sent from any thread to a defined **CARPC Context**, but if sent to the same **Context**, it must be done from a **Service Application Thread**.

Optionally, a `Runnable` object can be sent as blocked. This means the current thread will be blocked (suspended) until the `Runnable` object is processed by the consumer thread and then released. However, in this case, the sender and receiver threads must be different.

### Callable

A `Callable` object has similar purposes as a `Runnable` but with a few differences:
   - The function signature can contain parameters, and values for them must be set during the `Callable` object creation.
   - This object cannot be sent as blocked.

----

## Service

**Service** is a client-server architecture pattern that uses user-defined interfaces. Interfaces can be defined by an **idl** file, which is used to generate the required base code for client and server implementations. Each interface can provide the following:
   - Simple request
   - Request with a connected response
   - Attribute and notification

Each interface instance implementation has a unique **service name**, which consists of the **interface name** (permanent for each interface definition) and a **role name** (defined for each specific implementation).

Any number of servers can implement each interface, but each server must have a unique **role name**. Each server registers itself in the ***CARPC runtime***. Once registered, any client with the same **role name** can connect to this server and use the interface functionality it implements. There can be any number of clients for each interface with the same **role name**.
