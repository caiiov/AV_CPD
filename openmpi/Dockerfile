# Use uma imagem base com suporte a MPI e C++
FROM ubuntu:22.04

# Atualizar e instalar dependências necessárias
RUN apt-get update && apt-get install -y \
    build-essential \
    mpich \
    libnlohmann-json-dev \
    && rm -rf /var/lib/apt/lists/*

# Criar um diretório de trabalho
WORKDIR /usr/src/app

# Copiar o código fonte para o container
COPY main.cpp .

# Compilar o código
RUN mpicxx -o main main.cpp

# Definir o comando de execução
CMD ["mpirun", "-np", "3", "./main"]
