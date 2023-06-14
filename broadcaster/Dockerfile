# Utilisez une image de base avec les dépendances de base pour OpenGL
FROM nvidia/opengl:1.0-glvnd-runtime-ubuntu18.04

# Installe les dépendances système nécessaires pour OpenCV et FFmpeg
RUN apt-get update && apt-get install -y \
    libsm6 \
    libxext6 \
    libxrender-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libglfw3 \
    libglfw3-dev \
    ffmpeg

# Installe OpenCV à partir des sources
RUN apt-get install -y \
    build-essential \
    cmake \
    libgtk2.0-dev \
    pkg-config \
    libavcodec-dev \
    libavformat-dev \
    libswscale-dev \
    python3-dev \
    python3-numpy \
    libtbb2 \
    libtbb-dev \
    libjpeg-dev \
    libpng-dev \
    libtiff-dev \
    libv4l-dev \
    libdc1394-22-dev

ENV OPENCV_VERSION='4.7.0' DEBIAN_FRONTEND=noninteractive

COPY sources/opencv/ /opt/opencv-${OPENCV_VERSION}

RUN cd /opt/opencv-${OPENCV_VERSION} && \
    mkdir build && \
    cd build && \
    cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local .. && \
    make -j$(nproc) && \
    make install

# Définir la variable d'environnement pour OpenCV
ENV OPENCV_PKG_CONFIG_PATH=/usr/local/lib/pkgconfig/opencv4.pc

WORKDIR /broadcaster

COPY include/ ./include
COPY lib/ ./lib
COPY resources/ ./resources
COPY src/ ./src/
COPY CMakeLists.txt .
COPY main.cpp .

WORKDIR /broadcaster/out

RUN cmake -DCMAKE_BUILD_TYPE=Release .. && \
    cmake --build .

CMD [ "tail", "-f", "/dev/null" ]
