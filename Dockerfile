# 実機と同じ環境( bookworm AArch64)を構築するためのDockerfile
FROM debian:bookworm

# 必要なパッケージをインストール
RUN apt-get update && apt-get install -y \
  build-essential \
  ruby \
  git \
  cmake \
  sudo \
  && gem install shell \
  && apt-get clean && rm -rf /var/lib/apt/lists/*

# ビルド時に日本語が含まれていたらエラーになるので，C.UTF-8を指定
ENV LANG=C.UTF-8 

# RasPike-ART を clone
# libraspike-artは，RasPike-ART内で運営がリンクで紐づけていたため，直接cloneする必要がある
RUN git clone https://github.com/ETrobocon/RasPike-ART.git /RasPike-ART \
  && git clone https://github.com/ETrobocon/libraspike-art.git /RasPike-ART/libraspike-art

# 作業ディレクトリを/RasPike-ART/sdk/workspace/etrobocon2025に設定
WORKDIR /RasPike-ART/sdk/workspace/etrobocon2025
