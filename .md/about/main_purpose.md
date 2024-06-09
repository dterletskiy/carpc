[Go to contents](../../README.md#table-of_contents)

## Main purpose

***CARPC*** (**Component Architecture Procedure Call**) framework is designed to streamline the development of applications with a distributed architecture, offering a robust mechanism for **Remote Procedure Calls (RPC)** written in C++. The primary objectives of CARPC are to facilitate the creation of modular applications, enhance stability, and foster clear separation of concerns between components.

Key Features:

- Component-Based Architecture: CARPC employs a component-based architecture, enabling developers to easily construct applications with segmented functionality. This approach enhances application stability and promotes isolated logic between components, leading to more resilient software systems.

- Efficient Asynchronous Invocation: The framework provides a convenient mechanism for asynchronous function calls within a single application without the need for additional synchronization mechanisms. This feature enhances responsiveness and scalability by enabling parallel execution of tasks.

- Event Mechanism for Asynchronous Communication: CARPC offers a flexible event mechanism for seamless asynchronous communication between application components and across different processes, whether on the same or different computational nodes. This capability facilitates the construction of Event-Driven architectures, simplifying the development of responsive and scalable systems.

- RPC Infrastructure: Built upon the event mechanism, CARPC's RPC functionality allows for the description and implementation of interfaces between diverse components and applications. This enables seamless interaction between distributed elements of the system, fostering interoperability and modular design.

- Robust Logging System: CARPC includes a user-friendly logging system, capable of outputting information to various destinations such as the console, diagnostic utilities like [DLT Viewer](https://github.com/COVESA/dlt-viewer), and [Android logging system](https://developer.android.com/tools/logcat). This feature facilitates debugging and performance monitoring, enhancing overall system reliability.

- Command-Line Parameter Handling and Configuration File Parsing: The framework offers convenient utilities for processing command-line parameters during application startup and parsing configuration files. This simplifies the management of application settings, ensuring flexibility and ease of deployment.

- Code Generation: CARPC provides code generation capabilities based on descriptive application files, generating foundational code for service threads, components, and interface definitions akin to Interface Definition Language (IDL) and Android Interface Definition Language (AIDL). This automates repetitive tasks and accelerates development, ensuring consistency and reducing errors.

In summary, CARPC aims to empower developers by providing a comprehensive framework for building distributed applications with enhanced stability, scalability, and maintainability. Through its rich set of features and emphasis on modular design, CARPC facilitates the development of robust and efficient software solutions.


## Component Architecture in CARPC

Component Architecture, as defined, constitutes an application structure built upon independent, modular, and reusable building blocks known as components. This paradigm finds application within CARPC, albeit with certain adaptations.

### Components in CARPC

In CARPC, a Component stands as a pivotal element within the application framework. During application initialization, the CARPC runtime generates predefined components. All other entities within the application are instantiated within the context of these components, aligning their lifecycle with that of the parent component (with the exception of static objects).

- Each Component operates autonomously, devoid of interdependence with other Components or entities.
- Interaction between components is possible through the asynchronous interaction mechanism provided by the framework.
- Uniqueness characterizes each Component within the system, assigned to execute specific functionality tailored to its implementation.
- Typically, Components aren't designed for reuse within the same application instance. However, scenarios may arise where identical logic and functionality necessitate the instantiation of multiple Component entities within a process.

### Component as a Collection

The CARPC Component serves as a collection hub for the creation of subsequent service entities, whether server-side or client-side.
