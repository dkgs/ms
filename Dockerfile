FROM conanio/gcc11 AS builder-base

RUN conan profile new default --detect \
    && conan profile update settings.compiler.libcxx=libstdc++11 default \
    && conan profile update settings.compiler.cppstd=20 default

# better cache boost compilation
RUN conan install boost/1.81.0@ --build=missing

FROM builder-base AS builder

WORKDIR /ms
COPY . /ms

RUN mkdir docker_build && cd docker_build && conan install .. --build=missing && conan build ..

FROM scratch AS image
WORKDIR /ms-run
COPY --from=builder /ms/docker_build/bin/ms .

CMD ["ms", "8081"]
EXPOSE 8081

HEALTHCHECK --interval=30s --timeout=3s \
 CMD bash -c 'if [ $(curl -f http://localhost:8081/health -s) == "{}" ]; then echo "alive"; else echo 1; fi'
