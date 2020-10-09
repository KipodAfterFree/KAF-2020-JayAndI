FROM mingc/android-build-box:1.20.0 AS builder

# Create workdir
RUN mkdir /build

WORKDIR /build

# Copy app sources
COPY . .

# Compile app
RUN cd kaf2020android; ./gradlew assembleDebug

CMD ["/bin/bash"]

FROM thyrlian/android-sdk:5.0 AS client

COPY --from=builder /build/kaf2020android/app/build/outputs/apk/debug/app-debug.apk /client/app.apk

RUN apt -y update 
RUN apt -y install adb