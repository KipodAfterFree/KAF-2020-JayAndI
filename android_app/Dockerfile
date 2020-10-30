FROM mingc/android-build-box:1.20.0 AS builder

# Create workdir
RUN mkdir /build

WORKDIR /build

# Copy gradle wrapper
COPY kaf2020android/gradlew gradlew
COPY kaf2020android/gradle gradle
COPY kaf2020android/build.gradle build.gradle
COPY kaf2020android/gradle.properties gradle.properties
COPY kaf2020android/settings.gradle settings.gradle

RUN chmod +x ./gradlew
RUN ./gradlew

# Copy sources
COPY kaf2020android/client_app client_app

# Compile app
RUN ./gradlew assembleDebug

CMD ["/bin/bash"]

FROM ubuntu:20.04 AS administrator

RUN apt -y update
RUN apt -y install android-tools-adb

COPY docker/entrypoint.sh /entrypoint.sh

RUN chmod +x /entrypoint.sh

COPY --from=builder /build/app/build/outputs/apk/debug/app-debug.apk /client/app.apk

ENTRYPOINT ["/entrypoint.sh"]
