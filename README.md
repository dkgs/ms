# Movie Service

Simple movie service.

This movie service provides an HATEOAS style API. In order to use it directly, a CLI client is put at disposal, but any GUI can be develop on top of this service.

## Building steps

### Requirements

TODO

### Commands

Pull the code source from git, and just call conan to install the dependencies and build the project.

Here are the following steps:

``` cpp
mkdir build && cd build
conan install .. --build=missing
conan build ..
```

### Run

Once the project is built, just run the service as simple executable.

In order to exploit the service, you can just go with your web browser on the adress given, just follow the href to follow the actions.
