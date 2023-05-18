# Movie Service

Simple movie service.

This movie service provides an HATEOAS style API (actually serializes in HTML for convenience).

The application is composed of two parts.

The first, in the `namespace service` provides the business logic and is based on dynamic polymorphism
because we may expect heterogeneous implementations at runtime.

The second, in the `namespace api` provides an external API to discuss with the business logic and is
base on static polymorphism.

Both combined creats a REST server that is browsable by an internet navigator.


## Building steps

### Requirements

Make sure you have a C++20 compatible compiler, CMake >= 3.23 and Conan >= 1.60. Or just use Docker.

### Commands

Pull the code source from git, and just call conan to install the dependencies and build the project.

Here are the following steps:

``` shell
mkdir build && cd build
conan install .. --build=missing
conan build ..
```

Or, if you want you can just build through the docker image:

``` shell
docker build . -t ms:latest
```

### Run

Once the project is built, just run the service as simple executable. Two arguments are necessary, the address and the port:

``` shell
./build/bin/ms 127.0.0.1 8081
```

or if built with docker:

``` shell
docker run -p 8081 ms:latest
```

and just go to the url with your favorite web browser: <http://localhost:8081>.

In order to exploit the service, you can just go with your web browser on the adress given, just follow the href to follow the actions.
