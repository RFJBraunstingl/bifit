# BIFIT

**B**ytecode **I**nterpreter **F**or **I**nternet **T**hings

## Demo
- [Hello World](https://youtu.be/p13rb7PnpW0)
- [Garbage Collection](https://youtu.be/W1QmoSAzkl8)

## Background
I will turn this in as my assignment for the lecture Abstract Machines (2023S) at the technical university of Vienna.

The assignment was to create an abstract machine.

The vision for bifit was to create something that runs Java code on the ESP32, which is a popular SoC for IoT Applications. [[1]](https://en.wikipedia.org/wiki/ESP32)[[2]](https://www.espressif.com/en/products/socs/esp32)

The ESP32 vendor espressif sells various dev kits (e.g. [this one](https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/hw-reference/esp32c3/user-guide-devkitc-02.html)) which can be used to tinker with the chip.

Furthermore espressif provides [esp-idf](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/), which is a Software development environment based on a FreeRTOS port for the esp32.

## Assignment details
The goal I set for myself was to implement a bytecode interpreter which can run the following program in Java:

```java
package hello;

public class HelloWorld {
    public static void main(String[] args) {
        Greeter greeter = new Greeter();
        System.out.println(greeter.sayHello());
    }
}
```
```java
package hello;

public class Greeter {
    public String sayHello() {
        return "Hello World!";
    }
}
```
(The code above is also incuded here as the "hello-world" example)

The project currently supports sighly more opcodes than necessary for this (e.g. to implement loops).

Also, there is a garbage collector.

## How to use examples
1. build the example you want to run (examples are simply the top level directories in the `examples` directory - e.g. `hello-world`)
    ```
    cd examples
    ./build.sh hello-world
    ```
2. build the project using esp-idf - the bytecode is packed into the built image (if you are using the official VSCode integration there is a command for that)

## Implementation Overview
This repo contains simple Java examples, which are runnable using bifit (and one groovy example which is sadly not runnable on the hardware, because it includes the groovy library, which amounts to 19MB).

This repo further includes the code to take the Java code, compile it, and pack it into the image which is flashed on the chip.

The esp-idf main method simply calls the bifit main method, which loads the packed classes and starts executing the main class specified by the JAR file.

### the build system
esp-idf is built using CMake, CMake also has the task to process the Java Bytecode into the image to be flashed.

The code for this can be found in `./EmbedJarContent.cmake`. In order to run Java code, you 
1. need to compile it into a runnable JAR file (take a look at the examples to see how could be done using gradle)
2. unzip the JAR file into a top level directory called `./jar` (the `./examples/build.sh` script does this)
3. build the project using esp-idf, which invokes CMake (CMake then takes the content of the `./jar` directory and generates C-Header files for every class)

These header files are then included by bifit as the component "classes_combined".

### the interpreter
The bifit component (`./components/bifit`) is where all the code for executing the bytecode is implemented.

bifit includes a classloader, which parses all classes included via the `classes_combined` component.

The information extracted from the META-INF file is then used to lookup the main class, which is in turn scanned for the `public static void main(String... args)` method to be executed.

The heart of the interpreter can be found in `./components/bifit/interpreter/bifit_interpreter.h` which steps through the opcodes of the executed method and invokes the implementation of the current opcode (see the [official reference](https://docs.oracle.com/javase/specs/jvms/se11/html/jvms-6.html) for information on the opcodes).

In the same file one may find which opcodes are currently supported by the implementation (just enough opcodes to successfully run the examples is implemented).

### garbage collection
bifit implements a basic mark-and-sweep garbage collector ([here](https://journal.stuffwithstuff.com/2013/12/08/babys-first-garbage-collector/) is an excellent article on how to implement something like this).

The garbage collector actually does collect garbage - proof of that is the example "garbage-collection", where unnecessary objects are created which have to be collected to ensure the heap is not starved out.

To see the running example, please refer to the video link in the Demo section.

## Running locally
There is a test file included at `./test/run-local.c` which shows bifit's configuration options and incudes everything so the bytecode interpreter can be run locally (e.g. by using VSCode or CLion).

You can even monitor the local memory consumption (to check that garbage collection works) by using `./watch-rss.sh` on a UNIX-like system.