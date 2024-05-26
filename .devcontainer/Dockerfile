FROM debian:bookworm-slim

SHELL ["/bin/bash", "-c"]

RUN apt-get  update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y \
        build-essential \
        curl \
        git \
        gcc \
        g++ \
        cmake \
        ninja-build \
        nano \
        bash-completion \
        python3 \
        nodejs \
        wget \
        clang-format \
        locales \
        yarn \
        zip \
        unzip \
        tar \
        pkg-config

ARG USERNAME=wasm
ARG USER_UID=1000
ARG USER_GID=$USER_UID
ENV LANG en_US.UTF-8

RUN \
    groupadd --gid $USER_GID $USERNAME && \
    useradd --uid $USER_UID --gid $USER_GID -s /bin/bash -m $USERNAME && \
    sed -i -e 's/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/' /etc/locale.gen && \
    dpkg-reconfigure --frontend=noninteractive locales && \
    update-locale LANG=en_US.UTF-8

USER $USERNAME

RUN \
    cd /home/$USERNAME && \
    mkdir -p emsdk && \
    git clone --depth 1 https://github.com/emscripten-core/emsdk.git emsdk && \
    cd emsdk && \
    ./emsdk install latest && \
    ./emsdk activate latest && \
    echo "source \"/home/$USERNAME/emsdk/emsdk_env.sh\"" >> /home/$USERNAME/.bash_profile

RUN \
    curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.39.7/install.sh | bash && \
    export NVM_DIR="$([ -z "${XDG_CONFIG_HOME-}" ] && printf %s "${HOME}/.nvm" || printf %s "${XDG_CONFIG_HOME}/nvm")" && \
    [ -s "$NVM_DIR/nvm.sh" ] && \. "$NVM_DIR/nvm.sh" && \
    nvm install --lts && \
    corepack enable && \
    cd /home/$USERNAME/emsdk/upstream/emscripten && \
    npm install
