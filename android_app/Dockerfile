FROM mingc/android-build-box:1.20.0 AS builder

# Create workdir
RUN mkdir /build

WORKDIR /build

# Copy app sources
COPY . .

# Compile app
RUN cd kaf2020android; ./gradlew assembleDebug

RUN find . | grep apk