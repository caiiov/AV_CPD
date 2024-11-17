FROM gcc:latest

WORKDIR /app

COPY openmp.cpp .

COPY json.hpp .

RUN g++ -o openmp openmp.cpp -fopenmp

CMD ["./openmp"]