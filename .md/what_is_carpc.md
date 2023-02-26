[Go to contents](../README.md#table-of_contents)

# ***What is CARPC***

As you know there are huge amount of different frameworks and libraries what are allowes to implement Remote Procedure Call for your applications. Obviously each of them has its advantages and disadvantages. Each of them does not have some functionality what could be usefull in some particular cases just because there was no aim to implement and support such functionality.

CARPC is the Component Architecture Remote Procedure Call what allowes you implement RPC in your application.

CARPC allows you to:
   - implement your **component architecture** / **event driven architecture** / **client-server** application using functionality provided by current framework.
   - implement **remote procedure call** in your application locally and remoutly using **IPC**.
   - choose appropriate **transport protocol** from different supported by **CARPC**.
   - define, implement and use your **service interfaces** what are user friendly in usage.
   - use its own **serialization/deserialization** mechanism what also supports **GPB**.

But... what means "Component Architecture"?

## ***Component Architecture***

According to one of the definitions, **Component Architecture** is a type of application architecture composed of independent, modular, and reusable building blocks called components. It is also applicable in case of CARPC but with some adaptations.

In CARPC **Component** is one of the central parts of the application. Each application consisns of predefined components what are created by **CARPC runtime** during application initialization. All other entities defined in the application are created in scope of existing components and have a lifetime of corresponding components (excapt of static objects).

Each **Component** is independent from other **Components** and entities.

Each **Component** is unique in the system and perform specific functionality for which it is intended and implemented. So logically **Component** can't be reused in the same application excapt of cases when you need two or more different **Component** entities with the same logic and functionality in your process... but why?

**CARPC Component** is collection-like for further created service (server/client) entities.
