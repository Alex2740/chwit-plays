FROM alpine:3.17.0 AS build

RUN apk add --update --no-cache \
      build-base \
      openblas-dev \
      unzip \
      wget \
      cmake \
      g++ \
      libjpeg  \
      libjpeg-turbo-dev \
      libpng-dev \
      jasper-dev \
      tiff-dev \
      libwebp-dev \
      clang-dev \
      linux-headers

ENV CC /usr/bin/clang
ENV CXX /usr/bin/g++
ENV OPENCV_VERSION='4.7.0' DEBIAN_FRONTEND=noninteractive

COPY sources/opencv/ /opt/opencv-${OPENCV_VERSION}

RUN mkdir -p /opt/opencv-${OPENCV_VERSION}/build && \
  cd /opt/opencv-${OPENCV_VERSION}/build && \
  cmake \
    -D BUILD_DOCS=OFF \
    -D BUILD_EXAMPLES=OFF \
    -D BUILD_opencv_apps=OFF \
    -D BUILD_opencv_python2=OFF \
    -D BUILD_opencv_python3=OFF \
    -D BUILD_PERF_TESTS=OFF \
    -D BUILD_SHARED_LIBS=OFF \ 
    -D BUILD_TESTS=OFF \
    -D CMAKE_BUILD_TYPE=RELEASE \
    -D ENABLE_PRECOMPILED_HEADERS=OFF \
    -D FORCE_VTK=OFF \
    -D WITH_FFMPEG=OFF \
    -D WITH_GDAL=OFF \ 
    -D WITH_IPP=OFF \
    -D WITH_OPENEXR=OFF \
    -D WITH_OPENGL=OFF \ 
    -D WITH_QT=OFF \
    -D WITH_TBB=OFF \ 
    -D WITH_XINE=OFF \ 
    -D BUILD_JPEG=ON  \
    -D BUILD_TIFF=ON \
    -D BUILD_PNG=ON \
  .. && \
  make -j$(nproc) && \
  make install && \
  rm -rf /opt/opencv-${OPENCV_VERSION}

WORKDIR /broadcaster

COPY include/ ./include
COPY lib/ ./lib
COPY resources/ ./resources
COPY src/ ./src/
COPY CMakeLists.txt .
COPY main.cpp .

WORKDIR /broadcaster/out

RUN cmake -DCMAKE_BUILD_TYPE=Release .. && \
    cmake --build . --parallel 8

CMD [ "tail", "-f", "/dev/null" ]
