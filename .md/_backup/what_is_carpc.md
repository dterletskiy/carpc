[Go to contents](../README.md#table-of_contents)

# ***What is CARPC***

As you may know, there is a huge amount of different frameworks and libraries that allow you to implement Remote Procedure Call (RPC) and perform asynchronous development for your applications. Each of them has its own advantages and disadvantages. However, some of them may lack certain functionalities that could be useful in specific cases simply because they were not designed to support such functionalities.

CARPC is the Component Architecture Remote Procedure Call, which enables you to implement RPC in your application.

With CARPC, you can:
   - Implement your **component architecture** / **event-driven architecture** / **client-server** application using the functionalities provided by the current framework.
   - Implement **remote procedure call** in your application locally and remoutly using **IPC**.
   - Implement(establish) asynchronous communication between different entities within your application.
   - Choose an appropriate **transport protocol** from the different ones supported by **CARPC**.
   - Define, implement, and use your **service interfaces** in a user-friendly manner.
   - Utilize its own **serialization/deserialization** mechanism, which also supports **GPB**.

But what does "Component Architecture" mean?

## ***Component Architecture***

According to one definition, **Component Architecture** is a type of application architecture composed of independent, modular, and reusable building blocks called components. This concept is also applicable in the case of CARPC, but with some adaptations.

In CARPC, a **Component** is one of the central parts of the application. Each application consists of predefined components that are created by the **CARPC runtime** during application initialization. All other entities defined in the application are created within the scope of existing components and have a lifetime corresponding to those components (except for static objects).

Each **Component** is independent of other **Components** and entities.

Each **Component** is unique in the system and performs specific functionality for which it is intended and implemented. Therefore, logically, a **Component** cannot be reused within the same application, except in cases where you need two or more different **Component** entities with the same logic and functionality in your process... but why?

The **CARPC Component** is collection-like for further created service (server/client) entities.
